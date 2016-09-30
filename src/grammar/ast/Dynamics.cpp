/*
 * Dynamics.cpp
 *
 *  Created on: May 12, 2016
 *      Author: naxo
 */

#include "Dynamics.h"

namespace ast {

/****** Class Arrow **********/
Arrow::Arrow(): type(RIGHT){}
Arrow::Arrow(const location &loc,ArrType t):
		Node(loc), type(t) {};
Arrow::ArrType Arrow::getType(){
	return type;
};


/****** Class Link ***********/
Link::Link() : type(FREE){}
Link::Link(const location &l,LinkType t):
	Node(l), type(t)  {};
Link::Link(const location &l,LinkType t,int val):
	Node(l), type(t), value(val) {};
Link::Link(const location &l,LinkType t,const Id &ag,const Id &s):
	Node(l), type(t), ag_site{ag,s} {	};
Link::Link(const Link &l):Node(l){
	type = l.type;
	switch(type){
	case VALUE:
		value = l.value;
		break;
	case AG_SITE:
		ag_site = l.ag_site;
		break;
	default:
		break;
	}
};
Link& Link::operator=(const Link &l){
	loc = l.loc;
	type = l.type;
	switch(type){
	case VALUE:
		value = l.value;
		break;
	case AG_SITE:
		ag_site = l.ag_site;
		break;
	default:
		break;
	}
	return *this;
};
Link::~Link(){};


/****** Class Site ***********/
Site::Site(){}
Site::Site(const location &l,const Id &id,const list<Id> &s,const Link &lnk):
	Node(l), id(id), states(s), link(lnk) {};


/****** Class Agent **********/
Agent::Agent(){}
Agent::Agent(const location &l,const Id &id,const list<Site> s):
	Node(l), id(id),sites(s) {};




/****** Class Mixture ************/
Mixture::Mixture(const location &l,const list<Agent> &m):
	Node(l), mix(m) {};

/*TODO*/
pattern::Mixture Mixture::eval(pattern::Environment &env) const{

	for(list<Agent>::const_iterator it = mix.cbegin();it != mix.cend();it++){
		//it->;
	}
	return pattern::Mixture();
}


/****** Class MultipleMixture ****/
//MultipleMixture::MultipleMixture(): n(nullptr){};
MultipleMixture::MultipleMixture(const location &l,const list<Agent> &m,const Expression *e):
	Mixture(l,m), n(e) {};

/*TODO*/
pattern::Mixture MultipleMixture::eval(pattern::Environment &env) const{

	for(list<Agent>::const_iterator it = mix.cbegin();it != mix.cend();it++){
		//it->;
	}
	return pattern::Mixture();
}


/****** Class Effect *************/
Effect::Effect(){}
	//INTRO,DELETE
Effect::Effect(const location &l,const Action &a,const MultipleMixture &mix):
	Node(l),action(a),multi_exp(mix) {};
//UPDATE,UPDATE_TOK
Effect::Effect(const location &l,const Action &a,const VarValue &dec):
	Node(l),action(a),set(dec) {};
//CFLOW,CFLOW_OFF
Effect::Effect(const location &l,const Action &a,const Id &id):
	Node(l),action(a),name(id) {};
//STOP,SNAPSHOT,FLUX,FLUXOFF,PRINT
Effect::Effect(const location &l,const Action &a,const StringExpression &str):
	Node(l),action(a),string_expr(new StringExpression(str)) {};
//PRINTF
Effect::Effect(const location &l,const Action &a,const StringExpression &str1,const StringExpression &str2):
		Node(l),action(a),string_expr(new StringExpression[2]{str1,str2}) {};

Effect::Effect(const Effect &eff) : Node(eff.loc),action(eff.action){
	switch(action){
	case INTRO:case DELETE:
		multi_exp = eff.multi_exp;
		break;
	case UPDATE:case UPDATE_TOK:
		set = eff.set;
		break;
	case CFLOW:case CFLOW_OFF:
		name = eff.name;
		break;
	case STOP:case SNAPSHOT:case FLUX:case FLUX_OFF:case PRINT:
		string_expr = new StringExpression(*eff.string_expr);
		break;
	case PRINTF:
		string_expr = new StringExpression[2]{eff.string_expr[0],eff.string_expr[1]};
	}
}

Effect& Effect::operator=(const Effect& eff){
	loc = eff.loc;
	action = eff.action;
	switch(action){
	case INTRO:case DELETE:
		multi_exp = eff.multi_exp;
		break;
	case UPDATE:case UPDATE_TOK:
		set = eff.set;
		break;
	case CFLOW:case CFLOW_OFF:
		name = eff.name;
		break;
	case STOP:case SNAPSHOT:case FLUX:case FLUX_OFF:case PRINT:
		string_expr = new StringExpression(*eff.string_expr);
		break;
	case PRINTF:
		string_expr = new StringExpression[2]{eff.string_expr[0],eff.string_expr[1]};
	}
	return *this;
}

Effect::~Effect(){
	switch(action){
	case STOP:case SNAPSHOT:case FLUX:case FLUX_OFF:case PRINT:
		delete string_expr;
		break;
	case PRINTF:
		delete[] string_expr;
		break;
	default:
		break;
	}
}


/****** Class Perturbation *******/
Perturbation::Perturbation() : condition(nullptr),until(nullptr){};
Perturbation::Perturbation(const location &l,const Expression *cond,const list<Effect> &effs,const Expression* rep):
	Node(l), condition(cond), until(rep), effects(effs){};

Perturbation::~Perturbation() {
	if(until)
		delete until;
};


/****** Class Radius ****/
Radius::Radius(){}
Radius::Radius(const location &l,const Expression *k1):
	Node(l),k1(k1), opt(NULL) {};
Radius::Radius(const location &l,const Expression *k1,const Expression *opt):
	Node(l),k1(k1), opt(opt) {};
Radius::~Radius(){
	if(opt)
		delete opt;
}


/****** Class Rate ***************/
Rate::Rate(){}
Rate::Rate(const location &l,const Expression *def,const bool &fix):
	Node(l), def(def), fixed(fix), un(NULL), op(NULL) {};
Rate::Rate(const location &l,const Expression *def,const bool &fix,const Radius *un):
	Node(l), def(def), fixed(fix), un(new Radius(*un)), op(NULL) {};
Rate::Rate(const location &l,const Expression *def,const bool &fix,const Expression *ope):
	Node(l), def(def), fixed(fix), un(NULL), op(ope) {};
Rate::~Rate(){
	if(un)
		delete un;
	if(op)
		delete op;
}

/****** Class Token **************/
Token::Token(){}
Token::Token(const location &l,const Expression *e,const Id &id):
	Node(l), exp(e), id(id) {};

/****** Class RuleSide ***********/
RuleSide::RuleSide() : agents(location(),list<Agent>()){}
RuleSide::RuleSide(const location &l,const Mixture &agents,const list<Token> &tokens):
	Node(l), agents(agents), tokens(tokens) {};

/****** Class Rule ***************/

Rule::Rule(){}
Rule::Rule(	const location &l,
		const Id          &label,
		const RuleSide    &lhs,
		const RuleSide    &rhs,
		const Arrow       &arrow,
		const Expression* where,
		const Rate 		  &rate):
	Node(l), label(label), lhs(lhs), rhs(rhs),
	arrow(arrow),filter(where),rate(rate) {};
Rule::~Rule() {};

} /* namespace ast */
