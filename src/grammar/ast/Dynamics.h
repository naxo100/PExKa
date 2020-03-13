/*
 * Dynamics.h
 *
 *  Created on: May 12, 2016
 *      Author: naxo
 */

#ifndef GRAMMAR_AST_DYNAMICS_H_
#define GRAMMAR_AST_DYNAMICS_H_

#include <list>
#include <string>
#include <utility>

#include "AlgebraicExpressions.h"

namespace grammar {
namespace ast {

using namespace std;



/** \brief Link of an Agent */
class Link : public Node {
public:
	/** \brief the type of this Link pattern/state. */
	enum LinkType {
		FREE,	///< The site is disconnected (s).
		VALUE,	///< The site is bound to a explicit site in the mixture (s!1).
		ANY,	///< The site's link-state is unknown (s!?).
		SOME,	///< The site is bound to a not explicit site (s!_).
		AG_SITE	///< The site is bound to a not explicit site but with a pattern type (s!t.A).
	};
	Link();
	Link(const location &l,LinkType t);
	Link(const location &l,LinkType t,int val);
	Link(const location &l,LinkType t,const Id &ag,const Id &s);
	Link(const Link &l);
	Link& operator=(const Link &l);
	~Link();
	const LinkType& getType() const;

	/** \brief Evaluate and sets the link state of a site in a Mixture.
	 *
	 *  @param env The Environment of the simulation.
	 *  @param mix_site The Site owner of this Link pattern. The Link state is setted here.
	 *  @param links A mapping of the binds already found in this Mixture pattern
	 *  in the format {link-id -\> (mix_agent-id,site-id)}.	Allows to find syntax problems.
	 *  @param mix_ag_site The mix_agent-id and site-id of this site.
	 *  @param allow_pattern True if the context of this Site allows pattern LinkType. */
	void eval(const pattern::Environment &env,
			pattern::Mixture::Site& mix_site,
			unordered_map<unsigned,list<pair<short,short> > > &links,
			const pair<short,short> &mix_ag_site,bool allow_pattern) const;

protected:
	LinkType type;
	//union { TODO: BUG when try to write ag_site
		unsigned int value;
		/** The pattern for AG_SITE Link::LinkType. In Kappa the pattern
		 *  is A(a!b.B) but this attribute stores the pair (B,b).		*/
		pair<Id,Id> ag_site;
	//};
};

/** \brief The state of an agent site. */
class SiteState : public Node{
public:
	/** \brief The type of the agent state */
	enum {
		EMPTY,	///< An empty state. As pattern it means any value, as instance it means default value.
		LABEL,	///< The site state has a explicit label value.
		RANGE,	///< A range of values expressed as (min,max). Only valid for agent signatures.
		AUX,	///< An aux-var will catch the value of the site state in the pattern.
		EXPR	///< An algebraic expression to set/match the site value.
	} type;
	/** \brief Used for catch inequations in patterns that use auxiliary vars. */
	enum {MIN_EQUAL=1,MAX_EQUAL=2};
	//union {
		list<Id> labels;
		Id aux;
		const Expression* val;
		const Expression* range[3];//{min,default,max}
		char flag;
	//};
	SiteState();
	SiteState(const location& loc, const list<Id> &labs);
	SiteState(const location& loc, const Expression* min,
			const Expression* max,const Expression* def=nullptr);
	SiteState(const location& loc,const Expression* value);
	SiteState(const location& loc,const Id &aux,
			const Expression* equal = nullptr);
	SiteState(const location& loc,const Id &aux,char _flag,
			const Expression* min=nullptr,const Expression* max=nullptr);
	~SiteState();

	/** \brief Returns a vector of the site labels.
	 *
	 * 	Do not test if there are duplicates.	 */
	void evalLabels(pattern::Signature::LabelSite& site) const;

	/** \brief Set min and max pointers to the BaseExpression
	 * of min and max values for range. (set default TODO).
	 *
	 * @returns true if this is an int range, false if float and
	 * raise an exception if other.	 */
	bool evalRange(pattern::Environment &env,const vector<state::Variable*> &consts,
			expressions::BaseExpression** expr_values) const;

	void show( string tabs = "" ) const;
};

/** \brief The Site AST. It stores the value state and the link state. */
class Site: public Node {
public:
	Site();
	Site(const location &l,const Id &id,const SiteState &s,const Link &lnk);

