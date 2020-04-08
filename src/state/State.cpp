/*
 * State.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: naxo
 */

#include "State.h"
#include "../pattern/Environment.h"
#include "../matching/Injection.h"
#include "../matching/InjRandSet.h"
#include "../data_structs/MyMaskedBinaryRandomTree.h"
#include "../simulation/Plot.h"

#include "../pattern/mixture/Component.h"
#include "../pattern/mixture/Agent.h"
#include <cmath>

namespace state {
using Deps = pattern::Dependency::Dep;


void (State::*State::action[6])(const simulation::Rule::Action&) =
		{&State::modify,&State::bind,&State::free,&State::del,nullptr,&State::assign};

State::State(const simulation::Simulation& _sim,
		const std::vector<Variable*>& _vars,
		const BaseExpression& vol,simulation::Plot& _plot,
		const pattern::Environment& _env,int seed) :
	sim(_sim),env(_env),volume(vol),vars(_vars.size()),rates(env.size<simulation::Rule>()),
	tokens (new float[env.size<state::TokenVar>()]()),activityTree(nullptr),
	injections(nullptr),randGen(seed),
	plot(_plot),activeDeps(env.getDependencies()){
	for(unsigned i = 0; i < _vars.size(); i++){
		vars[i] = dynamic_cast<Variable*>(_vars[i]->clone());
		vars[i]->reduce(vars);//vars are simplified for every state
	}
	for(auto& rule : env.getRules()){
		int i = rule.getId();
		auto& lhs = rule.getLHS();
		auto lhs_comps = lhs.compsCount();
		bool same_ptrn = false;
		for(int cc_id = 0; cc_id < int(lhs_comps)-1; cc_id++)
			if(&(lhs.getComponent(cc_id)) != &(lhs.getComponent(cc_id+1)) )
				{same_ptrn = false; break;}
			else
				same_ptrn = true;

		if(rule.getRate().getVarDeps() & BaseExpression::AUX){
			if(same_ptrn){
				try{
					rates[i] = new simulation::SameAuxDepRate(rule,*this,false);
				}
				catch(invalid_argument& e){
					ADD_WARN_NOLOC("Same Pattern but not same pattern for rates?");
					rates[i] = new simulation::AuxDepRate(rule,*this);
				}
			}
			else
				rates[i] = new simulation::AuxDepRate(rule,*this);
		}
		else
			if(same_ptrn && lhs.size() > 1)
				rates[i] = new simulation::SamePtrnRate(rule,*this,false);
			else
				rates[i] = new simulation::NormalRate(rule,*this);
	}
	for(auto pert : env.getPerts()){
		perts.emplace(pert->getId(),*pert);
	}
}

//const State empty = State(0,std::vector<Variable*>(),new Constant<float>(1.0),
//		simulation::Plot(),pattern::Environment::empty)

const pattern::Environment& State::getEnv() const{
	return env;
}

const simulation::Simulation& State::getSim() const {
	return sim;
}


State::~State() {
	delete[] tokens;
	if(activityTree)
		delete activityTree;
	if(injections){
		for(unsigned i = 0; i < env.size<pattern::Mixture::Component>(); i++)
			delete injections[i];
		delete[] injections;
	}
	//test
	activityTree = nullptr;
	injections = nullptr;
	for(auto rate : rates)
		delete rate;
	for(auto it = vars.rbegin(); it != vars.rend(); it++)
		delete *it;//vars are deleted in main!
}


const simulation::Counter& State::getCounter() const {
	return counter;
}

void State::addTokens(float n,unsigned tok_id){
	tokens[tok_id] += n;
}
float State::getTokenValue(unsigned tok_id) const{
	return tokens[tok_id];
}

const simulation::Rule::Rate& State::getRuleRate(int _id) const {
	return *(rates[_id]);
}

SomeValue State::getVarValue(short_id var_id, const AuxMap& aux_values) const {
	return vars[var_id]->getValue(*this, aux_values);
}

FL_TYPE State::getTotalActivity() const {
	return activityTree->total();
}

default_random_engine& State::getRandomGenerator() const{
	return randGen;
}

const matching::InjRandContainer& State::getInjContainer(int cc_id) const{
	return *(injections[cc_id]);
}
matching::InjRandContainer& State::getInjContainer(int cc_id) {
	return *(injections[cc_id]);
}


void State::initNodes(unsigned n,const pattern::Mixture& mix){
	for(auto comp_p : mix){
		graph.addComponents(n,*comp_p,*this);
	}
}

void State::addNodes(unsigned n,const pattern::Mixture& mix){
	for(auto comp_p : mix)
		graph.addComponents(n,*comp_p,*this,move(ev.fresh_emb));
}

UINT_TYPE State::mixInstances(const pattern::Mixture& mix) const{
	//auto& mix = env.getMixtures().front();
	UINT_TYPE  count = 1;
	for(const auto cc : mix){
		count *= injections[cc->getId()]->count();
	}
	return count;
}

/*two<FL_TYPE> State::evalActivity(const simulation::Rule& r) const{
	//two<FL_TYPE> act;
	FL_TYPE a = 1.0;
	auto& lhs = r.getLHS();
	for(unsigned i = 0 ; i < lhs.compsCount() ; i++){
		auto& cc = lhs.getComponent(i);
		injections[cc.getId()]->selectRule(r.getId(), i);
		a *= injections[cc.getId()]->partialReactivity();
	}
	auto& rate = rates[r.getId()];
	if(rate.base.aux_functions.empty())
		a *= rate.baseRate->getValue(*this).valueAs<FL_TYPE>();
	else
		a *= rate.base.factor->getValue(*this).valueAs<FL_TYPE>();
	//Adjust rate cause reactants cc-injs intersection
	//a = rate.correct(a)
	return make_pair(a,0.0);
}*/
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

void State::bind(const simulation::Rule::Action& a){
	using Action = simulation::Rule::Action;
	auto n1 = (get<3>(a.trgt1) & Action::NEW ? ev.fresh_emb : ev.emb[get<0>(a.trgt1)])[get<1>(a.trgt1)];
	auto n2 = (get<3>(a.trgt2) & Action::NEW ? ev.fresh_emb : ev.emb[get<0>(a.trgt2)])[get<1>(a.trgt2)];
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

	n1->bind(ev,injections,get<2>(a.trgt1),n2,get<2>(a.trgt2),get<3>(a.trgt2) & Action::S_EFF);
}

void State::free(const simulation::Rule::Action& a){
	using Action = simulation::Rule::Action;
	auto node = ev.emb[get<0>(a.trgt1)][get<1>(a.trgt1)];
	try{
		auto new_node = ev.new_cc.at(node);
		if(new_node)
			new_node->unbind(ev,injections,get<2>(a.trgt1));
	}
	catch(std::out_of_range &ex){}
	node->unbind(ev,injections,get<2>(a.trgt1),get<3>(a.trgt1) & Action::S_EFF);
}

void State::modify(const simulation::Rule::Action& a){
	auto node = ev.emb[get<0>(a.trgt1)][get<1>(a.trgt1)];
	try{
		auto new_node = ev.new_cc.at(node);
		if(new_node)
			new_node->changeIntState(ev,injections,get<2>(a.trgt1),get<0>(a.trgt2));
	}
	catch(std::out_of_range &ex){}
	node->changeIntState(ev,injections,get<2>(a.trgt1),get<0>(a.trgt2));
}

void State::assign(const simulation::Rule::Action& a){
	using Action = simulation::Rule::Action;
	auto node = (get<3>(a.trgt1) & Action::NEW ? ev.fresh_emb : ev.emb[get<0>(a.trgt1)])[get<1>(a.trgt1)];
	auto val = a.value->getValue(*this,move(ev.aux_map));//TODO aux_values
	try{
		auto new_node = ev.new_cc.at(node);
		if(new_node)
			new_node->assign(ev,injections,get<2>(a.trgt1),val);
	}
	catch(std::out_of_range &ex){}
	node->assign(ev,injections,get<2>(a.trgt1),val);
}

void State::del(const simulation::Rule::Action& a){
	auto node = ev.emb[get<0>(a.trgt1)][get<1>(a.trgt1)];
	node->removeFrom(ev,injections,graph);
}

void State::del(Node* node){
	node->removeFrom(ev,injections,graph);
}

void State::apply(const simulation::Rule& r){
	//ADD action first
	//ev.fresh_emb = new Node*[r.getNewNodes().size()];//maybe not!

	//set aux vars from emb TODO only if assing??
	for(auto& aux : r.getLHS().getAux()){
		ev.aux_map[aux.first] =
				ev.emb[get<0>(aux.second)][get<1>(aux.second)]->
				getInternalState(get<2>(aux.second)).valueAs<FL_TYPE>();
	}

	int i = 0;
	for(auto n : r.getNewNodes()){
		auto node = new Node(*n,ev.new_cc);
		ev.new_cc[n] = node;
		graph.allocate(node);
		ev.fresh_emb.push_back(node);
		i++;
	}
	ev.new_cc.clear();
	for(auto& act : r.getScript()){
		(this->*(action)[act.t])(act);
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
			node_pair.second->copyDeps(*node_pair.first,ev,injections,*this);
			graph.allocate(node_pair.second);
		}
	}
	//apply token changes
	for(auto& change : r.getTokenChanges())
		tokens[change.first] += change.second->getValue(*this,move(ev.aux_map)).valueAs<FL_TYPE>();
}

