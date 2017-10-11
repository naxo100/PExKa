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


/** \brief Basic unit of the simulation dynamics.
 *
 * Rules are the basic unit of the simulation dynamics. They will be applied
 * to Nodes in a SiteGraph that match with its LHS. Actions are the changes
 * that the rule produce in those nodes. difference() must be called before
 * trying to apply the rule.
 */
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
	/** \brief Target mix_agent and the action to be applied.
	 */
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
	/** \brief Initialize a rule with a declared kappa label and its LHS.
	 * @param nme Declared kapa label of rule.
	 * @param mix LHS of the rule.
	 */
	Rule(const ast::Id& nme, const Mixture& mix);
	~Rule();

	/** \brief Set RHS of the rule.
	 * If this is a reversible rule, mix is declared in env and should not be
	 * freed by rule destructor.
	 */
	void setRHS(const Mixture* mix,bool is_declared = false);
	/** \brief Set basic rate for the rule.
	 * @param r basic rate of the rule.
	 */
	void setRate(const state::BaseExpression* r);
	/** \brief Set the unary rate for the rule.
	 * @param u_rate pair of (rate,radius) for unary instances of a binary rule.
	 * @param Radius indicate the range of search for connectivity.
	 */
	void setUnaryRate(pair<const state::BaseExpression*,int> u_rate = make_pair(nullptr,0));

	/** \brief Calculate actions to apply using the difference between LHS and RHS.
	 * Actions could create, delete, bind or unbind nodes, and change internal state
	 * of node sites. This method match the first agents of LHS and RHS by its signId,
	 * preserving those nodes, and after first unmatched agents, delete every node from
	 * LHS and create new nodes for every extra agent in RHS.
	 * @param env the environment of the simulation.
	 * @param lhs_order initial order of LHS agents (obtained from Mixture::declare()).
	 * @param rhs_order initial order of RHS agents.
	 */
	void difference(const Environment& env,const vector<ag_st_id>& lhs_order,const vector<ag_st_id>& rhs_order);

	const list<Action>& getScript() const;
	const vector<state::SiteGraph::Node>& getNewNodes() const;

	string toString(const pattern::Environment& env) const;
};

} /* namespace simulation */

#endif /* SRC_SIMULATION_RULE_H_ */