	/** \brief Evaluate a site of an agent signature. */
	void eval(pattern::Environment &env,const vector<state::Variable*> &consts,pattern::Signature &agent) const;

	/** \brief Evaluate a site in an agent pattern or instance init.
	 * @param env The environment of the simulation.
	 * @param consts %const statements evaluated as Variables. Site expression doesn't allow variables.
	 * @param id_agent The pair (mix_ag-id,Agent). mix_ag-id is the position of the agent in the mixture.
	 * @param mlinks A mapping of the binds already found in this pattern.
	 * @param ptrn_flag Flag-Byte with context information. The meaning of every bit in
	 * Expression::Info.
	 * @returns The pair (agent_sign_site-id,aux-name). Useful when this Site has an aux-pattern. 			*/
	pair<small_id,Id> eval(const pattern::Environment &env,const vector<state::Variable*> &consts,
			pair<short,pattern::Mixture::Agent&> id_agent,
			unordered_map<unsigned,list<two<short> > > &mlinks,char ptrn_flag = 0) const;
	//const Link& getLink();
	void show( string tabs = "" ) const;
protected:
	Id name;
	SiteState stateInfo;
	Link link;
};

/** \brief The AST of an agent kappa expression.
 *
 *  It can appear in a Signature (\%agent), in a pattern of a rule or
 *  an expression, or in the state initialization (%init). 				*/
class Agent: Node {
public:
	Agent();
	Agent(const location &l,const Id &id,const list<Site> s);

	/** \brief Evaluate an agent signature. */
	void eval(pattern::Environment &env,const vector<state::Variable*> &consts) const;

	/** \brief Evaluate an agent pattern or instance.
	 * A new pattern::Agent object is constructed and added to the mixture mix.
	 *
	 * @param env The environment of the simulation.
	 * @param consts Constants (that) can be used in pattern evaluation.
	 * @param mix The pattern::Mixture that contains this agent expression.
	 * @param lnks A mapping of the binds already found in this mixture.
	 * @param ptrn_flag Flag-Byte with context information. The meaning of every bit in
	 * Expression::Info. */
	void eval(const pattern::Environment &env,const vector<state::Variable*> &consts,pattern::Mixture &mix,
			unordered_map<unsigned,list <two<short> > > &lnks, char ptrn_flag) const;

	void show( string tabs = "" ) const;

protected:
	Id name;
	list<Site> sites;
};

/** \brief The AST of an agent pattern/instance. */
class Mixture : public Node {
protected:
	list<Agent>  agents;	///< The agent patterns of this mixture.
public:
	//enum {PATTERN=1,LHS=2,RHS=4};
	using Info = Expression::Info;
	Mixture();
	Mixture(const location &l,const list<Agent> &m);

	/** \brief Evaluate the agent patterns in this mixture and constructs
	 * a new pattern::Mixture object.
	 *
	 * The connected components of the mixture are not set.
	 * If the mixture is a pattern, it should be declared in the Environment.
	 * @param env The environment of the simulation.
	 * @param vars The variables and constants of the simulation.
	 * @param Flag-Byte with context information. See Expression::Info.	 */
	virtual pattern::Mixture* eval(const pattern::Environment &env,
			const vector<Variable*> &vars,char ptrn_flag = true) const;
	void show(string tabs = "") const;
	virtual ~Mixture();
};

/** \brief The AST of a triggering effect in a Perturbation. */
class Effect : public Node {
public:
	enum Action {
		INTRO,		///< Add new agent instances ($ADD).
		DELETE,		///< Delete agent instances that match a pettern ($DEL).
		UPDATE,		///< Sets a new value to a Variable (\%var) ($UPDATE).
		UPDATE_TOK,	///< Sets a new value to a token variable (\<-).
		STOP,		///< Stop the simulation.
		SNAPSHOT,	///< Save the current state of the simulation to a file.
		PRINT,		///< Write text to the standard output.
		PRINTF,		///< Write text to a file.
		CFLOW,
		CFLOW_OFF,
		FLUX,
		FLUX_OFF
	};

