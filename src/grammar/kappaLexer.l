%{
#include <stdio.h>
//#include <iostream>
#include "kappaParser.hh"

#define YY_USER_ACTION yylloc.filename = filename; 							\
    yylloc.first_line = yylloc.last_line = yylineno; 						\
    yylloc.first_column = yycolumn; yylloc.last_column = yycolumn+yyleng-1; \
    yycolumn += yyleng;

%}
digit		[0-9]
letter		[a-zA-Z]
id			[a-zA-Z][a-zA-Z0-9\-\+_]*
real		([0-9]+|[0-9]+.[0-9]*|[0-9]*.[0-9]+)(([eE][+-][0-9]+)|([eE][0-9]+))?
%%
\-?{digit}+	{
				yylval.integer = atoi(yytext); 
				return INT;
			}
\-?{real}	{
				yylval.real = atof(yytext);
				return FLOAT;
			}
"%"{id}":"	{
				if(!strcmp(yytext,"%agent:"))
					return SIGNATURE;
				else if(!strcmp(yytext,"%init:"))
					return INIT;
				else if(!strcmp(yytext,"%var:"))
					return LET;
				else if(!strcmp(yytext,"%plot:"))
					return PLOT;
				else if(!strcmp(yytext,"%mod:"))
					return PERT;
				else if(!strcmp(yytext,"%obs:"))
					return OBS;
				else if(!strcmp(yytext,"%def:"))
					return CONFIG;
				else if(!strcmp(yytext,"%token:"))
					return TOKEN;
				else if(!strcmp(yytext,"%compartment:"))
					return COMPARTMENT;
				else if(!strcmp(yytext,"%link:"))
					return C_LINK;
				else if(!strcmp(yytext,"%transport:"))
					return TRANSPORT;
				else if(!strcmp(yytext,"%use:"))
					return USE;
				else{
					printf("error2");return 1;
				}
			}
"do"		return DO;
"set"		return SET;
"repeat"	return REPEAT;
"until"		return UNTIL;
"&&"		return AND;
"||"		return OR;
"<->"		return KAPPA_LRAR;
"->"		return KAPPA_RAR;
"<-"		return LAR;
":="		return ASSIGN;
"<>"		return DIFF;
\${letter}+	{
				if(!strcmp(yytext,"$DEL"))
					return DELETE;
				else if(!strcmp(yytext,"$ADD"))
					return INTRO;
				else if(!strcmp(yytext,"$SNAPSHOT"))
					return SNAPSHOT;
				else if(!strcmp(yytext,"$STOP"))
					return STOP;
				else if(!strcmp(yytext,"$FLUX"))
					return FLUX;
				else if(!strcmp(yytext,"$TRACK"))
					return TRACK;
				else if(!strcmp(yytext,"$UPDATE"))
					return ASSIGN2;
				else if(!strcmp(yytext,"$PRINT"))
					return PRINT;
				else if(!strcmp(yytext,"$PRINTF"))
					return PRINTF;
				else{
					printf("error");return 1;
				}
			}
"[A]"		return ACTIVITY;
"[E]"		return EVENT;
"[E+]"		return PROD_EVENT;
"[E-]"		return NULL_EVENT;
"[T]"		return TIME;
"[Tsim]"	return CPUTIME;
"[log]"		return LOG;
"[sin]"		return SINUS;
"[cos]"		return COSINUS;
"[tan]"		return TAN;
"[exp]"		return EXPONENT;
"[abs]"		return ABS;
"[mod]"		return MODULO;
"[sqrt]"	return SQRT;
"[inf]"		return INFINITY;
"[true]"	return TRUE;
"[false]"	return FALSE;
"[pi]"		return FLOAT;
"[max]"		return MAX;
"[min]"		return MIN;
"[EMax]"	return EMAX;
"[TMax]"	return TMAX;
"[atan]"	return ATAN;
"[coin]"	return COIN;
"[randN]"	return RAND_N;
"[rand1]"	return RAND_1;
":"			return TYPE;
";"			return SEMICOLON;
\".*\"		return STRING;
\n			return NEWLINE;
\r			return NEWLINE;
"#"[\n]^\n	return 1;	
"'".*"'"	return LABEL;
{id}		strcpy(yylval.str,yytext); return ID;
"["			return OP_BRA;
"]"			return CL_BRA;
"@*"		return FIX;
"$"			return ATD;
"<move-free>"	return FREE;
"<move-join>"	return JOIN;
"@"				return AT;
","				return COMMA;
"("				return OP_PAR;
")"				return CL_PAR;
"{"				return OP_CUR;
"}"				return CL_CUR;
"|"				return PIPE;
"."				return DOT;
"+"				return PLUS;
"*"				return MULT;
"-"				return MINUS;
"^"				return POW;
"/"				return DIV;
"<"				return SMALLER;
">"				return GREATER;
"="				return EQUAL;
"!"				return KAPPA_LNK;
"~"[0-9a-zA-Z]+	return KAPPA_MRK;
"?"				return KAPPA_WLD;
"_"				return KAPPA_SEMI;


%%