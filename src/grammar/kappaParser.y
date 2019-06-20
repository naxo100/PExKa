%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0"
%defines
%define parse.assert
%define parser_class_name { KappaParser }
%define api.value.type variant
%define api.token.constructor
%define parse.error verbose
%param { grammar::KappaDriver &driver }
%locations
%initial-action
{
	// Initialize the initial location.
	//@$.initialize(driver.getCurrentFileName(),1,1);
	//yydebug_ = 1;
};

%code requires{
	#include <iostream>
	#include <string>
	#include "ast/Statements.h"
	#include "location.hh"
	#include "../util/Exceptions.h"
	#include <typeinfo>
	
	using namespace std;
	using namespace ast;
	
	namespace yy {
		class KappaLexer;
	}
	namespace grammar{
		class KappaDriver;
	}

}

%code top {
    #include "KappaLexer.h"
    #include "KappaParser.hpp"
    
	#define yylex(x) x.getNextToken()

	using namespace yy;
}

%code {
    #include "KappaDriver.h"
}


/**
 * DEFINITIONS
 */

%token END NEWLINE SEMICOLON
%token AT ATD FIX OP_PAR CL_PAR OP_BRA CL_BRA COMMA DOT TYPE LAR OP_CUR CL_CUR JOIN FREE
%token LOG PLUS MULT MINUS AND OR GREATER SMALLER EQUAL PERT INTRO DELETE DO SET UNTIL TRUE FALSE OBS KAPPA_RAR TRACK CPUTIME CONFIG REPEAT DIFF
%token KAPPA_WLD KAPPA_SEMI KAPPA_INTER KAPPA_VALUE SIGNATURE INF TIME EVENT ACTIVITY NULL_EVENT PROD_EVENT INIT LET CONST DIV PLOT SINE COSINE TAN ATAN COIN RAND_N SQRT EXPONENT POW ABS MODULO SUM AVG 
%token EMAX TMAX RAND_1 FLUX ASSIGN ASSIGN2 TOKEN KAPPA_LNK PIPE KAPPA_LRAR PRINT PRINTF /*CAT VOLUME*/ MAX MIN
%token <int> INT 
%token <std::string> ID LABEL KAPPA_MRK NAME 
%token <float> FLOAT 
%token <std::string> STRING
%token STOP SNAPSHOT

%token COMPARTMENT CHANNEL TRANSPORT USE

%left MINUS PLUS MIN MAX
%left MULT DIV
%left MODULO
%right POW
%nonassoc LOG SQRT EXPONENT SINUS COSINUS ABS TAN

%left OR
%left AND


%type <Declaration>				variable_declaration
%type <Expression*>				alg_expr bool_expr constant variable multiple where_expr
%type <bool>					rate_sep boolean join arrow
%type <std::list<std::string>>			value_list
%type <SiteState>				internal_state
%type <std::list<Id>>				state_enum
%type <Link>					link_state
%type <CompExpression>				comp_expr
%type <Site>					port_expression
%type <std::list<Site>>				interface_expression ne_interface_expression
%type <Agent>					agent_expression
%type <std::list<Agent>>			non_empty_mixture 
%type <Mixture>					mixture
//%type <MultipleMixture> 			multiple_mixture
//%type <std::pair<Expression*, Mixture>>		multiple_mixture
%type <std::list<CompExpression>>		comp_list
%type <std::list<const Expression*>>		dimension
%type <std::list<StringExpression>>		print_expr print_expr_list
%type <Effect>					effect
%type <std::list<Effect>>			effect_list
%type <Radius>					alg_with_radius   
%type <Rate>					rate
%type <Pert*>				perturbation_declaration
%type <std::list<Token>>			sum_token token_expr
%type <RuleSide> 				lhs_rhs
%type <Id>					rule_label
%type <Rule>					rule_expression
%type <Init>					init_declaration
%type <std::pair<BaseExpression::N_ary,Expression*>> distr_expr

%start statements

%%
/**
 * RULES
 */
statements:
| statements statement newline
	{}
| statements error
	{yy::KappaParser::error(@$ , "Statement not recognized");}
;


newline:
NEWLINE {}
| END {return 0;}
;


statement:
| rule_expression
	{this->driver.getAst().add($1);}
| instruction
	{}
;


instruction:
 COMPARTMENT comp_expr alg_expr
 	{this->driver.getAst().add(Compartment(@$,$2,$3));}
