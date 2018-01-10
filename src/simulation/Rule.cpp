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

namespace simulation {

Rule::Rule(int _id,const ast::Id& nme, const Mixture& mix) : id(_id),name(nme.getString()),loc(nme.loc),
		lhs(mix),rhs(nullptr),rate(nullptr),isRhsDeclared(false){}

Rule::~Rule() {
	// TODO Auto-generated destructor stub
	if(!isRhsDeclared)
		delete rhs;
	if(rate)
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

const state::BaseExpression& Rule::getRate() const {
	return *rate;
}

void Rule::setRHS(const Mixture* mix,bool is_declared){
	rhs = mix;
	isRhsDeclared = is_declared;
}
void Rule::setRate(const state::BaseExpression* r){
	rate = r;
}
void Rule::setUnaryRate(pair<const state::BaseExpression*,int> u_rate ){
	unaryRate = u_rate;
}

void Rule::difference(const Environment& env, const vector<ag_st_id>& lhs_unmask,const vector<ag_st_id>& rhs_unmask){
	small_id i;
	unsigned first_del = 9999;
	auto& warns = WarningStack::getStack();
	list<Action> binds;
	map<ag_st_id,small_id> lhs_mask,rhs_mask;
	for(unsigned i = 0; i<rhs_unmask.size() ; i++)
		rhs_mask[rhs_unmask[i]] = i;
	for(unsigned i = 0; i<lhs_unmask.size() ; i++)
		lhs_mask[lhs_unmask[i]] = i;

	//modify nodes
	for(i = 0; i < lhs_unmask.size(); i++){
		auto& lhs_ag = lhs.getAgent(lhs_unmask[i]);
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
				try{
					auto& lhs_site = lhs_ag.getSite(j);
					try{
						auto& rhs_site = rhs_ag.getSite(j);
						if(rhs_site.isEmptySite()){
							if(!lhs_site.isEmptySite())
								throw SemanticError("The state of site "+ sign.getSite(j).getName()+
									" in agent "+sign.getName()+" is underspecified on the right hand side.",loc);
						}
						else{
							if(lhs_site.isEmptySite())
								warns.emplace_back("Application of rule '"+name+
										"' will induce a null event when applied to an agent '"+sign.getName()+
										"' that have the same value ",loc);
							if(lhs_site.state.smallVal != rhs_site.state.smallVal){//TODO other type of value changes
								a.t = CHANGE;a.trgt1 = make_tuple(lhs_unmask[i].first,lhs_unmask[i].second,j,false);
								get<0>(a.trgt2) = rhs_site.state.smallVal;
								script.emplace_back(a);
								changes[make_pair(rhs_unmask[i],false)].first.emplace_back(j);
							}
						}
						switch(lhs_site.link_type){
						case Mixture::FREE:				/******* LHS_FREE *******/
							switch(rhs_site.link_type){
							case Mixture::FREE://nothing
								break;
							case Mixture::WILD://error
							case Mixture::BIND_TO://error
							case Mixture::PATH://error?
								throw SemanticError("The link status of agent "+sign.getName()+", site "+
										sign.getSite(j).getName()+" on the right hand side is underspecified.",loc);
								break;
							case Mixture::BIND://if semi error else connect
								lnk1 = rhs->follow(rhs_unmask[i].first,rhs_unmask[i].second,j);
								if(lnk1.first == rhs_unmask[i].second && lnk1.second == j)
									throw SemanticError("The link status of agent "+sign.getName()+", site "+
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
								warns.emplace_back("Application of rule '"+name+
										"' will induce a null event when applied to an agent '"+sign.getName()+
										"' that is free on site '"+sign.getSite(j).getName()+"'",loc);
								a.t = UNBIND;a.trgt1 = make_tuple(lhs_unmask[i].first,lhs_unmask[i].second,j,Action::S_EFF);
								script.emplace_back(a);
								changes[make_pair(rhs_unmask[i],false)].second.emplace_back(j);
								break;
							case Mixture::BIND_TO://error
							case Mixture::PATH://error
								throw SemanticError("The link status of agent "+sign.getName()+", site "+
										sign.getSite(j).getName()+" on the right hand side is underspecified",loc);
								break;
							case Mixture::BIND://if semi error else warning
								lnk1 = rhs->follow(rhs_unmask[i].first,rhs_unmask[i].second,j);
								if(lnk1.first == rhs_unmask[i].second && lnk1.second == j)
									throw SemanticError("The link status of agent '"+sign.getName()+
											"', site '"+sign.getSite(j).getName()+
											"' on the right hand side is inconsistent",loc);
								i2 = rhs_mask.at(make_pair(rhs_unmask[i].first,lnk1.first));
								if(i > i2)
									break;//link added before.
								else {
									warns.emplace_back("Rule '"+name+"': site '"+sign.getSite(j).getName()+
											"' of agent '"+sign.getName()+"' is bound in the right hand"+
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
							case Mixture::WILD://error
							case Mixture::BIND_TO://error
							case Mixture::PATH://error
								throw SemanticError("The link status of agent "+sign.getName()+", site "+
										sign.getSite(j).getName()+" on the right hand side is underspecified",loc);
								break;
							case Mixture::FREE://if semi warning?? and disconnect else disconnect
								lnk1 = lhs.follow(lhs_unmask[i].first,lhs_unmask[i].second,j);
								if(lnk1.first == lhs_unmask[i].second && lnk1.second == j){
									warns.emplace_back("Rule '"+name+"': breaking a semi-link on site '"+
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
									if(lnk2.first == lhs_unmask[i].second && lnk2.second == j){
										//semi-semi -> nothing
									}
									else {//semi-not
										i2 = rhs_mask.at(make_pair(rhs_unmask[i].first,lnk2.first));
										if(i > i2)
											break;//link added before.
										warns.emplace_back("The link state of site "+sign.getSite(j).getName()+
												" of agent "+sign.getName()+
												" is changed although it is a semi-link in the left hand side",loc);
										a.t = LINK; a.trgt1 = make_tuple(lhs_unmask[i].first,lhs_unmask[i].second,j,Action::S_EFF);
										a.trgt2 = make_tuple(rhs_unmask[i].first,lnk2.first,lnk2.second,i2);
										binds.emplace_back(a);
									}
								}
								else {
									if(lnk2.first == rhs_unmask[i].second && lnk2.second == j)//not-semi
										throw SemanticError("The link status of agent "+sign.getName()+", site "+
												sign.getSite(j).getName()+" on the right hand side is underspecified",loc);
									else {//not-not
										if(lhs_mask.at(make_pair(lhs_unmask[i].first,lnk1.first)) !=
												rhs_mask.at(make_pair(rhs_unmask[i].first,lnk2.first)) ||
												lnk1.second != lnk2.second){
											i2 = rhs_mask.at(make_pair(rhs_unmask[i].first,lnk2.first));
											warns.emplace_back(name+" rule induces a link permutation on site "
													+sign.getSite(j).getName()+" of agent "+sign.getName(),loc);
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
							case Mixture::PATH://error
							case Mixture::WILD://error
								throw SemanticError("The link status of agent "+sign.getName()+", site "+
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
									throw SemanticError("The link status of agent "+sign.getName()+", site "+
											sign.getSite(j).getName()+" on the right hand side is underspecified",loc);
								else {
									i2 = rhs_mask.at(make_pair(rhs_unmask[i].first,lnk2.first));
									if(i > i2)
										break;//link added before.
									warns.emplace_back("%s link state of site '%s' of agent '%s' is changed although it is a semi-link in the left hand side",loc);
									a.t = LINK; a.trgt1 = make_tuple(lhs_unmask[i].first,lhs_unmask[i].second,j,Action::S_EFF);
									a.trgt2 = make_tuple(rhs_unmask[i].first,lnk1.first,lnk1.second,i2);
									binds.emplace_back(a);
									sideEffects.emplace_back(lhs_site.lnk_ptrn);
								}
								break;
							case Mixture::BIND_TO://if different error else nothing
								if(lhs_site.lnk_ptrn != rhs_site.lnk_ptrn)
									throw SemanticError("The link status of agent '%s', site '%s' on the right hand side is inconsistent",loc);
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
					}
					catch(std::out_of_range &e){
						//WARNING
						throw SemanticError("The internal/lnk state of the site "+psite->getName()+
								" in agent "+sign.getName()+" must be specified in the right side.",loc);
					}
				}catch(std::out_of_range &e){
					try{
						//auto rhs_site =
						rhs_ag.getSite(j);
						//WARNING
						throw SemanticError("The internal state of agent "+sign.getName()+
								", site "+psite->getName()+" in the left side is underspecified.",loc);
					}
					catch(std::out_of_range &e){
						//it's ok, omitted in both sides.
					}
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
			auto lnk = rhs->follow(cc_ag_id.first,cc_ag_id.second,s.first);
			if(!s.second.isEmptySite())
				newNodes[j-i]->setState(s.first,s.second.state);
			switch(s.second.link_type){
			case Mixture::FREE:break;
			case Mixture::BIND:{
				auto k =rhs_mask.at(make_pair(cc_ag_id.first,lnk.first));
				if(k > j){
					Action a;
					a.t = LINK;a.trgt1 = make_tuple(cc_ag_id.first,j-i,s.first,true);
					a.trgt2 = make_tuple(cc_ag_id.first,k-i,lnk.second,true);
					script.emplace_back(a);
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
		const Mixture::Component& test_cc,multimap<ag_st_id,ag_st_id>& already_done,const Environment& env) const {
	map<small_id,small_id> visited;
	while(to_test.size()){//test connected agents of emb
		if(to_test.front().first.second != to_test.front().second.second)
			return false;//not connected to the same site
		auto& rhs_cc = rhs->getComponent(rhs_cc_id);
		auto rhs_ag_id = to_test.front().first.first;
		auto test_ag_id = to_test.front().second.first;
		to_test.pop_front();
		//auto key = make_pair(test_cc.getId(),test_ag_id);
		//auto val = make_pair(rhs_cc_id,rhs_ag_id);
		if(visited.count(test_ag_id))
			if(visited[test_ag_id] == rhs_ag_id)
				continue;
			else
				return false;
		else
			visited[test_ag_id] = rhs_ag_id;
		auto& rhs_ag = rhs_cc.getAgent(rhs_ag_id);
		auto& test_ag = test_cc.getAgent(test_ag_id);
		if(news.count(make_pair(rhs_cc_id,rhs_ag_id))){//testing for new node
			for(auto id_site : test_ag){
				auto& test_site = id_site.second;
				try{
					auto& rhs_site = rhs_ag.getSite(id_site.first);
					if(id_site.second.isEmptySite() ){//compare with default
						if(!test_site.isEmptySite() &&
								env.getSignature(rhs_ag.getId()).getSite(id_site.first).
								getDefaultValue() != test_site.state)
							return false;
					} else
						if(!test_site.isEmptySite() && id_site.second.state != test_site.state)
							return false;
					if(rhs_site.link_type == Pattern::LinkType::BIND){
						auto lnk = rhs_cc.getGraph().at(ag_st_id(rhs_ag_id,id_site.first));
						switch(test_site.link_type){
						case Pattern::BIND_TO:{
							if(lnk.second != test_site.lnk_ptrn.second ||
									test_site.lnk_ptrn.first != rhs_cc.getAgent(lnk.first).getId())
								return false;
						};break;
						case Pattern::FREE:
							return false;
						case Pattern::BIND:
							if(!test_site.isBindToAny())
								to_test.emplace_back(lnk,test_cc.getGraph().at(make_pair(test_ag_id,id_site.first)));
							break;
						case Pattern::PATH:
							WarningStack::getStack().emplace_back("PATH is not implemented yet",loc);
							break;
						default://WILD
							break;
						}
					}
					else { //can only be free
						switch(test_site.link_type){
						case Pattern::BIND_TO:
						case Pattern::BIND:
							return false;
						case Pattern::PATH:
							WarningStack::getStack().emplace_back("PATH is not implemented yet",loc);
							break;
						default://WILD,FREE
							break;
						}
					}
				}
				catch(out_of_range& ex){//test_site must have default values and be free
					if(!test_site.isEmptySite() && env.getSignature(rhs_ag.getId()).getSite(id_site.first).
							getDefaultValue() != test_site.state)
						return false;
					if(test_site.link_type > Pattern::WILD)//TODO warning for path
						return false;
				}
			}
		}
		else{//testing for conserved node
			try{
				set<small_id> done;
				test_cc.getAgent(test_ag_id).compare(rhs_cc.getAgent(rhs_ag_id),done);
				for(auto id_site : rhs_cc.getAgent(rhs_ag_id)){
					try{
						auto lnk = rhs_cc.getGraph().at(make_pair(rhs_ag_id,id_site.first));
						if(test_cc.getAgent(test_ag_id).getSite(id_site.first).link_type !=
								pattern::Mixture::WILD)//then is BIND
							to_test.emplace_back(lnk,
									test_cc.getGraph().at(make_pair(test_ag_id,id_site.first)));

					}catch(std::out_of_range &ex){}
				}
			}
			catch(False &ex){
				return false;
			}
		}

	}
	for(auto elem : visited)
		already_done.emplace(ag_st_id(test_cc.getId(),elem.first),ag_st_id(rhs_cc_id,elem.second));
	return true;
}

void Rule::checkInfluence(const Environment& env) {
	//TODO changes of BIND_TO -> FREE
	//TODO changes of semi-link -> free ???
	try{

	map<const pattern::Mixture::Component*,CandidateInfo> candidates;
	for(auto& ag_changes : changes){//for each agent that changes
		auto& change_cc = rhs->getComponent(ag_changes.first.first.first);
		auto& ag = rhs->getAgent(ag_changes.first.first);
		for(auto& cc_agid : ag.getIncludes()){
			candidates[cc_agid.first].set(cc_agid.second,ag_changes.first.first);
		}
		if(ag_changes.first.second){
			//new
		}
		else {
			for(auto change : ag_changes.second.first){//for each site_value changes
				for(auto emb : ag.getParentPatterns(change)){//for each parent pattern in changed agent site
					if(emb->getSite(change).isEmptySite())//if this pattern accept every value -> continue
						continue;
					for(auto& cc_agid : emb->getIncludes()){
						candidates[cc_agid.first].set(cc_agid.second,ag_changes.first.first);
					}
				}
				for(auto emb : ag.getChildPatterns(change)){
					for(auto& cc_agid : emb->getIncludes()){
						candidates[cc_agid.first].set(cc_agid.second,ag_changes.first.first);
					}
				}
			}
			for(auto change : ag_changes.second.second){//for each link val/type changes
				auto& rhs_graph = change_cc.getGraph();
				ag_st_id rhs_lnk;
				for(auto emb : ag.getParentPatterns(change)){
					switch(emb->getSite(change).link_type){
					case pattern::Pattern::FREE:
						for(auto& cc_agid : emb->getIncludes())
							candidates[cc_agid.first].set(cc_agid.second,ag_changes.first.first);
						break;
					//case pattern::Pattern::WILD://if this pattern accept every lnk-type -> continue
						//break;//TODO remove if its in candidates??
					case pattern::Pattern::BIND:
					case pattern::Pattern::BIND_TO:
						rhs_lnk = rhs_graph.at(make_pair(ag_changes.first.first.second,change));
						for(auto& cc_agid : emb->getIncludes()){
							auto lnk = cc_agid.first->getGraph().at(make_pair(cc_agid.second,change));
							if(rhs_lnk.second == lnk.second)
								try{
									set<small_id> s;
									rhs->getAgent(ag_changes.first.first.first,rhs_lnk.first).compare(
											cc_agid.first->getAgent(lnk.first),s);
									candidates[cc_agid.first].set(cc_agid.second,ag_changes.first.first);

								}
								catch(False& ex){
									candidates[cc_agid.first].is_valid = false;
								}
							else
								candidates[cc_agid.first].is_valid = false;
						}
						break;
					default:
						break;
					}

				}
				for(auto emb : ag.getChildPatterns(change)){
					list<two<ag_st_id> > to_test;//rhs-lnk,cc_lnk
					auto lt = emb->getSite(change).link_type;
					if(lt != pattern::Mixture::WILD)
						for(auto& cc_agid : emb->getIncludes())
							candidates[cc_agid.first].set(cc_agid.second,ag_changes.first.first);
					/*if(lt == pattern::Mixture::FREE){
						for(auto& cc_agid : emb->getIncludes())
							candidates[cc_agid.first].set(cc_agid.second,ag_changes.first.first);
						continue;
					}
					if(lt == pattern::Mixture::WILD)
						continue;
					rhs_lnk =  rhs_graph.at(make_pair(ag_changes.first.first.second,change));
					for(auto& cc_agid : emb->getIncludes()){
						auto cc = cc_agid.first;
						to_test.emplace_back(make_pair(rhs_lnk,cc->getGraph().at(//should not throw
								make_pair(cc_agid.second,change))));
						if(test_linked_agents(to_test,ag_changes.first.first.first,*cc,env))
							candidates[cc_agid.first].set(cc_agid.second,ag_changes.first.first);
						else
							candidates[cc_agid.first].is_valid = false;
					}*/
				}
			}
		}
	}

	small_id i = 0;
	for(auto n : news){
		auto& new_ag = rhs->getAgent(n.first);
		for(auto& ag : env.getAgentPatterns(new_ag.getId())){
			bool isEmb = true;
			for(auto& id_site : ag){
				try{
					/*auto& site = */new_ag.getSite(id_site.first);
					if(id_site.second.isEmptySite() ||
							!memcmp(&id_site.second.state,&newNodes[i]->getInternalState(id_site.first),sizeof(state::SomeValue))){
						/*TODO test for link
						switch(id_site.second.link_type){
						case Pattern::BIND:
							if(id_site.second.isBindToAny()){

							}

						}*/
						continue;
					}
				}
				catch(std::out_of_range &ex){
					if( (id_site.second.isEmptySite() ||
							!memcmp(&id_site.second.state,&newNodes[i]->getInternalState(id_site.first),sizeof(state::SomeValue)))
							&& id_site.second.link_type == Pattern::FREE)
						continue;//ok
				}
				isEmb = false;break;
			}
			if(isEmb)
				for(auto cc_agid : ag.getIncludes())
					candidates[cc_agid.first].set(cc_agid.second,n.first);//ag_st_id(-1,i));
		}
		i++;
	}
	//Checking candidates
	multimap<ag_st_id,ag_st_id> already_done;
	for(auto& cc_info : candidates){
		for(auto& info : cc_info.second.node_id){
			bool rep = false;
			auto key = make_pair(cc_info.first->getId(),info.second);
			for(auto val_it = already_done.find(key);val_it != already_done.end(); val_it++)
				if( val_it->second == info.first){
					rep = true;continue;
				}
			if(rep)
				continue;
			list<two<ag_st_id> > to_test;
			to_test.emplace_back(ag_st_id(info.first.second,0),ag_st_id(info.second,0));
			if(test_linked_agents(to_test,info.first.first,*cc_info.first,already_done,env))
				try{
					influence[cc_info.first].set(info.second,ag_st_id(-1,news.at(info.first)));
				} catch(out_of_range &ex){
					influence[cc_info.first].set(info.second,matches.at(info.first));
				}
		}
	}

	}//end testing try TODO
	catch(exception &ex){
		cout << "cannot check influences for " << this->getName() << endl;
		//throw ex;
	}
}

const map<const pattern::Mixture::Component*,Rule::CandidateInfo>& Rule::getInfluences() const{
	return influence;
}



/**** DEBUG ****/
string Rule::toString(const pattern::Environment& env) const {
	static string acts[] = {"CHANGE","BIND","FREE","DELETE","CREATE","TRANSPORT"};
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
		default: break;
		}
	}
	return s;
}

Rule::CandidateInfo::CandidateInfo() : is_valid(true),is_new(false) {};
void Rule::CandidateInfo::set(small_id root,ag_st_id node){
	//node_id[node.first] = make_pair(node.second,root);
	node_id[node] = root;
}



} /* namespace simulation */
