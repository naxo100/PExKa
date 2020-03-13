%{
	#include <iostream>
	#include "Lexer.h"
	#include "Parser.hpp"
	#include "../KappaDriver.h"
	#include "../location.hh"
	#include "../../util/Exceptions.h"
	
	#define loc (*(driver.getLocation()))
	#define yyterminate() Parser::make_END(loc) 
	
	// Scanner method signature is defined by this macro. Original yylex() returns int.
	// Since Bison 3 uses symbol_type, we must change returned type. We also rename it
	// to something sane, since you cannot overload return type.
	#undef YY_DECL
	#define YY_DECL grammar::kappa4::Parser::symbol_type grammar::kappa4::Lexer::get_next_token()
	
	using namespace yy;
	


	// Code run each time a pattern is matched.
	# define YY_USER_ACTION  loc.columns (yyleng);
	// The location of the current token.
		
	# undef yywrap
	# define yywrap() 1
%}

%option nounput batch debug noinput
%option yylineno
%option nodefault
%option noyywrap
%option c++
%option 8bit warn nodefault
%option yyclass="Lexer"
%option prefix="kappa4_"
/*%option debug*/


blank		[\t ]
digit		[0-9]
letter		[a-zA-Z]
id		[a-zA-Z][a-zA-Z0-9_]*
real		([0-9]+\.[0-9]*|[0-9]*\.[0-9]+)(([eE][+-][0-9]+)|([eE][0-9]+))?
%%

%{
  // Code run each time yylex is called.
  loc.step ();
%}

"\\\n"		{loc.step();loc.lines();}
"#"+[.]*"\\\n"	{loc.step();loc.lines();}//return Parser::make_NEWLINE(loc);	
"#"+[.]*[^\n]*	loc.step();//return Parser::make_NEWLINE(loc);
[\t ]+		{loc.step();}
{digit}+	{
				return Parser::make_INT(atoi(yytext),loc); 
			}
\-?{real}	{
				return Parser::make_FLOAT(atof(yytext),loc);
			}
"%"{id}":"	{
			if(!strcmp(yytext,"%agent:"))
				return Parser::make_SIGNATURE(loc);
			else if(!strcmp(yytext,"%init:"))
				return Parser::make_INIT(loc);
			else if(!strcmp(yytext,"%var:"))
				return Parser::make_LET(loc);
			else if(!strcmp(yytext,"%const:"))
				return Parser::make_CONST(loc);
			else if(!strcmp(yytext,"%plot:"))
				return Parser::make_PLOT(loc);
			else if(!strcmp(yytext,"%mod:"))
				return Parser::make_PERT(loc);
			else if(!strcmp(yytext,"%obs:"))
				return Parser::make_OBS(loc);
			else if(!strcmp(yytext,"%def:"))
				return Parser::make_CONFIG(loc);
			else if(!strcmp(yytext,"%token:"))
				return Parser::make_TOKEN(loc);
			else if(!strcmp(yytext,"%compartment:"))
				return Parser::make_COMPARTMENT(loc);
			else if(!strcmp(yytext,"%channel:"))
				return Parser::make_CHANNEL(loc);
			else if(!strcmp(yytext,"%transport:"))
				return Parser::make_TRANSPORT(loc);
			else if(!strcmp(yytext,"%use:"))
				return Parser::make_USE(loc);
			else{
				char msg[150];
				std::sprintf(msg,"Statement '%%%s' not recognized by expanded Kappa4.",yytext);

				throw SyntaxError(msg, loc);
				return Parser::make_NEWLINE(loc);
			}
		}
