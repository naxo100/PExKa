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
	#include "ast/AstStructs.h"
	
	using namespace std;
	
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
    #include "location.hh"
    
	#define yylex(x) x.getNextToken()

	using namespace yy;
}

%code {
    #include "KappaDriver.h"
}

%token END NEWLINE SEMICOLON
%token AT ATD FIX OP_PAR CL_PAR OP_BRA CL_BRA COMMA DOT TYPE LAR OP_CUR CL_CUR JOIN FREE
%token LOG PLUS MULT MINUS AND OR GREATER SMALLER EQUAL PERT INTRO DELETE DO SET UNTIL TRUE FALSE OBS KAPPA_RAR TRACK CPUTIME CONFIG REPEAT DIFF
%token KAPPA_WLD KAPPA_SEMI SIGNATURE INFINITY TIME EVENT ACTIVITY NULL_EVENT PROD_EVENT INIT LET DIV PLOT SINUS COSINUS TAN ATAN COIN RAND_N SQRT EXPONENT POW ABS MODULO 
%token EMAX TMAX RAND_1 FLUX ASSIGN ASSIGN2 TOKEN KAPPA_LNK PIPE KAPPA_LRAR PRINT PRINTF /*CAT VOLUME*/ MAX MIN
%token <int> INT 
%token <std::string> ID LABEL KAPPA_MRK NAME 
%token <float> FLOAT 
%token <std::string> STRING
%token STOP SNAPSHOT

%token COMPARTMENT C_LINK TRANSPORT USE

%left MINUS PLUS MIN MAX
%left MULT DIV
%left MODULO
%right POW 
%nonassoc LOG SQRT EXPONENT SINUS COSINUS ABS TAN

%left OR
%left AND


/*%type <ast::Agent> agent_expression*/
%type <ast::Declaration> variable_declaration
%type <ast::Expression> alg_expr constant variable
%type <ast::Link> link_state
%start statements

%%
statements:
| statements statement newline
	{}
;

newline:
NEWLINE {}
| END {return 0;}


statement:
| rule_expression
	{}
| instruction
	{} 
| error 
	{}
;


instruction:
/* */
| COMPARTMENT comp_expr alg_expr
 	{}
| C_LINK LABEL comp_expr arrow comp_expr ATD constant
 	{}
| TRANSPORT join LABEL mixture AT alg_expr
 	{}
| USE comp_list
 	{}
| SIGNATURE agent_expression  
	{}
| TOKEN ID
	{}
| SIGNATURE error
	{}
| INIT init_declaration 
	{}
| INIT error
	{}
| LET variable_declaration 
	{driver.addDeclaration($2);}
| OBS variable_declaration
	{}
| PLOT alg_expr 
	{}
| PLOT error 
	{}
| PERT perturbation_declaration {}
| PERT REPEAT perturbation_declaration UNTIL bool_expr 
	{}
| CONFIG STRING value_list 
	{}  
| PERT bool_expr DO effect_list UNTIL bool_expr /*for backward compatibility*/
	{}
;

init_declaration:
| multiple non_empty_mixture 
	{}
| ID LAR multiple {}
| ID OP_CUR init_declaration CL_CUR {}
;


/*SPATIAL*/
join:
/*empty*/
	{}
| JOIN
	{}
| FREE
	{}

comp_expr:
LABEL dimension where_expr
	{}
;

dimension: 
/*empty*/
	{}
| OP_BRA index_expr CL_BRA dimension
	{}
;

index_expr:
INT
	{}
| ID /*iter var*/
	{}
| OP_PAR index_expr CL_PAR 
	{}
| index_expr MULT index_expr
	{}
| index_expr PLUS index_expr
	{}
| index_expr DIV index_expr
	{}
| index_expr MINUS index_expr
	{}
| index_expr POW index_expr
	{}
| index_expr MODULO index_expr
	{}	
;

value_list: 
| STRING 
	{}
| STRING value_list 
	{}
;

comp_list:
	{}
| "$ALL"
  	{}
| comp_expr
	{}
| comp_expr comp_list
	{}
| comp_expr COMMA comp_list
	{}
;

where_expr: 
/* empty */
	{}
| OP_CUR bool_expr CL_CUR
	{}
;


perturbation_declaration:
| OP_PAR perturbation_declaration CL_PAR {}
| bool_expr DO effect_list {}
| bool_expr SET effect_list {} /*For backward compatibility*/
;

