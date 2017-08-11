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
#include "../state/SiteGraph.h"
#include <list>

//class pattern::Environment;

namespace simulation {

using namespace std;
using namespace pattern;

class Rule {
public:
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
		//	cc_id/val	ag_id	 sit_id	  isNew
		tuple<small_id,small_id,small_id,small_id> trgt1,trgt2;
	};
protected:
	yy::location loc;
	string name;
	const Mixture &lhs;
	const Mixture *rhs;
	bool isRhsDeclared;
	vector<ag_st_id> lhsMask,rhsMask;//[order] -> (comp_id,ag_id)
	const state::BaseExpression *rate;//basic rate
	pair<const state::BaseExpression*,int> unaryRate;//rate,radius
	list<Action> script;
	vector<state::SiteGraph::Node> newNodes;
public:
	Rule(const ast::Id& nme, const Mixture& mix);
	~Rule();

	void setRHS(const Mixture* mix,bool is_declared = false);
	void setRate(const state::BaseExpression* r);
	void setUnaryRate(pair<const state::BaseExpression*,int> u_rate = make_pair(nullptr,0));

	void difference(const Environment& env,const vector<ag_st_id>& lhs_order,const vector<ag_st_id>& rhs_order);

	const list<Action>& getScript() const;
	const vector<state::SiteGraph::Node>& getNewNodes() const;

	string toString(const pattern::Environment& env) const;
};

} /* namespace simulation */

#endif /* SRC_SIMULATION_RULE_H_ */
