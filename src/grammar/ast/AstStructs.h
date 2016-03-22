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
//Declaration of structures to implement duplas from Ocaml

class Node {
	yy::location loc;
public:
	/*Token(int c,short l,const string &s)
		:col(c),line(l),file(s){};*/
	Node(const yy::location& l)
		:loc(l)	{cout << "construct Node at " << l << endl;}
	Node(){};
};

//Base Class for Math Algebraic Expression
class Expression : public Node{

public:
	Expression(const yy::location& l)
		:Node(l){};
	Expression(){};
};

//Agent Name and Similars
class Id : public Node {
protected:
	string id;
public:
	Id(const string &s): Node(),id(s){};
	Id(const string &s,Node t): Node(t),id(s){};
	Id(const string &s,const yy::location &l): id(s),Node(l){};
	Id(){};
};

//User variables
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

//Constants in a Math Expression or Rate value
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


//Direction of a Rule
class Arrow : public Node {
public:
	enum ArrType {LEFT,RIGHT,BI};
	Arrow(){};
	Arrow(ArrType t): Node(),thetype(t) {};
	Arrow(ArrType t,const yy::location &loc): Node(loc), thetype(t) {};
	ArrType type(){return thetype;};
protected:
	ArrType thetype;
};

//The "True" and "False" Variable
class Bool : public Expression {
public:
	Bool(bool val): val(val) {};
	Bool(bool val,const yy::location &l): Expression(l), val(val) {};
protected:
	bool val;
};
//Link of an Agent
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

//Base Rule For All Clases Conected
class Site: Node {
public:
	Site() {};
	Site(const std::string &id,const std::list<string> &s,const Link &l,const yy::location &loc): Node(loc), id(id), states(s), link(l) {};
protected:
	Id id;
	std::list<string> states;
	Link link;
};

//Agent created by User 
class Agent: Node {
public:
	Agent() {};
	Agent(const std::string &id,const std::list<Site> s,const yy::location &loc): Node(loc), id(id),sites(s) {};
protected:
	Id id;
	std::list<Site> sites;
};

//A boolean Operation between two boolean Expression
class BoolOperation: public Expression {
public:
	enum Operator {AND,OR,GREATER,SMALLER,EQUAL,DIFF,TRUE,FALSE};	
	BoolOperation(Expression &e1,Expression &e2,Operator o,yy::location &l) : Expression(l),exp1(e1),exp2(e2),op(o){cout<<"Bool Operation"<<endl;};
protected:
	Expression exp1,exp2;
	Operator op;
};

//An Algebratic Operation or Function with 2 arguments
class BinaryOperation: public Expression {
public:
	enum Operator {SUM,MULT,DIV,MINUS,POW,MODULO,MAX,MIN} op;
	BinaryOperation(Expression &e1,Expression &e2,Operator o,yy::location &l)
		: Expression(l),exp1(e1),exp2(e2),op(o){};
protected:
	Expression exp1;
	Expression exp2;
};

//An Algebraic Operation or Function with only 1 argument
class UnaryOperation: public Expression{
public:
	enum Func {EXPONENT,LOG,SQRT,EXP,SINUS,COSINUS,TAN,ABS,ATAN,COIN,RAND_N} func;
	UnaryOperation(Expression &e,const Func f,const yy::location &t)
		:Expression(t),exp(e),func(f){};
protected:	
	const Expression exp;
};

//A Math "Procedure" or Function witout arguments
class NullaryOperation: public Expression
{
public:
	enum Func {RAND_1} func;
	NullaryOperation(const Func f,const yy::location &t): func(f), Expression(t) {};
};

//The Number of Agents and the Agents at the start
class Init_t : public Node
{
	enum InitType {MIX,TOK} type;
	Expression  alg;
	list<Agent> mix;
	string      str;
public:
	Init_t() {};
	Init_t(const Expression &e, const list<Agent> &mix): alg(e),mix(mix),type(MIX) {};
	Init_t(const Expression &e, const string      &str): alg(e),str(str),type(TOK) {};
		
};