effect_list:
| OP_PAR effect_list CL_PAR {}
| effect {}
| effect SEMICOLON effect_list {}
;

effect:
| LABEL ASSIGN alg_expr /*updating the rate of a rule -backward compatibility*/
	{}
| ASSIGN2 LABEL alg_expr /*updating the rate of a rule*/
	{}
| TRACK LABEL boolean 
	{}
| FLUX opt_string boolean 
	{}
| INTRO multiple_mixture 
	{}
| INTRO error
	{}
| DELETE multiple_mixture 
	{}
| DELETE error
	{}
| ID LAR alg_expr /*updating the value of a token*/
	{}
| SNAPSHOT opt_string
	{}
| STOP opt_string
	{}
| PRINT SMALLER print_expr GREATER {}
| PRINTF string_or_pr_expr SMALLER print_expr GREATER 
	{}
;


print_expr:
/*empty*/ {}
| STRING {}
| alg_expr {}
| STRING DOT print_expr {}
| alg_expr DOT print_expr {}
;

boolean:
| TRUE {}
| FALSE {}
;

variable_declaration:
 LABEL non_empty_mixture 
 	{}
| LABEL alg_expr 
	{$$ = ast::Declaration(ast::Id($1,@1),$2,@$);}
| LABEL error 
	{}
;

bool_expr:
| OP_PAR bool_expr CL_PAR 
	{}
| bool_expr AND bool_expr 
	{}
| bool_expr OR bool_expr 
	{}
| alg_expr GREATER alg_expr 
	{}
| alg_expr SMALLER alg_expr 
	{}
| alg_expr EQUAL alg_expr 
	{}
| alg_expr DIFF alg_expr  
	{}
| TRUE
	{}
| FALSE
	{}
;

opt_string:
/*empty*/ {}
| STRING {}
| SMALLER print_expr GREATER {}
;

string_or_pr_expr:
| STRING {}
| SMALLER print_expr GREATER {}
;


multiple:
| INT {  }
| FLOAT { }
| LABEL { }
;

rule_label: 
/*empty */
	{}
| LABEL 
	{}
;

lhs_rhs:
mixture token_expr {}
;

token_expr:
/*empty*/ {}
| PIPE sum_token {} 
| PIPE error 
	{}
;

sum_token:
| OP_PAR sum_token CL_PAR 
	{} 
| alg_expr TYPE ID 
	{}
| alg_expr TYPE ID PLUS sum_token 
	{}

mixture:
/*empty*/ 
	{}
| non_empty_mixture 
	{}
;

rate_sep:
| AT {}
| FIX {}

/*(**  **)*/

rule_expression:
| rule_label lhs_rhs arrow lhs_rhs rate_sep rate 
	{ }
| rule_label lhs_rhs arrow lhs_rhs 
	{}
;



arrow:
| KAPPA_RAR 
	{}
| KAPPA_LRAR
	{}
;

constant:
 INFINITY
	{$$ = ast::Const(ast::Const::INF,@1);}
| FLOAT
	{$$ = ast::Const($1,@1);}
| INT 
	{$$ = ast::Const($1,@1);}
| EMAX
	{$$ = ast::Const(ast::Const::EMAX,@1);}
| TMAX
	{$$ = ast::Const(ast::Const::TMAX,@1);}
;

variable:
 PIPE ID PIPE 
	{ast::Var($2,ast::Var::TOKEN,@$);}
| LABEL 
	{ast::Var($1,ast::Var::VAR,@$);}
| TIME
	{$$ = ast::Var(ast::Var::TIME,@$);}
| EVENT
	{$$ = ast::Var(ast::Var::EVENT,@$);}
| NULL_EVENT
	{$$ = ast::Var(ast::Var::NULL_EVENT,@$);}
| PROD_EVENT
	{$$ = ast::Var(ast::Var::PROD_EVENT,@$);}
| ACTIVITY
	{$$ = ast::Var(ast::Var::ACTIVITY,@$);}
| CPUTIME
	{$$ = ast::Const(ast::Var::CPUTIME,@1);}
;

alg_expr:
 OP_PAR alg_expr CL_PAR 
	{$$ = $2;}
| constant 
	{$$ = $1;}
| variable
	{$$ = $1;}
| ID
	{}
| alg_expr MULT alg_expr
	{$$ = ast::BinaryOperation($1,$3,ast::BinaryOperation::MULT,@2);}
