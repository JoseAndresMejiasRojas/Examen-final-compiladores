%{
#include <iostream>
#include "gramaticas.tab.h"
using namespace std;
#define YY_DECL extern "C" int yylex()
#define guardar std::string(yytext, yylen)
extern void printError(std::string, char);

%}
%option yylineno
%option noyywrap

%%
(\s)+|(\t)+|(\n)+|(\R)+ {
		//Ignoro los espacios en blanco.
	}
\(	{
		yylval.hilera = new std::string(yytext,yyleng);
 		return PARENTESIS_DERECHO;
 	}
\)	{
		yylval.hilera = new std::string(yytext,yyleng);
 		return PARENTESIS_IZQUIERDO;
 	}
\-	{
		yylval.hilera = new std::string(yytext,yyleng);
		return SEPARADOR;
	}
print	{
		yylval.hilera = new std::string(yytext,yyleng);
 		return PRINT;
	}
\= {
		yylval.hilera = new std::string(yytext,yyleng);
 		return IGUAL;
 	}
\;	{
		yylval.hilera = new std::string(yytext,yyleng);
		return PUNTOYCOMA;
	}
[a-zñ][a-zA-Z0-9_ñ]*	{
		yylval.hilera = new std::string(yytext,yyleng);
 		return ID;
 	}
[0-9]+ {
		yylval.intVal = atoi(yytext);
 		return NUM;
	}
[A-Z_Ñ0-9][a-zA-Z0-9_ñ]*|[0-9]+(\.|\,)[0-9]+	{
		printError("Error lexico", 'a');
 		return ERROR;
 	}
%%