/*
 * AstStructs.h
 *
 *  Created on: Dec 28, 2015
 *      Author: naxo
 */

#ifndef GRAMMAR_AST_ASTSTRUCTS_H_
#define GRAMMAR_AST_ASTSTRUCTS_H_

#include <string>
#include <list>
#include "../location.hh"
using namespace std;

namespace ast {
class Token {
	yy::location loc;
public:
	/*Token(int c,short l,const string &s)
		:col(c),line(l),file(s){};*/
	Token(const yy::location& l)
		:loc(l)	{cout << "location: " << l << endl;}
	Token(){};
};

class Expression : public Token{

public:
	Expression(const yy::location& l)
		:Token(l){};
	Expression(){};
};


class Id : public Token {
	string id;
public:
	Id(char* label,Token t): Token(t),id(label){};
};

class Var : Expression {
	string id;
	const Expression* exp;
public:
	Var(char* label,const Expression* e,const yy::location& l):Expression(l),id(label),exp(e){};
};

class Num : public Expression {
	bool isInteger;
	union num {int n;float f;} val;
public:
	Num (int n,float f,const yy::location &loc): Expression(loc){
		if(n == f)
			val.n = n;
		else
			val.f = f;
		isInteger = n == f ? true : false;
	};
};

class Arrow : public Token {
	enum ARR {LEFT,RIGHT,BI} dir;
};

class Bool : public Expression {
	bool val;
};

class Link : public Token {
	int value;
	enum LNK {VALUE,FREE,ANY,SOME,TYPE} lnk;
};

class Site {
	Id id;
	list<Id> values;
	Link link;
};

class Agent {
	Id id;
	list<Site> sites;
};

class BinaryOperation: public Expression {
public:
	enum Operator {SUM,MULT,DIV,MINUS,POW,MODULO,MAX,MIN};
	BinaryOperation(Expression* e1,Expression* e2,Operator o,yy::location &l)
		:Expression(l),exp1(e1),exp2(e2),op(o){};
protected:
	Expression *exp1,*exp2;
	Operator op;
};

class UnaryOperation: public Expression{
	Expression *exp;
	enum Func {LOG,SQRT,EXP,SINUS,COSINUS,TAN,ABS} func;
public:
	UnaryOperation(Expression* e,Func f,Token t)
		:exp(e),func(f){};

};

class Constant: public Expression {
	enum Cons {TIME,EVENT,NULL_EVENT,PROD_EVENT,TMAX,EMAX,CPUTIME,INFINITY} cons;
public:
	Constant(Cons c): cons(c){};
};

}

#endif /* GRAMMAR_AST_ASTSTRUCTS_H_ */
