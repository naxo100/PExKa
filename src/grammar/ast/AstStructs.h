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
///The base Class of all Ast Classes
class Node {
	yy::location loc;
public:
	/*Token(int c,short l,const string &s)
		:col(c),line(l),file(s){};*/
	Node(const yy::location& l)
		:loc(l)	{cout << "construct Node at " << l << endl;}
	Node(){};
};

///The Base Class of All type of Math or Algebraic Expressions
class Expression : public Node{

public:
	///Init a expression with a location loc
	Expression(const yy::location& loc)
		:Node(loc){};
	Expression(){};
};

///A Name or ID of an entity of the kappa Lenguafe (Agents, Rules, etc)
class Id : public Node {
protected:
	string id;///< String containg the name.
public:
	Id(const string &s): Node(),id(s){};
	Id(const string &s,Node t): Node(t),id(s){};
	Id(const string &s,const yy::location &l): id(s),Node(l){};
	Id(){};
};

///A kappa Variable
class Var : public Expression {
public:
	enum VarType {VAR,TOKEN,TIME,EVENT,NULL_EVENT,PROD_EVENT,CPUTIME,ACTIVITY};
	Var(const string &label,const VarType &t,const yy::location& l):
		Expression(l),id(label),type(t){};
	Var(const VarType &t,const yy::location& l):
		Expression(l),type(t){};
protected:
	string id; ///< The name of the variable
	VarType type;///< The type of the variable
};


///A Numeric Constant in a Algebraic Expression 
class Const : public Expression {
public:
	enum ConstType {INTEGER,FLOAT,INF,INF_NEG,TMAX,EMAX};
	///Init a floating point (real number) constant with a value f
	Const (const float f,const yy::location &loc):
		Expression(loc),f(f),type(FLOAT){}
	///Init an integer constant of value i
	Const (const int i,const yy::location &loc):
		Expression(loc),n(i),type(INTEGER){}
	///Init a constant of special type t
	Const (const ConstType t,const yy::location &loc):
		Expression(loc),type(t){}
protected:
	ConstType type; ///< The type of the constant. It support special type for non-numeric values.
	union {	int n;///< value for a integer constant
			float f;///< value for a float constant
	}; ///< The value of the constant.
};


///The Arrow use for the direction of a Kappa Rule or other kappa sintaxes
class Arrow : public Node {
public:
	enum ArrType {LEFT,RIGHT,BI};
	///Empty Constructor
	Arrow(){};
	///Init an arrow with a direcction t
	Arrow(ArrType t): Node(),thetype(t) {};
	///Init an arrow with a direccion t and location loc
	Arrow(ArrType t,const yy::location &loc): Node(loc), thetype(t) {};
	ArrType type(){return thetype;};
protected:
	ArrType thetype;///<  
};

///The "True" and "False" Variables
class Bool : public Expression {
public:
	Bool(bool val): val(val) {};
	Bool(bool val,const yy::location &l): Expression(l), val(val) {};
protected:
	bool val;
};

///A Link of a Site in a Kappa Agent
class Link : public Node {
public:
	///The types of links.
	enum LinkType {VALUE,///< The site is bound to a specific edge.
	FREE,///< The site is not bound (it is not member of an edge).
	ANY,///< The state of the link doesn't matter. 
	SOME,///< The site is bound but it doesn't matter to what.
	TYPE///< The site is bound to a specific site of a specific agent.
	};
	///Empty Constructor
	Link() {};
	///Init a FREE, ANY or SOME link.
	Link(LinkType t,const yy::location &loc): Node(loc), type(t)  {};
	///Init a VALUE link.
	Link(LinkType t,int ed/**/, const yy::location &loc): Node(loc),type(t), edge(ed) {};
	///Init a TYPE link.
	Link(LinkType t,const Id &s,const Id &a,const yy::location &loc): type(t), site(s), agent(a), Node(loc) {};
	//Link(LinkType t,std::string &id1,std::string &id2,const yy::location &loc): type(t), id1(id1), id2(id2), Node(loc) {};
protected:
	LinkType type;///< The type of this link.
	int edge;///< The edge identifier (for VALUE links).
	Id site;///< The site id (for TYPE links)
	Id agent;///< The agent id (for TYPE links)
};