void State::positiveUpdate(const simulation::Rule::CandidateMap& wake_up){
	//TODO vars_to_wake_up
	//auto& wake_up = r.getInfluences();
	for(auto& cc_info : wake_up){
		auto cc = cc_info.first;
		auto& info = cc_info.second;
		Node* node;
		for(auto cc_ag_root : info.node_id){
			if(cc_ag_root.first.first == small_id(-1))//new
				node = ev.fresh_emb[cc_ag_root.first.second];
			else
				try {
					node = ev.emb[cc_ag_root.first.first][cc_ag_root.first.second];
					node = ev.new_cc.at(node);
				}catch(out_of_range &ex) {}//not in new_cc
			//if(cc->getAgent(cc_ag_root.second).getId() != node->getId())
			//	continue;
			/*bool b=true;//TODO this is WROOOOONGGGG!!
			for(auto elem : info.infl_by)
				if(ev.null_actions.count(make_pair(static_cast<const Node*>(node),elem)) == 0){
					b=false;break;
				}
			if(b)
				continue;*/
			two<std::set<Internal*> > port_lists;
			auto inj_p = injections[cc->getId()]->emplace(*node,port_lists,*this,cc_ag_root.second);

			if(inj_p){
				if(port_lists.first.empty() && port_lists.second.empty())
					node->addDep(inj_p);
				else{
					for(auto port : port_lists.first)
						port->deps.first->emplace(inj_p);
					for(auto port : port_lists.second)
						port->deps.second->emplace(inj_p);
				}
				//cout << "matching Node " << node_p->toString(env) << " with CC " << comp.toString(env) << endl;
				ev.to_update.emplace(cc);
				updateDeps(pattern::Dependency(Deps::KAPPA,cc->getId()));

			}
		}
	}
	for(auto side_eff : ev.side_effects){//trying to create injection for side-effects
		for(auto& cc_ag : env.getFreeSiteCC(side_eff.first->getId(),side_eff.second)){
			two<std::set<Internal*> > port_lists;
			auto inj_p = injections[cc_ag.first->getId()]->emplace(*side_eff.first,port_lists,*this,cc_ag.second);
			if(inj_p){
				if(port_lists.first.empty() && port_lists.second.empty())
					side_eff.first->addDep(inj_p);
				else{
					for(auto port : port_lists.first)
						port->deps.first->emplace(inj_p);
					for(auto port : port_lists.second)
						port->deps.second->emplace(inj_p);
				}
				ev.to_update.emplace(cc_ag.first);
			}
		}
	}

	//prepare negative_update
	//set<small_id> rule_ids;
	for(auto cc : ev.to_update){
		for(auto r_id : cc->includedIn())
			ev.rule_ids.emplace(r_id);
	}
	//total update
	//cout << "rules to update: ";
	for(auto r_id : ev.rule_ids){
		//cout << env.getRules()[r_id].getName() << ", ";
		auto act = rates[r_id]->evalActivity(*this);
		activityTree->add(r_id,act.first+act.second);
	}
	//cout << endl;
	counter.incNullActions(ev.warns);

	ev.clear();
}

