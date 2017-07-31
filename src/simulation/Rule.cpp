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

namespace simulation {

/*Rule::Rule() {
	// TODO Auto-generated constructor stub

}*/

Rule::~Rule() {
	// TODO Auto-generated destructor stub
}



list<Rule::Action> Rule::difference(const Environment& env, const vector<ag_st_id>& lhs_order,const vector<ag_st_id>& rhs_order){
	unsigned i;
	unsigned first_del = 9999;
	auto& warns = WarningStack::getStack();
	list<Action> actions;
	//modify nodes
	for(i = 0; i < lhs_order.size(); i++){
		auto& lhs_ag = lhs.getAgent(lhs_order[i]);
		try{
			auto& rhs_ag = rhs.getAgent(rhs_order[i]);
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
						if(lhs_site.state != rhs_site.state){
							//a.t = CHANGE;a.trgt1 = {rhs_order[i].first,rhs_order[i].second,j,false};
							actions.emplace_back(a);
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
								lnk1 = rhs.follow(rhs_order[i].first,rhs_order[i].second,j);
								if(lnk1.first == rhs_order[i].second && lnk1.second == j)
									throw SemanticError("The link status of agent "+sign.getName()+", site "+
											sign.getSite(j).getName()+" on the right hand side is inconsistent",loc);
								else{
									//a.t = BIND;a.trgt1 = {rhs_order[i].first,rhs_order[i].second,j,false};
									//a.trgt2 = {rhs_order[i],lnk.first,lnk.second,false};//TODO
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
								//a.t = FREE;a.trgt1 = {lhs_order[i].first,lhs_order[i].second,j,false};
								break;
							case Mixture::BIND_TO://error
							case Mixture::PATH://error
								throw SemanticError("The link status of agent "+sign.getName()+", site "+
										sign.getSite(j).getName()+" on the right hand side is underspecified",loc);
								break;
							case Mixture::BIND://if semi error else warning
								lnk1 = rhs.follow(rhs_order[i].first,rhs_order[i].second,j);
								if(lnk1.first == rhs_order[i].second && lnk1.second == j)
									throw SemanticError("The link status of agent '"+sign.getName()+
											"', site '"+sign.getSite(j).getName()+
											"' on the right hand side is inconsistent",loc);
								else {
									warns.emplace_back("Rule '"+name+"': site '"+sign.getSite(j).getName()+
											"' of agent '"+sign.getName()+"' is bound in the right hand"+
											"side although it is unspecified in the left hand side",loc);
									//a.t = BIND;a.trgt1 = {rhs_order[i].first,rhs_order[i].second,j,false};
									//a.trgt2 = {rhs_order[i].first,lnk.first,lnk.second,false};
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
								if(lnk1.first == lhs_order[i].second && lnk1.second == j)
									warns.emplace_back("Rule '"+name+"': breaking a semi-link on site '"+
											sign.getSite(j).getName()+"' will induce a side effect",loc);
								//a.t = FREE;a.trgt1 = {lhs_order[i].first,lhs_order[i].second,j,false};
								break;
							case Mixture::BIND://if semi-semi nothing if semi-not warning if not-semi error if not-not warn(swap)
								lnk1 = lhs.follow(lhs_order[i].first,lhs_order[i].second,j);
								lnk2 = rhs.follow(rhs_order[i].first,rhs_order[i].second,j);
								if(lnk1.first == lhs_order[i].second && lnk1.second == j){
									if(lnk2.first == lhs_order[i].second && lnk2.second == j){
										//nothing
									}
									else {
										warns.emplace_back("%s link state of site '%s' of agent '%s' is changed although it is a semi-link in the left hand side");
										//a.t = BIND; a.trgt1 = {rhs_order[i].first,rhs_order[i].second,j,false};
										//a.trgt2 = {rhs_order[i].first,lnk2.first,lnk2.second,false};
									}
								}
								else {
									if(lnk2.first == lhs_order[i].second && lnk2.second == j)
										throw SemanticError("The link status of agent "+sign.getName()+", site "+
												sign.getSite(j).getName()+" on the right hand side is underspecified",loc);
									else {
										if(lnk1 != lnk2){
											warns.emplace_back("%s rule induces a link permutation on site '%s' of agent '%s'",loc);
											//a.t = BIND; a.trgt1 = {rhs_order[i].first,rhs_order[i].second,j,false};
											//a.trgt2 = {rhs_order[i].first,lnk2.first,lnk2.second,false};
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
								//a.t = FREE;a.trgt1 = {lhs_order[i].first,lhs_order[i].second,j};
								break;
							case Mixture::BIND://if semi error else warning(swap)
								lnk1 = rhs.follow(rhs_order[i].first,rhs_order[i].second,j);
								if(lnk1.first == rhs_order[i].second && lnk1.second == j)
									throw SemanticError("The link status of agent "+sign.getName()+", site "+
											sign.getSite(j).getName()+" on the right hand side is underspecified",loc);
								else {
									warns.emplace_back("%s link state of site '%s' of agent '%s' is changed although it is a semi-link in the left hand side",loc);
									//a.t = BIND; a.trgt1 = {lhs_order[i].first,lhs_order[i].second,j};
								}
								break;
							case Mixture::BIND_TO://if same nothing else error
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
						auto rhs_site = rhs_ag.getSite(j);
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
	//new nodes
	newNodes.reserve(rhs_order.size()-i);
	for(unsigned j = i; j < rhs_order.size(); j++){
		auto& rhs_ag = rhs.getAgent(rhs_order[j]);
		env.getSignature(rhs_ag.getId());
		newNodes.push_back(state::SiteGraph::Node(env.getSignature(rhs_ag.getId())));
		for(auto s : rhs_ag)
			newNodes[j-i].setState(s.first,s.second.state);
	}
	//deleted nodes
	for(unsigned j = i; i < lhs_order.size(); j++){
		auto& lhs_ag = lhs.getAgent(lhs_order[j]);
		Action a;
		//a.t = DELETE;a.trgt1 = {lhs_order[j].first,lhs_order[j].second};
	}

}

} /* namespace simulation */