///A site of a Kappa Agent
class Site: Node {
public:
	///Empty constructor.
	Site() {};
	///Init a site id located on loc and a list of internal states. 
	Site(const std::string &id,const std::list<string> &s,const Link &l,const yy::location &loc): Node(loc), id(id), states(s), link(l) {};
protected:
	Id id;//< The "name" of the site
	std::list<string> states;///< The list of the states that can be taken by this site.
	Link link;///< The link of this site.
};

///A Kappa Agent
class Agent: Node {
public:
	///Empty constructor.
	Agent() {};
	///Init the Agent id located on loc with a list of sites s.
	Agent(const std::string &a,const std::list<Site> s,const yy::location &loc): Node(loc), id(id),sites(s) {};
protected:
	Id id;///< The name of this agent.
	std::list<Site> sites;///< The list of sites of this agents.
};

///A boolean Operation between two boolean Expression
class BoolOperation: public Expression {
public:
	enum Operator {AND,OR,GREATER,SMALLER,EQUAL,DIFF,TRUE,FALSE};
	///Init a Boolean Operation op on the localation l between two boolean wxpression e1 and e2
	BoolOperation(Expression &e1,Expression &e2,Operator op,yy::location &l) : Expression(l),exp1(e1),exp2(e2),op(op){cout<<"Bool Operation"<<endl;};
protected:
	Expression exp1;///< The first operand.
	Expression exp2;///< The Second operand.
	Operator op;///< The operation.
};

///A Math Operation beetween two algebraic expression
class BinaryOperation: public Expression {
public:
	enum Operator {SUM,MULT,DIV,MINUS,POW,MODULO,MAX,MIN};
	///Init a Binary Operation op located on l operating on two algebraic expression e1 and e2 
	BinaryOperation(Expression &e1,Expression &e2,Operator op,yy::location &l)
		: Expression(l),exp1(e1),exp2(e2),op(op){};
protected:
	Expression exp1;///< The first operand.
	Expression exp2;///< The Second operand.
	Operator op;///< The operation.
};

///A Math Operation with only one operand
class UnaryOperation: public Expression{
public:
	enum Func {EXPONENT,LOG,SQRT,EXP,SINUS,COSINUS,TAN,ABS,ATAN,COIN,RAND_N};
	///< Init an unary operation f located on l operating on the algebraic expresion e1
	UnaryOperation(Expression &e,const Func f,const yy::location &l)
		:Expression(l),exp(e),func(f){};
protected:	
	const Expression exp;///< the operand.
	Func func;///< The operation.
};

///A Math Operation with no arguments
class NullaryOperation: public Expression
{
public:
	enum Func {RAND_1};
	///< Init a Nullary Operation f located on l
	NullaryOperation(const Func f,const yy::location &l): func(f), Expression(l) {};
protected:
	Func func;///< the operation
};

///Initial Conditions of Agents and Tokens declared with a "%init" instruction
class Init_t : public Node
{
public:
	///Specify the what information has the initial condition.
	enum InitType {
		MIX,///< The initial condition is the quantity of agents in a mixture.
		TOK///< The initial condition is the-concetration of a token.
	}; 
	/// Empty Constructor
	Init_t() {};
	/// Specify a initial a number n of agents in the mixture mix.
	Init_t(const Expression &n, const list<Agent> &mix): n(n),mix(mix),type(MIX) {};
	/// Specify a initial concentration c of the token t
	Init_t(const Expression &c, const Id &t) : n(c), tok(t) ,type(TOK) {};
protected:
	InitType    type;//> 
	Id          tok;///> The token (of a TOK condition)
	list<Agent> mix;///> The initial agent mixture (of a MIX condition)
	Expression  n;///> The initial concentration of the declared token (TOK condition) or the initial number of agents of the declared mixture (MIX condition).

};

///A declaration of a initial State
struct Init_Declaration : public Node
{
	/// Empty Constructor.
	Init_Declaration() {};
	/// Create a initial state with an optional name id and init conditions init_t
	Init_Declaration(const Init_t &i,Id* id): init_t(i),id(id) {};
	Init_t init_t;///< Initial Conditions of Agents and Tokens.
	Id* id;///< Identificator of this declaration.
};

/// A declaration of a variable
class Declaration: public Node{
public:
	//the two type of variable declaration
	enum VarType{
		ALG,///< The value of the variable is an algebraic expression.
		KAPPA///< The value of the variable is the number of occurences of a agent pattern on a kappa_expression (mixture).
	};
	