| CHANNEL LABEL comp_expr arrow comp_expr where_expr 
 	{this->driver.getAst().add(Channel(@$,Id(@2,$2),$3,$5,$4,$6));}
| CHANNEL LABEL comp_expr arrow comp_expr where_expr ATD alg_expr
 	{this->driver.getAst().add(Channel(@$,Id(@2,$2),$3,$5,$4,$6,$8));}
| CHANNEL error
	{yy::KappaParser::error(@2, "Bad channel declaration");}
| TRANSPORT join LABEL mixture AT alg_expr
 	{}
| USE MULT
	{this->driver.getAst().add(new Use(@$));}
| USE comp_list where_expr
 	{this->driver.getAst().add(new Use(@$,$2,$3));}
| SIGNATURE agent_expression  
	{this->driver.getAst().add($2);}
| TOKEN ID
	{this->driver.getAst().add(Id(@2,$2));}
| TOKEN error
	{}
| SIGNATURE error
	{}
| INIT init_declaration 
	{this->driver.getAst().add($2);}
| LET variable_declaration 
	{this->driver.getAst().add($2);}
| LET error
	{yy::KappaParser::error(@2, "Bad variable declaration");}
| CONST variable_declaration
	{$2.setConstant(true); this->driver.getAst().add($2);}
| CONST error
	{}
| OBS variable_declaration
	{$2.setObservable();this->driver.getAst().add($2);}
| PLOT alg_expr 
	{}
| PLOT error 
	{}
| PERT perturbation_declaration 
	{this->driver.getAst().add($2);}
/*| PERT REPEAT perturbation_declaration UNTIL bool_expr 
	{}*/
| CONFIG STRING value_list 
	{}
;


init_declaration:
 multiple non_empty_mixture 
	{$$=Init(@$,$1,Mixture(@2,$2));}
| ID LAR alg_expr
	{$$=Init(@$,$3,Id(@1,$1));}
/*** TODO ???
| ID OP_CUR init_declaration CL_CUR 
	{
		ast::Id id($1);
		$$=ast::Init_Declaration($3.Init,&id);
	}
*/
;


/*SPATIAL*/
join:
/*empty*/
	{$$=true;}
| JOIN
	{$$=true;}
| FREE
	{$$=false;}
;


comp_expr:
 LABEL dimension
	{ $$ = CompExpression(@$,Id(@1,$1),$2); }
;


dimension: 
/*empty*/
	{$$=std::list<const ast::Expression*>();}
| OP_BRA alg_expr CL_BRA dimension
	{
		$4.push_front($2);
 		$$=$4;
	}
;


where_expr:
/*empty*/
	{$$ = nullptr;}
| OP_CUR bool_expr CL_CUR
	{$$=$2;}
;


/*index_expr:
INT
	{}
| ID 
	{}
| OP_PAR index_expr CL_PAR 
	{$$=$2;}
| index_expr MULT index_expr
	{$$=ast::IndexOperation(@$,$1,$3,ast::IndexOperation::MULT);}
| index_expr PLUS index_expr
	{$$=ast::IndexOperation(@$,$1,$3,ast::IndexOperation::SUM);}
| index_expr DIV index_expr
	{$$=ast::IndexOperation(@$,$1,$3,ast::IndexOperation::DIV);}
| index_expr MINUS index_expr
	{$$=ast::IndexOperation(@$,$1,$3,ast::IndexOperation::MINUS);}
| index_expr POW index_expr
	{$$=ast::IndexOperation(@$,$1,$3,ast::IndexOperation::POW);}
| index_expr MODULO index_expr
	{$$=ast::IndexOperation(@$,$1,$3,ast::IndexOperation::MODULO);}
;*/

value_list: 
 STRING 
	{$$=std::list<std::string>(1,$1); }
| STRING value_list 
	{
		$2.push_front($1);
		$$=$2;
	 }
;


comp_list:
  comp_expr
	{$$=std::list<ast::CompExpression>(1,$1); }
| comp_expr comp_list
	{ 
		$2.push_front($1);
 		$$=$2; 
	}
| comp_expr COMMA comp_list
	{	
		$3.push_front($1);
		$$=$3; 
	}
;


perturbation_declaration:
 bool_expr DO effect_list 
	{ $$ = new Pert(@$,$1,$3); }
| REPEAT bool_expr DO effect_list UNTIL bool_expr
	{ $$ = new Pert(@$,$2,$4,$6); }
