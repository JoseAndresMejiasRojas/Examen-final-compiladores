%{
#include <stdio.h>
#include <string>
#include <stack>

#include "ArbolC++.h"
using namespace std;
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern "C" char* yytext;
void yyerror(const char *s);
Arbol arbol;
std::stack<Caja*> pila;

/*______________________________________________________________________________

INICIO DE LOS METODOS DE IMPRESION.

________________________________________________________________________________
*/



/*______________________________________________________________________________

FIN DE LOS METODOS DE IMPRESION
________________________________________________________________________________
*/

/*______________________________________________________________________________

INICIO DE LOS METODOS DE BUSQUEDA DE ANALISIS
________________________________________________________________________________
*/



/*______________________________________________________________________________

FIN DE LOS METODOS DE BUSQUEDA DE EXISTENCIA EN LA TABLA Y ALCANCE
________________________________________________________________________________
*/

%}
%error-verbose

%code requires{
	#include <list>
	#include <string>
	using namespace std;
}
%union {
	string* hilera;
	int intVal;
}

%token <intVal> NUM "numero"
%token <hilera> SEPARADOR "-"
%token <hilera> ID "identificador"
%token <hilera> ERROR
%token <hilera> PARENTESIS_DERECHO ")"
%token <hilera> PARENTESIS_IZQUIERDO "("
%token <hilera> IGUAL "="
%token <hilera> PRINT "imprimir"
%token <hilera> PUNTOYCOMA ";"

%%
//This is where the fun begins.

super:
	principal
	;
	
principal:
	instrucciones
	;
	
instrucciones:
	ID PUNTOYCOMA instrucciones
	| PRINT ID PUNTOYCOMA instrucciones
	| metodo_retorno PUNTOYCOMA instrucciones
	| 
	;

metodo_retorno:
	varios_parametros IGUAL ID PARENTESIS_IZQUIERDO NUM PARENTESIS_DERECHO PUNTOYCOMA
	;
	
varios_parametros:
	ID
	| ID SEPARADOR varios_parametros
	;

//This is where we end our suffering.
%%
int main(int argc, char** argv) {
	if(argc > 1)
	{
		yyin = fopen(argv[0],"r");
	} else 
	{
		yyin = stdin;
	}
	yyparse();
	return 0;
}

void printError(string errormsg, char tipo)
{
	extern int yylineno;
	cout<< errormsg<<" en la linea: "<<yylineno<<"\n";
	if(tipo == 'a')
	{
		printf("El error es: %s\n",yytext);
		return;
	}
}

void yyerror(const char *s) 
{
	extern int yylineno;
	printf("\n%s   , en la linea %d\n",s,yylineno);
	return;
}