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
class Node {
	yy::location loc;
public:
	/*Token(int c,short l,const string &s)
		:col(c),line(l),file(s){};*/
	Node(const yy::location& l)
		:loc(l)	{cout << "location: " << l << endl;}
	Node(){};
};

class Expression : public Node{

public:
	Expression(const yy::location& l)
		:Node(l){};
	Expression(){};
};


class Id : public Node {
	string id;
public:
	Id(const string &s,Node t): Node(t),id(s){};
};

class Var : public Expression {
public:
	enum VarType {VAR,TOKEN,TIME,EVENT,NULL_EVENT,PROD_EVENT,CPUTIME,ACTIVITY};
	Var(const string &label,const VarType &t,const yy::location& l):
		Expression(l),id(label),type(t){};
	Var(const VarType &t,const yy::location& l):
		Expression(l),type(t){};
protected:
	string id;
	VarType type;
};

class Const : public Expression {
public:
	enum ConstType {INTEGER,FLOAT,INF,INF_NEG,TMAX,EMAX};
	Const (const float f,const yy::location &loc):
		Expression(loc),f(f),type(FLOAT){}
	Const (const int i,const yy::location &loc):
		Expression(loc),n(i),type(INTEGER){}
	Const (const ConstType t,const yy::location &loc):
		Expression(loc),type(t){}
protected:
	ConstType type;
	union {int n;float f;};
};

class Arrow : public Node {
	enum ARR {LEFT,RIGHT,BI} dir;
};

class Bool : public Expression {
	bool val;
};

class Link : public Node {
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
	BinaryOperation(Expression &e1,Expression &e2,Operator o,yy::location &l)
		:Expression(l),exp1(e1),exp2(e2),op(o){};
protected:
	Expression exp1,exp2;
	Operator op;
};

class UnaryOperation: public Expression{
	const Expression exp;
	enum Func {LOG,SQRT,EXP,SINUS,COSINUS,TAN,ABS} func;
public:
	UnaryOperation(Expression &e,const Func f,const yy::location &t)
		:Expression(t),exp(e),func(f){};

};

class Effect : Node{
	Node mod;
	enum Mod {ASSIGN,TRACK,FLUX,INTRO,DELETE,TOKEN,SNAPSHOT,STOP,PRINT,PRINTF};
	Id str1,str2;
	Expression exp;
	bool b;
	list<Agent> mixture;

};

class Perturbation: public Node {
protected:
	Node repeat;
	Expression test;
	Node do_tok;
	list<Effect> mods;
public:
	Perturbation();
	virtual ~Perturbation();
};

class Declaration: public Node{
	Id label;
	Expression exp;
public:
	Declaration(const Id &lab,const Expression e,const yy::location &loc):
		Node(loc),exp(e),label(lab){}
	Declaration():label(Id("",yy::location())){}
};

class CompExpression: public Node {

};

class Compartment : public Node {

};

class Rule : public Node {
protected:
	list<Agent> lhs;
	list<Agent> rhs;
	Arrow arrow;
	Expression	rate;

	Id label;


public:
	Rule();
	virtual ~Rule();
};

}

#endif /* GRAMMAR_AST_ASTSTRUCTS_H_ */
