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
%type <ast::Expression> alg_expr bool_expr  constant variable multiple 
%type <ast::Arrow> arrow 
%type <bool> rate_sep boolean join
%type <std::list<std::string>> value_list internal_state
%type <ast::Link> link_state
%type <ast::CompExpression> comp_expr
%type <ast::Site> port_expression
%type <std::list<ast::Site>> interface_expression
%type <ast::Agent> agent_expression
%type <std::list<ast::Agent>> non_empty_mixture
%type <std::list<ast::Agent>> mixture
%type <ast::Multiple_Mixture> multiple_mixture
%type <std::list<ast::CompExpression>> comp_list
%type <std::list<ast::Expression>> dimension
%type <ast::Expression*> where_expr
%type <std::list<ast::PrintObj>> print_expr
%type <ast::Effect> effect
%type <std::list<ast::Effect>> effect_list
%type <ast::OptString> opt_string string_or_pr_expr
%type <ast::Radius>   alg_with_radius   
%type <ast::Rate> rate
%type <ast::Perturbation> perturbation_declaration
%type <std::list<ast::Token>> sum_token token_expr
%type <ast::RuleSide> lhs_rhs
%type <ast::Id> rule_label
%type <ast::Rule> rule_expression
%type <ast::Init_Declaration> init_declaration
/*%type <std::list> value_list*/
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
 	{cout<<"Hola";}
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
| PERT perturbation_declaration 
	{}
| PERT REPEAT perturbation_declaration UNTIL bool_expr 
	{}
| CONFIG STRING value_list 
	{}  
| PERT bool_expr DO effect_list UNTIL bool_expr /*for backward compatibility*/
	{}
;

init_declaration:
| multiple non_empty_mixture 
	{$$=ast::Init_Declaration(ast::Init_t($1,$2),NULL);}
| ID LAR multiple
	{$$=ast::Init_Declaration(ast::Init_t($3,$1),NULL);}
| ID OP_CUR init_declaration CL_CUR 
	{
		ast::Id id($1);
		$$=ast::Init_Declaration($3.init_t,&id);
	}
;


/*SPATIAL*/
join:
/*empty*/
	{$$=true;}
| JOIN
	{$$=true;}
| FREE
	{$$=false;}

comp_expr:
  LABEL dimension where_expr
	{$$=ast::CompExpression($1,std::list<ast::Expression>(),NULL,@1);}
;

dimension: 
/*empty*/
	{$$=std::list<ast::Expression>();}
| OP_BRA alg_expr CL_BRA dimension
	{
		$4.push_front($2);
 		$$=$4;
	}
;

where_expr:
 /*empty*/
 	{$$=NULL;}
| OP_CUR bool_expr CL_CUR
	{$$=&$2;}
;

/*index_expr:
INT
	{}
| ID 
	{}
| OP_PAR index_expr CL_PAR 
	{$$=$2;}
| index_expr MULT index_expr
	{$$=ast::IndexOperation($1,$3,ast::IndexOperation::MULT,@2);}
| index_expr PLUS index_expr
	{$$=ast::IndexOperation($1,$3,ast::IndexOperation::SUM,@2);}
| index_expr DIV index_expr
	{$$=ast::IndexOperation($1,$3,ast::IndexOperation::DIV,@2);}
| index_expr MINUS index_expr
	{$$=ast::IndexOperation($1,$3,ast::IndexOperation::MINUS,@2);}
| index_expr POW index_expr
	{$$=ast::IndexOperation($1,$3,ast::IndexOperation::POW,@2);}
| index_expr MODULO index_expr
	{$$=ast::IndexOperation($1,$3,ast::IndexOperation::MODULO,@2);}
;*/

value_list: 
| STRING 
	{$$=std::list<std::string>(1,$1); }
| STRING value_list 
	{
		$2.push_front($1);
		$$=$2;
	 }
;

comp_list:
	{$$=std::list<ast::CompExpression>(); }
