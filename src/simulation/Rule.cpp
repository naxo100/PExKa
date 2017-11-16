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
								a.t = UNBIND;a.trgt1 = make_tuple(lhs_unmask[i].first,lhs_unmask[i].second,j,false);
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
									get<3>(a.trgt2) = true;
								}
								else{
									i2 = lhs_mask.at(make_pair(lhs_unmask[i].first,lnk1.first));
									if(i > i2){
										changes[make_pair(rhs_unmask[i],false)].second.emplace_back(j);
										break;//link added before.
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
										a.t = LINK; a.trgt1 = make_tuple(lhs_unmask[i].first,lhs_unmask[i].second,j,false);
										a.trgt2 = make_tuple(rhs_unmask[i].first,lnk2.first,lnk2.second,i2);
										binds.emplace_back(a);
									}
								}
								else {
									if(lnk2.first == lhs_unmask[i].second && lnk2.second == j)//not-semi
										throw SemanticError("The link status of agent "+sign.getName()+", site "+
												sign.getSite(j).getName()+" on the right hand side is underspecified",loc);
									else {//not-not
										if(lnk1 != lnk2){
											i2 = rhs_mask.at(make_pair(rhs_unmask[i].first,lnk2.first));
											if(i > i2)
												break;//link added before.
											warns.emplace_back("%s rule induces a link permutation on site '%s' of agent '%s'",loc);
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
								a.t = UNBIND;a.trgt1 = make_tuple(lhs_unmask[i].first,lhs_unmask[i].second,j,false);
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
									a.t = LINK; a.trgt1 = make_tuple(lhs_unmask[i].first,lhs_unmask[i].second,j,false);
									a.trgt2 = make_tuple(rhs_unmask[i].first,lnk1.first,lnk1.second,i2);
									binds.emplace_back(a);
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
			get<3>(bind.trgt2) = k-i+1;
		}
		else{
			changes[make_pair(rhs_unmask[k],false)].second.emplace_back(get<2>(bind.trgt2));
			get<0>(bind.trgt2) = lhs_unmask[k].first;
			get<1>(bind.trgt2) = lhs_unmask[k].second;
			k = false;
			//changes[make_pair(rhs_unmask[j],false)].second.emplace_back(get<2>(bind.trgt1));
		}
		script.emplace_back(bind);
	}

	//new nodes
	newNodes.reserve(rhs_unmask.size()-i);
	for(unsigned j = i; j < rhs_unmask.size(); j++){
		auto& rhs_ag = rhs->getAgent(rhs_unmask[j]);
		env.getSignature(rhs_ag.getId());
		newNodes.emplace_back(new state::Node(env.getSignature(rhs_ag.getId())));
		news.emplace_back(rhs_unmask[j]);
		for(auto& s : rhs_ag)
			newNodes[j-i]->setState(s.first,s.second.state);
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



void Rule::checkInfluence(const Environment& env) {
	//TODO changes of BIND_TO -> FREE
	//TODO changes of semi-link -> free ???
	try{

	map<const pattern::Mixture::Component*,CandidateInfo> candidates;
	for(auto& ag_changes : changes){//for each agent that changes
		auto& change_cc = rhs->getComponent(ag_changes.first.first.first);
		auto& ag = rhs->getAgent(ag_changes.first.first);
		if(ag_changes.first.second){
			//new
		}
		else {
			for(auto change : ag_changes.second.first){//for each site_value changes
				for(auto emb : ag.getParentPatterns(change)){//for each parent pattern in changed agent site
					if(emb->getSite(change).isEmptySite())//if this pattern accept every value -> continue
						continue;
					for(auto& cc_agid : emb->getIncludes()){
						candidates[cc_agid.first].set(cc_agid.second,matches.at(ag_changes.first.first));
					}
				}
				for(auto emb : ag.getChildPatterns(change)){
					for(auto& cc_agid : emb->getIncludes()){
						candidates[cc_agid.first].set(cc_agid.second,matches.at(ag_changes.first.first));
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
							candidates[cc_agid.first].set(cc_agid.second,matches.at(ag_changes.first.first));
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
									candidates[cc_agid.first].set(cc_agid.second,matches.at(ag_changes.first.first));

								}
								catch(False& ex){
									candidates[cc_agid.first].is_valid = false;
								}
							else
								candidates[cc_agid.first].is_valid = false;
						}
						break;
					//case pattern::Pattern::PATH:
					//	break;//TODO
					default:
						break;
					}

				}
				for(auto emb : ag.getChildPatterns(change)){
					list<two<ag_st_id> > to_test;//rhs-lnk,cc_lnk
					auto lt = emb->getSite(change).link_type;
					if(lt == pattern::Mixture::FREE){
						for(auto& cc_agid : emb->getIncludes())
							candidates[cc_agid.first].set(cc_agid.second,matches.at(ag_changes.first.first));
						continue;
					}
					if(lt == pattern::Mixture::WILD)
						continue;
					rhs_lnk =  rhs_graph.at(make_pair(ag_changes.first.first.second,change));
					for(auto& cc_agid : emb->getIncludes()){
						auto cc = cc_agid.first;
						to_test.emplace_back(make_pair(rhs_lnk,cc->getGraph().at(//should not throw
								make_pair(cc_agid.second,change))));
						while(to_test.size()){//test connected agents of emb
							auto rhs_lnk = to_test.front().first;
							auto cc_lnk = to_test.front().second;
							to_test.pop_front();
							if(rhs_lnk.second != cc_lnk.second){
								candidates[cc_agid.first].is_valid = false;
								break;
							}
							try{
								set<small_id> done;
								cc_agid.first->getAgent(cc_lnk.first).compare(change_cc.getAgent(rhs_lnk.first),done);
								for(auto id_site : change_cc.getAgent(rhs_lnk.first)){
									//TODO already done
									try{
										auto lnk = rhs_graph.at(make_pair(rhs_lnk.first,id_site.first));
										if(cc_agid.first->getAgent(cc_lnk.first).getSite(id_site.first).link_type !=
												pattern::Mixture::WILD)//then is BIND
											to_test.emplace_back(make_pair(lnk,
													cc_agid.first->getGraph().at(make_pair(cc_lnk.first,id_site.first))));

									}catch(std::out_of_range &ex){}
								}
							}
							catch(False &ex){
								candidates[cc_agid.first].is_valid = false;
								break;
							}
						}
						candidates[cc_agid.first].set(cc_agid.second,matches.at(ag_changes.first.first));
					}
				}
			}
		}
	}
	//no need to track deletes
	/*for(auto del : deletes){
		auto& del_ag = lhs.getAgent(del);
		for(auto& cc_agid : del_ag.getIncludes())
			candidates[cc_agid.first];
		for(auto& embs : del_ag.getParentPatterns())
			for(auto emb : embs.second)
				for(auto& cc_agid : emb->getIncludes())
					candidates[cc_agid.first];
		for(auto& embs : del_ag.getChildPatterns())
			for(auto emb : embs.second)
				for(auto& cc_agid : emb->getIncludes())
					candidates[cc_agid.first];
	}*/
	int i = 0;
	for(auto n : news){
		auto& new_ag = rhs->getAgent(n);
		for(auto& ag : env.getAgentPatterns(new_ag.getId())){
			bool isEmb = true;
			for(auto& id_site : ag){
				try{
					auto& site = new_ag.getSite(id_site.first);
					if(id_site.second.isEmptySite() ||
							memcmp(&id_site.second.state,&newNodes[i]->getInternalState(id_site.first),sizeof(state::SomeValue))){
						/*TODO test for link
						switch(id_site.second.link_type){
						case Pattern::BIND:
							if(id_site.second.isBindToAny())

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
					candidates[cc_agid.first].set(cc_agid.second,ag_st_id(-1,i));
		}
		i++;
	}
	for(auto& cc_info : candidates)
		if(cc_info.second.is_valid)
			influence.emplace_back(cc_info);
	}
	catch(exception &ex){
		cout << "cannot check influences for " << this->getName() << endl;
		//throw ex;
	}
}

const list<pair<const pattern::Mixture::Component*,Rule::CandidateInfo> >& Rule::getInfluences() const{
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
			if(get<3>(act.trgt1)){//new node
				sign1 = &env.getSignature(newNodes[get<3>(act.trgt1)]->getId());
				s += "(new) " + sign1->getName()+"."+sign1->getSite(get<2>(act.trgt1)).getName()+" and ";
			}
			else{
				sign1 = &env.getSignature(lhs.getAgent(get<0>(act.trgt1),get<1>(act.trgt1)).getId());
				s += sign1->getName()+"."+sign1->getSite(get<2>(act.trgt1)).getName()+" and ";
			}
			if(get<3>(act.trgt2)){//new node
				sign2 = &env.getSignature(newNodes[get<3>(act.trgt2)-1]->getId());
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
				" and "+sign2->getName()+"."+sign2->getSite(get<2>(act.trgt2)).getName()+"\n";
			break;
		default: break;
		}
	}
	return s;
}

Rule::CandidateInfo::CandidateInfo() : is_valid(true) {};
void Rule::CandidateInfo::set(small_id root,ag_st_id node){
	node_id[node.first] = make_pair(node.second,root);
}



} /* namespace simulation */
