%{
	#include <iostream>
	#include "KappaLexer.h"
	#include "KappaParser.hpp"
	#include "ast/KappaAst.h"
	#include "location.hh"
	
	#define yyterminate() KappaParser::make_END(location());  
	
	using namespace yy;
	
	// Code run each time a pattern is matched.
	# define YY_USER_ACTION  loc.columns (yyleng);
	// The location of the current token.
	static yy::location loc;
%}


%option nodefault
%option noyywrap
%option c++
%option yyclass="KappaLexer"
%option prefix="kappa_"



blank		[\t ]
digit		[0-9]
letter		[a-zA-Z]
id			[a-zA-Z][a-zA-Z0-9\-\+_]*
real		([0-9]+\.[0-9]*|[0-9]*\.[0-9]+)(([eE][+-][0-9]+)|([eE][0-9]+))?
%%

%{
  // Code run each time yylex is called.
  loc.step ();
%}

[\t ]+		{loc.step();}
\-?{digit}+	{
				return KappaParser::make_INT(atoi(yytext),location()); 
			}
\-?{real}	{
				return KappaParser::make_FLOAT(atof(yytext),location());
			}
"%"{id}":"	{
				if(!strcmp(yytext,"%agent:"))
					return KappaParser::make_SIGNATURE(location());
				else if(!strcmp(yytext,"%init:"))
					return KappaParser::make_INIT(location());
				else if(!strcmp(yytext,"%var:"))
					return KappaParser::make_LET(location());
				else if(!strcmp(yytext,"%plot:"))
					return KappaParser::make_PLOT(location());
				else if(!strcmp(yytext,"%mod:"))
					return KappaParser::make_PERT(location());
				else if(!strcmp(yytext,"%obs:"))
					return KappaParser::make_OBS(location());
				else if(!strcmp(yytext,"%def:"))
					return KappaParser::make_CONFIG(location());
				else if(!strcmp(yytext,"%token:"))
					return KappaParser::make_TOKEN(location());
				else if(!strcmp(yytext,"%compartment:"))
					return KappaParser::make_COMPARTMENT(location());
				else if(!strcmp(yytext,"%link:"))
					return KappaParser::make_C_LINK(location());
				else if(!strcmp(yytext,"%transport:"))
					return KappaParser::make_TRANSPORT(location());
				else if(!strcmp(yytext,"%use:"))
					return KappaParser::make_USE(location());
				else{
					cout << "Error at\n" << location() << ": " << yytext << endl;
					cout << "No such instruction in expanded Kappa" << endl;
					return KappaParser::make_NEWLINE(location());
				}
			}
"do"		return KappaParser::make_DO(location());
"set"		return KappaParser::make_SET(location());
"repeat"	return KappaParser::make_REPEAT(location());
"until"		return KappaParser::make_UNTIL(location());
"&&"		return KappaParser::make_AND(location());
"||"		return KappaParser::make_OR(location());
"<->"		return KappaParser::make_KAPPA_LRAR(location());
"->"		return KappaParser::make_KAPPA_RAR(location());
"<-"		return KappaParser::make_LAR(location());
":="		return KappaParser::make_ASSIGN(location());
"<>"		return KappaParser::make_DIFF(location());
\${letter}+	{
				if(!strcmp(yytext,"$DEL"))
					return KappaParser::make_DELETE(location());
				else if(!strcmp(yytext,"$ADD"))
					return KappaParser::make_INTRO(location());
				else if(!strcmp(yytext,"$SNAPSHOT"))
					return KappaParser::make_SNAPSHOT(location());
				else if(!strcmp(yytext,"$STOP"))
					return KappaParser::make_STOP(location());
				else if(!strcmp(yytext,"$FLUX"))
					return KappaParser::make_FLUX(location());
				else if(!strcmp(yytext,"$TRACK"))
					return KappaParser::make_TRACK(location());
				else if(!strcmp(yytext,"$UPDATE"))
					return KappaParser::make_ASSIGN2(location());
				else if(!strcmp(yytext,"$PRINT"))
					return KappaParser::make_PRINT(location());
				else if(!strcmp(yytext,"$PRINTF"))
					return KappaParser::make_PRINTF(location());
				else{					
					cout << "Error at\n" << location() << ": " << yytext << endl;
					cout << "No such perturbation in expanded Kappa" << endl;
					return KappaParser::make_NEWLINE(location());
				}
			}