| "$ALL"
	{$$=std::list<ast::CompExpression>(); }
| comp_expr
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
| OP_PAR perturbation_declaration CL_PAR {$$=$2;}
| bool_expr DO effect_list {$$=ast::Perturbation($1,$3,@2);}
| bool_expr SET effect_list
	{
		// temporal, preguntarle a naxo
		cout<<"Deprecated perturbation syntax: 'set' keyword is replaced by 'do'"<<endl;
		$$=ast::Perturbation($1,$3,@2);
	} /*For backward compatibility*/
;

effect_list:
| OP_PAR effect_list CL_PAR {$$=$2;}
| effect {$$=std::list<ast::Effect>(1,$1);}
| effect SEMICOLON effect_list 
	{
		$3.push_front($1);
		$$=$3;
	}
;

effect:
| LABEL ASSIGN alg_expr /*updating the rate of a rule -backward compatibility*/
	{$$=ast::Effect(ast::Effect::UPDATE,std::string($1),$3,@2);}
| ASSIGN2 LABEL alg_expr /*updating the rate of a rule*/
	{$$=ast::Effect(ast::Effect::UPDATE,std::string($2),$3,@1);}
| TRACK LABEL boolean 
	{
		if($3)
			$$=ast::Effect(ast::Effect::CFLOW   ,std::string($2),@1);
		else 
			$$=ast::Effect(ast::Effect::CFLOWOFF,std::string($2),@1);
	}

| FLUX opt_string boolean 
	{
		ast::Effect::Action action= $3 ? ast::Effect::FLUX : ast::Effect::FLUXOFF;

		if($2.str_ptr   != NULL) 
			$$=ast::Effect(action,std::list<ast::PrintObj>(1,ast::PrintObj(*($2.str_ptr),@2)),@1);
		else if($2.print_list_ptr != NULL) 
			$$=ast::Effect(action,*($2.print_list_ptr),@1);
		else
			$$=ast::Effect(action,std::list<ast::PrintObj>(),@1);
	}
| INTRO multiple_mixture 
	{$$=ast::Effect(ast::Effect::INTRO,$2.alg,$2.mix,@1);}
| INTRO error
	{}
| DELETE multiple_mixture 
	{$$=ast::Effect(ast::Effect::DELETE,$2.alg,$2.mix,@1);}
| DELETE error
	{}
| ID LAR alg_expr /*updating the value of a token*/
	{$$=ast::Effect(ast::Effect::UPDATE_TOK,$1,$3,@2);}
| SNAPSHOT opt_string
	{	
		if($2.str_ptr   != NULL) 
			$$=ast::Effect(ast::Effect::SNAPSHOT,std::list<ast::PrintObj>(1,ast::PrintObj(*($2.str_ptr),@2)),@1);
		else if($2.print_list_ptr != NULL) 
			$$=ast::Effect(ast::Effect::SNAPSHOT,*($2.print_list_ptr),@1);
		else
			$$=ast::Effect(ast::Effect::SNAPSHOT,std::list<ast::PrintObj>(),@1);
	}
| STOP opt_string
	{
		if($2.str_ptr   != NULL) 
			$$=ast::Effect(ast::Effect::STOP,std::list<ast::PrintObj>(1,ast::PrintObj(*($2.str_ptr),@2)),@1);
		else if($2.print_list_ptr != NULL) 
			$$=ast::Effect(ast::Effect::STOP,*($2.print_list_ptr),@1);
		else
			$$=ast::Effect(ast::Effect::STOP,std::list<ast::PrintObj>(),@1);

	}
| PRINT SMALLER print_expr GREATER 
	{$$=ast::Effect(ast::Effect::PRINT,std::list<ast::PrintObj>(),$3,@1);}
