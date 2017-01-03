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

Node::~Node(){};
void Node::show( string tabs ) const {
	cout << "Node" << endl;
}


/****** Class Id *********************/
Id::Id(){}
Id::Id(const Node &t,const string &s): Node(t),id(s){};
Id::Id(const location &l,const string &s): Node(l),id(s){};
Id::~Id(){};

const string& Id::getString() const {return id;};

void Id::show( string tabs ) const {
	cout << "Id: " << id;
}

/****** Class Expression *************/
Expression::Expression(){}
Expression::Expression(const yy::location& l):
		Node(l){}
Expression::~Expression(){};

bool Expression::isConstant(){
	return false;
}
state::BaseExpression* Expression::eval(pattern::Environment& env,
		const VAR &vars,
		const char flags) const{
	cout << "do not call this function" << endl;
	throw;
	return nullptr;
}
void Expression::show( string tabs ) const {
	cout << "Expression : location: " << loc.begin << " " << loc.end;
}


/****** Class VarValue ***************/
VarValue::VarValue() : value(nullptr){}
VarValue::VarValue(const location &l,const Id &name,const Expression *exp) :
	Node(l),var(name),value(exp) {}
void VarValue::show( string tabs ) const{
	cout << "VarValue : ";
}

/****** Class StringExpression *******/
StringExpression::StringExpression() : t(STR),str(nullptr),next(nullptr){}
StringExpression::StringExpression(const location &l,const string &s):
	Node(l),t(STR),str(new string(s)),next(nullptr) {};
StringExpression::StringExpression(const location &l,const Expression *e):
	Node(l),t(ALG),alg(e),next(nullptr) {};
StringExpression::StringExpression(const location &l,const Expression *e, const StringExpression* nxt):
	Node(l),t(ALG),alg(e),next(new StringExpression(*nxt)) {};
StringExpression::StringExpression(const location &l,const string &s, const StringExpression* nxt):
	Node(l),t(STR),str(new string(s)),next(new StringExpression(*nxt)) {};

StringExpression::~StringExpression(){
	if(t)
		delete alg;
	else
		delete str;
	if(next)
		delete next;
}


/****** Class Arrow **********//*
Arrow::Arrow(): type(RIGHT){}
Arrow::Arrow(const location &loc,ArrType t):
		Node(loc), type(t) {};
Arrow::ArrType Arrow::getType(){
	return type;
};
*/

} //namespace ast
