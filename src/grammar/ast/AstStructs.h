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

#define DEBUG

using namespace std;

namespace ast {
class Node {
	yy::location loc;
public:
	/*Token(int c,short l,const string &s)
		:col(c),line(l),file(s){};*/
	Node(const yy::location& l)
		:loc(l)	{cout << "construct Node at " << l << endl;}
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
	Id(const string &s): Node(),id(s){};
	Id(const string &s,Node t): Node(t),id(s){};
//	Id(const string &s,const yy::locattion &l): Node(l),id(s){};
	Id(): Node(),id(){};
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
public:
	enum ArrType {LEFT,RIGHT,BI};
	Arrow(){};
	Arrow(ArrType t): Node(),type(t) {};
	Arrow(ArrType t,const yy::location &loc): Node(loc), type(t) {};
protected:
	ArrType type;
};

class Bool : public Expression {
public:
	Bool(bool val): val(val) {};
	Bool(bool val,const yy::location &l): Expression(l), val(val) {};
protected:
	bool val;
};

class Link : public Node {
public:
	enum LinkType {VALUE,FREE,ANY,SOME,TYPE};
	Link() {};
	Link(LinkType t,const yy::location &loc): Node(loc), type(t)  {};
	Link(LinkType t,int val, const yy::location &loc): Node(loc),type(t), value(val) {};
	Link(LinkType t,const Id &id1,const Id &id2,const yy::location &loc): type(t), id1(id1), id2(id2), Node(loc) {};
	//Link(LinkType t,std::string &id1,std::string &id2,const yy::location &loc): type(t), id1(id1), id2(id2), Node(loc) {};
protected:
	LinkType type;
	int value;
	Id id1;
	Id id2;
};

//class PortExpression : public Node {
//public:
//	enum PortType {EMPTY,NONEMPTY};
//	PortExpression() {};
//	PortExpression(const std::string &id,const std::list<string> &is,const Link &l,const yy::location &loc): Node(loc),id(id), internal_state(is), link(l) {};
//protected:
//	std::string id;
//	std::list<string> internal_state;
//	Link link;
//};

class Site: Node {
public:
	Site() {};
	Site(const std::string &id,const std::list<string> &s,const Link &l,const yy::location &loc): Node(loc), id(id), states(s), link(l) {};
protected:
	Id id;
	std::list<string> states;
	Link link;
};
//class Site {
//	Id id;
//	list<Id> values;
//	Link link;
//};

class Agent: Node {
public:
	Agent() {};
	Agent(const std::string &id,const std::list<Site> s,const yy::location &loc): Node(loc), id(id),sites(s) {};
protected:
	Id id;
	std::list<Site> sites;
};

//class IndexOperation: public Expression {
//public:
//	enum Operator {SUM,MULT,DIV,MINUS,POW,MODULO};
//	IndexOperation(Expression &e1,Expression &e2,Operator o,yy::location &l) : Expression(l),exp1(e1),exp2(e2),op(o){};
//protected:
//	Expression exp1,exp2;
//	Operator op;
//};

class BoolOperation: public Expression {
public:
	enum Operator {AND,OR,GREATER,SMALLER,EQUAL,DIFF,TRUE,FALSE};	
	BoolOperation(Expression &e1,Expression &e2,Operator o,yy::location &l) : Expression(l),exp1(e1),exp2(e2),op(o){cout<<"Bool Operation"<<endl;};
protected:
	Expression exp1,exp2;
	Operator op;
};

class BinaryOperation: public Expression {
public:
	enum Operator {SUM,MULT,DIV,MINUS,POW,MODULO,MAX,MIN};
	BinaryOperation(Expression &e1,Expression &e2,Operator o,yy::location &l)
		: Expression(l),exp1(e1),exp2(e2),op(o){};
protected:
	Expression exp1;
	Expression exp2;
	Operator op;
};

class UnaryOperation: public Expression{
public:
	enum Func {EXPONENT,LOG,SQRT,EXP,SINUS,COSINUS,TAN,ABS,ATAN,COIN,RAND_N};
	UnaryOperation(Expression &e,const Func f,const yy::location &t)
		:Expression(t),exp(e),func(f){};
protected:	
	const Expression exp;
	Func func;
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
public:
	Perturbation();
	virtual ~Perturbation();
protected:
	Node repeat;
	Expression test;
	Node do_tok;
	list<Effect> mods;
};

class Declaration: public Node{
public:
	enum VarType{ALG,KAPPA};	
	Declaration(const Id &lab,const Expression e,const yy::location &loc): 
		Node(loc),exp(e),label(lab),type(ALG) {};
	