| PRINTF string_or_pr_expr SMALLER print_expr GREATER 
	{
		if($2.str_ptr   != NULL) 
			$$=ast::Effect(ast::Effect::PRINT,std::list<ast::PrintObj>(1,ast::PrintObj(*($2.str_ptr),@2)),$4,@1);
		else if($2.print_list_ptr != NULL) 
			$$=ast::Effect(ast::Effect::PRINT,*($2.print_list_ptr),$4,@1);
		else
			$$=ast::Effect(ast::Effect::PRINT,std::list<ast::PrintObj>(),$4,@1);
	}
;


print_expr:
/*empty*/ 
	{ $$=std::list<ast::PrintObj>(); }
| STRING 
	{$$=std::list<ast::PrintObj>(1,ast::PrintObj($1,@1)); }
| alg_expr 
	{$$=std::list<ast::PrintObj>(1,ast::PrintObj($1,@1)); }
| STRING DOT print_expr 
	{
		$3.push_front(ast::PrintObj($1,@1));
		$$=$3;
	}
| alg_expr DOT print_expr 
	{
		$3.push_front(ast::PrintObj($1,@1));
		$$=$3;
	}
;

boolean:
| TRUE {$$=true;}
| FALSE {$$=false;}
;

variable_declaration:
 LABEL non_empty_mixture 
	{$$ = ast::Declaration(ast::Id($1,@1),$2,@$);}
| LABEL alg_expr 
	{$$ = ast::Declaration(ast::Id($1,@1),$2,@$);}
| LABEL error 
	{}
;

bool_expr:
| OP_PAR bool_expr CL_PAR 
	{$$=$2;}
| bool_expr AND bool_expr 
	{$$ = ast::BoolOperation($1,$3,ast::BoolOperation::AND,@2);}
| bool_expr OR bool_expr 
	{$$ = ast::BoolOperation($1,$3,ast::BoolOperation::OR,@2);}
| alg_expr GREATER alg_expr 
	{$$ = ast::BoolOperation($1,$3,ast::BoolOperation::GREATER,@2);}
| alg_expr SMALLER alg_expr 
	{$$ = ast::BoolOperation($1,$3,ast::BoolOperation::SMALLER,@2);}
| alg_expr EQUAL alg_expr 
	{$$ = ast::BoolOperation($1,$3,ast::BoolOperation::EQUAL,@2);}
| alg_expr DIFF alg_expr  
	{$$ = ast::BoolOperation($1,$3,ast::BoolOperation::DIFF,@2);}
| TRUE
	{$$ = ast::Bool(true,@1);}
| FALSE
	{$$ = ast::Bool(false,@1);}
;

opt_string:
/*empty*/ {$$=(ast::OptString(NULL,NULL));}
| STRING  {$$=(ast::OptString(&$1,NULL));}
| SMALLER print_expr GREATER {$$=(ast:: OptString(NULL,&$2));}
;

string_or_pr_expr:
| STRING {$$=(ast::OptString(&$1,NULL));}
| SMALLER print_expr GREATER {$$=(ast::OptString(NULL,&$2));}
;


multiple:
| INT   {$$ = ast::Const($1,@1);}
| FLOAT	{$$ = ast::Const($1,@1);}
| LABEL {$$ = ast::Var($1,ast::Var::VAR,@1); }
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

rule_label: 
/*empty */
	{$$=ast::Id();}
| LABEL 
	{$$=ast::Id($1,@1);}
;

lhs_rhs:
mixture token_expr {$$=ast::RuleSide($1,$2);}
;

token_expr:
/*empty*/ {$$=std::list<ast::Token>();}
| PIPE sum_token {$$=$2;} 
| PIPE error 
	{}
;

sum_token:
| OP_PAR sum_token CL_PAR 
	{$$=$2;} 
| alg_expr TYPE ID 
	{$$=std::list<ast::Token>(1,ast::Token($1,ast::Id($3)));}
| alg_expr TYPE ID PLUS sum_token 
	{
		$5.push_front(ast::Token($1,ast::Id($3)));
		$$=$5;
	}

mixture:
/*empty*/ 
	{$$=std::list<ast::Agent>();}
| non_empty_mixture 
	{$$=$1;}
