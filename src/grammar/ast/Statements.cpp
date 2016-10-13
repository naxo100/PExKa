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

Declaration::Declaration(const location &l,const Id &lab,const Mixture &m):
	Node(loc),name(lab),type(KAPPA),mixture(new Mixture(m)) {};

Declaration::Declaration(const Declaration &d) : name(d.name),type(d.type){
	if(type)
		mixture = new Mixture(*(d.mixture));
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
		mixture = new Mixture(*(d.mixture));
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

Variable* Declaration::eval(pattern::Environment &env,
		unordered_map<string,Variable*> &vars) const{
	Variable* var;
	short id = 0;
	try {
		id = env.declareVariable(name.getString(),type);
	} catch(SemanticError &ex) {
		ex.setLocation(this->loc);
		throw ex;
	}
	if(type){
		BaseExpression* b_expr = expr->eval(env,vars);
		switch(b_expr->getType()){
		case BaseExpression::FLOAT:
			var = new state::AlgebraicVar<float>(id,name.getString(),false,
				dynamic_cast<AlgebraicVar<float>*>(b_expr));
			break;
		case BaseExpression::INT:
			var = new state::AlgebraicVar<int>(id,name.getString(),false,
				dynamic_cast<AlgebraicVar<int>*>(b_expr));
			break;
		case BaseExpression::BOOL:
			var = new state::AlgebraicVar<bool>(id,name.getString(),false,
				dynamic_cast<AlgebraicVar<bool>*>(b_expr));
			break;
		}
	}else
		var = new state::KappaVar(id,name.getString(),false,mixture->eval(env));
	return var;
}

bool Declaration::isKappa(){
	return type;
}



/****** Class Init ***********/
Init::Init(){}
Init::Init(const location &l,const Expression *e, const Mixture &mix):
		Node(l),type(MIXTURE),alg(e),mixture(mix) {};
Init::Init(const location &l,const Expression *e, const Id &tok):
		Node(l),type(TOKEN),alg(e),token(tok) {};

Init::Init(const Init &init) :
		Node(init.loc),type(init.type),alg(init.alg) {
	if(type)
		token = init.token;
	else
		mixture = init.mixture;
}
Init& Init::operator=(const Init &init) {
	loc = init.loc;
	type = init.type;
	if(type)
		token = init.token;
	else
		mixture = init.mixture;
	return *this;
}
Init::~Init(){};

} /* namespace ast */