| alg_expr PLUS alg_expr
	{$$ = ast::BinaryOperation($1,$3,ast::BinaryOperation::SUM,@2);}
| alg_expr DIV alg_expr
	{$$ = ast::BinaryOperation($1,$3,ast::BinaryOperation::DIV,@2);}
| alg_expr MINUS alg_expr
	{$$ = ast::BinaryOperation($1,$3,ast::BinaryOperation::MINUS,@2);}
| alg_expr POW alg_expr
	{$$ = ast::BinaryOperation($1,$3,ast::BinaryOperation::POW,@2);}
| alg_expr MODULO alg_expr
	{$$ = ast::BinaryOperation($1,$3,ast::BinaryOperation::MODULO,@2);}	
| MAX alg_expr alg_expr
	{$$ = ast::BinaryOperation($2,$3,ast::BinaryOperation::MAX,@2);}
| MIN alg_expr alg_expr
	{$$ = ast::BinaryOperation($2,$3,ast::BinaryOperation::MIN,@2);}
| EXPONENT alg_expr 
	{$$ = ast::UnaryOperation($2,ast::UnaryOperation::EXPONENT,@2);}
| SINUS alg_expr
	{$$ = ast::UnaryOperation($2,ast::UnaryOperation::SINUS,@2);}
| COSINUS alg_expr
	{$$ = ast::UnaryOperation($2,ast::UnaryOperation::COSINUS,@2);}
| TAN alg_expr 
	{$$ = ast::UnaryOperation($2,ast::UnaryOperation::TAN,@2);}
| ABS alg_expr
	{$$ = ast::UnaryOperation($2,ast::UnaryOperation::ABS,@2);}
| SQRT alg_expr
	{$$ = ast::UnaryOperation($2,ast::UnaryOperation::SQRT,@2);}
| LOG alg_expr
	{$$ = ast::UnaryOperation($2,ast::UnaryOperation::LOG,@2);}
/*(***)*/
| ATAN alg_expr
	{$$ = ast::UnaryOperation($2,ast::UnaryOperation::ATAN,@2);}
| COIN alg_expr
	{$$ = ast::UnaryOperation($2,ast::UnaryOperation::COIN,@2);}
| RAND_N alg_expr
	{$$ = ast::UnaryOperation($2,ast::UnaryOperation::RAND_N,@2);}
| RAND_1
	{}
;

rate:
| alg_expr OP_PAR alg_with_radius CL_PAR 
	{}
| alg_expr 
	{}
| alg_expr COMMA alg_expr 
	{}
;

alg_with_radius:
| alg_expr {}
| alg_expr TYPE alg_expr {}
;

multiple_mixture:
| alg_expr non_empty_mixture /*conflict here because ID (blah) could be token non_empty mixture or mixture...*/
	{}
| non_empty_mixture 
	{}
;

non_empty_mixture:
| OP_PAR non_empty_mixture CL_PAR
	{}
| agent_expression COMMA non_empty_mixture  
	{}
| agent_expression 
	{}
;

agent_expression:
 ID OP_PAR interface_expression CL_PAR 
	{}
| ID error 
	{}
;

interface_expression:
/*empty*/ 
	{}
| ne_interface_expression 
	{}
;

ne_interface_expression:
| port_expression COMMA ne_interface_expression 
	{}
| port_expression  
	{}
;


port_expression:
| ID internal_state link_state 
	{}
;

internal_state:
/*empty*/ {}
| KAPPA_MRK internal_state 
	{}
| error 
	{}
;

link_state:
/*empty*/ 
	{$$ = ast::Link(ast::Link::FREE,@$);}
| KAPPA_LNK INT 
	{$$ = ast::Link(ast::Link::VALUE,$2,@2);}
| KAPPA_LNK KAPPA_SEMI 
	{$$ = ast::Link(ast::Link::SOME,@2);}
| KAPPA_LNK ID DOT ID
	{$$ = ast::Link(ast::Link::TYPE,ast::Id($2,@2),ast::Id($4,@4),@1);}
| KAPPA_WLD 
	{$$ = ast::Link(ast::Link::ANY,@1);}
| KAPPA_LNK error 
	{}
;

%%

void yy::KappaParser::error(const location &loc , const std::string &message) {
        
        // Location should be initialized inside scanner action, but is not in this example.
        // Let's grab location directly from driver class.
	// cout << "Error: " << message << endl << "Location: " << loc << endl;
	
        cout << "Error: " << message << endl << "Error location: " << loc << endl;
}