/*| REPEAT OP_PAR bool_expr DO effect_list CL_PAR UNTIL bool_expr
	{ $$ = Pert(@$,$3,$5,$8); }*/
;


effect_list:
 OP_PAR effect_list CL_PAR {$$=$2;}
| effect
	{
		$$=std::list<ast::Effect>(1,$1);
	}
| effect SEMICOLON effect_list 
	{
		$3.push_front($1);
		$$=$3;
	}
;


effect:
// INTRO multiple_mixture 
  INTRO alg_expr non_empty_mixture
	{ $$ = Effect(@$ ,Effect::INTRO, $2, $3); }
// | DELETE multiple_mixture
| DELETE alg_expr non_empty_mixture
	{ $$ = Effect(@$, Effect::DELETE, $2, $3); }
| STOP print_expr_list
	{ $$ = Effect(@$,Effect::STOP,$2); }
| FLUX print_expr_list boolean 
	{ $$ = Effect(@$,$3 ? Effect::FLUX : Effect::FLUX_OFF,$2); }
| TRACK LABEL boolean 
	{ $$ = Effect(@$,$3 ? Effect::CFLOW : Effect::CFLOW_OFF , Id(@2,$2)); }
| LABEL ASSIGN alg_expr /*updating the rate of a rule -backward compatibility*/
	{ $$ = Effect(@$,Effect::UPDATE,VarValue(@2,Id(@1,$1),$3)); }
| ASSIGN2 LABEL alg_expr /*updating the rate of a rule*/
	{ $$ = Effect(@$,Effect::UPDATE,VarValue(@1,Id(@2,$2),$3)); }
| ID LAR alg_expr /*updating the value of a token*/
	{ $$ = Effect(@$,Effect::UPDATE_TOK,VarValue(@2,Id(@1,$1),$3)); }
| SNAPSHOT print_expr_list
	{ $$ = Effect(@$,Effect::SNAPSHOT,$2); }
| PRINT print_expr_list 
	{ $$ = Effect(@$,Effect::PRINT,$2); }
| PRINTF print_expr_list print_expr_list 
	{ $$ = Effect(@$,Effect::PRINTF,$2,$3); }
;


print_expr_list: // list
  STRING
	{ $$ = std::list<StringExpression>(1,StringExpression(@$,$1)); }
| SMALLER print_expr GREATER {$$ = $2;}
;

print_expr: // list
  STRING 
	{ $$.push_front( StringExpression(@$,$1) ); }
| OP_BRA alg_expr CL_BRA
	{ $$.push_front( StringExpression(@$,$2) ); }
| OP_BRA alg_expr CL_BRA DOT print_expr
	{ $5.push_front( StringExpression(@$,$2) ); 
	  $$ = $5;
	}
| STRING DOT print_expr
	{ $3.push_front( StringExpression(@$,$1) );
	  $$=$3; 
	}
;

/*string_or_pr_expr:
  STRING {$$ = StringExpression(@$,$1);}
| SMALLER print_expr GREATER {$$ = $2;}
;*/


boolean:
  TRUE {$$=true;}
| FALSE {$$=false;}
;


variable_declaration:
  LABEL non_empty_mixture 
	{$$ = Declaration(@$,Id(@1,$1),Mixture(@2,$2));}
| LABEL alg_expr 
	{$$ = Declaration(@$,Id(@1,$1),$2);}
| LABEL OP_CUR distr_expr CL_CUR non_empty_mixture
	{$$ = Declaration(@$,Id(@1,$1),$3,Mixture(@5,$5));}
| LABEL error 
	{yy::KappaParser::error(@2 , "error in LABEL error");}
;


distr_expr:
  SUM alg_expr
	{$$ = make_pair(BaseExpression::SUMATORY,$2);}
| AVG alg_expr
	{$$ = make_pair(BaseExpression::AVERAGE,$2);}



bool_expr:
  OP_PAR bool_expr CL_PAR 
	{$$=$2;}
| bool_expr AND bool_expr 
	{$$ = new BoolBinaryOperation(@$,$1,$3,BaseExpression::BoolOp::AND);}
| bool_expr OR bool_expr 
	{$$ = new BoolBinaryOperation(@$,$1,$3,BaseExpression::BoolOp::OR);}
| alg_expr GREATER alg_expr 
	{$$ = new BoolBinaryOperation(@$,$1,$3,BaseExpression::BoolOp::GREATER);}
| alg_expr SMALLER alg_expr 
	{$$ = new BoolBinaryOperation(@$,$1,$3,BaseExpression::BoolOp::SMALLER);}
