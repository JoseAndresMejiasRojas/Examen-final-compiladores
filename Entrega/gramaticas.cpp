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

std::list<std::string> lista_variables;
int cantidad_variables = 0;

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
}
%union {
	std::string* hilera;
	int intVal;
	std::list<std::string>* parametros;
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

%type <parametros> varios_parametros

%%
//This is where the fun begins.

super:
	principal
	;
	
principal:
	instrucciones
	{
		// Aquí verifico que no tenga ninguna variable declarada repetida.
	}
	;
	
instrucciones:
	ID PUNTOYCOMA instrucciones
	{
		// Con *$1 obtengo el valor del token.
		lista_variables.push_front(*$1);	// Agrego variables.
		++cantidad_variables;				// Tengo un registro de la cantidad de variables.  Note que también cuenta las repetidas.
	}
	| PRINT ID PUNTOYCOMA instrucciones
	{
		// Hago el juego de MIPS.
	}
	| metodo_retorno PUNTOYCOMA instrucciones
	{
	}
	| 
	;

metodo_retorno:
	varios_parametros IGUAL ID PARENTESIS_IZQUIERDO NUM PARENTESIS_DERECHO
	{
		// Verifico que varios_parametros se encuentren en lista_variables.
	}
	;
	
varios_parametros:
	ID
	{
		$$ = new list<std::string>();	// Creo la lista de parámetros.
		$$->push_front(*$1);				// Agrego el parámetro.
	}
	| ID SEPARADOR varios_parametros
	{
		$$ = new list<std::string>();	// Creo la lista de parámetros.
		$$->push_front(*$1);			// Agrego el parámetro.
		
		$$->merge(*$3);					// Hago un merge para tener una sola lista.
	}
	;

// This is where we end our suffering.
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