	/// Empty Contructor
	Declaration():label(Id("",yy::location())){};
	/// Specify a variable of name n with the value give by the algebraic expression e.
	Declaration(const Id &n,const Expression e,const yy::location &l):
    Node(loc),exp(e),name(n),type(ALG) {};
	/// Specify a variable of name n with the number of occurences of the agent pattern in the mixture m.
	Declaration(const Id &n,const std::list<Agent> m,const yy::location &loc): 
		Node(loc),mixture(m),name(n),type(KAPPA) {};
protected:
	VarType type;///< Specify what type of variable is this.
	Id name;///< The name of the variable.
	Expression exp;///< The algebraic expression to evaluate the value the variable.
	std::list<Agent> mixture;///< The agent parttern to determinate the number of occurrences to evaluate the variable.
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

///Contains a string or an algebraic expression (use to determinate a numeric value) to be writen to the standard output (or to a file)
class PrintObj : public Node {
public:
	/// Indicate what type of the object contains
    enum PrintType{
		STR, ///< it contains a string.
		ALG ///< it contains a algebraic expression.
	};
	///Init the printing object with a string s.
	PrintObj(const std::string     &s, const yy::location &l): str(s),tag(STR),Node(l) {};
	///Init the printing object with a algebraic expression e.
	PrintObj(const ast::Expression &e, const yy::location &l): alg(e),tag(ALG),Node(l) {};  
protected:
    enum{STR,ALG} tag;///> Identify what object contains.
	std::string str;///> The string to be printing.
	Expression  alg;///> The algebraic expression to determinate a value to be printing.
};

///A 
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

///A 
class Perturbation: public Node {
public:
	Perturbation(){};
	Perturbation(const Expression &t,const list<Effect> &le, const yy::location &tok):
	mods(le), test(t), do_tok(tok) {};
	virtual ~Perturbation() {};
protected:
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

///Structure containing the unary-instance rate
struct Radius
{
	Radius(): k1(),opt(NULL) {};
	Radius(const Expression &k1): k1(k1), opt(NULL) {};
	Radius(const Expression &k1, Expression *opt): k1(k1), opt(opt) {};
	Expression   k1;///< Unary-instance rate of an ambigous rule.
	Expression*  opt;///< ( I dont know what is this :/ )
};


///Structure containing all Setting-Rate Values of a kappa Rule 
struct Rate
{
	Rate():def(),un(NULL),op(NULL) {};
	Rate(const Expression &def,Radius *un,Expression *op): def(def), un(un), op(op) {};
	Expression   def;///< Right-direction Rate of a normal or bidirectional rule; or in case of an ambiguous rule, the binary-instance rate.
	Radius*       un;///< Unary-instance Rate of an ambiguous rule.
	Expression*   op;///< Left-direction (Opposite) Rate of a bidirectional rule.
};

///Structure containing a token and its used in a Rule.
struct Token
{	
	Token() {};
	Token(const Expression &e,const Id &id): exp(e), id(id) {};	
	Expression	exp;///> Concentration of the token (consumed or produced)
	Id			id;///> ID or Name of the Tokens
};

///Structure containing the list of agents and tokens in one of the side (left or right) of a Kappa Rule
struct RuleSide
{
	RuleSide(){};
	RuleSide(const std::list<ast::Agent> &agents,const std::list<ast::Token> &tokens):
	agents(agents), tokens(tokens) {};
	std::list<ast::Agent> agents;///> List of Agents Consumed or Produced of the Rule
	std::list<ast::Token> tokens;///> List of Tokens Consumed or Produced of the Rule
};

///Transport_To, setting of a Transport Rule
struct tt
{
	string str;
	int i;
	float f;
	bool b;
};

/*! \brief A Kappa Rule.
 * 
 * ashlkdlask asñdañsld sadkljsa
 */
class Rule : public Node {
protected:
	Id label;
	list<Agent> lhs;///<Consumed (Left hand side) Agents
	list<Agent> rhs;///<Produced (Right hand side) Agents
	list<Token> rm_token;///<Consumed (Left hand side) Tokens
	list<Token> add_token;///<Produced (Right hand side) Tokens
	Arrow       arrow;///<Direction of the Rule
	Expression	k_def;///<Right-direction Rate of a normal or bidirectional rule; or in case of an ambiguous rule, the binary-instance rate. 
	Radius*     k_un; ///<Unary-instance Rate of an ambiguous rule.
	Expression* k_op; ///<(Opposite) Left-direction Rate of bidirectional rule.
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