| alg_expr EQUAL alg_expr 
	{$$ = new BoolBinaryOperation(@$,$1,$3,BaseExpression::BoolOp::EQUAL);}
| alg_expr DIFF alg_expr  
	{$$ = new BoolBinaryOperation(@$,$1,$3,BaseExpression::BoolOp::NOT_EQUAL);}
| TRUE
	{$$ = new Const(@$,true);}
| FALSE
	{$$ = new Const(@$,false);}
;


multiple:
  INT   {$$ = new Const(@$,$1);}
| FLOAT	{$$ = new Const(@$,$1);}
| LABEL {$$ = new ast::Var(@$,Var::VAR,Id(@1,$1)); }
;


constant:
  INF
	{$$ = new Const(@$,Const::INF);}
| FLOAT
	{$$ = new Const(@$,$1);}
| INT 
	{$$ = new Const(@$,$1);}
| EMAX
	{$$ = new Const(@$,Const::EMAX);}
| TMAX
	{$$ = new Const(@$,Const::TMAX);}
;


rule_label: 
/*empty */
	{$$=Id(yy::location(),"");}
| LABEL 
	{$$=Id(@$,$1);}
;


lhs_rhs:
  mixture token_expr 
	{ $$ = RuleSide(@$,$1,$2); }
;


token_expr:
/*empty*/
	{ $$ = std::list<Token>();}
| PIPE sum_token 
	{ $$ = $2; } 
| PIPE error 
	{}
;


sum_token:
  OP_PAR sum_token CL_PAR 
	{ $$ = $2; } 
| alg_expr TYPE ID 
	{$$=std::list<Token>(1,Token(@$,$1,Id(@3,$3)));}
| alg_expr TYPE ID PLUS sum_token 
	{
		$5.push_front(Token(@1+@2+@3,$1,Id(@3,$3)));
		$$=$5;
	}
;


mixture:
//empty 
	{ $$ = Mixture(location(),std::list<ast::Agent>()); }
| non_empty_mixture 
	{ $$ = Mixture(@1,$1); }
;


rate_sep:
  AT {$$=false;}
| FIX {$$=true;}

/*(**  **)*/
/*{$$=ast::Rule($1,$2.agents,$4.agents,$2.tokens,$4.tokens,$3,$6.);}*/
;

rule_expression:
  rule_label lhs_rhs arrow lhs_rhs where_expr rate 
	{
		$$=Rule(@$,$1,$2,$4,$3,$5,$6);
	}
| rule_label lhs_rhs arrow lhs_rhs where_expr
	{
		cout << "Warning: Rule has no kinetics. Default rate of 0.0 is assumed. (" << @$ << ")\n" << endl;
		Rate rate(yy::location(),new Const(yy::location(),0.0f),false);
		$$=ast::Rule(@$,$1,$2,$4,$3,$5,rate);;
	}
;


arrow:
  KAPPA_RAR 
	{$$=false;/*ast::Arrow(@$,ast::Arrow::RIGHT);*/}
| KAPPA_LRAR
	{$$=true;/*ast::Arrow(@$,ast::Arrow::BI);*/}
;


variable:
  ID
	{$$ = new Var(@$,Var::AUX,Id(@1,$1));}
| PIPE ID PIPE 
	{$$ = new Var(@$,Var::TOKEN,Id(@2,$2));}
| LABEL 
	{$$ = new Var(@$,Var::VAR,Id(@1,$1));}
;


alg_expr:
  OP_PAR alg_expr CL_PAR 
	{$$ = $2;}
| constant 
	{$$ = $1;}
/**************************/
| variable
	{$$ = $1;}
/*| bool_expr
	{$$ = $1;}*/
| alg_expr MULT alg_expr
	{$$ = new AlgBinaryOperation(@$,$1,$3,BaseExpression::AlgebraicOp::MULT);}
| alg_expr PLUS alg_expr
	{$$ = new AlgBinaryOperation(@$,$1,$3,BaseExpression::AlgebraicOp::SUM);}
| alg_expr DIV alg_expr
	{$$ = new AlgBinaryOperation(@$,$1,$3,BaseExpression::AlgebraicOp::DIV);}
| alg_expr MINUS alg_expr
	{$$ = new AlgBinaryOperation(@$,$1,$3,BaseExpression::AlgebraicOp::MINUS);}
| alg_expr POW alg_expr
	{$$ = new AlgBinaryOperation(@$,$1,$3,BaseExpression::AlgebraicOp::POW);}
