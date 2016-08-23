/*
 * Basics.cpp
 *
 *  Created on: May 19, 2016
 *      Author: naxo
 */

#include "Basics.h"

namespace ast {

/****** Class Node *******************/
Node::Node() : loc(location()){}
Node::Node(const yy::location& l)
	:loc(l)	{/*cout << "construct Node at " << l << endl;*/}


/****** Class Id *********************/
Id::Id(){}
Id::Id(const Node &t,const string &s): Node(t),id(s){};
Id::Id(const location &l,const string &s): Node(l),id(s){};
string Id::getString() const {return id;};


/****** Class Expression *************/
Expression::Expression(){}
Expression::Expression(const yy::location& l):
		Node(l){}
Expression::~Expression(){};

bool Expression::isConstant(){
	return false;
}
state::SomeAlgExpression Expression::eval(pattern::Environment& env) const{
	cout << "do not call this function" << endl;
	throw;
	return state::SomeAlgExpression();
}


/****** Class VarValue ***************/
VarValue::VarValue(){}
VarValue::VarValue(const location &l,const Id &name,const Expression *exp) :
	Node(l),var(name),value(exp) {}


/****** Class StringExpression *******/
StringExpression::StringExpression(){}
StringExpression::StringExpression(const location &l,const string &s):
	Node(l),str(new string(s)),t(STR),next(NULL) {};
StringExpression::StringExpression(const location &l,const Expression *e):
	Node(l),alg(e),t(ALG),next(NULL) {};
StringExpression::StringExpression(const location &l,const Expression *e, const StringExpression* nxt):
	Node(l),alg(e),t(ALG),next(new StringExpression(*nxt)) {};
StringExpression::StringExpression(const location &l,const string &s, const StringExpression* nxt):
	Node(l),str(new string(s)),t(STR),next(new StringExpression(*nxt)) {};

StringExpression::~StringExpression(){
	if(t)
		delete alg;
	else
		delete str;
	if(next)
		delete next;
}


} //namespace ast