	Declaration(const Id &lab,const std::list<Agent> m,const yy::location &loc): 
		Node(loc),mixture(m),label(lab),type(KAPPA) {};

	Declaration():label(Id("",yy::location())){}
protected:
	Id label;
	VarType type;
	Expression exp;
	std::list<Agent> mixture;
};

class CompExpression: public Node {
public:
	CompExpression() {};
	CompExpression(const std::string &la,const std::list<Expression> &d,Expression* w,const yy::location &l): Node(l),label(la), dim(d), where(w) {};
protected:
	std::string            label;
	std::list<Expression>    dim;
	Expression*            where;
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

//struct PrintElement
//{
//    enum{STR,EXP} tag;
//    union
//    {
//        std::string s;
//        ast::Expression;
//    };
//};
 


class print_expr : public Node {
public:
	print_expr(const std::string     &s, const yy::location &l): str(s),tag(STR),Node(l) {};
	print_expr(const ast::Expression &e, const yy::location &l): exp(e),tag(EXP),Node(l) {};  
protected:
    enum{STR,EXP} tag;
	std::string str;
	Expression  exp;
};

class modif_expr : public Node {
public:
	enum Action {INTRO,DELETE,UPDATE,UPDATE_TOK,STOP,SNAPSHOT,PRINT,CFLOW,CFLOWOFF,FLUX,FLUXOFF};
	modif_expr() {};
	modif_expr(const Action &a,const Expression &e,const std::list<Agent> &m,const yy::location &l): action(a),expr(e),mixture(m),Node(l) {};
	modif_expr(const Action &a,const std::string &s,const Expression &e,const yy::location &l): action(a),str_pos(s),expr(e),Node(l) {};
	modif_expr(const Action &a,const std::list<print_expr> &pe,const yy::location &l): action(a),pexpr(),Node(l) {};
	modif_expr(const Action &a,const std::list<print_expr> &pe,const std::list<print_expr> &pe2,const yy::location &l): action(a),pexpr(pe),pexpr2(pe2),Node(l) {};
	modif_expr(const Action &a,const std::string &s,const yy::location &l): action(a),str_pos(s),Node(l) {};
	
protected:
	Action                   action;
	Expression               expr;
	std::string              str_pos;
	std::list<print_expr>    pexpr;
	std::list<print_expr>    pexpr2;
	std::list<Agent>         mixture;
};

struct ptr_pair
{
	ptr_pair(): str(NULL), pexpr(NULL) {};
	ptr_pair(std::string* s,std::list<print_expr>* pe): str(s), pexpr(pe) {};
	std::string*             str;
	std::list<print_expr>*   pexpr;
};

struct mix_pair
{
	mix_pair() {};
	mix_pair(const Expression &e,const std::list<ast::Agent> &m): alg(e), mix(m) {};
	Expression             alg;
	std::list<ast::Agent>  mix;
};

struct alg_pair
{
	alg_pair():exp1(),exp2(NULL) {};
	alg_pair(const Expression &e1, Expression *e2): exp1(e1), exp2(e2) {};
	Expression   exp1;
	Expression*  exp2;
};

struct rate_triplet
{
	rate_triplet():exp(),pair(NULL),ptr_exp(NULL) {};
	rate_triplet(const Expression &e,alg_pair *p,Expression *ptr_e): exp(e), pair(p),ptr_exp(ptr_e) {};
	Expression   exp;
	alg_pair*    pair;
	Expression*  ptr_exp;
};

//class Plot : public Node {
//public:
//	Plot():{};
//	Plot(const Expression &e, const yy::location &l) : e(e),loc(l) {};
//protected:
//	Expression e;	
//};
//
//class Obs : public None {
//public:
//	Obs():{};
//	Obs(const Declaration &e, const yy::location &l) : e(e),loc(l) {};
//protected:
//	Declaration e;	
//
}



#endif /* GRAMMAR_AST_ASTSTRUCTS_H_ */
