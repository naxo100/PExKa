/*
 * Rule.cpp
 *
 *  Created on: May 9, 2017
 *      Author: naxo
 */

#include "../util/Exceptions.h"
#include "../util/Warning.h"
#include "Rule.h"
#include "../pattern/Environment.h"
#include "../grammar/ast/Basics.h"

namespace simulation {

Rule::Rule(const ast::Id& nme, const Mixture& mix) : name(nme.getString()),loc(nme.loc),
		lhs(mix),rhs(nullptr),rate(nullptr),isRhsDeclared(false){}

Rule::~Rule() {
	// TODO Auto-generated destructor stub
	if(isRhsDeclared)
		delete rhs;
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

void Rule::difference(const Environment& env, const vector<ag_st_id>& lhs_order,const vector<ag_st_id>& rhs_order){
	unsigned i;
	unsigned first_del = 9999;
	auto& warns = WarningStack::getStack();
	list<Action> binds;
	//modify nodes
	for(i = 0; i < lhs_order.size(); i++){
		auto lhs_ag = lhs.getAgent(lhs_order[i]);
		try{
			auto& rhs_ag = rhs->getAgent(rhs_order[i]);
			if(lhs_ag.getId() != rhs_ag.getId()){
				first_del = i;
				break;
			}//modify
			auto& sign = env.getSignature(lhs_ag.getId());
			small_id j = 0;
			for(auto psite : sign.getSites()){
				Action a;
				a.t = CHANGE;
				ag_st_id lnk1,lnk2;
				try{
					auto& lhs_site = lhs_ag.getSite(j);
					try{
						auto rhs_site = rhs_ag.getSite(j);
						if(lhs_site.state.smallVal != rhs_site.state.smallVal){
							a.t = CHANGE;a.trgt1 = make_tuple(rhs_order[i].first,rhs_order[i].second,j,false);
							get<0>(a.trgt2) = rhs_site.state.smallVal;
							script.emplace_back(a);
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
										sign.getSite(j).getName()+" on the right hand side is underspecified",loc);
								break;
							case Mixture::BIND://if semi error else connect
								lnk1 = rhs->follow(rhs_order[i].first,rhs_order[i].second,j);
								if(lnk1.first == rhs_order[i].second && lnk1.second == j)
									throw SemanticError("The link status of agent "+sign.getName()+", site "+
											sign.getSite(j).getName()+" on the right hand side is inconsistent",loc);
								if(lnk1 < make_pair(rhs_order[i].second,j))
									break;//link added before.
								else{
									a.t = BIND;a.trgt1 = make_tuple(rhs_order[i].first,rhs_order[i].second,j,false);
									a.trgt2 = make_tuple(rhs_order[i].first,lnk1.first,lnk1.second,false);
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
								a.t = FREE;a.trgt1 = make_tuple(lhs_order[i].first,lhs_order[i].second,j,false);
								script.emplace_back(a);
								break;
							case Mixture::BIND_TO://error
							case Mixture::PATH://error
								throw SemanticError("The link status of agent "+sign.getName()+", site "+
										sign.getSite(j).getName()+" on the right hand side is underspecified",loc);
								break;
							case Mixture::BIND://if semi error else warning
								lnk1 = rhs->follow(rhs_order[i].first,rhs_order[i].second,j);
								if(lnk1.first == rhs_order[i].second && lnk1.second == j)
									throw SemanticError("The link status of agent '"+sign.getName()+
											"', site '"+sign.getSite(j).getName()+
											"' on the right hand side is inconsistent",loc);
								if(lnk1 < make_pair(rhs_order[i].second,j))
									break;//link added before.
								else {
									warns.emplace_back("Rule '"+name+"': site '"+sign.getSite(j).getName()+
											"' of agent '"+sign.getName()+"' is bound in the right hand"+
											"side although it is unspecified in the left hand side",loc);
									a.t = BIND;a.trgt1 = make_tuple(rhs_order[i].first,rhs_order[i].second,j,false);
									a.trgt2 = make_tuple(rhs_order[i].first,lnk1.first,lnk1.second,false);
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
								lnk1 = lhs.follow(lhs_order[i].first,lhs_order[i].second,j);
								if(lnk1.first == lhs_order[i].second && lnk1.second == j){
									warns.emplace_back("Rule '"+name+"': breaking a semi-link on site '"+
											sign.getSite(j).getName()+"' will induce a side effect",loc);
									get<3>(a.trgt2) = true;
								}
								else{
									if(lnk1 < make_pair(lhs_order[i].second,j))
										break;//link added before.
									a.trgt2 = make_tuple(lhs_order[i].first,lnk1.first,lnk1.second,false);
								}
								a.t = FREE;a.trgt1 = make_tuple(lhs_order[i].first,lhs_order[i].second,j,false);
								script.emplace_back(a);
								break;
							case Mixture::BIND://if semi-semi nothing if semi-not warning if not-semi error if not-not warn(swap)
								lnk1 = lhs.follow(lhs_order[i].first,lhs_order[i].second,j);
								lnk2 = rhs->follow(rhs_order[i].first,rhs_order[i].second,j);
								if(lnk1.first == lhs_order[i].second && lnk1.second == j){
									if(lnk2.first == lhs_order[i].second && lnk2.second == j){
										//semi-semi -> nothing
									}
									else {//semi-not
										if(lnk2 < make_pair(rhs_order[i].second,j))
											break;//link added before.
										warns.emplace_back("The link state of site "+sign.getSite(j).getName()+
												" of agent "+sign.getName()+
												" is changed although it is a semi-link in the left hand side",loc);
										a.t = BIND; a.trgt1 = make_tuple(rhs_order[i].first,rhs_order[i].second,j,false);
										a.trgt2 = make_tuple(rhs_order[i].first,lnk2.first,lnk2.second,false);
										binds.emplace_back(a);
									}
								}
								else {
									if(lnk2.first == lhs_order[i].second && lnk2.second == j)//not-semi
										throw SemanticError("The link status of agent "+sign.getName()+", site "+
												sign.getSite(j).getName()+" on the right hand side is underspecified",loc);
									else {//not-not
										if(lnk1 != lnk2){
											if(lnk2 < make_pair(rhs_order[i].second,j))
												break;//link added before.
											warns.emplace_back("%s rule induces a link permutation on site '%s' of agent '%s'",loc);
											a.t = BIND; a.trgt1 = make_tuple(rhs_order[i].first,rhs_order[i].second,j,false);
											a.trgt2 = make_tuple(rhs_order[i].first,lnk2.first,lnk2.second,false);
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
								a.t = FREE;a.trgt1 = make_tuple(lhs_order[i].first,lhs_order[i].second,j,false);
								break;
							case Mixture::BIND://if semi error else warning(swap)
								lnk1 = rhs->follow(rhs_order[i].first,rhs_order[i].second,j);
								if(lnk1.first == rhs_order[i].second && lnk1.second == j)
									throw SemanticError("The link status of agent "+sign.getName()+", site "+
											sign.getSite(j).getName()+" on the right hand side is underspecified",loc);
								else {
									warns.emplace_back("%s link state of site '%s' of agent '%s' is changed although it is a semi-link in the left hand side",loc);
									a.t = BIND; a.trgt1 = make_tuple(lhs_order[i].first,lhs_order[i].second,j,false);
									a.trgt2 = make_tuple(rhs_order[i].first,lnk1.first,lnk1.second,false);
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
	map<ag_st_id,small_id> antimap;
	for(unsigned i = 0; i<rhs_order.size() ; i++)
		antimap[rhs_order[i]] = i;
	for(auto& bind : binds){
		auto k = antimap[make_pair(get<0>(bind.trgt2),get<1>(bind.trgt2))];
		if( k >= i ){
			get<3>(bind.trgt2) = true;
			get<1>(bind.trgt2) = k-i;
		}
		script.emplace_back(bind);
	}

	//new nodes
	newNodes.reserve(rhs_order.size()-i);
	for(unsigned j = i; j < rhs_order.size(); j++){
		auto& rhs_ag = rhs->getAgent(rhs_order[j]);
		env.getSignature(rhs_ag.getId());
		newNodes.emplace_back(env.getSignature(rhs_ag.getId()));
		for(auto s : rhs_ag)
			newNodes[j-i].setState(s.first,s.second.state);
	}
	//deleted nodes
	for(unsigned j = first_del; j < lhs_order.size(); j++){
		auto& lhs_ag = lhs.getAgent(lhs_order[j]);
		Action a;
		a.t = DELETE;a.trgt1 = make_tuple(lhs_order[j].first,lhs_order[j].second,0,false);
		script.emplace_back(a);
	}

}


const list<Rule::Action>& Rule::getScript() const{
	return script;
}
const vector<state::SiteGraph::Node>& Rule::getNewNodes() const{
	return newNodes;
}




/**** DEBUG ****/
string Rule::toString(const pattern::Environment& env) const {
	static string acts[] = {"CREATE","DELETE","TRANSPORT","CHANGE","BIND","FREE"};
	string s = name+"'s actions:\n";
	for(auto& nn : newNodes){
		s += "INSERT agent "+nn.toString(env)+"\n";
	}
	const pattern::Signature *sign1,*sign2;
	for(auto act : script){
		switch(act.t){
		case DELETE:
			s += acts[act.t] + " of agent "+lhs.getAgent(get<0>(act.trgt1),get<1>(act.trgt1)).toString(env)+"\n";
			break;
		case TRANSPORT:
			break;
		case CHANGE:
			sign1 = &env.getSignature(lhs.getAgent(get<0>(act.trgt1),get<1>(act.trgt1)).getId());
			s += acts[act.t] + " of agent site "+sign1->getName()+"."+sign1->getSite(get<2>(act.trgt1)).getName()+
					" to value "+static_cast<const pattern::Signature::LabelSite&>(sign1->getSite(get<2>(act.trgt1)))
					.getLabel(get<0>(act.trgt2))+"\n";
			break;
		case BIND:
			s += acts[act.t] + " of agents sites ";
			if(get<3>(act.trgt1)){//new node
				sign1 = &env.getSignature(newNodes[get<1>(act.trgt1)].getId());
				s += "(new) " + sign1->getName()+"."+sign1->getSite(get<2>(act.trgt1)).getName()+" and ";
			}
			else{
				sign1 = &env.getSignature(rhs->getAgent(get<0>(act.trgt1),get<1>(act.trgt1)).getId());
				s += sign1->getName()+"."+sign1->getSite(get<2>(act.trgt1)).getName()+" and ";
			}
			if(get<3>(act.trgt2)){//new node
				sign2 = &env.getSignature(newNodes[get<1>(act.trgt2)].getId());
				s += "(new) " + sign2->getName()+"."+sign2->getSite(get<2>(act.trgt2)).getName()+"\n";
			}
			else{
				sign2 = &env.getSignature(rhs->getAgent(get<0>(act.trgt2),get<1>(act.trgt2)).getId());
				s += sign2->getName()+"."+sign2->getSite(get<2>(act.trgt2)).getName()+"\n";
			}
			break;
		case FREE:
			sign1 = &env.getSignature(lhs.getAgent(get<0>(act.trgt1),get<1>(act.trgt1)).getId());
			sign2 = &env.getSignature(lhs.getAgent(get<0>(act.trgt2),get<1>(act.trgt2)).getId());
			s += acts[act.t] + " of agents site "+sign1->getName()+"."+sign1->getSite(get<2>(act.trgt1)).getName()+
				" and "+sign2->getName()+"."+sign2->getSite(get<2>(act.trgt2)).getName()+"\n";
			break;
		default: break;
		}
	}
	return s;
}



} /* namespace simulation */
