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
using namespace std;

class Token {
	int character;
	short line;
public:
	Token(int c,short l):character(c),line(l){};
};

class Expression : public Token{

};


class Id : public Token {
	string id;
public:
	Id(char* label,Token t): Token(t),id(label){};
};

class Var : Expression {
	string id;
	Expression* exp;
public:
	Var(char* label,Expression* e,Token t):Token(t),id(label),exp(e){};
};

class Num : public Expression {
	bool isInteger;
	union num {int n;float f;} val;
public:
	Num (int* n,float* f,Token t):Token(t){
		if(n)
			val.n = n;
		else
			val.f = f;
		isInteger = n ? true :false;
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
	Expression *exp1,*exp2;
	enum Operator {SUM,MULT,DIV,MINUS,POW,MODULO,MAX,MIN} op;
public:
	BinaryOperation(Expression* e1,Expression* e2,Operator o,Token t)
		:Token(t),exp1(e1),exp2(e2),op(o){};
};

class UnaryOperation: public Expression{
	Expression *exp;
	enum Func {LOG,SQRT,EXP,SINUS,COSINUS,TAN,ABS} func;
public:
	UnaryOperation(Expression* e,Func f,Token t)
		:Token(t),exp(e),func(f){};

};

class Constant: public Expression {
	enum Cons {TIME,EVENT,NULL_EVENT,PROD_EVENT,TMAX,EMAX,CPUTIME,INFINITY} cons;
public:
	Constant(Cons c): cons(c){};
};



#endif /* GRAMMAR_AST_ASTSTRUCTS_H_ */
