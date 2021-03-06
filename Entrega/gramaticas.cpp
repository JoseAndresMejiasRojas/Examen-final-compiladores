/*
─────────────────────────────────────────────────────────────────────────────────────────────────
																			Inicio del análisis
─────────────────────────────────────────────────────────────────────────────────────────────────
*/
%{
#include <cstdio>
#include <iostream>
#include <string>
#include <stack>
#include <list>
#include <fstream>

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

	while( it_variables != lista_variables.end() && error == false )
	{
		while(  it_metodo != lista_metodo.end() && existe == false )
		{
			if( it_variables->compare(*it_metodo) == 0 )
			{
				existe = true;
			}
			else
			{
				++it_metodo;
			}
		}

		if( existe == false )
		{
			std::cout << "Error: variable " << *it_variables << std::endl;
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
				std::cout << "Error: print " <<(*it_instrucciones).substr(1) << std::endl;
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

// Devuelve la posición del elemento de la variable en la lista.
int obtener_index_variable( std::string variable, std::list<std::string>  lista )
{
	int posicion = 0;

	std::list<std::string>::iterator it = lista.begin();

	while( it != lista.end() )
	{
		if( it->compare(variable) == 0 )	// Si es igual.
		{
			return posicion;
		}
		++posicion;
		++it;
	}

	return posicion;
}

void generar_mips(std::list<std::string> lista_parametros)
{
	std::ofstream codigo_mips("codigo.s");	// Creo el archivo para MIPS.
	int index = -1;

	// Generar variables.
	codigo_mips << ".data" << std::endl;

	std::list<std::string>::iterator it_parametros = lista_parametros.begin();

	codigo_mips << "input:\t.asciiz \"Ingrese los " + std::to_string(lista_parametros.size()) + " valores que se van a guardar:\\n\"" << std::endl;
	codigo_mips << "output:\t.asciiz \"Valores retornados:\\n\"" << std::endl;
	codigo_mips << "cambio_linea:\t.asciiz \"\\n\"" << std::endl;

	codigo_mips << "arreglo_variables:\t.word 0";

	// Escribo el arreglo con las variables.  Por deafult es 0.  *Es obligatorio poner un valor.*
	for( int contador = 0; contador < lista_parametros.size() - 1; ++contador )
	{
		codigo_mips << ", 0";
	}
	codigo_mips << std::endl;

	codigo_mips << ".text" << std::endl;
	codigo_mips << "main:" << std::endl;

	// Imprimo la hilera del input.
	codigo_mips << "li $v0, 4" << std::endl;
	codigo_mips << "la $a0, input" << std::endl;
	codigo_mips << "syscall" << std::endl;

	// Recorro todas las instrucciones.
	for( std::list<std::string>::iterator it = lista_instrucciones.begin(); it != lista_instrucciones.end(); ++it )
	{
		if( (*it)[0] == 'P' )	// Imprimir.
		{
			index = obtener_index_variable( (*it).substr(1), lista_parametros );

			codigo_mips << "li $t1,"+std::to_string(index) << std::endl;
			codigo_mips << "mul $t0, $t1, 4" << std::endl;					// *4 para moverme de word en word.
			codigo_mips << "la $t2,arreglo_variables" << std::endl;	// Cargo el arreglo.
			codigo_mips << "add $t2, $t0, $t2" << std::endl;				// Me muevo en el arreglo para obtener el valor.
			codigo_mips << "lw $a0, 0($t2)" << std::endl;

			codigo_mips << "li $v0, 1" << std::endl;							 	// Para el system call.

			codigo_mips << "syscall" << std::endl;

			codigo_mips << "li $v0, 4" << std::endl;
			codigo_mips << "la $a0, cambio_linea" << std::endl;
			codigo_mips << "syscall" << std::endl;

		}
		else									// Metodo de retorno.
		{
			// Ciclo para pedir números al usuario.
			codigo_mips << "li $t0, 0" << std::endl;									// $t0 = contador.
			codigo_mips << "la $t1, arreglo_variables" << std::endl;	// Cargo el arreglo para guardar los valores.

			codigo_mips << "ciclo:" << std::endl;

			codigo_mips << "li $v0, 5" << std::endl;
			codigo_mips << "syscall" << std::endl;										// Leo.
			codigo_mips << "sw $v0, 0($t1)" << std::endl;							// Guardo en el arreglo el valor del usuario.

			codigo_mips << "add $t0, 1" << std::endl;									// Incremeneto contador.
			codigo_mips << "add $t1, 4" << std::endl;									// Me muevo en el arreglo.

			codigo_mips << "bge $t0," + std::to_string(lista_parametros.size()) + ",salir_ciclo" << std::endl;
			codigo_mips << "j ciclo" << std::endl;

			codigo_mips << "salir_ciclo:" << std::endl;

			codigo_mips << "li $v0, 4" << std::endl;
			codigo_mips << "la $a0, output" << std::endl;
			codigo_mips << "syscall" << std::endl;
		}
	}

	// Termino ejecución.
	codigo_mips << "li $v0, 10" << std::endl;
	codigo_mips << "syscall" << std::endl;

}
%}

/*
─────────────────────────────────────────────────────────────────────────────────────────────────
																			Fin del análisis
─────────────────────────────────────────────────────────────────────────────────────────────────
*/


/*
─────────────────────────────────────────────────────────────────────────────────────────────────
																			Inicio de las gramáticas
─────────────────────────────────────────────────────────────────────────────────────────────────
*/
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

		if( $1 == NULL )
		{
			std::cout << "Error: el método no existe o es incorrecto." << std::endl;
			remove("codigo.s");
			exit(-1);
		}
		else if( revisar_existencia_parametros(*$1) == true && lista_variables.size() > 0 )
		{
			std::cout << "Imprimiendo una variable que no existe." << std::endl;
			remove("codigo.s");
			exit(-1);
		}
		else if( revisar_scope() == true )
		{
			std::cout << "Imprimiendo antes del método" << std::endl;
			remove("codigo.s");
			exit(-1);
		}

		// Genero MIPS acorde a la lista que tengo.
		generar_mips(*$1);

		std::cout << "Todo correcto." << std::endl;
	}
	;

instrucciones:
	PRINT ID PUNTOYCOMA instrucciones
	{
		$$ = $4;

		lista_variables.push_front(*$2);

		lista_instrucciones.push_front("P"+*$2);	// Con P ya que no existe un ID que empiece con mayúscula.
	}
	| metodo_retorno PUNTOYCOMA instrucciones
	{
		$$ = $1;

		if( metodo_declarado == true )
		{
			std::cout << "Error: varios métodos declarados." << std::endl;
			exit(-1);
		}
		metodo_declarado = true;

		lista_instrucciones.push_front("metodo_retorno");
	}
	| { $$ = NULL; }
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
/*
─────────────────────────────────────────────────────────────────────────────────────────────────
																			Fin de las gramáticas
─────────────────────────────────────────────────────────────────────────────────────────────────
*/


/*
─────────────────────────────────────────────────────────────────────────────────────────────────
																			Código en C++
─────────────────────────────────────────────────────────────────────────────────────────────────
*/
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
	std::cout<< errormsg<<" en la linea: "<< yylineno << std::endl;
	if(tipo == 'a')
	{
		remove("codigo.s");
		std::cout << "El error es: " << yytext << std::endl;
		exit(-1);
	}
}

void yyerror(const char *s)
{
	extern int yylineno;
	remove("codigo.s");
	std::cout << s << " en la línea " << yylineno << std::endl;
	exit(-1);
}