void State::advanceUntil(FL_TYPE sync_t) {
	counter.next_sync_at = sync_t;
	plot.fill(*this,env);
	while(counter.getTime() < sync_t){
		try{
			const NullEvent ex(event());
			if(ex.error){
				counter.incNullEvents(ex.error);
				#ifdef DEBUG
					cout << "\tnull-event (" << ex.what() << ")" << endl;
				#endif
			}
			else
				counter.incEvents();
		}
		catch(const NullEvent &ex){
			#ifdef DEBUG
				cout << "\tnull-event (" << ex.what() << ")" << endl;
			#endif
			counter.incNullEvents(ex.error);
		}
		WarningStack::getStack().show(false);
		plot.fill(*this,env);
	}
}

void State::selectBinaryInj(const simulation::Rule& r,bool clsh_if_un) const {
	set<Node*> total_cod;
	//map<int,SiteGraph::Node*> total_inj;
	//SiteGraph::Node*** total_inj = new SiteGraph::Node**[mix.compsCount()];
	//ev->emb = new Node**[mix.compsCount()];
	auto& mix = r.getLHS();
	ev.cc_count = mix.compsCount();
	for(unsigned i = 0 ; i < mix.compsCount() ; i++){
		auto& cc = mix.getComponent(i);
		injections[cc.getId()]->selectRule(r.getId(), i);
		auto& inj = injections[cc.getId()]->chooseRandom(randGen);
		try{
			inj.codomain(ev.emb[i],total_cod);
		}
		catch(False& ex){
			throw NullEvent(2);//overlapped codomains
		}
	}
	if(clsh_if_un)
		return;//TODO
	return;
}