| alg_expr MODULO alg_expr
	{$$ = new AlgBinaryOperation(@$,$1,$3,BaseExpression::AlgebraicOp::MODULO);}	
| MAX alg_expr alg_expr
	{$$ = new AlgBinaryOperation(@$,$2,$3,BaseExpression::AlgebraicOp::MAX);}
| MIN alg_expr alg_expr
	{$$ = new AlgBinaryOperation(@$,$2,$3,BaseExpression::AlgebraicOp::MIN);}
| EXPONENT alg_expr 
	{$$ = new UnaryOperation(@$,$2,BaseExpression::Unary::EXPONENT);}
| SINE alg_expr
	{$$ = new UnaryOperation(@$,$2,BaseExpression::Unary::SINE);}
| COSINE alg_expr
	{$$ = new UnaryOperation(@$,$2,BaseExpression::Unary::COSINE);}
| TAN alg_expr 
	{$$ = new UnaryOperation(@$,$2,BaseExpression::Unary::TAN);}
| ABS alg_expr
	{$$ = new UnaryOperation(@$,$2,BaseExpression::Unary::ABS);}
| SQRT alg_expr
	{$$ = new UnaryOperation(@$,$2,BaseExpression::Unary::SQRT);}
| LOG alg_expr
	{$$ = new UnaryOperation(@$,$2,BaseExpression::Unary::LOG);}
/*(***)*/
| ATAN alg_expr
	{$$ = new UnaryOperation(@$,$2,BaseExpression::Unary::ATAN);}
| COIN alg_expr
	{$$ = new UnaryOperation(@$,$2,BaseExpression::Unary::COIN);}
| RAND_N alg_expr
	{$$ = new UnaryOperation(@$,$2,BaseExpression::Unary::RAND_N);}
| RAND_1
	{$$ = new NullaryOperation(@$,BaseExpression::Nullary::RAND_1);}
| TIME
	{$$ = new NullaryOperation(@$,BaseExpression::Nullary::SIM_TIME);}
| EVENT
	{$$ = new NullaryOperation(@$,BaseExpression::Nullary::SIM_EVENT);}
| NULL_EVENT
	{$$ = new NullaryOperation(@$,BaseExpression::Nullary::NULL_EVENT);}
| PROD_EVENT
	{$$ = new NullaryOperation(@$,BaseExpression::Nullary::PROD_EVENT);}
| ACTIVITY
	{$$ = new NullaryOperation(@$,BaseExpression::Nullary::ACTIVITY);}
| CPUTIME
	{$$ = new NullaryOperation(@$,BaseExpression::Nullary::CPUTIME);}
| MINUS alg_expr
	{$$ = new AlgBinaryOperation(@$,new Const(location(),0),$2,BaseExpression::AlgebraicOp::MINUS);}
| error
	{}
;


rate:
  rate_sep alg_expr OP_PAR alg_with_radius CL_PAR 
	{$$=ast::Rate(@$,$2,$1,&$4);}
| rate_sep alg_expr 
	{$$=ast::Rate(@$,$2,$1);}
| rate_sep alg_expr COMMA alg_expr 
	{$$=ast::Rate(@$,$2,$1,$4);}
;


alg_with_radius:
  alg_expr {$$=ast::Radius(@$,$1);}
| alg_expr TYPE alg_expr {$$=ast::Radius(@$,$1,$3);}
;

/*
multiple_mixture:
 alg_expr non_empty_mixture //conflict here because ID (blah) could be token non_empty mixture or mixture...
	{//$$=ast::MultipleMixture(@$,$2,$1);
		$$=std::make_pair($1,$2);
	}
| non_empty_mixture 
	{//$$=ast::MultipleMixture(@$,$1,new Const(yy::location(),1));
		$$=std::make_pair(new Const(yy::location(),1),$1);
	}
;
*/

non_empty_mixture:
  OP_PAR non_empty_mixture CL_PAR
	{ $$ = $2; }
| agent_expression COMMA non_empty_mixture  
	{
		$3.push_front($1);
		$$ = $3;
	}
| agent_expression 
	{ $$ = std::list<Agent>(1,$1); }
;


/*Make a list for interface_expression*/
agent_expression:
  ID OP_PAR interface_expression CL_PAR 
	{ $$ = Agent(@$,Id(@1,$1),$3); }
| ID error 
	{yy::KappaParser::error(@1,std::string("Malformed agent ")+$1);}
