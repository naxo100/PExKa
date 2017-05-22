%{
	#include <iostream>
	#include "KappaLexer.h"
	#include "KappaParser.hpp"
	#include "KappaDriver.h"
	#include "location.hh"
	#include "../util/Exceptions.h"
	
	#define loc (*(driver.getLocation()))
	#define yyterminate() KappaParser::make_END(loc) 
	
	using namespace yy;
	
	/**
	 * \brief Message control to syntax errors
	*/		    
	void MakeMsg(char* msg, const char* str) {
		strcpy(msg, "\"");
		strcat(msg, str);
		strcat(msg, "\"");
		strcat(msg, " No such perturbation in expanded Kappa");
	}


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
%option yyclass="KappaLexer"
%option prefix="kappa_"
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

"#"+[.]*[^\n]*	loc.step();//return KappaParser::make_NEWLINE(loc);	
[\t ]+		{loc.step();}
{digit}+	{
				return KappaParser::make_INT(atoi(yytext),loc); 
			}
\-?{real}	{
				return KappaParser::make_FLOAT(atof(yytext),loc);
			}
"%"{id}":"	{
			if(!strcmp(yytext,"%agent:"))
				return KappaParser::make_SIGNATURE(loc);
			else if(!strcmp(yytext,"%init:"))
				return KappaParser::make_INIT(loc);
			else if(!strcmp(yytext,"%var:"))
				return KappaParser::make_LET(loc);
			else if(!strcmp(yytext,"%const:"))
				return KappaParser::make_CONST(loc);
			else if(!strcmp(yytext,"%plot:"))
				return KappaParser::make_PLOT(loc);
			else if(!strcmp(yytext,"%mod:"))
				return KappaParser::make_PERT(loc);
			else if(!strcmp(yytext,"%obs:"))
				return KappaParser::make_OBS(loc);
			else if(!strcmp(yytext,"%def:"))
				return KappaParser::make_CONFIG(loc);
			else if(!strcmp(yytext,"%token:"))
				return KappaParser::make_TOKEN(loc);
			else if(!strcmp(yytext,"%compartment:"))
				return KappaParser::make_COMPARTMENT(loc);
			else if(!strcmp(yytext,"%channel:"))
				return KappaParser::make_CHANNEL(loc);
			else if(!strcmp(yytext,"%transport:"))
				return KappaParser::make_TRANSPORT(loc);
			else if(!strcmp(yytext,"%use:"))
				return KappaParser::make_USE(loc);
			else{
				char msg[150];
				MakeMsg(msg, yytext);

				throw SyntaxError(msg, loc);
				return KappaParser::make_NEWLINE(loc);
			}
		}
"do"		return KappaParser::make_DO(loc);
"set"		return KappaParser::make_SET(loc);
"repeat"	return KappaParser::make_REPEAT(loc);
"until"		return KappaParser::make_UNTIL(loc);
"&&"		return KappaParser::make_AND(loc);
"||"		return KappaParser::make_OR(loc);
"<->"		return KappaParser::make_KAPPA_LRAR(loc);
"->"		return KappaParser::make_KAPPA_RAR(loc);
"<-"		return KappaParser::make_LAR(loc);
":="		return KappaParser::make_ASSIGN(loc);
"<>"		return KappaParser::make_DIFF(loc);
\${letter}+	{
			if(!strcmp(yytext,"$DEL"))
				return KappaParser::make_DELETE(loc);
			else if(!strcmp(yytext,"$ADD"))
				return KappaParser::make_INTRO(loc);
			else if(!strcmp(yytext,"$SNAPSHOT"))
				return KappaParser::make_SNAPSHOT(loc);
			else if(!strcmp(yytext,"$STOP"))
				return KappaParser::make_STOP(loc);
			else if(!strcmp(yytext,"$FLUX"))
				return KappaParser::make_FLUX(loc);
			else if(!strcmp(yytext,"$TRACK"))
				return KappaParser::make_TRACK(loc);
			else if(!strcmp(yytext,"$UPDATE"))
				return KappaParser::make_ASSIGN2(loc);
			else if(!strcmp(yytext,"$PRINT"))
				return KappaParser::make_PRINT(loc);
			else if(!strcmp(yytext,"$PRINTF"))
				return KappaParser::make_PRINTF(loc);
			else{					
				char msg[150];
				MakeMsg(msg, yytext);

				throw SyntaxError(msg, loc);
				return KappaParser::make_NEWLINE(loc);
			}
		}
"[A]"		return KappaParser::make_ACTIVITY(loc);
"[E]"		return KappaParser::make_EVENT(loc);
"[E+]"		return KappaParser::make_PROD_EVENT(loc);
"[E-]"		return KappaParser::make_NULL_EVENT(loc);
"[T]"		return KappaParser::make_TIME(loc);
"[Tsim]"	return KappaParser::make_CPUTIME(loc);
"[log]"		return KappaParser::make_LOG(loc);
"[sin]"		return KappaParser::make_SINUS(loc);
"[cos]"		return KappaParser::make_COSINUS(loc);
"[tan]"		return KappaParser::make_TAN(loc);
"[exp]"		return KappaParser::make_EXPONENT(loc);
"[abs]"		return KappaParser::make_ABS(loc);
"[mod]"		return KappaParser::make_MODULO(loc);
"[sqrt]"	return KappaParser::make_SQRT(loc);
"[inf]"		return KappaParser::make_INF(loc);
"[true]"	return KappaParser::make_TRUE(loc);
"[false]"	return KappaParser::make_FALSE(loc);
"[pi]"		return KappaParser::make_FLOAT(3.14159,loc);
"[max]"		return KappaParser::make_MAX(loc);
"[min]"		return KappaParser::make_MIN(loc);
"[EMax]"	return KappaParser::make_EMAX(loc);
"[TMax]"	return KappaParser::make_TMAX(loc);
"[atan]"	return KappaParser::make_ATAN(loc);
"[coin]"	return KappaParser::make_COIN(loc);
"[randN]"	return KappaParser::make_RAND_N(loc);
"[rand1]"	return KappaParser::make_RAND_1(loc);
":"			return KappaParser::make_TYPE(loc);
";"			return KappaParser::make_SEMICOLON(loc);
"\""[^\"]*"\""	{ 
	return KappaParser::make_STRING(yytext,loc); }

[\n]+		loc.lines (yyleng); loc.step ();return KappaParser::make_NEWLINE(loc);
[\r]+		loc.lines (yyleng); loc.step ();return KappaParser::make_NEWLINE(loc);
"'"[^']*"'"	return KappaParser::make_LABEL(yytext,loc);
{id}		{
				return KappaParser::make_ID(yytext,loc);
			}
"["			return KappaParser::make_OP_BRA(loc);
"]"			return KappaParser::make_CL_BRA(loc);
"@*"		return KappaParser::make_FIX(loc);
"$"			return KappaParser::make_ATD(loc);
"<move-free>"	return KappaParser::make_FREE(loc);
"<move-join>"	return KappaParser::make_JOIN(loc);
"@"				return KappaParser::make_AT(loc);
","				return KappaParser::make_COMMA(loc);
"("				return KappaParser::make_OP_PAR(loc);
")"				return KappaParser::make_CL_PAR(loc);
"{"				return KappaParser::make_OP_CUR(loc);
"}"				return KappaParser::make_CL_CUR(loc);
"|"				return KappaParser::make_PIPE(loc);
"."				return KappaParser::make_DOT(loc);
"+"				return KappaParser::make_PLUS(loc);
"*"				return KappaParser::make_MULT(loc);
"-"				return KappaParser::make_MINUS(loc);
"^"				return KappaParser::make_POW(loc);
"/"				return KappaParser::make_DIV(loc);
"<"				return KappaParser::make_SMALLER(loc);
">"				return KappaParser::make_GREATER(loc);
"="				return KappaParser::make_EQUAL(loc);
"!"				return KappaParser::make_KAPPA_LNK(loc);
"~"[0-9a-zA-Z]+	return KappaParser::make_KAPPA_MRK(&yytext[1],loc);
"~["			return KappaParser::make_KAPPA_INTER(loc);
"?"				return KappaParser::make_KAPPA_WLD(loc);
"_"				return KappaParser::make_KAPPA_SEMI(loc);
.	{ 
	char buffer[150];
	sprintf(buffer, "The token \"%s\" is not recognized", yytext);
	throw SyntaxError(buffer, loc);
}

<<EOF>> { return yyterminate(); }

%%
		    
