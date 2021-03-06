/*
 * Rule.cpp
 *
 *  Created on: May 9, 2017
 *      Author: naxo
 */

#include <cstring>
#include "../util/Exceptions.h"
#include "../util/Warning.h"
#include "Rule.h"
#include "../pattern/Environment.h"
#include "../grammar/ast/Basics.h"
#include "../expressions/Vars.h"
#include "../pattern/mixture/Agent.h"
#include "../pattern/mixture/Component.h"
#include "../state/State.h"
#include "../matching/InjRandSet.h"

namespace simulation {

Rule::Rule(int _id,const string& nme, const Mixture& mix,const yy::location& _loc) : id(_id),name(nme),loc(_loc),
		lhs(mix),rhs(nullptr),rate(nullptr),isRhsDeclared(false){}

Rule::~Rule() {
	if(!isRhsDeclared)
		delete rhs;
	if (!dynamic_cast<const state::Variable*>(rate))
		delete rate;
	for(auto node : newNodes)
		delete node;
}


int Rule::getId() const{
	return id;
}

const string& Rule::getName() const {
	return name;
}

const pattern::Mixture& Rule::getLHS() const {
	return lhs;
}

const BaseExpression& Rule::getRate() const {
	return *rate;
}
/*const BaseExpression::Reduction& Rule::getReduction() const {
	return basic;
}
const BaseExpression::Reduction& Rule::getUnaryReduction() const {
	return unary;
}*/

void Rule::setRHS(const Mixture* mix,bool is_declared){
	rhs = mix;
	isRhsDeclared = is_declared;
}
void Rule::setRate(const BaseExpression* r){
	if(rate)
		delete rate;
	rate = r;
	//if(r->getVarDeps() & BaseExpression::AUX)
	//	basic = r->factorize();
}
void Rule::setUnaryRate(pair<const BaseExpression*,int> u_rate ){
	unaryRate = u_rate;
	//if(u_rate.first->getVarDeps() & BaseExpression::AUX)
	//	unary = u_rate.first->factorize();
}


const list<pair<unsigned,const BaseExpression*> > Rule::getTokenChanges() const{
	return tokenChanges;
}
void Rule::addTokenChange(pair<unsigned,const BaseExpression*> tok) {
	tokenChanges.emplace_back(tok);
}



/** Rule::difference()
 *
 * translate difference between LHS and RHS to Actions
 * Also produce Changes to calculate candidates for influence map
 *
 *
 */
void Rule::difference(const Environment& env, const vector<ag_st_id>& lhs_unmask,
		const vector<ag_st_id>& rhs_unmask,const VarVector& consts){
	small_id i;
	unsigned first_del = 9999;
	list<Action> binds;
	map<ag_st_id,small_id> lhs_mask,rhs_mask;
	for(unsigned i = 0; i<rhs_unmask.size() ; i++)
		rhs_mask[rhs_unmask[i]] = i;
	for(unsigned i = 0; i<lhs_unmask.size() ; i++)
		lhs_mask[lhs_unmask[i]] = i;

	//modify nodes
	string s_i;
	for(i = 0; i < lhs_unmask.size(); i++){
		auto& lhs_ag = lhs.getAgent(lhs_unmask[i]);
		s_i = "("+to_string(i)+")";
		try{
			if(i >= rhs_unmask.size()) { // fixbug: when the right mixture have less agents than left
				first_del = i;
				break;
			}

			auto& rhs_ag = rhs->getAgent(rhs_unmask[i]);
			if(lhs_ag.getId() != rhs_ag.getId()){
				first_del = i;
				break;
			}
			//match -> modify
			matches[rhs_unmask[i]] = lhs_unmask[i];

			auto& sign = env.getSignature(lhs_ag.getId());
			small_id j = 0;
			for(auto psite : sign.getSites()){
				Action a;
				ag_st_id lnk1,lnk2,buff;
				small_id i2;
				auto& lhs_site = lhs_ag.getSiteSafe(j);
				auto& rhs_site = rhs_ag.getSiteSafe(j);
				try{
					lhs_ag.getSite(j);
				}catch(std::out_of_range &e){
					if(rhs_site.link_type != Mixture::WILD)
						throw SemanticError("The link status of agent "+sign.getName()+s_i+", site "+
							sign.getSite(j).getName()+" on the left side must be specified specified to change.",loc);
				}
				try{
					rhs_ag.getSite(j);
				}catch(std::out_of_range &e){
					if(!lhs_site.isEmptySite() || lhs_site.link_type != Mixture::WILD)
						ADD_WARN("The internal/lnk state of the site "+psite->getName()+
							" in agent "+sign.getName()+s_i+" should be specified in the right side.",loc);
					j++;
					continue;
					/*if(lhs_site.link_type >)
						throw SemanticError("The state of site "+ sign.getSite(j).getName()+
							" in agent "+sign.getName()+" is underspecified on the right side.",loc);*/
				}

				if(lhs_site.isEmptySite() && !rhs_site.isEmptySite()){
					ADD_WARN("Application of rule '"+name+
							"' will induce a null event when applied to an agent "+sign.getName()+
							s_i+" that have the same value in site "+psite->getName(),loc);
				}
				if(rhs_site.values[1]){
					auto aux_exp = dynamic_cast<const Auxiliar<FL_TYPE>*>(rhs_site.values[1]);
					auto cc_ag_st_aux = make_tuple(lhs_unmask[i].first,lhs_unmask[i].second,j);
					if(!aux_exp || ( aux_exp && lhs.getAux().at(aux_exp->toString()) != cc_ag_st_aux ) ){
						a.t = ASSIGN;
						a.trgt1 = make_tuple(lhs_unmask[i].first,lhs_unmask[i].second,j,false);
						a.value = rhs_site.values[1];
						//TODO warning when lhs == nullptr ?
						script.emplace_back(a);
						changes[make_pair(rhs_unmask[i],false)].first.emplace_back(j);
					}
				}
				else if(!rhs_site.isEmptySite() && lhs_site.label != rhs_site.label){//TODO other type of value changes
					a.t = CHANGE;a.trgt1 = make_tuple(lhs_unmask[i].first,lhs_unmask[i].second,j,false);
					get<0>(a.trgt2) = rhs_site.label;
					script.emplace_back(a);
					changes[make_pair(rhs_unmask[i],false)].first.emplace_back(j);
				}
				else{//expression valued sites

				}
				switch(lhs_site.link_type){
				case Mixture::FREE:				/******* LHS_FREE *******/
					switch(rhs_site.link_type){
					case Mixture::FREE://nothing
						break;
					case Mixture::WILD://warn cause no mention in the rhs
						break;
					case Mixture::BIND_TO://error
					case Mixture::PATH://error?
						throw SemanticError("The link status of agent "+sign.getName()+s_i+", site "+
								sign.getSite(j).getName()+" on the right hand side is underspecified.",loc);
						break;
					case Mixture::BIND://if semi error else connect
						lnk1 = rhs->follow(rhs_unmask[i].first,rhs_unmask[i].second,j);
						if(lnk1.first == rhs_unmask[i].second && lnk1.second == j)
							throw SemanticError("The link status of agent "+sign.getName()+s_i+", site "+
									sign.getSite(j).getName()+" on the right hand side is inconsistent.",loc);
						i2 = rhs_mask.at(make_pair(rhs_unmask[i].first,lnk1.first));
						if(i > i2)//TODO possible error
							break;//link added before.
						else{
							a.t = LINK;a.trgt1 = make_tuple(lhs_unmask[i].first,lhs_unmask[i].second,j,false);
							//auto buff = lhs_unmask[rhs_mask.at(ag_st_id(rhs_unmask[i].first,lnk1.first))];
							a.trgt2 = make_tuple(rhs_unmask[i].first,lnk1.first,lnk1.second,i2);
							binds.emplace_back(a);
						}
						break;
					}
					break;
				case Mixture::WILD:				/******* LHS_WILD *******/
					switch(rhs_site.link_type){
					case Mixture::WILD://nothing
						break;
					case Mixture::FREE://warning
						ADD_WARN("Application of rule '"+name+
									"' will induce a null event when applied to an agent "+sign.getName()+
									s_i+" that is free on site '"+sign.getSite(j).getName()+"'",loc);
						a.t = UNBIND;a.trgt1 = make_tuple(lhs_unmask[i].first,lhs_unmask[i].second,j,Action::S_EFF);
						script.emplace_back(a);
						changes[make_pair(rhs_unmask[i],false)].second.emplace_back(j);
						break;
					case Mixture::BIND_TO://error
					case Mixture::PATH://error
						throw SemanticError("The link status of agent "+sign.getName()+s_i+", site "+
								sign.getSite(j).getName()+" on the right hand side is underspecified",loc);
						break;
					case Mixture::BIND://if semi error else warning
						lnk1 = rhs->follow(rhs_unmask[i].first,rhs_unmask[i].second,j);
						if(lnk1.first == rhs_unmask[i].second && lnk1.second == j)
							throw SemanticError("The link status of agent "+sign.getName()+
									s_i+", site '"+sign.getSite(j).getName()+
									"' on the right hand side is inconsistent",loc);
						i2 = rhs_mask.at(make_pair(rhs_unmask[i].first,lnk1.first));
						if(i > i2)
							break;//link added before.
						else {
							ADD_WARN("Rule '"+name+"': site '"+sign.getSite(j).getName()+
									"' of agent "+sign.getName()+s_i+" is bound in the right hand"+
									"side although it is unspecified in the left hand side",loc);
							a.t = LINK;a.trgt1 = make_tuple(lhs_unmask[i].first,lhs_unmask[i].second,j,false);
							a.trgt2 = make_tuple(rhs_unmask[i].first,lnk1.first,lnk1.second,i2);
							binds.emplace_back(a);
						}
						break;
					}
					break;
				case Mixture::BIND:				/******* LHS_BIND *******/
					switch(rhs_site.link_type){
					case Mixture::WILD://warn cause no mention in the rhs
						break;
					case Mixture::BIND_TO://error
					case Mixture::PATH://error
						throw SemanticError("The link status of agent "+sign.getName()+s_i+", site "+
								sign.getSite(j).getName()+" on the right hand side is underspecified",loc);
						break;
					case Mixture::FREE://if semi warning?? and disconnect else disconnect
						lnk1 = lhs.follow(lhs_unmask[i].first,lhs_unmask[i].second,j);
						if(lnk1.first == lhs_unmask[i].second && lnk1.second == j){
							ADD_WARN("Rule '"+name+"': breaking a semi-link on site '"+
									sign.getSite(j).getName()+"' will induce a side effect",loc);
							get<3>(a.trgt2) = Action::S_EFF;
						}
						else{
							i2 = lhs_mask.at(make_pair(lhs_unmask[i].first,lnk1.first));
							if(i > i2){
								changes[make_pair(rhs_unmask[i],false)].second.emplace_back(j);
								break;//unlink added before.
							}
							a.trgt2 = make_tuple(lhs_unmask[i].first,lnk1.first,lnk1.second,false);
						}
						a.t = UNBIND;a.trgt1 = make_tuple(lhs_unmask[i].first,lhs_unmask[i].second,j,false);
						script.emplace_back(a);
						changes[make_pair(rhs_unmask[i],false)].second.emplace_back(j);
						break;
					case Mixture::BIND://if semi-semi nothing if semi-not warning if not-semi error if not-not warn(swap)
						lnk1 = lhs.follow(lhs_unmask[i].first,lhs_unmask[i].second,j);
						lnk2 = rhs->follow(rhs_unmask[i].first,rhs_unmask[i].second,j);
						if(lnk1.first == lhs_unmask[i].second && lnk1.second == j){
							if(lnk2.first == rhs_unmask[i].second && lnk2.second == j){
								//semi-semi -> nothing
							}
							else {//semi-not
								i2 = rhs_mask.at(make_pair(rhs_unmask[i].first,lnk2.first));
								if(i > i2)
									break;//link added before.
								ADD_WARN("The link state of site "+sign.getSite(j).getName()+
										" of agent "+sign.getName()+
										s_i+" is changed although it is a semi-link in the left hand side",loc);
								a.t = LINK; a.trgt1 = make_tuple(lhs_unmask[i].first,lhs_unmask[i].second,j,Action::S_EFF);
								a.trgt2 = make_tuple(rhs_unmask[i].first,lnk2.first,lnk2.second,i2);
								binds.emplace_back(a);
							}
						}
						else {
							if(lnk2.first == rhs_unmask[i].second && lnk2.second == j)//not-semi
								throw SemanticError("The link status of agent "+sign.getName()+s_i+", site "+
										sign.getSite(j).getName()+" on the right hand side is underspecified",loc);
							else {//not-not
								if(lhs_mask.at(make_pair(lhs_unmask[i].first,lnk1.first)) !=
										rhs_mask.at(make_pair(rhs_unmask[i].first,lnk2.first)) ||
										lnk1.second != lnk2.second){
									i2 = rhs_mask.at(make_pair(rhs_unmask[i].first,lnk2.first));
									ADD_WARN(name+" rule induces a link permutation on site "
											+sign.getSite(j).getName()+" of agent "+sign.getName()+s_i,loc);
									if(i > i2)
										break;//link added before.
									a.t = LINK; a.trgt1 = make_tuple(lhs_unmask[i].first,lhs_unmask[i].second,j,false);
									a.trgt2 = make_tuple(rhs_unmask[i].first,lnk2.first,lnk2.second,i2);
									binds.emplace_back(a);
								}
								//else nothing
							}
						}
						break;
					}
					break;
				case Mixture::BIND_TO:			/******* LHS_BIND_TO *******/
					switch(rhs_site.link_type){
					case Mixture::WILD://warn cause no mention in the rhs
						break;
					case Mixture::PATH://error
						throw SemanticError("The link status of agent "+sign.getName()+s_i+", site "+
								sign.getSite(j).getName()+" on the right hand side is underspecified",loc);
						break;
					case Mixture::FREE://disconnect
						a.t = UNBIND;a.trgt1 = make_tuple(lhs_unmask[i].first,lhs_unmask[i].second,j,Action::S_EFF);
						script.emplace_back(a);
						changes[make_pair(rhs_unmask[i],false)].second.emplace_back(j);
						break;
					case Mixture::BIND://if semi error else warning(swap)
						lnk1 = rhs->follow(rhs_unmask[i].first,rhs_unmask[i].second,j);
						if(lnk1.first == rhs_unmask[i].second && lnk1.second == j)
							throw SemanticError("The link status of agent "+sign.getName()+s_i+", site "+
									sign.getSite(j).getName()+" on the right hand side is underspecified",loc);
						else {
							i2 = rhs_mask.at(make_pair(rhs_unmask[i].first,lnk2.first));
							if(i > i2)
								break;//link added before.
							ADD_WARN("The link state of site "+sign.getSite(j).getName()+
									" in agent "+sign.getName()+s_i+" is changed although it is a semi-link in the left hand side",loc);
							a.t = LINK; a.trgt1 = make_tuple(lhs_unmask[i].first,lhs_unmask[i].second,j,Action::S_EFF);
							a.trgt2 = make_tuple(rhs_unmask[i].first,lnk1.first,lnk1.second,i2);
							binds.emplace_back(a);
							sideEffects.emplace_back(lhs_site.lnk_ptrn);
						}
						break;
					case Mixture::BIND_TO://if different error else nothing
						if(lhs_site.lnk_ptrn != rhs_site.lnk_ptrn)
							throw SemanticError("The link status of agent '%s's_i+, site '%s' on the right hand side is inconsistent",loc);
						//else its OK
						break;
					}
					break;
				case Mixture::PATH:				/******* LHS_PATH *******/
					switch(rhs_site.link_type){
					case Mixture::FREE://disconnect?
						break;
					case Mixture::WILD://error
						break;
					case Mixture::BIND://warning swap?
						break;
					case Mixture::BIND_TO://error
						break;
					case Mixture::PATH://if same nothing else ???
						break;
					}
					break;
				}
				j++;
			}
		}
		catch(std::out_of_range &e){
			first_del = i;
			break;
		}

	}
	//check if there are binds to new nodes
	for(auto& bind : binds){
		auto& k = get<3>(bind.trgt2);
		//auto j = rhs_mask.at(make_pair(get<0>(bind.trgt1),get<1>(bind.trgt1)));//TODO bind of 2 new nodes
		if( k >= i ){
			changes[make_pair(rhs_unmask[k]/*-i? TODO*/,true)].second.emplace_back(get<2>(bind.trgt2));
			//changes[make_pair(rhs_unmask[j],false)].second.emplace_back(get<2>(bind.trgt1));
			get<0>(bind.trgt2) = rhs_unmask[k].first;
			get<1>(bind.trgt2) = k-i;
			get<3>(bind.trgt2) = k-i+1;//+1 to indicate new node TODO
		}
		else{
			changes[make_pair(rhs_unmask[k],false)].second.emplace_back(get<2>(bind.trgt2));
			get<0>(bind.trgt2) = lhs_unmask[k].first;
			get<1>(bind.trgt2) = lhs_unmask[k].second;
			k -= k & Action::NEW;
			//changes[make_pair(rhs_unmask[j],false)].second.emplace_back(get<2>(bind.trgt1));
		}
		script.emplace_back(bind);
	}

	//new nodes
	newNodes.reserve(rhs_unmask.size()-i);
	for(unsigned j = i; j < rhs_unmask.size(); j++){
		auto cc_ag_id = rhs_unmask[j];
		auto& rhs_ag = rhs->getAgent(cc_ag_id);
		auto& sign = env.getSignature(rhs_ag.getId());
		newNodes.emplace_back(new state::Node(env.getSignature(rhs_ag.getId())));
		news.emplace(cc_ag_id,j-i);
		for(auto& s : rhs_ag){
			if(s.second.values[1]){
				if(s.second.values[1]->getVarDeps() <= BaseExpression::CONSTS)
					newNodes[j-i]->setState(s.first,s.second.values[1]->getValue(consts));
				else{
					newNodes[j-i]->setState(s.first,SomeValue());
					Action a;
					a.t = ASSIGN;
					a.trgt1 = make_tuple(cc_ag_id.first,j-i,s.first,true);
					a.value = s.second.values[1];
					//TODO warning when lhs == nullptr ?
					script.emplace_back(a);
					changes[make_pair(make_pair(cc_ag_id.first,j-i),true)].first.emplace_back(s.first);
				}
			}
			else if(s.second.label != Mixture::Site::EMPTY)
				newNodes[j-i]->setState(s.first,s.second.label);

			auto lnk = rhs->follow(cc_ag_id.first,cc_ag_id.second,s.first);
			switch(s.second.link_type){
			case Mixture::FREE:break;
			case Mixture::BIND:{
				auto k =rhs_mask.at(make_pair(cc_ag_id.first,lnk.first));
				if(k > i && k < j){//only bind if both agents new?
					/*Action a;
					a.t = LINK;a.trgt1 = make_tuple(cc_ag_id.first,j-i,s.first,true);
					a.trgt2 = make_tuple(cc_ag_id.first,k-i,lnk.second,true);
					script.emplace_back(a);*/
					newNodes[k-i]->setLink(s.first, newNodes[j-i], lnk.second);
					newNodes[j-i]->setLink(lnk.second, newNodes[k-i], s.first);
				}
			};break;
			default:
				throw SemanticError("The link state of site "+sign.getSite(s.first).getName()+
						" in the new agent "+sign.getName()+" is underspecified.",loc);
			}
		}
	}
	//deleted nodes
	for(unsigned j = first_del; j < lhs_unmask.size(); j++){
		auto& lhs_ag = lhs.getAgent(lhs_unmask[j]);
		Action a;
		a.t = DELETE;a.trgt1 = make_tuple(lhs_unmask[j].first,lhs_unmask[j].second,0,false);
		script.emplace_back(a);
		//deletes.emplace_back(lhs_unmask[j]);
	}

}


const list<Rule::Action>& Rule::getScript() const{
	return script;
}
const vector<state::Node*>& Rule::getNewNodes() const{
	return newNodes;
}




bool Rule::test_linked_agents(list<two<ag_st_id>>& to_test,small_id rhs_cc_id,
		const Mixture::Component& test_cc,multimap<ag_st_id,ag_st_id>& already_done,
		const VarVector& vars,const Environment& env) const {
	map<small_id,small_id> visited;
	//expressions::AuxMap empty_aux;
	auto& rhs_cc = rhs->getComponent(rhs_cc_id);
	while(to_test.size()){//test connected agents of emb
		if(to_test.front().first.second != to_test.front().second.second)
			return false;//not connected to the same site
		auto rhs_ag_id = to_test.front().first.first;
		auto test_ag_id = to_test.front().second.first;
		to_test.pop_front();
		//auto key = make_pair(test_cc.getId(),test_ag_id);
		//auto val = make_pair(rhs_cc_id,rhs_ag_id);
		if(visited.count(test_ag_id))
			if(visited[test_ag_id] == rhs_ag_id)
				continue;//with 'to-test' agents
			else
				return false;//doesn't match with previous match
		else
			visited[test_ag_id] = rhs_ag_id;
		auto& rhs_ag = rhs_cc.getAgent(rhs_ag_id);
		auto& test_ag = test_cc.getAgent(test_ag_id);
		if(news.count(make_pair(rhs_cc_id,rhs_ag_id))){//testing for new node
			auto new_node = newNodes.at((int)news.at(make_pair(rhs_cc_id,rhs_ag_id)));
			for(auto& id_site : test_ag){
				auto& test_site = id_site.second;
				if(!test_site.testValue(new_node->getInternalState(id_site.first), vars))
					return false;
				try{
					auto& rhs_site = rhs_ag.getSite(id_site.first);
					switch(test_site.link_type){
					case Pattern::BIND_TO:
						if(rhs_site.lnk_ptrn != test_site.lnk_ptrn)
							return false;
						break;
					case Pattern::BIND:
						if(rhs_site.link_type == Pattern::BIND){
							auto lnk = rhs_cc.follow(rhs_ag_id,id_site.first);
							to_test.emplace_back(lnk,test_cc.follow(test_ag_id,id_site.first));
						}
						else
							return false;
						break;
					case Pattern::FREE:
						if(rhs_site.link_type != Pattern::FREE)
							return false;
						break;
					case Pattern::PATH:
						ADD_WARN("PATH is not implemented yet",loc);
						break;
					case Pattern::WILD:
						break;
					}

				}
				catch(out_of_range& ex){//test_site must have default values and be free
					if(test_site.link_type > Pattern::WILD)//TODO warning for path
						return false;
				}
			}
		}
		else{//testing for conserved node
			list<small_id> link_sites;
			if(rhs_ag.testEmbed(test_ag,link_sites,vars))
				for(auto site_id : link_sites){
					to_test.emplace_back(rhs_cc.follow(rhs_ag_id, site_id),
							test_cc.follow(test_ag_id, site_id));
				}
			else
				return false;
		}
	}
	for(auto elem : visited)
		already_done.emplace(ag_st_id(test_cc.getId(),elem.first),ag_st_id(rhs_cc_id,elem.second));
	return true;
}


void Rule::addAgentIncludes(CandidateMap &candidates,
			const Pattern::Agent& ag,ag_st_id rhs_cc_ag,int change){
	for(auto& cc_agid : ag.getIncludes()){//add Agent includes to candidates
		candidates[cc_agid.first].setRoot(cc_agid.second,rhs_cc_ag,change);
	}
}


void Rule::checkInfluence(const Environment &env,const VarVector& vars) {
	//TODO changes of BIND_TO -> FREE
	//TODO changes of semi-link -> free ???
	//try{
	//we first check all candidates to avoid do complete evaluation of same CC twice
	CandidateMap candidates;
	for(auto& ag_changes : changes){//for each agent that changes
		//auto& change_cc = rhs->getComponent(ag_changes.first.first.first);
		auto& ag = rhs->getAgent(ag_changes.first.first);
		//addAgentIncludes(candidates,ag,ag_changes.first.first);
		if(ag_changes.first.second){//new node change, possibly assign of non const value
			/*for(auto change : ag_changes.second.first){//for each site_value changes
				for(auto& ag_ptrn : env.getAgentPatterns(ag.getId())){
					//if(ag_ptrn == ag || ag_ptrn.getSite(change).isEmptySite())
					//	continue;//because it's new
					addAgentIncludes(candidates,ag_ptrn,ag_changes.first.first);
				}
			}*/
		}
		else {//change of preserved node
			for(auto change : ag_changes.second.first){//for each site_value changes
				for(auto& ag_ptrn : env.getAgentPatterns(ag.getId())){
					if(ag_ptrn.getSiteSafe(change).isEmptySite())
						continue;
					//if testValue for site_ptrns
					addAgentIncludes(candidates,ag_ptrn,ag_changes.first.first,change);
					/*for(auto& cc_agid : ag_ptrn.getIncludes())//add Agent includes to candidates
						cout << "adding: " << cc_agid.first->toString(env) << " -> ("
							<< int(ag_changes.first.first.first) << "," << int(ag_changes.first.first.second)
							<< ") -> " << int(cc_agid.second) << endl;*/
				}
			}
			for(auto change : ag_changes.second.second){//for each link val/type changes
				//auto& rhs_graph = change_cc.getGraph();
				ag_st_id rhs_lnk;
				for(auto& emb : env.getAgentPatterns(ag.getId())){
					switch(emb.getSiteSafe(change).link_type){
					case Pattern::WILD:break;
					case Pattern::FREE:
						if(ag.getSite(change).link_type == Pattern::FREE)
							addAgentIncludes(candidates,emb,ag_changes.first.first,-change);
						break;
					case Pattern::BIND:
					case Pattern::BIND_TO:
						if(ag.getSite(change).link_type == Pattern::BIND)
							addAgentIncludes(candidates,emb,ag_changes.first.first,-change);
						break;
					default:
						break;
					}
				}
			}
		}
	}
	//small_id i = 0;
	//expressions::AuxMap aux_map;
	for(auto n : news){
		auto& new_ag = rhs->getAgent(n.first);
		for(auto& ag : env.getAgentPatterns(new_ag.getId())){
			bool isEmb = true;
			for(auto& id_site : ag){
				auto& newNode_val = newNodes[/*i*/n.second]->getInternalState(id_site.first);
				if(!id_site.second.testValue(newNode_val,vars))
					isEmb=false;

				try{//site is declared in new agent
					auto& site = new_ag.getSite(id_site.first);
					switch(id_site.second.link_type){
					case Pattern::FREE:
						if(site.link_type != Pattern::FREE)
							isEmb=false;
						break;
					case Pattern::BIND:
					case Pattern::BIND_TO:
						if(site.link_type != Pattern::BIND)
							isEmb=false;
						break;
					default://WILD,PATH
						break;
					}
				}
				catch(std::out_of_range &ex){//site not declared in new agent
					if(id_site.second.link_type != Pattern::FREE)
						isEmb = false;
				}
				if(!isEmb)
					break;
			}
			if(isEmb)
				addAgentIncludes(candidates,ag,n.first,999);
		}
		//i++;
	}
	//Checking candidates
	multimap<ag_st_id,ag_st_id> already_done;
	//cout << "testing if rule " << this->getName() << " have influence on candidates CC:" << endl;
	for(auto& cc_info : candidates){
		//cout << "\t" << cc_info.first->toString(env);
		for(auto& info : cc_info.second.node_id){
			bool rep = false;
			auto key = make_pair(cc_info.first->getId(),info.second);
			auto range = already_done.equal_range(key);
			for(auto val_it = range.first;val_it != range.second; val_it++)
				if( val_it->second == info.first){
					rep = true;cout << " | DONE!";continue;
				}
			if(rep)
				continue;
			list<two<ag_st_id> > to_test;
			to_test.emplace_back(ag_st_id(info.first.second,0),ag_st_id(info.second,0));
			if(test_linked_agents(to_test,info.first.first,*cc_info.first,already_done,vars,env)){
				//cout << " | YES!";
				try{
					influence[cc_info.first].setRoot(info.second,ag_st_id(-1,news.at(info.first)),0);
					influence[cc_info.first].infl_by = cc_info.second.infl_by;
				} catch(out_of_range &ex){
					influence[cc_info.first].setRoot(info.second,matches.at(info.first),0);
					influence[cc_info.first].infl_by = cc_info.second.infl_by;
				}
			}
			//else cout << " | NO!";
		}
		//cout << endl;
	}

	/*}//end testing try TODO
	catch(exception &ex){
		cout << "cannot check influences for " << this->getName() << endl;
		throw ex;
	}*/
}

const Rule::CandidateMap& Rule::getInfluences() const{
	return influence;
}

/*void Rule::initialize(const state::State& state,VarVector& vars){
	if(rate)


	checkInfluence(state);
}*/



/**** DEBUG ****/
string Rule::toString(const pattern::Environment& env) const {
	static string acts[] = {"CHANGE","BIND","FREE","DELETE","CREATE","ASSIGN","TRANSPORT"};
	string s = name+"'s actions:\n";
	for(auto nn : newNodes){
		s += "\tINSERT agent "+nn->toString(env)+"\n";
	}
	const pattern::Signature *sign1,*sign2;
	for(auto act : script){
		s += "\t";
		switch(act.t){
		case DELETE:
			s += acts[act.t] + " agent "+lhs.getAgent(get<0>(act.trgt1),get<1>(act.trgt1)).toString(env)+"\n";
			break;
		case TRANSPORT:
			break;
		case CHANGE:
			sign1 = &env.getSignature(lhs.getAgent(get<0>(act.trgt1),get<1>(act.trgt1)).getId());
			s += acts[act.t] + " agent's site "+sign1->getName()+"."+sign1->getSite(get<2>(act.trgt1)).getName()+
					" to value ";
			s += dynamic_cast<const pattern::Signature::LabelSite&>(sign1->getSite(get<2>(act.trgt1)))
					.getLabel(get<0>(act.trgt2))+"\n";
			break;
		case LINK:
			s += acts[act.t] + " agent's sites ";
			if(get<3>(act.trgt1) & Action::NEW){//new node
				sign1 = &env.getSignature(newNodes[get<1>(act.trgt1)]->getId());
				s += "(new) " + sign1->getName()+"."+sign1->getSite(get<2>(act.trgt1)).getName()+" and ";
			}
			else{
				sign1 = &env.getSignature(lhs.getAgent(get<0>(act.trgt1),get<1>(act.trgt1)).getId());
				s += sign1->getName()+"."+sign1->getSite(get<2>(act.trgt1)).getName()+" and ";
			}
			if(get<3>(act.trgt2) & Action::NEW){//new node
				sign2 = &env.getSignature(newNodes[get<1>(act.trgt2)]->getId());
				s += "(new) " + sign2->getName()+"."+sign2->getSite(get<2>(act.trgt2)).getName()+"\n";
			}
			else{
				sign2 = &env.getSignature(lhs.getAgent(get<0>(act.trgt2),get<1>(act.trgt2)).getId());
				s += sign2->getName()+"."+sign2->getSite(get<2>(act.trgt2)).getName()+"\n";
			}
			break;
		case UNBIND:
			sign1 = &env.getSignature(lhs.getAgent(get<0>(act.trgt1),get<1>(act.trgt1)).getId());
			sign2 = &env.getSignature(lhs.getAgent(get<0>(act.trgt2),get<1>(act.trgt2)).getId());
			s += acts[act.t] + " agent's sites "+sign1->getName()+"."+sign1->getSite(get<2>(act.trgt1)).getName()+
				" and "+sign2->getName()+"."+sign2->getSite(get<2>(act.trgt2)).getName()+
				(get<3>(act.trgt1) & Action::S_EFF ? " (" : " (no ")+ "side effects)\n";
			break;
		case ASSIGN:
			if(get<3>(act.trgt1) & Action::NEW){//new node
				sign1 = &env.getSignature(newNodes[get<1>(act.trgt1)]->getId());
				s += acts[act.t] + " new-agent's site ";
			}else{
				sign1 = &env.getSignature(lhs.getAgent(get<0>(act.trgt1),get<1>(act.trgt1)).getId());
				s += acts[act.t] + " agent's site ";
			}
			s += sign1->getName()+"."+sign1->getSite(get<2>(act.trgt1)).getName()+
					" to expression "+act.value->toString() + "\n";
			break;
		default: break;
		}
	}
	return s;
}

/*
two<FL_TYPE> Rule::evalActivity(const matching::InjRandContainer* const * injs,
		const VarVector& vars) const{
	//auto& auxs = lhs.getAux();
	FL_TYPE a = 1.0;
	for(unsigned i = 0 ; i < lhs.compsCount() ; i++){
		auto& cc = lhs.getComponent(i);
		injs[cc.getId()]->selectRule(id, i);
		a *= injs[cc.getId()]->partialReactivity();
	}
	if(basic.aux_functions.empty())
		a *= rate->getValue(vars).valueAs<FL_TYPE>();
	else
		for(auto factor : basic.factors)
			a *= factor->getValue(vars).valueAs<FL_TYPE>();
	return make_pair(a,0.0);
}
*/

Rule::CandidateInfo::CandidateInfo() : is_valid(true),is_new(false) {};
void Rule::CandidateInfo::setRoot(small_id root,ag_st_id node,int change){
	//node_id[node.first] = make_pair(node.second,root);
	node_id[node] = root;
	infl_by.emplace(change);
}




/********** Rule::Rate ****************/


Rule::Rate::Rate(const Rule& r,state::State& state) :
		rule(r),baseRate(nullptr),unaryRate(nullptr,-1) {
	auto new_rate = rule.getRate().clone();
	baseRate = new_rate->reduce(state.vars);//rates also have to be reduced for every state
	if(new_rate != baseRate)//reduce can return the same pointer on expression but no with vars.
		delete new_rate;
}

const BaseExpression* Rule::Rate::getExpression(small_id cc_index) const {
	return baseRate;
}


NormalRate::NormalRate(const Rule& r,state::State& state) : Rule::Rate(r,state) {}

SamePtrnRate::SamePtrnRate(const Rule& r,state::State& state,bool normalize) :
		Rule::Rate(r,state),norm(1.0){
	if(normalize)
		for(auto i = r.getLHS().size(); i > 1; i--)
			norm *= i;
}

AuxDepRate::AuxDepRate(const Rule& r,state::State& state) :
		Rule::Rate(r,state){
	map<string,small_id> aux_ccindex;
	for(auto& aux_cc__ : rule.getLHS().getAux())
		aux_ccindex[aux_cc__.first] = get<0>(aux_cc__.second);
	base = baseRate->reduceAndFactorize(aux_ccindex);
}

const BaseExpression* AuxDepRate::getExpression(small_id cc_index) const {
	return base.aux_functions.at(cc_index);
}

SameAuxDepRate::SameAuxDepRate(const Rule& r,state::State& state,bool normalize) :
		Rule::Rate(r,state),SamePtrnRate(r,state,normalize),AuxDepRate(r,state) {
	for(auto i = 1; i < base.aux_functions.size();i++)
		if(base.aux_functions[i-1] != base.aux_functions[i]){
			delete base.factor;
			for(auto& elem : base.aux_functions)
				delete elem.second;
			throw invalid_argument("Not the same aux function.");
		}
}


Rule::Rate::~Rate() {
	delete baseRate;
	if(unaryRate.first)
		delete unaryRate.first;
}

NormalRate::~NormalRate(){}

SamePtrnRate::~SamePtrnRate() {}

AuxDepRate::~AuxDepRate() {
	delete base.factor;
	for(auto& elem : base.aux_functions)
		delete elem.second;

	if(unary.factor){
		delete unary.factor;
		for(auto& elem : unary.aux_functions)
			delete elem.second;
	}
}

SameAuxDepRate::~SameAuxDepRate() {}


two<FL_TYPE> NormalRate::evalActivity(const state::State& state) const {
	FL_TYPE a = baseRate->getValue(state).valueAs<FL_TYPE>();
	auto& lhs = rule.getLHS();
	for(unsigned i = 0 ; i < lhs.compsCount() ; i++){
		auto& cc = lhs.getComponent(i);
		auto& injs = state.getInjContainer(cc.getId());
		a *= injs.count();
	}
	return make_pair(a,0.0);
}

two<FL_TYPE> SamePtrnRate::evalActivity(const state::State& state) const {
	FL_TYPE a = baseRate->getValue(state).valueAs<FL_TYPE>()/norm;
	auto& lhs = rule.getLHS();
	auto count = state.getInjContainer(lhs.getComponent(0).getId()).count();
	for(unsigned i = 0 ; i < lhs.compsCount() ; i++){
		//injs.selectRule(rule.getId(), i);
		a *= (count-i);
	}
	return make_pair(a,0.0);
}

two<FL_TYPE> AuxDepRate::evalActivity(const state::State& state) const {
	FL_TYPE a = base.factor->getValue(state).valueAs<FL_TYPE>();
	auto& lhs = rule.getLHS();
	for(unsigned i = 0 ; i < lhs.compsCount() ; i++){
		auto& cc = lhs.getComponent(i);
		auto& injs = state.getInjContainer(cc.getId());
		injs.selectRule(rule.getId(), i);
		a *= injs.partialReactivity();
	}
	return make_pair(a,0.0);
}

two<FL_TYPE> SameAuxDepRate::evalActivity(const state::State& state) const {
	FL_TYPE a = base.factor->getValue(state).valueAs<FL_TYPE>()/norm;
	auto& lhs = rule.getLHS();
	auto& injs = state.getInjContainer(lhs.getComponent(0).getId());
	injs.selectRule(rule.getId(), 0);
	auto ract = injs.partialReactivity();
	for(unsigned i = 0 ; i < lhs.compsCount() ; i++){
		a *= ract;
	}
	a -= injs.getM2();
	return make_pair(a,0.0);
}


















} /* namespace simulation */
