/*
 * Rule.h
 *
 *  Created on: May 9, 2017
 *      Author: naxo
 */

#ifndef SRC_SIMULATION_RULE_H_
#define SRC_SIMULATION_RULE_H_

#include "../util/params.h"
#include "../pattern/mixture/Mixture.h"
#include "../grammar/location.hh"
#include "../state/SiteGraph.h"
#include <list>
#include <unordered_map>

//class pattern::Environment;

namespace matching {
	class InjRandTree;
}


namespace simulation {

using namespace std;
using namespace pattern;
using namespace expressions;


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
		CHANGE,
		LINK,
		UNBIND,
		DELETE,//id_lhs
		CREATE,//id_rhs,id_ag_mix
		ASSIGN,
		TRANSPORT//id_lhs,id_trgt_comp
	};
	/** \brief Target mix_agent and the action to be applied.
	 */
	struct Action {
		ActionType t;
		//	cc_id/val	ag_id	 sit_id	  flag
		tuple<small_id,small_id,small_id,small_id> trgt1,trgt2;
		const state::BaseExpression* value;
		enum {NEW=1,S_EFF=2};
	};

	struct CandidateInfo {
		bool is_valid;
		bool is_new;
		set<int> infl_by;//negative for lnk change
		//small_id cc_root;
		map<ag_st_id,small_id> node_id;//(lhs_cc,lhs_cc_ag) -> cc_root 		XXXXX //lhs_cc-> (lhs_cc_ag,cc_root)
		CandidateInfo();
		void setRoot(small_id root,ag_st_id node,int change);
	};
	typedef map<const pattern::Mixture::Component*,CandidateInfo> CandidateMap;
	class Rate {
	protected:
		const Rule& rule;
		BaseExpression* baseRate;
		pair<BaseExpression*,int> unaryRate;
	public:
		Rate(const Rule& r,state::State& state);
		virtual ~Rate();

		virtual two<FL_TYPE> evalActivity(const state::State& state) const = 0;
		virtual const BaseExpression* getExpression(small_id cc_index = 0) const;
		//CandidateMap influence;
	};
protected:
	yy::location loc;
	int id;
	string name;
	const Mixture &lhs;
	const Mixture *rhs;
	bool isRhsDeclared;
	vector<ag_st_id> lhsMask,rhsMask;//[order] -> (comp_id,ag_id)
	const BaseExpression *rate;//basic rate
	pair<const BaseExpression*,int> unaryRate;//rate,radius
	//BaseExpression::Reduction basic,unary;
	list<Action> script;
	vector<state::Node*> newNodes;
	map<pair<ag_st_id,bool>,two<list<small_id > > > changes;//(rhs_ag,new?) -> modified_sites ( [by_value],[by_lnk] )
	list<ag_st_id> sideEffects;
	map<ag_st_id,small_id> news; //rhs(cc,ag) -> newNodes[i]
	CandidateMap influence;
	map<ag_st_id,ag_st_id> matches;//rhs-ag -> lhs-ag
	list<pair<unsigned,const BaseExpression*> > tokenChanges;

	bool test_linked_agents(list<two<ag_st_id>>& to_test,small_id rhs_cc_id,
		const Mixture::Component& test_cc,multimap<ag_st_id,ag_st_id>& already_done,
		const VarVector& vars,const Environment& env) const;

public:
	/** \brief Initialize a rule with a declared kappa label and its LHS.
	 * @param nme Declared kapa label of rule.
	 * @param mix LHS of the rule.
	 */
	Rule(int _id,const string& nme, const Mixture& mix,const yy::location& _loc);
	~Rule();

	int getId() const;
	const string& getName() const;
	const Mixture& getLHS() const;
	const Mixture& getRHS() const;
	const BaseExpression& getRate() const;
//	const BaseExpression::Reduction& getReduction() const;
	const BaseExpression& getUnaryRate() const;
//	const BaseExpression::Reduction& getUnaryReduction() const;
	const CandidateMap& getInfluences() const;
	const list<pair<unsigned,const BaseExpression*> > getTokenChanges() const;

	//two<FL_TYPE> evalActivity(const matching::InjRandContainer* const * injs,const VarVector& vars) const;

	/** \brief Set RHS of the rule.
	 * If this is a reversible rule, mix is declared in env and should not be
	 * freed by rule destructor.
	 */
	void setRHS(const Mixture* mix,bool is_declared = false);
	/** \brief Set basic rate for the rule.
	 * @param r basic rate of the rule.
	 */
	void setRate(const BaseExpression* r);
	/** \brief Set the unary rate for the rule.
	 * @param u_rate pair of (rate,radius) for unary instances of a binary rule.
	 * @param Radius indicate the range of search for connectivity.
	 */
	void setUnaryRate(pair<const BaseExpression*,int> u_rate = make_pair(nullptr,0));

	/** \brief Calculate actions to apply using the difference between LHS and RHS.
	 * Actions could create, delete, bind or unbind nodes, and change internal state
	 * of node sites. This method match the first agents of LHS and RHS by its signId,
	 * preserving those nodes, and after first unmatched agents, delete every node from
	 * LHS and create new nodes for every extra agent in RHS.
	 * @param env the environment of the simulation.
	 * @param lhs_order initial order of LHS agents (obtained from Mixture::declare()).
	 * @param rhs_order initial order of RHS agents.
	 * @param consts to evaluate const-expressions.
	 */
	void difference(const Environment& env,const vector<ag_st_id>& lhs_order,
			const vector<ag_st_id>& rhs_order, const VarVector& consts);

	void addTokenChange(pair<unsigned,const BaseExpression*> tok);

	const list<Action>& getScript() const;
	const vector<state::Node*>& getNewNodes() const;

	static void addAgentIncludes(CandidateMap &m,
		const Pattern::Agent& ag,ag_st_id rhs_cc_ag,int change);


	void checkInfluence(const Environment &env,const VarVector& vars);
	void initialize(const state::State& state,VarVector& vars);


	string toString(const pattern::Environment& env) const;
};



class NormalRate : public virtual Rule::Rate {
public:
	NormalRate(const Rule& r,state::State& state);
	virtual ~NormalRate();

	two<FL_TYPE> evalActivity(const state::State& state) const override;
};

class SamePtrnRate : public virtual Rule::Rate {
protected:
	float norm;
public:
	SamePtrnRate(const Rule& r,state::State& state,bool normalize = false);
	virtual ~SamePtrnRate();

	two<FL_TYPE> evalActivity(const state::State& state) const override;
};

class AuxDepRate : public virtual Rule::Rate {
protected:
	BaseExpression::Reduction base,unary;
public:
	AuxDepRate(const Rule& r,state::State& state);
	virtual ~AuxDepRate();

	virtual const BaseExpression* getExpression(small_id cc_index = 0) const override;
	virtual two<FL_TYPE> evalActivity(const state::State& state) const override;
};

class SameAuxDepRate : public SamePtrnRate,public AuxDepRate {
public:
	SameAuxDepRate(const Rule& r,state::State& state,bool normalize);
	virtual ~SameAuxDepRate();

	virtual two<FL_TYPE> evalActivity(const state::State& state) const override;
};




} /* namespace simulation */

#endif /* SRC_SIMULATION_RULE_H_ */
