/*
 * Rule.h
 *
 *  Created on: May 9, 2017
 *      Author: naxo
 */

#ifndef SRC_SIMULATION_RULE_H_
#define SRC_SIMULATION_RULE_H_

#include "../util/params.h"
#include "../pattern/Mixture.h"
#include "../grammar/location.hh"
#include <list>

//class pattern::Environment;

namespace simulation {

using namespace std;
using namespace pattern;

class Rule {
	enum ActionType {
		CREATE,//id_rhs,id_ag_mix
		DELETE,//id_lhs
		TRANSPORT,//id_lhs,id_trgt_comp
		CHANGE,
		BIND,
		FREE
	};
	struct Action {
		ActionType t;
		//		cc_id	ag_id	sit_id	isNew
		tuple<small_id,small_id,small_id,bool> trgt1,trgt2;
	};

	yy::location loc;
	string name;
	const Mixture &lhs;
	const Mixture rhs;
	vector<ag_st_id> lhsMask,rhsMask;//[order] -> (comp_id,ag_id)
	const state::BaseExpression* rate;//basic rate
	pair<const state::BaseExpression*,const state::BaseExpression*> unaryRate;//rate,radius
	list<Action> script;
	vector<state::SiteGraph::Node> newNodes;
public:
	//Rule();
	~Rule();



	list<Action> difference(const Environment& env,const vector<ag_st_id>& lhs_order,const vector<ag_st_id>& rhs_order);
};

} /* namespace simulation */

#endif /* SRC_SIMULATION_RULE_H_ */