;

rate_sep:
| AT {$$=false;}
| FIX {$$=true;}

/*(**  **)*/
/*{$$=ast::Rule($1,$2.agents,$4.agents,$2.tokens,$4.tokens,$3,$6.);}*/

rule_expression:
| rule_label lhs_rhs arrow lhs_rhs rate_sep rate 
	{
		if(($3.type() == ast::Arrow::BI && $6.op == NULL) || ($3.type() == ast::Arrow::RIGHT && $6.op != NULL)) 
			yy::KappaParser::error(@1,"Malformed bi-directional rule expression");
		$$=ast::Rule($1,$2,$4,$3,$6,-1,NULL,$5,@1);
	}
| rule_label lhs_rhs arrow lhs_rhs 
	{
		cout<<"Warning: Rule has no kinetics. Default rate of 0.0 is assumed."<<endl;
		ast::Rate rate(ast::Const(0.0f,yy::location()),NULL,NULL);
		$$=ast::Rule($1,$2,$4,$3,rate,-1,NULL,true,@1);;
	}
;



arrow:
 KAPPA_RAR 
	{$$=ast::Arrow(ast::Arrow::RIGHT,@1);}
| KAPPA_LRAR
	{$$=ast::Arrow(ast::Arrow::BI,@1);}
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
	{$$ = ast::Var($1,ast::Var::TOKEN,@1);}
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
	{$$=ast::Rate($1,&$3,NULL);}
| alg_expr 
	{$$=ast::Rate($1,NULL,NULL);}
| alg_expr COMMA alg_expr 
	{$$=ast::Rate($1,NULL,&$3);}
;

alg_with_radius:
| alg_expr {$$=ast::Radius($1);}
| alg_expr TYPE alg_expr {$$=ast::Radius($1,&$3);}
;

multiple_mixture:
| alg_expr non_empty_mixture /*conflict here because ID (blah) could be token non_empty mixture or mixture...*/
	{$$=ast::Multiple_Mixture($1,$2);}
| non_empty_mixture 
	{$$=ast::Multiple_Mixture(ast::Const(1.0f,yy::location()),$1);}
;

non_empty_mixture:
| OP_PAR non_empty_mixture CL_PAR
	{$$=$2;}
| agent_expression COMMA non_empty_mixture  
	{
		$3.push_front($1);
		$$=$3;
	}
| agent_expression 
	{$$=std::list<ast::Agent>(1,$1);}
;
/*Make a list for interface_expression*/
agent_expression:
 ID OP_PAR interface_expression CL_PAR 
	{$$=ast::Agent($1,$3,@1);}
| ID error 
	{yy::KappaParser::error(@1,std::string("Malformed agent ")+$1);}
;

interface_expression: 
	/*empty*/
	{$$=std::list<ast::Site>();}
| port_expression
	{$$=std::list<ast::Site>(1,$1);}
| port_expression COMMA interface_expression
{
	$3.push_front($1);
	$$=$3;
};

/*interface_expression: 
	{}
| ne_interface_expression 
	{}
;*/

/*ne_interface_expression:
| port_expression COMMA ne_interface_expression 
	{}
| port_expression  
	{}
;*/


port_expression:
| ID internal_state link_state 
	{$$=ast::Site($1,$2,$3,@1);}
;

internal_state:
/*empty*/ 
	{$$=std::list<std::string>(); }
| KAPPA_MRK internal_state 
	{
		$2.push_front($1);
		$$=$2;
	 }
| error 
	{yy::KappaParser::error(@1,"Invalid internal state");}
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
	{yy::KappaParser::error(@1,"Invalid link state");}
;

%%

void yy::KappaParser::error(const location &loc , const std::string &message) {
        
        // Location should be initialized inside scanner action, but is not in this example.
        // Let's grab location directly from driver class.
	// cout << "Error: " << message << endl << "Location: " << loc << endl;
	
        cout << "Error: " << message << endl << "Error location: " << loc << endl;
}
