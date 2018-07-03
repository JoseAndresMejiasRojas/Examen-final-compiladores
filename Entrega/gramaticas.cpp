%{
#include <iostream>
#include <string>
#include <stack>
#include <list>




extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern "C" char* yytext;
void yyerror(const char *s);

std::list<std::string> lista_variables;
int cantidad_variables = 0;

// retorna -1 si hay error.
int analisis_semantico(std::list<std::string> lista)
{
	
	return 0;
}

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
		// MIPS.
	}
	| metodo_retorno PUNTOYCOMA instrucciones
	{
		// MIPS.
	}
	| 
	;

metodo_retorno:
	varios_parametros IGUAL ID PARENTESIS_IZQUIERDO NUM PARENTESIS_DERECHO
	{
		extern int yylineno;
		// Verifico que varios_parametros se encuentren en lista_variables.
		
		// Note que aquí ya tengo todos los parámetros agregados.
		if( revisar_variables_repetidas(*$1) == true )	// Si hay errores, me salgo.
		{
			std::cout << "Error en la linea " << yylineno << ", no pueden haber parametros repetidos." << std::endl;
			exit(-1);
		}
		
	}
	;
	
varios_parametros:
	ID
	{
		$$ = new std::list<std::string>();	// Creo la lista de parámetros.
		$$->push_front(*$1);				// Agrego el parámetro.
	}
	| ID SEPARADOR varios_parametros
	{
		$$ = new std::list<std::string>();	// Creo la lista de parámetros.
		$$->push_front(*$1);			// Agrego el parámetro.
		
		$$->merge(*$3);					// Hago un merge para tener una sola lista.
		
		delete $3;						// Elimino las listas que se crearon que ya están vacías.
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

void printError(std::string errormsg, char tipo)
{
	extern int yylineno;
	std::cout<< errormsg<<" en la linea: "<<yylineno<<"\n";
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