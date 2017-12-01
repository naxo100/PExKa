/*
 * State.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: naxo
 */

#include "State.h"
#include "../pattern/Environment.h"
#include "../matching/Injection.h"
#include "../data_structs/MaskedBinaryRandomTree.h"
#include <cmath>

namespace state {

State::State(size_t tok_count,const std::vector<Variable*>& _vars,
		const state::BaseExpression& vol,const pattern::Environment& _env) :
	env(_env),volume(vol),vars(_vars),tokens (new float[tok_count]),
	activityTree(nullptr),injections(nullptr),randGen(0/*TODO seed*/){}

State::~State() {
	delete[] tokens;
	if(activityTree)
		delete activityTree;
	if(injections)
		delete[] injections;
	//test
	activityTree = nullptr;
	injections = nullptr;
}


void State::addTokens(float n,short tok_id){
	tokens[tok_id] += n;
}

void State::addNodes(unsigned n,const pattern::Mixture& mix){
	if(n == 0)//test?
		return;
	for(auto comp_p : mix){
		graph.addComponents(n,*comp_p,env);
	}
}

unsigned State::mixInstances(const pattern::Mixture& mix) const{
	//auto& mix = env.getMixtures().front();
	unsigned count = 1;
	for(auto cc : mix){
		count *= injections[cc->getId()].count();
	}
	return count;
}

two<FL_TYPE> State::evalActivity(const simulation::Rule& r) const{
	two<FL_TYPE> act;
	act.first = mixInstances(r.getLHS()) * r.getRate().getValue(*this).valueAs<float>();
	//TODO act.second = ...
	act.second = 0.;
	return act;
}
/*
template <int n> //for both
void State::negativeUpdate(SiteGraph::Internal& intf){
	auto& lifts = intf.deps;

}
template <> //for state
void State::negativeUpdate<0>(SiteGraph::Internal& intf){
	matching::InjSet* lifts = intf.deps.first;
	for(auto lift_it = lifts->begin(); lift_it != lifts->end(); lift_it++){
		if((*lift_it)->isTrashed()){
			lifts->erase(lift_it);//TODO better way?
			continue;
		}
		//TODO injections[(*lift_it)->]
	}
	lifts->clear();
}
template <> //for links
void State::negativeUpdate<1>(SiteGraph::Internal& intf){
	auto& lifts = intf.deps.second;

}*/

void State::bind(const simulation::Rule::Action& a,EventInfo& ev){
	auto n1 = (get<3>(a.trgt1) ? ev.fresh_emb : ev.emb[get<0>(a.trgt1)])[get<1>(a.trgt1)];
	auto n2 = (get<3>(a.trgt2) ? ev.fresh_emb : ev.emb[get<0>(a.trgt2)])[get<1>(a.trgt2)];
	try {
		auto new_node = ev.new_cc.at(n1);
		if(!new_node)
			n1 = new Node(*n1,ev.new_cc);
	}catch(...){}
	try {
		auto new_node = ev.new_cc.at(n2);
		if(!new_node)
			n2 = new Node(*n2,ev.new_cc);
	}catch(...){}

	n1->bind(ev,injections,get<2>(a.trgt1),n2,get<2>(a.trgt2));
}

void State::free(const simulation::Rule::Action& a,EventInfo& ev){
	auto node = ev.emb[get<0>(a.trgt1)][get<1>(a.trgt1)];
	try{
		auto new_node = ev.new_cc.at(node);
		if(new_node)
			new_node->unbind(ev,injections,get<2>(a.trgt1));
	}
	catch(std::out_of_range &ex){}
	node->unbind(ev,injections,get<2>(a.trgt1));
}

void State::modify(const simulation::Rule::Action& a,EventInfo& ev){
	auto node = ev.emb[get<0>(a.trgt1)][get<1>(a.trgt1)];
	try{
		auto new_node = ev.new_cc.at(node);
		if(new_node)
			new_node->changeIntState(ev,injections,get<2>(a.trgt1),get<0>(a.trgt2));
	}
	catch(std::out_of_range &ex){}
	node->changeIntState(ev,injections,get<2>(a.trgt1),get<0>(a.trgt2));
}

void State::del(const simulation::Rule::Action& a,EventInfo& ev){
	auto node = ev.emb[get<0>(a.trgt1)][get<1>(a.trgt1)];
	node->removeFrom(ev,injections,graph);
}

void (State::*State::action[4])(const simulation::Rule::Action&,EventInfo&) =
		{&State::modify,&State::bind,&State::free,&State::del};

void State::apply(const simulation::Rule& r,EventInfo& ev){
	//ADD action first
	ev.fresh_emb = new Node*[r.getNewNodes().size()];//maybe not!
	int i = 0;
	for(auto n : r.getNewNodes()){
		auto node = new Node(*n,ev.new_cc);
		graph.allocate(node);
		ev.fresh_emb[i] = node;
		i++;
	}
	for(auto& act : r.getScript()){
		(this->*(action)[act.t])(act,ev);
	}
	for(auto inj : ev.inj_mask)//only injs that will no be copied
		ev.to_update.emplace(&inj.first->pattern());
	//TODO create left nodes from ev.new_cc
	for(auto& node_pair : ev.new_cc){
		if(!node_pair.second){
			node_pair.second = new Node(*node_pair.first,ev.new_cc);
		}
	}
	//copy deps and alloc
	for(auto& node_pair : ev.new_cc){
		if(node_pair.first != node_pair.second){
			node_pair.second->copyDeps(*node_pair.first,ev,injections);
			graph.allocate(node_pair.second);
		}
	}
}

void State::positiveUpdate(const simulation::Rule& r,EventInfo& ev){
	//TODO vars_to_wake_up
	auto& wake_up = r.getInfluences();
	for(auto& cc_info : wake_up){
		auto cc = cc_info.first;
		auto& info = cc_info.second;
		Node* node;
		for(auto cc_ag_root : info.node_id){
			if(cc_ag_root.first == small_id(-1))//new
				node = ev.fresh_emb[cc_ag_root.second.first];
			else
				try {
					node = ev.emb[cc_ag_root.first][cc_ag_root.second.first];
					node = ev.new_cc.at(node);
				}catch(out_of_range &ex) {}
			two<std::list<Node::Internal*> > port_lists;
			try{
				matching::Injection* inj_p = injections[cc->getId()].emplace(*cc,*node,port_lists,cc_ag_root.second.second);
				for(auto port : port_lists.first)
					port->deps.first->emplace(inj_p);
				for(auto port : port_lists.second)
					port->deps.second->emplace(inj_p);
				if(!port_lists.first.size() && !port_lists.second.size())
					node->addDep(inj_p);
				//cout << "matching Node " << node_p->toString(env) << " with CC " << comp.toString(env) << endl;
				ev.to_update.emplace(cc);
			}
			catch(False& e){
				//Not a match with candidate
			}
		}
	}
}

void State::advanceUntil(FL_TYPE sync_t) {
	while(counter.getTime() < sync_t){
		try{
			counter.advanceTime(event());
			counter.incEvents();
		}
		catch(NullEvent &ex){
			#if DEBUG
				cout << "\t(null-event)" << endl;
			#endif
			counter.incNullEvents(ex.error);
		}
	}
}

EventInfo* State::selectBinaryInj(const pattern::Mixture& mix,bool clsh_if_un) const {
	set<Node*> roots,total_cod;
	//map<int,SiteGraph::Node*> total_inj;
	//SiteGraph::Node*** total_inj = new SiteGraph::Node**[mix.compsCount()];
	EventInfo* ev = new EventInfo();
	ev->emb = new Node**[mix.compsCount()];
	ev->cc_count = mix.compsCount();
	int i = 0;
	for(auto cc : mix){
		ev->emb[i] = new Node*[cc->size()];
		auto& inj = injections[cc->getId()].chooseRandom(randGen);
		try{
			inj.codomain(ev->emb[i],total_cod);
		}
		catch(False& ex){
			delete[] ev->emb;
			delete ev;
			throw NullEvent(2);//overlapped codomains
		}
		i++;
	}
	if(clsh_if_un)
		return ev;//TODO
	return ev;
}

EventInfo* State::selectUnaryInj(const pattern::Mixture& mix) const {
	return nullptr;
}

EventInfo* State::selectInjection(const pattern::Mixture& mix,two<FL_TYPE> bin_act,
		two<FL_TYPE> un_act) {
	//if mix.is empty TODO
	//if mix.unary -> select_binary
	if(std::isinf(bin_act.first)){
		if(std::isinf(un_act.first)){
			auto rd = uniform_int_distribution<int>(1)(randGen);
			if(rd)
				return selectBinaryInj(mix,true);
			else
				return selectUnaryInj(mix);
		}
		else
			return selectBinaryInj(mix,true);
	}
	else{
		if(std::isinf(un_act.first))
			return selectUnaryInj(mix);
		else{
			auto rd = uniform_real_distribution<FL_TYPE>(bin_act.first+un_act.first)(randGen);
			if(rd > bin_act.first)
				return selectBinaryInj(mix,true);
			else
				return selectUnaryInj(mix);
		}
	}
	return nullptr;
}

pair<const simulation::Rule&,EventInfo*> State::drawRule(){
	auto rid_alpha = activityTree->chooseRandom();
	auto& rule = env.getRules()[rid_alpha.first];

	auto a1a2 = evalActivity(rule);
	auto alpha = a1a2.first + a1a2.second;

	//*?
	if(alpha == 0.)
		activityTree->add(rid_alpha.first,0.);

	if(!std::isinf(alpha)){
		if(alpha > rid_alpha.second){
			//TODO if IntSet.mem rule_id state.silenced then (if !Parameter.debugModeOn then Debug.tag "Real activity is below approximation... but I knew it!") else invalid_arg "State.draw_rule: activity invariant violation"
		}
		auto rd = uniform_real_distribution<FL_TYPE>(0.0,1.0)(randGen);
		if(rd > (alpha / rid_alpha.second) ){
			activityTree->add(rid_alpha.first,alpha);
			throw NullEvent(3);//TODO (Null_event 3)) (*null event because of over approximation of activity*)
		}
	}
	int radius = 0;//TODO
	EventInfo* ev_p;
	ev_p = selectInjection(rule.getLHS(),make_pair(a1a2.second,radius),
			make_pair(a1a2.first,radius));
	return pair<const simulation::Rule&,EventInfo*>(rule,ev_p);
}

FL_TYPE State::event() {
	FL_TYPE dt,act;
	#if DEBUG
		printf("Event %3lu",counter.getEvent());
	#endif
	act = activityTree->total();
	if(act < 0.)
		throw invalid_argument("Activity falls below zero.");
	auto exp_distr = exponential_distribution<FL_TYPE>(act);
	dt = exp_distr(randGen);
	if(act == 0 || std::isinf(dt)){
		return dt;//TODO
	}
	//TODO perts

	#if DEBUG
		printf("  dt = %3.4f",dt);
	#endif
	EventInfo* ev = nullptr;
	auto rule_eventp = drawRule();
	ev = rule_eventp.second;
	#if DEBUG
		printf( "  | Rule: %-11.11s",rule_eventp.first.getName().c_str());
		cout << "  Root-node: ";
		for(int i = 0; i < ev->cc_count; i++)
			cout << ev->emb[i][0]->getAddress() << ",";
		cout << endl;
		//printf("  Root-node: %03lu\n",(ev->cc_count ?
		//		ev->emb[0][0]->getAddress() : -1L));
	#endif
	apply(rule_eventp.first,*rule_eventp.second);
	positiveUpdate(rule_eventp.first,*rule_eventp.second);
	//prepare negative_update
	set<small_id> rule_ids;
	for(auto cc : rule_eventp.second->to_update){
		for(auto r_id : cc->includedIn())
			rule_ids.emplace(r_id);
	}
	//total update
	for(auto r_id : rule_ids){
		auto act = evalActivity(env.getRules()[r_id]);
		activityTree->add(r_id,act.first+act.second);
	}
	counter.incNullActions(ev->warns);
	delete ev;
	return dt;

}

void State::initInjections() {
	if(injections)
		delete[] injections;
	injections = new matching::InjRandSet[env.size<pattern::Mixture::Component>()];
	for(auto node_p : graph){
		unsigned i = 0;
		for(auto& comp : env.getComponents()){
			two<std::list<Node::Internal*> > port_lists;
			try{
				matching::Injection* inj_p = injections[i].emplace(comp,*node_p,port_lists);
				for(auto port : port_lists.first)
					port->deps.first->emplace(inj_p);
				for(auto port : port_lists.second)
					port->deps.second->emplace(inj_p);
				if(!port_lists.first.size() && !port_lists.second.size())
					node_p->addDep(inj_p);
				//cout << "matching Node " << node_p->toString(env) << " with CC " << comp.toString(env) << endl;
			}
			catch(False& e){

			}
			i++;
		}
	}
}
void State::initActTree() {
	if(activityTree)
		delete activityTree;
	activityTree = new data_structs::MaskedBinaryRandomTree<stack>(env.size<simulation::Rule>(),randGen);
	unsigned i = 0;
	for(auto& rule : env.getRules()){
		auto act_pr = evalActivity(rule);
		activityTree->add(i,act_pr.first+act_pr.second);
		i++;
	}
}


void State::print() const {
	cout << "state with {SiteGraph.size() = " << graph.getPopulation();
	cout << "\n\tvolume = " << volume.getValue().valueAs<float>();
	cout << "\n\tInjections {\n";
	int i = 0;
	for(auto& cc : env.getComponents()){
		if(injections[i].count())
			cout << "\t\t" << injections[i].count() << " injs of " << cc.toString(env) << endl;
		i++;
	}
	cout << "\t}\n}" << endl;
	cout << counter.toString() << endl;
	cout << graph.toString(env) << endl;
}

} /* namespace ast */
