%{
#include <cstdio>
#include <iostream>
#include <string>
#include <stack>
#include <list>

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern "C" char* yytext;
void yyerror(const char *s);

std::list<std::string> lista_instrucciones;
std::list<std::string> lista_variables;
int cantidad_variables = 0;
bool metodo_declarado = false;

// retorna -1 si hay error.
bool revisar_variables_repetidas(std::list<std::string> lista)
{
	bool repetido = false;

	std::list<std::string>::iterator it = lista.begin();
	std::list<std::string>::iterator it2 = it;
	++it2;

	while( it != lista.end() && repetido == false )
	{
		while( it2 != lista.end() )
		{
			if(  it2->compare(*it) == 0 )
			{
				repetido = true;
			}
			++it2;
		}
		++it;
		it2 = it;
		++it2;
	}

	return repetido;
}

bool revisar_existencia_parametros(std::list<std::string> lista_metodo)
{
	bool error = false;
	bool existe = false;

	std::list<std::string>::iterator it_metodo=lista_metodo.begin();
	std::list<std::string>::iterator it_variables=lista_variables.begin();
/*
	for (std::list<std::string>::iterator it=lista_variables.begin(); it != lista_variables.end(); ++it)
	{
		std::cout << *it  << std::endl;
	}
*/

	while( it_variables != lista_variables.end() && error == false )
	{
		while(  it_metodo != lista_metodo.end() && existe == false )
		{
			if( it_variables->compare(*it_metodo) == 0 )
			{
				existe = true;
			}
			++it_metodo;
		}

		if( existe == false )
		{
			error = true;
		}

		// Reinicio.
		existe = false;
		it_metodo = lista_metodo.begin();

		++it_variables;
	}

	return error;
}

bool revisar_scope()
{
	bool error = false;
	bool antes = false;	// Para verificar si el print (si es que hay) está antes o después del método.

	std::list<std::string>::iterator it_instrucciones=lista_instrucciones.begin();

	while( it_instrucciones != lista_instrucciones.end() && error == false )
	{
		if( (*it_instrucciones)[0] == 'P' )	// Si detecto un print.
		{
			if( antes == false )	// Si es falso, hay un print antes que el método.
			{
				error = true;
			}
		}
		else	// Si es el método.
		{
			antes = true;
		}
		++it_instrucciones;
	}

	return error;
}

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
%type <parametros> metodo_retorno
%type <parametros> instrucciones

%%
//This is where the fun begins.

super:
	principal
	;

principal:
	instrucciones
	{
		// Necesito verificar que las variables que imprimo se hayan declarado en el método.
		if( revisar_existencia_parametros(*$1) == true && lista_variables.size() > 0 )
		{
			std::cout << "Esta imprimiendo algo que no existe." << std::endl;
			exit(-1);
		}
		else if( revisar_scope() == true )
		{
			std::cout << "Hay un print antes del método" << std::endl;
			exit(-1);
		}

	}
	;

instrucciones:
	PRINT ID PUNTOYCOMA instrucciones
	{
		$$ = $4;

		lista_variables.push_front(*$2);

		lista_instrucciones.push_front("P"+*$2);	// Con P ya que no existe un ID que empiece con mayúscula.

		// MIPS.
	}
	| metodo_retorno PUNTOYCOMA instrucciones
	{
		$$ = $1;

		if( metodo_declarado == true )
		{
			std::cout << "Error: Varios métodos declarados." << std::endl;
			exit(-1);
		}
		metodo_declarado = true;

		lista_instrucciones.push_front("metodo_retorno");
	}
	| {  }
	;

metodo_retorno:
	varios_parametros IGUAL ID PARENTESIS_IZQUIERDO NUM PARENTESIS_DERECHO
	{
		extern int yylineno;

		// Note que aquí ya tengo todos los parámetros agregados.
		if( revisar_variables_repetidas(*$1) == true )	// Si hay errores, me salgo.
		{
			std::cout << "Error en la linea " << yylineno << ", no pueden haber parametros repetidos." << std::endl;
			exit(-1);
		}

		if( $1->size() != $5 )
		{
			std::cout << "Error en la linea " << yylineno << ", El valor del parámetro tiene que ser igual a la cantidad de variables." << std::endl;
			exit(-1);
		}
	}
	;

varios_parametros:
	ID
	{
		$$ = new std::list<std::string>();	// Creo la lista de parámetros.
		$$->push_front(*$1);								// Agrego el parámetro.
	}
	| ID SEPARADOR varios_parametros
	{
		$$ = new std::list<std::string>();	// Creo la lista de parámetros.
		$$->push_front(*$1);								// Agrego el parámetro.

		$$->merge(*$3);											// Hago un merge para tener una sola lista.

		delete $3;													// Elimino las listas que se crearon que ya están vacías.
	}
	;

// This is where we end our suffering.
%%
int main(int argc, char** argv)
{
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

void printError(std::string errormsg, char tipo)
{
	extern int yylineno;
	std::cout<< errormsg<<" en la linea: "<<yylineno<< std::endl;
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
