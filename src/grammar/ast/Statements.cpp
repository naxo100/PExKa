/*
 * Statements.cpp
 *
 *  Created on: May 12, 2016
 *      Author: naxo
 */

#include "Statements.h"
#include "../../util/Exceptions.h"

namespace ast {



/****** Class Declaration *******/
Declaration::Declaration() : type(ALG),expr(NULL){};
Declaration::Declaration(const location &l,const Id &lab,const Expression *e):
	Node(loc),name(lab),type(ALG),expr(e) {};

Declaration::Declaration(const location &l,const Id &lab,const list<Agent> &m):
	Node(loc),name(lab),type(KAPPA),mixture(new MultipleMixture(m)) {};

Declaration::Declaration(const Declaration &d) : name(d.name),type(d.type){
	if(type)
		mixture = new MultipleMixture(*(d.mixture));
	else
		if(d.expr) expr = d.expr->clone();
		else expr=NULL;
}

Declaration& Declaration::operator=(const Declaration &d){
	name = d.name;
	loc = d.loc;
	type = d.type;
	if(expr)
		delete expr;

	if(type)
		mixture = new MultipleMixture(*(d.mixture));
	else
		if(d.expr) expr = d.expr->clone();
		else expr=NULL;
	return *this;
}
int Declaration::count = 0;

/*
Declaration::Declaration(const Declaration &&d) : type(d.type){
	cout << "const-copying decl.expr = " << d.expr << endl;
	if(type)
		mixture = new list<Agent>(*(d.mixture));
	else
		if(d.expr) expr = d.expr->clone();
		else expr=NULL;
	count++;
}

Declaration& Declaration::operator =(const Declaration &&d){
	cout << "op-copying decl.expr = " << d.expr << endl;
	//count++;
	loc = d.loc;
	type = d.type;
	if(type)
		mixture = new list<Agent>(*(d.mixture));
	else
		if(d.expr) expr = d.expr->clone();
		else expr=NULL;
	return *this;
}*/

Declaration::~Declaration(){
	if(type)
		delete mixture;
	else
		if (expr) delete expr;
};

Variable* Declaration::eval(pattern::Environment &env){
	Variable* var;
	short id = 0;
	try {
		id = env.declareVariable(name.getString(),type);
	} catch(SemanticError &ex) {
		ex.setLocation(this->loc);
		throw ex;
	}
	if(type)
		var = new state::AlgebraicVar(name.getString(),false,
				SomeAlgExpression(expr->eval(env)));
	else
		var = new state::KappaVar(name.getString(),false,mixture->eval(env));
	return var;
}

bool Declaration::isKappa(){
	return type;
}

} /* namespace ast */