void State::selectUnaryInj(const simulation::Rule& mix) const {
	return;
}

void State::selectInjection(const simulation::Rule& r,two<FL_TYPE> bin_act,
		two<FL_TYPE> un_act) {
	//if mix.is empty TODO
	//if mix.unary -> select_binary
	if(std::isinf(bin_act.first)){
		if(std::isinf(un_act.first)){
			auto rd = uniform_int_distribution<int>(1)(randGen);
			if(rd)
				return selectBinaryInj(r,true);
			else
				return selectUnaryInj(r);
		}
		else
			return selectBinaryInj(r,true);
	}
	else{
		if(std::isinf(un_act.first))
			return selectUnaryInj(r);
		else{
			auto rd = uniform_real_distribution<FL_TYPE>(bin_act.first+un_act.first)(randGen);
			if(rd > un_act.first)
				return selectBinaryInj(r,true);
			else
				return selectUnaryInj(r);
		}
	}
	return;
}

const simulation::Rule& State::drawRule(){
	auto rid_alpha = activityTree->chooseRandom();
	auto& rule = env.getRules()[rid_alpha.first];

	auto a1a2 = rates[rid_alpha.first]->evalActivity(*this);
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
	//EventInfo* ev_p;
	selectInjection(rule,make_pair(a1a2.first,radius),
			make_pair(a1a2.second,radius));
	return rule;
}

