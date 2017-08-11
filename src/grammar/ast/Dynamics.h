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

namespace ast {

using namespace std;



//Link of an Agent
class Link : public Node {
public:			//       !1    !?  !_   !A.s
	enum LinkType {FREE,VALUE,ANY,SOME,AG_SITE};
	Link();
	Link(const location &l,LinkType t);
	Link(const location &l,LinkType t,int val);
	Link(const location &l,LinkType t,const Id &ag,const Id &s);
	Link(const Link &l);
	Link& operator=(const Link &l);
	~Link();
	const LinkType& getType() const;

	void eval(const pattern::Environment &env,
			pattern::Mixture::Site& mix_site,
			unordered_map<unsigned,list<pair<short,short> > > &,
			const pair<short,short> &mix_ag_site,bool allow_pattern) const;

protected:
	LinkType type;
	//union { TODO: BUG when try to write ag_site
		unsigned int value;
		pair<Id,Id> ag_site;
	//};
};

class SiteState : public Node{
public:
	enum {EMPTY,LABEL,RANGE,AUX,EXPR} type;
	//union {
		list<Id> labels;
		Id aux;
		const Expression* val;
		const Expression* range[3];//{min,default,max}
	//};
	SiteState();
	SiteState(const location& loc, const list<Id> &labs);
	SiteState(const location& loc, const Expression* min,
			const Expression* max,const Expression* def=nullptr);
	SiteState(const location& loc,const Id &aux);
	~SiteState();

	/** \brief return a vector of the site labels.
	 * 	Do not test if there are duplicates.
	 *
	 */
	void evalLabels(pattern::Signature::LabelSite& site) const;
	/** \brief Set min and max pointers to the BaseExpression
	 * of min and max values for range. (set default TODO).
	 * return true if this is an int range, false if float and
	 * raise an exception if other.
	 *
	 */
	bool evalRange(pattern::Environment &env,const vector<state::Variable*> &consts,
			BaseExpression** expr_values) const;

	void show( string tabs = "" ) const;
};

class Site: public Node {
public:
	Site();
	Site(const location &l,const Id &id,const SiteState &s,const Link &lnk);
	void eval(pattern::Environment &env,const vector<state::Variable*> &consts,pattern::Signature &agent) const;
	void eval(const pattern::Environment &env,const vector<state::Variable*> &consts,
			pair<short,pattern::Mixture::Agent&> id_agent,
			unordered_map<unsigned,list<pair<short,short> > > &m) const;
	//const Link& getLink();
	void show( string tabs = "" ) const;
protected:
	Id name;
	SiteState stateInfo;
	Link link;
};

//Agent created by User
class Agent: Node {
public:
	Agent();
	Agent(const location &l,const Id &id,const list<Site> s);

	void eval(pattern::Environment &env,const vector<state::Variable*> &consts) const;
	void eval(const pattern::Environment &env,const vector<state::Variable*> &consts,pattern::Mixture &mix,
			unordered_map<unsigned,list <pair<short,short> > > &lnks, bool is_pattern) const;

	void show( string tabs = "" ) const;

protected:
	Id name;
	list<Site> sites;
};


class Mixture : public Node {
protected:
	list<Agent>  agents;
public:
	Mixture();
	Mixture(const location &l,const list<Agent> &m);
	virtual pattern::Mixture* eval(const pattern::Environment &env,
			const vector<Variable*> &vars,bool is_pattern = true) const;
	virtual ~Mixture();
};
/*
class MultipleMixture : public Mixture{
	const Expression *n;
public:
	MultipleMixture();
	MultipleMixture(const location &l,const list<Agent> &m,
			const Expression *e);

	pattern::Mixture& eval(pattern::Environment &env) const;
	MultipleMixture& operator=( const MultipleMixture& multimix );
	~MultipleMixture();
};
*/
class Effect : public Node {
public:
	enum Action {INTRO,DELETE,UPDATE,UPDATE_TOK,STOP,SNAPSHOT,PRINT,PRINTF,CFLOW,CFLOW_OFF,FLUX,FLUX_OFF};

	Effect();
	//INTRO,DELETE
	// Effect(const location &l,const Action &a,const MultipleMixture &mix);
	//Effect(const location &l,const Action &a, pair<Expression*,Mixture> &mix);
	Effect(const location &l,const Action &a,const Expression *n ,list<Agent>& mix);
	//UPDATE,UPDATE_TOK
	Effect(const location &l,const Action &a,const VarValue &dec);
	//CFLOW,CFLOW_OFF
	Effect(const location &l,const Action &a,const Id &id);
	//STOP,SNAPSHOT,FLUX,FLUXOFF,PRINT
	Effect(const location &l,const Action &a,const StringExpression &str);
	//PRINTF
	Effect(const location &l,const Action &a,const StringExpression &str1,const StringExpression &str2);

	Effect(const Effect &eff);
	Effect& operator=(const Effect& eff);

	~Effect();

protected:
	Action action;
	const Expression* n;
	Mixture *mix;
	VarValue set;
	Id name;
	StringExpression* string_expr;

	// not working - it does not work
	/*union {
		VarValue set;
		Id name;
		StringExpression* string_expr;
	};*/
};


class Perturbation: public Node {
public:
	Perturbation();
	Perturbation(const location &l,const Expression *cond,
			const list<Effect> &effs,const Expression* rep = nullptr);

	~Perturbation();
protected:
	const Expression *condition,*until;
	list<Effect> effects;
};


struct Radius : Node {
	const Expression *k1;
	const Expression *opt;
public:
	Radius();
	Radius(const location &l,const Expression *k1);
	Radius(const location &l,const Expression *k1,const Expression *opt);
	~Radius();
};

//Structure to storages Rate Values
//def: (defined)   precise    rate for rule in the right direction
//un : (undefined) a range of rate for rule in the right direction (optional)
//op : (defined)   precise    rate for rule in the left  direction (optional)
class Rate : Node {
	const Expression *base;
	const Expression *reverse;
	Radius *unary;
	bool volFixed;//do not depend on volume
	bool fixed;//do not depend on concentrations
public:
	Rate();
	Rate(const Rate& rate);
	Rate& operator=(const Rate& rate);
	Rate(const location &l,const Expression *def,const bool fix);
	Rate(const location &l,const Expression *def,const bool fix,const Radius *un);
	Rate(const location &l,const Expression *def,const bool fix,const Expression *op);
	const state::BaseExpression* eval(const pattern::Environment& env,simulation::Rule& r,
			const vector<state::Variable*> &vars,bool is_bi = false) const;
	~Rate();
};

class Token : Node {
	const Expression *exp;
	Id id;
public:
	Token();
	Token(const location &l,const Expression *e,const Id &id);
};

//Structure to storage Agents and Tokens that are part of either the right or left side in a Rule
//Agents: list of Agents
//Tokens: list of Tokens used
class RuleSide : Node{
public:
	Mixture agents;
	list<Token> tokens;
public:
	RuleSide();
	RuleSide(const location &l,const Mixture &agents,const list<Token> &tokens);
};



//Class to storage all data about a rule
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

#endif /* GRAMMAR_AST_DYNAMICS_H_ */
