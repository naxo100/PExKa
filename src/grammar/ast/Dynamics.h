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

#include "AlgebraicExpressions.h"

namespace ast {

using namespace std;

/** \brief Direction of a Reaction
 *
 */
class Arrow : public Node {
public:
	Arrow();
	enum ArrType {RIGHT,LEFT,BI};
	Arrow(const location &l,ArrType t);
	ArrType getType();
protected:
	ArrType type;
};

//Link of an Agent
class Link : public Node {
public:
	enum LinkType {VALUE,FREE,ANY,SOME,AG_SITE};
	Link();
	Link(const location &l,LinkType t);
	Link(const location &l,LinkType t,int val);
	Link(const location &l,LinkType t,const Id &ag,const Id &s);
	Link(const Link &l);
	Link& operator=(const Link &l);
	~Link();

protected:
	LinkType type;
	union {
		int value;
		struct {
			Id agent;
			Id site;
		} ag_site;
	};
};
class Site: public Node {
public:
	Site();
	Site(const location &l,const Id &id,const list<Id> &s,const Link &lnk);
	void eval(pattern::Environment &env,pattern::Signature &agent);
	void eval(pattern::Environment &env,pattern::Mixture::Agent &agent);
protected:
	Id id;
	list<Id> states;
	Link link;
};

//Agent created by User
class Agent: Node {
public:
	Agent();
	Agent(const location &l,const Id &id,const list<Site> s);

	pattern::Signature* eval(pattern::Environment &env);
	pattern::Mixture::Agent* eval(pattern::Environment &env,bool is_pattern);
protected:
	Id id;
	list<Site> sites;
};


class Mixture : public Node {
protected:
	list<Agent>  mix;
public:
	Mixture();
	Mixture(const location &l,const list<Agent> &m);
	virtual pattern::Mixture eval(pattern::Environment &env) const;
	virtual ~Mixture();
};

class MultipleMixture : public Mixture{
	const Expression *n;
public:
	MultipleMixture();
	MultipleMixture(const location &l,const list<Agent> &m,
			const Expression *e);

	pattern::Mixture eval(pattern::Environment &env) const;
	~MultipleMixture();
};

class Effect : public Node {
public:
	enum Action {INTRO,DELETE,UPDATE,UPDATE_TOK,STOP,SNAPSHOT,PRINT,PRINTF,CFLOW,CFLOW_OFF,FLUX,FLUX_OFF};

	Effect();
	//INTRO,DELETE
	Effect(const location &l,const Action &a,const MultipleMixture &mix);
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
	union {
		MultipleMixture multi_exp;
		VarValue set;
		Id name;
		StringExpression* string_expr;
	};
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


class Radius : Node {
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
	const Expression *def;
	bool fixed;
	Radius *un;
	const Expression *op;
public:
	Rate();
	Rate(const location &l,const Expression *def,const bool &fix);
	Rate(const location &l,const Expression *def,const bool &fix,const Radius *un);
	Rate(const location &l,const Expression *def,const bool &fix,const Expression *op);
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
	Arrow arrow;
	const Expression* filter;
	Rate rate;
public:
	Rule();
	Rule(const location &l,const Id &label,const RuleSide &lhs,
		const RuleSide &rhs,const Arrow &arrow,const Expression* where,const Rate &rate);
	~Rule();
};


} /* namespace ast */

#endif /* GRAMMAR_AST_DYNAMICS_H_ */