int State::event() {
	//if(counter.getEvent() == 27511)
	//	cout << "aca!!!"
	//			<< endl;
	FL_TYPE dt,act;
	act = activityTree->total();
	if(act < 0.)
		throw invalid_argument("Activity falls below zero.");
	auto exp_distr = exponential_distribution<FL_TYPE>(act);
	dt = exp_distr(randGen);
	if(act == 0 || std::isinf(dt)){
		counter.advanceTime(dt);
		//plot.fill(*this,env);
		return 0;//TODO
	}

	counter.advanceTime(dt);
	updateDeps(pattern::Dependency(pattern::Dependency::TIME));
	//timePerts = pertIds;
	//pertIds.clear();
	FL_TYPE stop_t = 0.0;
	for(auto& time_dPert : timePerts){//fixed-time-perts
		if(time_dPert.first > counter.getTime())
			break;
		auto p_id = time_dPert.second.id;
		bool abort = false;
		auto& pert = perts.at(p_id);
		stop_t = pert.timeTest(*this);
		//if(stop_t){
			counter.time = (stop_t > 0) ? stop_t : counter.time;
			plot.fillBefore(*this,env);
			pert.apply(*this);
			counter.time = std::nextafter(counter.time,counter.time + 1.0);//TODO inf?
			abort = pert.testAbort(*this,true);
		//}
		//if(!abort)
		//	abort = pert.testAbort(*this,false);
		if(abort)
			perts.erase(p_id);
		else
			activeDeps.addTimePertDependency(p_id, pert.nextStopTime());
	}
	timePerts.clear();
	tryPerturbate();//trying no-fixed-time perturbations

	#ifdef DEBUG
		printf("Event %3lu | Time %.4f \t  act = %.4f",
				counter.getEvent(),counter.getTime(),act);
	#endif
	//EventInfo* ev = nullptr;
	if(stop_t)
		return 6;//NullEvent caused by perturbation
	auto& rule = drawRule();
	plot.fillBefore(*this,env);

	#ifdef DEBUG
		printf( "  | Rule: %-11.11s",rule.getName().c_str());
		cout << "  Root-node: ";
		for(int i = 0; i < ev.cc_count; i++)
			cout << ev.emb[i][0]->getAddress() << ",";
		cout << endl;
		//printf("  Root-node: %03lu\n",(ev->cc_count ?
		//		ev->emb[0][0]->getAddress() : -1L));
	#endif
	apply(rule);
	positiveUpdate(rule.getInfluences());

	/*list<const simulation::Rule*> rules;
	for(auto& rule : env.getRules())
			rules.emplace_back(&rule);
	rules.sort([](const simulation::Rule * a, const simulation::Rule* b) { return a->getName() < b->getName(); });
	for(auto rule_p : rules){
			auto& rule = *rule_p;
			auto act_pr = rates[rule.getId()]->evalActivity(*this);
			//activityTree->add(rule.getId(),act_pr.first+act_pr.second);
	#ifdef DEBUG
			printf("\t%s\t%.6f\n", rule.getName().c_str(),(act_pr.first+act_pr.second));
	#endif
	}*/
	return 0;

}

void State::tryPerturbate() {
	while(pertIds.size()){
		auto curr_perts = pertIds;
		pertIds.clear();
		for(auto p_id : curr_perts){
			bool abort = false;
			auto& pert = perts.at(p_id);
			auto trigger = pert.test(*this);
			if(trigger){
				pert.apply(*this);
				abort = pert.testAbort(*this,true);
			}
			if(!abort)
				abort = pert.testAbort(*this,false);
			if(abort)
				perts.erase(p_id);

		}
	}
}

void State::updateVar(const Variable& var,bool by_value){
	//delete vars[var.getId()];
	AuxNames aux_map;//TODO delete all this method
	if(by_value)
		vars[var.getId()]->update(var.getValue(*this,aux_map));
	else
		vars[var.getId()]->update(var);
	updateDeps(pattern::Dependency(Deps::VAR,var.getId()));
}