	Effect();
	//INTRO,DELETE
	Effect(const location &l,const Action &a,const Expression *n ,list<Agent>& mix);
	//UPDATE,UPDATE_TOK
	Effect(const location &l,const Action &a,const VarValue &dec);
	//CFLOW,CFLOW_OFF
	Effect(const location &l,const Action &a,const Id &id);
	//STOP,SNAPSHOT,FLUX,FLUXOFF,PRINT
	Effect(const location &l,const Action &a,const list<StringExpression> &str);
	//PRINTF
	Effect(const location &l,const Action &a,const list<StringExpression> &str1,
			const list<StringExpression> &str2);
	//HISTOGRAM
	Effect(const location &l,const list<StringExpression> &str,
			const VarValue &bins,list<Agent>& mix,const Expression* aux_expr = nullptr);

	//Effect(const Effect &eff);
	//Effect& operator=(const Effect& eff);

	simulation::Perturbation::Effect* eval(pattern::Environment& env,
			const vector<state::Variable*> &vars) const;

	void show(string tabs = "") const;

	~Effect();

protected:
	Action action;
	const Expression* n;
	Mixture* mix;
	VarValue set;
	Id name;
	//const StringExpression* string_expr;
	list<StringExpression> string_expr;
	list<StringExpression> filename;

	// not working - it does not work
	/*union {
		VarValue set;
		Id name;
		StringExpression* string_expr;
	};*/
};

/** \brief The AST of a Kappa perturbation. */
class Pert: public Node {
public:
	Pert();
	Pert(const location &l,const Expression *cond,
			const list<Effect> &effs,const Expression* rep = nullptr);
	void show(string tabs = "") const;
	~Pert();

	void eval(pattern::Environment& env,const vector<state::Variable*>& vars) const;
protected:
	const Expression *condition,*until;
	list<Effect> effects;
};

/** \brief The rate and radius for the unary instance of a rule. */
struct Radius : Node {
	const Expression *k1;
	const Expression *opt;
public:
	Radius();
	Radius(const location &l,const Expression *k1);
	Radius(const location &l,const Expression *k1,const Expression *opt);
	~Radius();
};

/** \brief The AST of the rate(s) of a rule. */
class Rate : Node {
	const Expression *base;		///< The base rate of the rule.
	const Expression *reverse;	///< The rate for the backward rule.
	Radius *unary;				///< The rate for unary instances of the rule.
	bool volFixed;				///< do not depend on volume
	bool fixed;					///< do not depend on concentrations
public:
	Rate();
	Rate(const Rate& rate);
	Rate& operator=(const Rate& rate);
	Rate(const location &l,const Expression *def,const bool fix);
	Rate(const location &l,const Expression *def,const bool fix,const Radius *un);
	Rate(const location &l,const Expression *def,const bool fix,const Expression *op);
	const state::BaseExpression* eval(const pattern::Environment& env,simulation::Rule& r,
			const vector<state::Variable*> &vars,two<pattern::DepSet>& deps,
			bool is_bi = false) const;
	~Rate();
};

class Token : Node {
	const Expression *exp;
	Id id;
public:
	Token();
	Token(const location &l,const Expression *e,const Id &id);
	pair<unsigned,const BaseExpression*> eval(const pattern::Environment& env,
			const vector<state::Variable*> &vars,bool neg = false) const;
};

/** \brief The AST to store Agents and Tokens that are part of
 * either the right or left side in a Rule.						*/
class RuleSide : Node{
public:
	Mixture agents;
	list<Token> tokens;
public:
	RuleSide();
	RuleSide(const location &l,const Mixture &agents,const list<Token> &tokens);
};



/** \brief The AST of a kappa rule. */
class Rule : public Node {
protected:
	Id label;
	RuleSide lhs,rhs;
	bool bi;
	const Expression* filter;
	Rate rate;

	static size_t count;
public:
	Rule();
	Rule(const location &l,const Id &label,const RuleSide &lhs,
		const RuleSide &rhs,const bool arrow,const Expression* where,const Rate &rate);

	Rule(const Rule& rule);
	Rule& operator=(const Rule& rule);

	void eval(pattern::Environment& env,const vector<state::Variable*>& vars) const;

	static size_t getCount();


	~Rule();
};


} /* namespace ast */
}

#endif /* GRAMMAR_AST_DYNAMICS_H_ */