"do"		return Parser::make_DO(loc);
"set"		return Parser::make_SET(loc);
"repeat"	return Parser::make_REPEAT(loc);
"until"		return Parser::make_UNTIL(loc);
"&&"		return Parser::make_AND(loc);
"||"		return Parser::make_OR(loc);
"<->"		return Parser::make_KAPPA_LRAR(loc);
"->"		return Parser::make_KAPPA_RAR(loc);
"<-"		return Parser::make_LAR(loc);
":="		return Parser::make_ASSIGN(loc);
"<>"		return Parser::make_DIFF(loc);
\${letter}+	{
	if(!strcmp(yytext,"$DEL"))
		return Parser::make_DELETE(loc);
	else if(!strcmp(yytext,"$ADD"))
		return Parser::make_INTRO(loc);
	else if(!strcmp(yytext,"$SNAPSHOT"))
		return Parser::make_SNAPSHOT(loc);
	else if(!strcmp(yytext,"$HISTOGRAM"))
		return Parser::make_SNAPSHOT(loc);
	else if(!strcmp(yytext,"$STOP"))
		return Parser::make_STOP(loc);
	else if(!strcmp(yytext,"$FLUX"))
		return Parser::make_FLUX(loc);
	else if(!strcmp(yytext,"$TRACK"))
		return Parser::make_TRACK(loc);
	else if(!strcmp(yytext,"$UPDATE"))
		return Parser::make_ASSIGN2(loc);
	else if(!strcmp(yytext,"$PRINT"))
		return Parser::make_PRINT(loc);
	else if(!strcmp(yytext,"$PRINTF"))
		return Parser::make_PRINTF(loc);
	else{					
		char msg[150];
		std::sprintf(msg,"Perturbation '[%s]' not recognized by expanded Kappa4.",yytext);

		throw SyntaxError(msg, loc);
		return Parser::make_NEWLINE(loc);
	}
}
"[ID]"		return Parser::make_RUN_ID(loc);
"[run-id]"	return Parser::make_RUN_ID(loc);
"[RUNS]"	return Parser::make_RUNS(loc);
"[A]"		return Parser::make_ACTIVITY(loc);
"[E]"		return Parser::make_EVENT(loc);
"[E+]"		return Parser::make_PROD_EVENT(loc);
"[E-]"		return Parser::make_NULL_EVENT(loc);
"[T]"		return Parser::make_TIME(loc);
"[Tsim]"	return Parser::make_CPUTIME(loc);
"[log]"		return Parser::make_LOG(loc);
"[sin]"		return Parser::make_SINE(loc);
"[cos]"		return Parser::make_COSINE(loc);
"[tan]"		return Parser::make_TAN(loc);
"[exp]"		return Parser::make_EXPONENT(loc);
"[abs]"		return Parser::make_ABS(loc);
"[mod]"		return Parser::make_MODULO(loc);
"[sqrt]"	return Parser::make_SQRT(loc);
"[inf]"		return Parser::make_INF(loc);
"[true]"	return Parser::make_TRUE(loc);
"[false]"	return Parser::make_FALSE(loc);
"[pi]"		return Parser::make_FLOAT(3.14159,loc);
"[max]"		return Parser::make_MAX(loc);
"[min]"		return Parser::make_MIN(loc);
"[EMax]"	return Parser::make_EMAX(loc);
"[TMax]"	return Parser::make_TMAX(loc);
"[atan]"	return Parser::make_ATAN(loc);
"[coin]"	return Parser::make_COIN(loc);
"[randN]"	return Parser::make_RAND_N(loc);
"[rand1]"	return Parser::make_RAND_1(loc);
"[sum]"		return Parser::make_SUM(loc); 
"[average]"	return Parser::make_AVG(loc);
"[avg]"		return Parser::make_AVG(loc);

":"			return Parser::make_TYPE(loc);
";"			return Parser::make_SEMICOLON(loc);
"\""[^\"]*"\""	{
	yytext[strlen(yytext)-1] = 0;
	return Parser::make_STRING(&(yytext[1]),loc); }

[\n]+		loc.lines (yyleng); loc.step ();return Parser::make_NEWLINE(loc);
[\r]+		loc.lines (yyleng); loc.step ();return Parser::make_NEWLINE(loc);
"'"[^']*"'"	return Parser::make_LABEL(yytext,loc);
{id}		{
				return Parser::make_ID(yytext,loc);
			}
"["			return Parser::make_OP_BRA(loc);
"]"			return Parser::make_CL_BRA(loc);
"@*"		return Parser::make_FIX(loc);
"$"			return Parser::make_ATD(loc);
"<move-free>"	return Parser::make_FREE(loc);
"<move-join>"	return Parser::make_JOIN(loc);
"@"				return Parser::make_AT(loc);
","				return Parser::make_COMMA(loc);
"("				return Parser::make_OP_PAR(loc);
")"				return Parser::make_CL_PAR(loc);
"{"				return Parser::make_OP_CUR(loc);
"}"				return Parser::make_CL_CUR(loc);
"|"				return Parser::make_PIPE(loc);
"."				return Parser::make_DOT(loc);
"+"				return Parser::make_PLUS(loc);
"*"				return Parser::make_MULT(loc);
"-"				return Parser::make_MINUS(loc);
"^"				return Parser::make_POW(loc);
"/"				return Parser::make_DIV(loc);
"<"				return Parser::make_SMALLER(loc);
">"				return Parser::make_GREATER(loc);
"="				return Parser::make_EQUAL(loc);
"!"				return Parser::make_KAPPA_LNK(loc);
"~"[0-9a-zA-Z]+	return Parser::make_KAPPA_MRK(&yytext[1],loc);
"~["			return Parser::make_KAPPA_INTER(loc);
"~{"			return Parser::make_KAPPA_VALUE(loc);
"?"				return Parser::make_KAPPA_WLD(loc);
"_"				return Parser::make_KAPPA_SEMI(loc);
.	{ 
	char buffer[150];
	sprintf(buffer, "The token \"%s\" is not recognized", yytext);
	throw SyntaxError(buffer, loc);
}

<<EOF>> { return yyterminate(); }

%%
		    