void State::updateDeps(const pattern::Dependency& d){
	pattern::DepSet deps;
	deps.emplace(d);
	pattern::Dependency last_dep;
	auto dep_it = deps.begin();
	while(!deps.empty()){
		//auto& dep = *deps.begin();
		switch(dep_it->type){
		case Deps::RULE:
			ev.rule_ids.emplace(dep_it->id);
			break;
		case Deps::KAPPA:
			break;
			//auto act = evalActivity(env.getRules()[dep.id]);
			//activityTree->add(dep.id,act.first+act.second);
		case Deps::VAR:
			break;
		case Deps::PERT:
			if(perts.count(dep_it->id))
				pertIds.push_back(dep_it->id);
			else //pert was aborted
				activeDeps.erase(*dep_it,last_dep);
			break;
		case Deps::TIME:{
			auto& deps = activeDeps.getDependencies(*dep_it).ordered_deps;
			if(deps.size() && deps.begin()->first <= counter.time ){
				auto nextDeps = deps.equal_range(deps.begin()->first);
				timePerts.insert(timePerts.begin(),nextDeps.first,nextDeps.second);
				activeDeps.eraseTimePerts(nextDeps.first,nextDeps.second);
			}
			}break;
		}
		auto more_deps = activeDeps.getDependencies(*dep_it).deps;
		deps.insert(more_deps.begin(),more_deps.end());
		last_dep = *dep_it;
		dep_it = deps.erase(dep_it);
	}
}

void State::initInjections() {
	if(injections){
		for(unsigned i = 0; i < env.size<pattern::Mixture::Component>(); i++)
			delete injections[i];
		delete[] injections;
	}
	injections = (matching::InjRandContainer**)(new matching::InjRandContainer*[env.size<pattern::Mixture::Component>()]);
	for(auto& cc : env.getComponents())
		if(cc.getRateDeps().size())
			injections[cc.getId()] = new matching::InjRandTree(cc,rates);
		else
			injections[cc.getId()] = new matching::InjRandSet(cc);
	for(auto node_p : graph){
		//map<const Node*,bool> visits;
		//cout << node_p->toString(env,true,&visits) << endl;
		for(auto& comp : env.getComponents()){
			if(comp.getAgent(0).getId() != node_p->getId())//very little speed-up
				continue;
			//cout << comp.toString(env) << endl;
			two<std::set<Internal*> > port_lists;
			auto inj_p = injections[comp.getId()]->emplace(*node_p,port_lists,*this);
			if(inj_p){
				for(auto port : port_lists.first)
					port->deps.first->emplace(inj_p);
				for(auto port : port_lists.second)
					port->deps.second->emplace(inj_p);
				if(port_lists.first.empty() && port_lists.second.empty())
					node_p->addDep(inj_p);
			}
		}
	}
}

void State::initActTree() {
	if(activityTree)
		delete activityTree;
	activityTree = new data_structs::MyMaskedBinaryRandomTree<stack>(env.size<simulation::Rule>(),randGen);
#ifdef DEBUG
	cout << "[Initial activity tree]" << endl;
#endif
	list<const simulation::Rule*> rules;
	for(auto& rule : env.getRules())
		rules.emplace_back(&rule);
	rules.sort([](const simulation::Rule * a, const simulation::Rule* b) { return a->getName() < b->getName(); });
	for(auto rule_p : rules){
		auto& rule = *rule_p;
		auto act_pr = rates[rule.getId()]->evalActivity(*this);
		activityTree->add(rule.getId(),act_pr.first+act_pr.second);
#ifdef DEBUG
		printf("\t%s\t%.6f\n", rule.getName().c_str(),(act_pr.first+act_pr.second));
#endif
	}
}


void State::print() const {
	cout << "state with {SiteGraph.size() = " << graph.getPopulation();
	//cout << "\n\tvolume = " << volume.getValue().valueAs<FL_TYPE>();
	cout << "\n\tInjections {\n";
	int i = 0;
	for(auto& cc : env.getComponents()){
		if(injections[i]->count())
			cout << "\t("<< i <<")\t" << injections[i]->count() <<
			" injs of " << cc.toString(env) << endl;
		i++;
	}
	cout << "\t}\n\tRules {\n";
	for(auto& r : env.getRules()){
		auto act = rates[r.getId()]->evalActivity(*this);
		cout << "\t(" << r.getId() << ")\t" << r.getName() << "\t("
			<< act.first << " , " << act.second << ")" << endl;
	}
	cout << "\t}\n}\n";
	cout << counter.toString() << endl;
	cout << graph.toString(env) << endl;
}

} /* namespace ast */