struct Init_Declaration : public Node
{
	Init_Declaration() {};
	Init_Declaration(const Init_t &i,Id* id): init_t(i),id(id) {};
	Init_t init_t;
	Id* id;
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

class PrintObj : public Node {
public:
	PrintObj(const std::string     &s, const yy::location &l): str(s),tag(STR),Node(l) {};
	PrintObj(const ast::Expression &e, const yy::location &l): alg(e),tag(ALG),Node(l) {};  
protected:
    enum{STR,ALG} tag;
	std::string str;
	Expression  alg;
};

class Effect : public Node {
public:
	enum Action {INTRO,DELETE,UPDATE,UPDATE_TOK,STOP,SNAPSHOT,PRINT,CFLOW,CFLOWOFF,FLUX,FLUXOFF};
	Effect() {};
	Effect(const Action &a,const Expression &e,const std::list<Agent> &m,const yy::location &l): action(a),expr(e),mixture(m),Node(l) {};
	Effect(const Action &a,const std::string &s,const Expression &e,const yy::location &l): action(a),str_pos(s),expr(e),Node(l) {};
	Effect(const Action &a,const std::list<PrintObj> &pe,const yy::location &l): action(a),pexpr(),Node(l) {};
	Effect(const Action &a,const std::list<PrintObj> &pe,const std::list<PrintObj> &pe2,const yy::location &l): action(a),pexpr(pe),pexpr2(pe2),Node(l) {};
	Effect(const Action &a,const std::string &s,const yy::location &l): action(a),str_pos(s),Node(l) {};
	
protected:
	Action                   action;
	Expression               expr;
	std::string              str_pos;
	std::list<PrintObj>    pexpr;
	std::list<PrintObj>    pexpr2;
	std::list<Agent>         mixture;
};

class Perturbation: public Node {
public:
	Perturbation(){};
	Perturbation(const Expression &t,const list<Effect> &le, const yy::location &tok):
	mods(le), test(t), do_tok(tok) {};
	//preguntar a naxo
	virtual ~Perturbation() {};
protected:
	//preguntar al naxo
	Node repeat;
	Expression test;
	Node do_tok;
	list<Effect> mods;
};

struct OptString
{
	OptString(): str_ptr(NULL), print_list_ptr(NULL) {};
	OptString(std::string* s,std::list<PrintObj>* plist): str_ptr(s), print_list_ptr(plist) {};
	std::string*             str_ptr;
	std::list<PrintObj>*   print_list_ptr;
};

struct Multiple_Mixture
{
	Multiple_Mixture() {};
	Multiple_Mixture(const Expression &e,const std::list<ast::Agent> &m): alg(e), mix(m) {};
	Expression             alg;
	std::list<ast::Agent>  mix;
};


struct Radius
{
	Radius(): k1(),opt(NULL) {};
	Radius(const Expression &k1): k1(k1), opt(NULL) {};
	Radius(const Expression &k1, Expression *opt): k1(k1), opt(opt) {};
	Expression   k1;
	Expression*  opt;
};


//Structure to storages Rate Values
//def: (defined)   precise    rate for rule in the right direction
//un : (undefined) a range of rate for rule in the right direction (optional)
//op : (defined)   precise    rate for rule in the left  direction (optional)
struct Rate
{
	Rate():def(),un(NULL),op(NULL) {};
	Rate(const Expression &def,Radius *un,Expression *op): def(def), un(un), op(op) {};
	Expression   def;
	Radius*       un;
	Expression*   op;
};

struct Token
{	
	Token() {};
	Token(const Expression &e,const Id &id): exp(e), id(id) {};	
	Expression	exp;
	Id			id;
};

//Structure to storage Agents and Tokens that are part of either the right or left side in a Rule
//Agents: list of Agents
//Tokens: list of Tokens used
struct RuleSide
{
	RuleSide(){};
	RuleSide(const std::list<ast::Agent> &agents,const std::list<ast::Token> &tokens):
	agents(agents), tokens(tokens) {};
	std::list<ast::Agent> agents;
	std::list<ast::Token> tokens;
};

//transport_to
//structure used inside Rule Class 
struct tt
{
	string str;
	int i;
	float f;
	bool b;
};

//Class to storage all data aboute a rule 
class Rule : public Node {
protected:
	Id label;
	list<Agent> lhs;
	list<Agent> rhs;
	list<Token> rm_token;
	list<Token> add_token;
	Arrow       arrow;
	Expression	k_def;
	Radius*     k_un;
	Expression* k_op;
	//(***)
	int         use_id;
	bool        fixed;
	tt*         transport_to;
	
public:
	Rule() {};
	//constructors
	Rule(	const Id          &label,
			const RuleSide    &lhs,
			const RuleSide    &rhs,
			const Arrow       &arrow,
			const Rate 		  &rate,
			int               id,
			tt*               tr,
			bool              fixed,
			const yy::location &pos
	):
	label(label), lhs(lhs.agents), rhs(rhs.agents),
	rm_token(lhs.tokens), add_token(rhs.tokens),
	arrow(arrow), k_def(rate.def), k_un(rate.un),
	k_op(rate.op), use_id(id),	transport_to(tr),
	fixed(fixed), Node(pos) {};
	virtual ~Rule() {};
};

}



#endif /* GRAMMAR_AST_ASTSTRUCTS_H_ */