;


interface_expression:
/* empty */
	{$$=std::list<ast::Site>();}
| ne_interface_expression 
	{$$ = $1;}
;


ne_interface_expression:
  port_expression COMMA ne_interface_expression 
	{
		$3.push_front($1);
		$$=$3;
	}
| port_expression  
	{$$=std::list<ast::Site>(1,$1);}
;


port_expression:
 ID internal_state link_state 
	{$$=Site(@$,Id(@1,$1),$2,$3);}
;


internal_state:
 state_enum 
	{$$ = SiteState(@$,$1);}
| KAPPA_VALUE alg_expr SMALLER EQUAL ID SMALLER EQUAL alg_expr CL_CUR
	{$$ = SiteState(@$,Id(@5,$5),SiteState::MIN_EQUAL + SiteState::MAX_EQUAL,$2,$8);}
| KAPPA_VALUE alg_expr SMALLER EQUAL ID SMALLER alg_expr CL_CUR
	{$$ = SiteState(@$,Id(@5,$5),SiteState::MIN_EQUAL,$2,$7);}
| KAPPA_VALUE alg_expr SMALLER EQUAL ID CL_CUR
	{$$ = SiteState(@$,Id(@5,$5),SiteState::MIN_EQUAL,$2,nullptr);}
| KAPPA_VALUE alg_expr SMALLER ID SMALLER EQUAL alg_expr CL_CUR
	{$$ = SiteState(@$,Id(@4,$4),SiteState::MAX_EQUAL,$2,$7);}
| KAPPA_VALUE alg_expr SMALLER ID SMALLER alg_expr CL_CUR
	{$$ = SiteState(@$,Id(@4,$4),0,$2,$6);}
| KAPPA_VALUE alg_expr SMALLER ID CL_CUR
	{$$ = SiteState(@$,Id(@4,$4),0,$2,nullptr);}
| KAPPA_VALUE ID SMALLER EQUAL alg_expr CL_CUR
	{$$ = SiteState(@$,Id(@2,$2),SiteState::MAX_EQUAL,nullptr,$5);}
| KAPPA_VALUE ID SMALLER alg_expr CL_CUR
	{$$ = SiteState(@$,Id(@2,$2),0,nullptr,$4);}
| KAPPA_VALUE ID EQUAL alg_expr CL_CUR
	{$$ = SiteState(@$,Id(@2,$2),$4);}
| KAPPA_VALUE ID CL_CUR
	{$$ = SiteState(@$,Id(@2,$2),nullptr);}
| KAPPA_VALUE alg_expr CL_CUR
	{$$ = SiteState(@$,$2);}

| KAPPA_INTER alg_expr COMMA alg_expr CL_BRA //default is smallest
	{$$ = SiteState(@$,$2,$4);}
| KAPPA_INTER alg_expr COMMA alg_expr CL_BRA OP_PAR alg_expr CL_PAR //default is third alg_expr
	{$$ = SiteState(@$,$2,$4,$7);}

| error
	{yy::KappaParser::error(@1,"Invalid internal state");}
;


state_enum:
/*empty*/ 
	{$$=std::list<Id>(); }
| KAPPA_MRK state_enum
	{
		$2.push_front(Id(@1,$1));
		$$=$2;
	 }
| error 
	{yy::KappaParser::error(@1,"Invalid internal state");}
;


link_state:
/*empty*/ 
	{$$ = ast::Link(@$,ast::Link::FREE);}
| KAPPA_LNK INT 
	{$$ = ast::Link(@$,ast::Link::VALUE,$2);}
| KAPPA_LNK KAPPA_SEMI 
	{$$ = ast::Link(@$,ast::Link::SOME);}
| KAPPA_LNK ID DOT ID
	{$$ = ast::Link(@$,ast::Link::AG_SITE,ast::Id(@4,$4),ast::Id(@2,$2));}
| KAPPA_WLD 
	{$$ = ast::Link(@$,ast::Link::ANY);}
| KAPPA_LNK error 
	{yy::KappaParser::error(@1,"Invalid link state");}
;

%%
/**
 * CODE
 */

void yy::KappaParser::error(const location &loc , const std::string &message) {
	// Location should be initialized inside scanner action, but is not in this example.
	// Let's grab location directly from driver class.
	// cout << "Error: " << message << endl << "Location: " << loc << endl;
	//cout << "Error in " << loc << " : " << message << "." << endl;

	throw SyntaxError( message, loc );
}
