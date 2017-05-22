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
#include <list>


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
		tuple<small_id,small_id,bool> trgt1,trgt2;
	};

	const Mixture &lhs;
	const Mixture rhs;
	const state::BaseExpression* rate;//basic rate
	pair<const state::BaseExpression*,const state::BaseExpression*> unaryRate;//rate,radius
	list<Action> script;
public:
	//Rule();
	~Rule();



	static list<Action> difference(const Mixture& lhs,const Mixture& rhs);
};

} /* namespace simulation */

#endif /* SRC_SIMULATION_RULE_H_ */