"[A]"		return KappaParser::make_ACTIVITY(location());
"[E]"		return KappaParser::make_EVENT(location());
"[E+]"		return KappaParser::make_PROD_EVENT(location());
"[E-]"		return KappaParser::make_NULL_EVENT(location());
"[T]"		return KappaParser::make_TIME(location());
"[Tsim]"	return KappaParser::make_CPUTIME(location());
"[log]"		return KappaParser::make_LOG(location());
"[sin]"		return KappaParser::make_SINUS(location());
"[cos]"		return KappaParser::make_COSINUS(location());
"[tan]"		return KappaParser::make_TAN(location());
"[exp]"		return KappaParser::make_EXPONENT(location());
"[abs]"		return KappaParser::make_ABS(location());
"[mod]"		return KappaParser::make_MODULO(location());
"[sqrt]"	return KappaParser::make_SQRT(location());
"[inf]"		return KappaParser::make_INFINITY(location());
"[true]"	return KappaParser::make_TRUE(location());
"[false]"	return KappaParser::make_FALSE(location());
"[pi]"		return KappaParser::make_FLOAT(3.14159,location());
"[max]"		return KappaParser::make_MAX(location());
"[min]"		return KappaParser::make_MIN(location());
"[EMax]"	return KappaParser::make_EMAX(location());
"[TMax]"	return KappaParser::make_TMAX(location());
"[atan]"	return KappaParser::make_ATAN(location());
"[coin]"	return KappaParser::make_COIN(location());
"[randN]"	return KappaParser::make_RAND_N(location());
"[rand1]"	return KappaParser::make_RAND_1(location());
":"			return KappaParser::make_TYPE(location());
";"			return KappaParser::make_SEMICOLON(location());
\".*\"		return KappaParser::make_STRING(yytext,location());
\n			return KappaParser::make_NEWLINE(location());
\r			return KappaParser::make_NEWLINE(location());
"#"[\n]^*\n	return KappaParser::make_NEWLINE(location());	
"'".*"'"	cout << "making label" << endl;return KappaParser::make_LABEL(yytext,location());
{id}		{
				return KappaParser::make_ID(yytext,location());
			}
"["			return KappaParser::make_OP_BRA(location());
"]"			return KappaParser::make_CL_BRA(location());
"@*"		return KappaParser::make_FIX(location());
"$"			return KappaParser::make_ATD(location());
"<move-free>"	return KappaParser::make_FREE(location());
"<move-join>"	return KappaParser::make_JOIN(location());
"@"				return KappaParser::make_AT(location());
","				return KappaParser::make_COMMA(location());
"("				return KappaParser::make_OP_PAR(location());
")"				return KappaParser::make_CL_PAR(location());
"{"				return KappaParser::make_OP_CUR(location());
"}"				return KappaParser::make_CL_CUR(location());
"|"				return KappaParser::make_PIPE(location());
"."				return KappaParser::make_DOT(location());
"+"				return KappaParser::make_PLUS(location());
"*"				return KappaParser::make_MULT(location());
"-"				return KappaParser::make_MINUS(location());
"^"				return KappaParser::make_POW(location());
"/"				return KappaParser::make_DIV(location());
"<"				return KappaParser::make_SMALLER(location());
">"				return KappaParser::make_GREATER(location());
"="				return KappaParser::make_EQUAL(location());
"!"				return KappaParser::make_KAPPA_LNK(location());
"~"[0-9a-zA-Z]+	return KappaParser::make_KAPPA_MRK(&yytext[1],location());
"?"				return KappaParser::make_KAPPA_WLD(location());
"_"				return KappaParser::make_KAPPA_SEMI(location());

<<EOF>> { return yyterminate(); }

%%

int ast::KappaAst::parse(){
	int r;
	if(files.empty()){
		yyin = stdin;
		r = parser.parse();
	}
	else{
		for(std::string f : files){
			if (!(yyin = fopen (f.c_str (), "r"))){
		      //error ("cannot open " + file + ": " + strerror(errno));
		      exit (EXIT_FAILURE);
		    }
		    r = parser.parse();
		    fclose(yyin);
		}
	}
	return r;
}

		    
		    