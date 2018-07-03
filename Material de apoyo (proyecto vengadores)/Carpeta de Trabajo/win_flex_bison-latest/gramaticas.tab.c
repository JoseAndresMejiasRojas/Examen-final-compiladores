/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 1 "gramaticas.cpp"

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
int cuenta = 0;
bool parametros = false;
bool operaciones = false;
bool comparaciones = false;
bool semanticERROR = false;

/*______________________________________________________________________________

INICIO DE LOS METODOS DE IMPRESION.

________________________________________________________________________________
*/
void printList(list<Nodo>* lst){
	for(list<Nodo>::iterator it = lst->begin(); it != lst->end(); it++){
		if(*(*it)->tokenName == "*NULL*"){
			cout<<(*it)->tokenValue;
		} else {
			cout<<*(*it)->tokenName;
		}
		it++;
		if(parametros && it != lst->end()){
			cout<<", ";
		}
		it--;
	}
}


void imprimir_tabs(int cantidad_tabs)
{
	for(int contador = 0; contador < cantidad_tabs; ++contador)
	{
		cout << '\t';
	}
}

/**
* EFE: Lista las etiquetas del arbol haciendo un recorrido en pre-orden.
* REQ: Arbol inicializado.
* MOD: N/A.
*/
void listPreORecur(Arbol& arbol, Nodo nodo, int cantidad_tabs)
{
	imprimir_tabs(cantidad_tabs);\
	if(cantidad_tabs == 0){
		cout << *nodo->tokenName << endl;
	} else {
		if(arbol.esHoja(nodo)){
			cout<<"Instruccion ";
		} else {
			if(cantidad_tabs == 1){
				cout << "Metodo ";
			} else {
				cout<<"Bloque ";
			}
		}
		if(*nodo->tokenName != "*asignacion*" && *nodo->tokenName != "*NULL*"){
			cout << *nodo->tokenName << " ";
		} else if(*nodo->tokenName == "*NULL*"){
			cout << nodo->tokenValue << " ";
		}
	}				// Imprimo instrucción.
	if(nodo->params != 0x0){
		parametros = true;
		if(nodo->params->size() != 0){
			cout<<" Parametros: ";
			printList(nodo->params);
		}
		parametros = false;
	}

	if(nodo->array != 0x0){
		if(nodo->array->size() != 0){
			cout<<" Tamaño Arreglo: ";
			printList(nodo->array);
		}
		parametros = false;
	}

	if(nodo->asignacion != 0x0){
		if(nodo->asignacion->size() != 0){
			cout<<" Asignacion: ";
			printList(nodo->asignacion);
		}
	}
	if(nodo->comparacion != 0x0){
		if(nodo->comparacion->size() != 0){
			cout<<" Comparacion: ";
			printList(nodo->comparacion);
		}
	}
	if(nodo->operaciones != 0x0){
		if(nodo->operaciones->size() != 0){
			cout<<" Operaciones: ";
			printList(nodo->operaciones);
		}
	}
	cout<<"\n";
	Nodo hijo = arbol.hijoMasIzq(nodo);
	while (hijo != nodoNulo) {
		listPreORecur(arbol, hijo, cantidad_tabs+1);
		hijo = arbol.hermanoDer(hijo);
	}
}

// El llamado que se hace en el main.
void listPreO(Arbol& arbol)
{
	cout<<"\n";
	if (!arbol.vacio())
	{
		listPreORecur(arbol, arbol.raiz(), 0);
	}
}

void printTree(){
	listPreO(arbol);
	//listPostO(arbol);
	arbol.printTable();
}
/*______________________________________________________________________________

FIN DE LOS METODOS DE IMPRESION
________________________________________________________________________________
*/

/*______________________________________________________________________________

INICIO DE LOS METODOS DE BUSQUEDA DE ANALISIS
________________________________________________________________________________
*/

bool caracterEspecial(string* tokenName){
		if(*tokenName == "+" || *tokenName == "-" || *tokenName == "/" || *tokenName == "*" || *tokenName == "%" ||
			 *tokenName == "==" || *tokenName == ">" || *tokenName == "<" || *tokenName == "<=" || *tokenName == ">=" ||
			 *tokenName == "||" || *tokenName == "&&" || *tokenName == "=" || *tokenName == "*NULL*" || (*tokenName)[0] == '\"'
		 || *tokenName == "verdadero" || *tokenName == "falso" || *tokenName == "nulo"){
			 return true;
		 }
		return false;
}

/*Obtiene el tipo de retorno de un método*/
Nodo retType(Nodo n){
	Nodo tmp = n;
	bool rst = true;
	Nodo found;
	while(tmp->padre != nodoNulo){
		if(*tmp->padre->tokenName == "Programa:"){
			return tmp;
		}
		tmp = tmp->padre;
	}
}

bool esParametro(Nodo n, list<Caja*>* lst){
		for(list<Caja*>::iterator it = lst->begin(); it != lst->end(); it++){
			if(*n->tokenName == *(*it)->tokenName){
				return true;
			}
		}
		return false;
}

bool nodoEnRango(Nodo nodo, Simbolo* symbol){
	/*=============================================================================================================*/
	//return true;
	bool rst = false;
	if(symbol == nodo->where){//Para metodos.
		rst = true;
	} else {
		if(symbol->scope->params != 0x0){
			if(*symbol->scope->tokenName == "para"){
				if(*(*symbol->scope->asignacion->begin())->tokenName == *nodo->tokenName){
						rst = true;
				}
			} else {
				rst = esParametro(nodo,symbol->scope->params);
			}
		}
		Nodo tmpNode = nodo->HI;
		Nodo tmpParent = nodo->padre;

		while(tmpParent != nodoNulo && !rst){
			while(tmpNode != nodoNulo && !rst){
				if(*tmpNode->tokenName == *nodo->tokenName){
					rst = true;
				}
				if(*tmpNode->tokenName == "*asignacion*" || *tmpNode->tokenName == "para"){
					if(*(*tmpNode->asignacion->begin())->tokenName == *symbol->tokenName){
						rst = true;
					} else if (tmpParent->params!=0x0){
						rst = esParametro(nodo, tmpParent->params);
					}
				} else if(tmpParent->params != 0x0 && !rst){
						rst = esParametro(nodo, tmpParent->params);
				}
				tmpNode = tmpNode->HI;
			}
			tmpNode = tmpParent;
			tmpParent = tmpParent->padre;
		}
	}

	if(rst && nodo->where == 0x0){
		nodo->where = symbol;
	}
	return rst;
}

/*Verifica que el token este en la tabla de simbolos, si lo esta, verifica que el encontrado este en el
rango correcto.*/
bool nodoEnTabla(Nodo nodo){
	bool encontrado = false;
	for(list<Simbolo*>::iterator it = arbol.getTable()->begin(); it != arbol.getTable()->end() && !encontrado; it++){
		if(*(*it)->tokenName == *nodo->tokenName){
			nodo->where = (*it);
			encontrado = nodoEnRango(nodo, (*it));
			if(operaciones && encontrado){
				if((*it)->tipo != entero && (*it)->tipo != unknown){
					cout<<"Error: "<<*(*it)->tokenName<<" no es tipo entero, por lo que no se puede usar en esta operacion.\n";
				}
			}
		}
	}

	if(!encontrado){
		cout<<"Token: "<< *nodo->tokenName<<" no ha sido declarado en este scope.\n";
		semanticERROR = true;
	}
	return encontrado;
}

//CREO QUE FUNCA PERO DEBO ARREGLAR LOS PARAMETROS.
void validezDeComparaciones(list<Caja*>* lst){
	list<Caja*>::iterator it1, it2, it3, pivot;
	int cnt1, cnt3;
	bool comparisonError = false;
	type tipo1;
	type tipo3;
	cnt1 = 0;
	cnt3 = 0;
	it1 = lst->begin();
	it2 = it1;//It2 will be the compration operator
	while(*(*it2)->tokenName != "==" && *(*it2)->tokenName != "<=" && *(*it2)->tokenName != ">=" &&
				*(*it2)->tokenName != "<" && *(*it2)->tokenName != ">=" && it2 !=lst->end()){
		it2++;
		cnt1++;
	}
	if(it2 != lst->end()){
		it3 = it2;
		it3++;
	}
		pivot = it3;
	do{
		while(*(*pivot)->tokenName != "||" && *(*pivot)->tokenName != "&&" && pivot != lst->end()){
			++pivot;
			cnt3++;
			if(pivot == lst->end()){//to get out of this without an arror.
				break;
			}
		}
		//==========================================================================================================
		//comparisons go here
		if(*(*it1)->tokenName == "*NULL*"){//if it1 is a number.
			if(*(*it3)->tokenName != "*NULL*"){
				if((*it3)->where != 0x0){//if it3 is a valid symbol.
					if((*it3)->where->tipo != unknown && (*it3)->where->tipo != entero){
						comparisonError = true;
					}
				}
			}
		} else {//if it1 has a symbol.
			if(*(*it3)->tokenName != "*NULL*"){//comparison between two variables
				if((*it3)->where != 0x0 && (*it3)->where != 0x0){//if both symbols are valid.
					if((*it3)->where->tipo != (*it1)->where->tipo){
						if((*it3)->where->tipo != unknown || (*it3)->where->tipo != unknown){//we can compare unknown with anything.
							comparisonError = true;
						}
					}
				}
			} else {//it1 contains a number
				if((*it1)->where != 0x0){//if it3 is a valid symbol.
					if((*it1)->where->tipo != unknown && (*it1)->where->tipo != entero){
						comparisonError = true;
					}
				}
			}
		}
		if(comparisonError){
			cout<<"No se pueden comparar ";
			if(*(*it1)->tokenName == "*NULL*"){
				cout<< (*it1)->tokenValue;
			} else {
				cout<< (*(*it1)->tokenName);
			}
			cout<<" y ";
			if(*(*it3)->tokenName == "*NULL*"){
				cout<< (*it3)->tokenValue;
			} else {
				cout<< *(*it3)->tokenName;
			}
			cout<<" ya que son distintos tipos.\n";
			semanticERROR = true;
		}
		comparisonError = false;
		//==========================================================================================================
		cnt3 = 0;
		cnt1 = 0;
		if(pivot != lst->end()){//There are || or &&.
			it1 = pivot;
			it1++;
			it2 = it1;
			while(*(*it2)->tokenName != "==" && *(*it2)->tokenName != "<=" && *(*it2)->tokenName != ">=" &&
						*(*it2)->tokenName != "<" && *(*it2)->tokenName != ">=" && it2 !=lst->end()){
				it2++;
				cnt1++;
			}
				if(it2 != lst->end()){
				it3 = it2;
				it3++;
			}
			pivot = it3;
		} else {
			break;
		}
	} while(pivot != lst->end());
}

//******************************************************************************************

//******************************************************************************************

void buscarEnTablaListasDeNodos(Nodo nodo){
	if(nodo->params != 0x0){
	if(nodo->params->size() != 0){
		for(list<Nodo>::iterator it = nodo->params->begin(); it != nodo->params->end(); it++){
				if(!caracterEspecial((*it)->tokenName)){
					nodoEnTabla(*it);
				}
			}
		}
	}

	if(nodo->comparacion != 0x0){
		if(nodo->comparacion->size() != 0){
			list<Nodo>::iterator it = nodo->comparacion->begin();
				while(it != nodo->comparacion->end()){
					if(!caracterEspecial((*it)->tokenName)){
						nodoEnTabla(*it);
					}
					it++;
				}
				validezDeComparaciones(nodo->comparacion);
			}
		}

		if(nodo->operaciones != 0x0){
			if(nodo->operaciones->size() != 0){
				list<Nodo>::iterator it = nodo->operaciones->begin();
					while(it != nodo->operaciones->end()){
						if(!caracterEspecial((*it)->tokenName)){
							operaciones = true;
							nodoEnTabla(*it);
							operaciones = false;
						}
						it++;
					}
				}
		}

		if(nodo->asignacion != 0x0){
			if(nodo->asignacion->size() != 0){
				list<Nodo>::iterator it = nodo->asignacion->begin();
					it++;
					it++;
					while(it != nodo->asignacion->end()){
						if(!caracterEspecial((*it)->tokenName)){
							nodoEnTabla(*it);
						}
						it++;
					}
				}
			}

			if(nodo->array != 0x0){
				if(nodo->array->size() != 0){
					list<Nodo>::iterator it = nodo->array->begin();
						it++;
						it++;
						while(it != nodo->array->end()){
							if(!caracterEspecial((*it)->tokenName)){
								operaciones = true;
								nodoEnTabla(*it);
								operaciones = false;
							}
							it++;
						}
					}
				}

}

void printType(type tipo){
		switch(tipo){
			case hilera: cout<<"hilera ";
										break;
			case entero: cout<<"entero ";
										break;
			case booleano: cout<<"booleano ";
										break;
		}
}

void semanticAnalisisPreORecur(Arbol& arbol, Nodo nodo, int nivel)
{
	if(nivel > 0){
		if(*nodo->tokenName != "*NULL*"){
			if(*nodo->tokenName != "si" && *nodo->tokenName != "sino" &&
					*nodo->tokenName != "entrada" && *nodo->tokenName != "salida" &&
					*nodo->tokenName != "para" && *nodo->tokenName != "mientras" &&
					*nodo->tokenName != "*asignacion*" && *nodo->tokenName != "ret")
					{
						nodoEnTabla(nodo);
					}

						if(nodo->params != 0x0){
							if(nodo->params->size() != 0){
								for(list<Nodo>::iterator it = nodo->params->begin(); it != nodo->params->end(); it++){
										if(!caracterEspecial((*it)->tokenName)){
											nodoEnTabla(*it);
											buscarEnTablaListasDeNodos(*it);
										}
									}
								}
							}

							if(nodo->comparacion != 0x0){
								if(nodo->comparacion->size() != 0){
									list<Nodo>::iterator it = nodo->comparacion->begin();
										while(it != nodo->comparacion->end()){
											if(!caracterEspecial((*it)->tokenName)){
												nodoEnTabla(*it);
												buscarEnTablaListasDeNodos(*it);
											}
											it++;
										}
										validezDeComparaciones(nodo->comparacion);
									}
								}

								if(nodo->operaciones != 0x0){
									if(nodo->operaciones->size() != 0){
										list<Nodo>::iterator it = nodo->operaciones->begin();
											while(it != nodo->operaciones->end()){
												if(!caracterEspecial((*it)->tokenName)){
													operaciones = true;
													nodoEnTabla(*it);
												  operaciones = false;
													buscarEnTablaListasDeNodos(*it);
												}
												it++;
											}
										}
								}

								if(nodo->asignacion != 0x0){
									if(nodo->asignacion->size() != 0){
										list<Nodo>::iterator it = nodo->asignacion->begin();
											it++;
											it++;
											while(it != nodo->asignacion->end()){
												if(!caracterEspecial((*it)->tokenName)){
													nodoEnTabla(*it);
													buscarEnTablaListasDeNodos(*it);
												}
												it++;
											}
										}
									}

									if(nodo->array != 0x0){
										if(nodo->array->size() != 0){
											list<Nodo>::iterator it = nodo->array->begin();
												it++;
												it++;
												while(it != nodo->array->end()){
													if(!caracterEspecial((*it)->tokenName)){
														operaciones = true;
														nodoEnTabla(*it);
														operaciones = false;
														buscarEnTablaListasDeNodos(*it);
													}
													it++;
												}
											}
										}

										//Se verifica que el valor de retorno sea real.
										if(*nodo->tokenName == "ret"){
												Nodo padre = retType(nodo);
												if(padre->where->tipo == unknown){//Actualiza el tipo del método.
													if(nodo->tipo != unknown){
														padre->tipo = nodo->tipo;
														padre->where->tipo = nodo->tipo;
													} else {
														padre->tipo = (*nodo->params->begin())->where->tipo;
														padre->where->tipo = (*nodo->params->begin())->where->tipo;
													}
												}
											if(nodo->tipo == unknown){//When a variable is being returned.
												if((*nodo->params->begin())->where != 0x0){//If the symbol exists in the table.
													if(padre->tipo != (*nodo->params->begin())->where->tipo && padre->tipo != unknown && (*nodo->params->begin())->where->tipo != unknown){
															cout<<"Retorno inadecuado para el método "<<*padre->tokenName<<"\n"<<
															"Retorno esperado: ";
															printType(padre->tipo);
															cout<<"\n";
															cout<<"Retorno obtenido: ";
															cout<<" en "<<*(*nodo->params->begin())->tokenName;
															cout<<"\n";
														}
													}
												} else {
													if(padre->tipo != nodo->tipo && nodo->tipo != unknown){
														cout<<"Retorno inadecuado para el método "<<*padre->tokenName<<"\n"<<
														"Retorno esperado: ";
														printType(padre->tipo);
														cout<<"\n";
														cout<<"Retorno obtenido: ";
														printType(nodo->tipo);
														cout<<" en "<<*nodo->tokenName;
														cout<<"\n";
													}
												}
										}
			}
		}
	Nodo hijo = arbol.hijoMasIzq(nodo);
	if( hijo != nodoNulo ){
		semanticAnalisisPreORecur(arbol, hijo,nivel+1);
	}

	Nodo hermano_derecho = arbol.hermanoDer(nodo);
	if( hermano_derecho != nodoNulo )				// Verifico si tiene hermano.
	{
		semanticAnalisisPreORecur(arbol, hermano_derecho, nivel);
	}
	// Si no tiene hermano, termino (sea nivel o método).
}

// El llamado que se hace en el main.
void semanticAnalisisPreO(Arbol& arbol)
{
	cout<<"\n";
	if (!arbol.vacio())
	{
		semanticAnalisisPreORecur(arbol, arbol.raiz(), 0);
	}
}
/*______________________________________________________________________________

FIN DE LOS METODOS DE BUSQUEDA DE EXISTENCIA EN LA TABLA Y ALCANCE
________________________________________________________________________________
*/


/* Line 371 of yacc.c  */
#line 649 "gramaticas.tab.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "gramaticas.tab.h".  */
#ifndef YY_YY_GRAMATICAS_TAB_H_INCLUDED
# define YY_YY_GRAMATICAS_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
/* Line 387 of yacc.c  */
#line 583 "gramaticas.cpp"

	#include <list>
	#include <string>
	using namespace std;


/* Line 387 of yacc.c  */
#line 688 "gramaticas.tab.c"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     HILERA = 258,
     FOR = 259,
     WHILE = 260,
     IF = 261,
     ELSE = 262,
     COMPARACION = 263,
     AND_OR = 264,
     BOOL = 265,
     NUM = 266,
     INI = 267,
     FIN = 268,
     RET = 269,
     MAS = 270,
     MENOS = 271,
     POR = 272,
     ENTRE = 273,
     MOD = 274,
     ENTRADA = 275,
     SALIDA = 276,
     ID = 277,
     ERROR = 278,
     EJECUCION = 279,
     PARD = 280,
     CORD = 281,
     PARI = 282,
     CORI = 283,
     IGUAL = 284,
     DOSP = 285,
     COM = 286,
     PUNTO = 287,
     POINTER = 288,
     PYC = 289,
     LINK = 290,
     JAAH = 291
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 588 "gramaticas.cpp"

	string* hilera;
	int intVal;
	struct Caja* nodo;
	struct Simbolos* simbolo;
	std::list<Caja*>* params;


/* Line 387 of yacc.c  */
#line 748 "gramaticas.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_GRAMATICAS_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 776 "gramaticas.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   206

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  37
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  68
/* YYNRULES -- Number of states.  */
#define YYNSTATES  148

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   291

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    14,    17,    21,    25,
      26,    30,    34,    38,    42,    46,    50,    53,    57,    61,
      63,    65,    67,    69,    71,    74,    76,    79,    84,    88,
      94,    97,   100,   103,   106,   112,   116,   119,   123,   127,
     131,   132,   138,   141,   145,   148,   152,   156,   158,   160,
     164,   166,   170,   174,   178,   182,   185,   188,   191,   198,
     204,   215,   222,   224,   226,   228,   230,   232,   234
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      38,     0,    -1,    39,    -1,    43,    40,    -1,    40,    -1,
      41,    49,    40,    -1,    41,    49,    -1,    42,    34,    41,
      -1,    22,    34,    41,    -1,    -1,    22,    29,    45,    -1,
      22,    29,    44,    -1,    47,    29,    45,    -1,    47,    29,
      44,    -1,    22,    29,    50,    -1,    35,    36,    43,    -1,
      35,    36,    -1,    44,    61,    46,    -1,    46,    61,    46,
      -1,    46,    -1,    10,    -1,     3,    -1,    22,    -1,    11,
      -1,    16,    11,    -1,    47,    -1,    33,    50,    -1,    22,
      28,    60,    26,    -1,    42,    34,    48,    -1,    21,    30,
      45,    34,    48,    -1,    58,    48,    -1,    57,    48,    -1,
      56,    48,    -1,    59,    48,    -1,    20,    30,    22,    34,
      48,    -1,    55,    34,    48,    -1,    34,    48,    -1,    50,
      34,    48,    -1,    22,    34,    48,    -1,    47,    34,    48,
      -1,    -1,    12,    50,    30,    48,    13,    -1,    22,    51,
      -1,    27,    52,    25,    -1,    27,    25,    -1,    45,    31,
      52,    -1,    44,    31,    52,    -1,    45,    -1,    44,    -1,
      54,     9,    53,    -1,    54,    -1,    45,     8,    45,    -1,
      45,     8,    44,    -1,    44,     8,    45,    -1,    44,     8,
      44,    -1,    14,    45,    -1,    14,    44,    -1,    14,    53,
      -1,     6,    30,    53,    27,    48,    25,    -1,    56,     7,
      27,    48,    25,    -1,     4,    30,    42,    31,    53,    31,
      44,    27,    48,    25,    -1,     5,    30,    53,    27,    48,
      25,    -1,    46,    -1,    44,    -1,    15,    -1,    16,    -1,
      17,    -1,    18,    -1,    19,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   659,   659,   691,   692,   696,   713,   730,   738,   747,
     751,   766,   780,   791,   801,   817,   818,   822,   834,   853,
     854,   859,   867,   872,   879,   887,   892,   900,   909,   915,
     922,   928,   934,   940,   946,   955,   965,   968,   974,   982,
     988,   992,  1037,  1045,  1046,  1050,  1058,  1066,  1072,  1080,
    1087,  1091,  1102,  1113,  1122,  1134,  1141,  1147,  1156,  1175,
    1195,  1218,  1236,  1240,  1247,  1248,  1249,  1250,  1251
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "\"Hilera\"", "\"para\"", "\"mientras\"",
  "\"si\"", "\"sino\"", "\"comparador\"", "\"&& o ||\"",
  "\"verdadero o falso\"", "\"numero\"", "\"ini\"", "\"fin\"", "\"ret\"",
  "\"+\"", "\"-\"", "\"*\"", "\"/\"", "\"%\"", "\"entrada\"", "\"salida\"",
  "\"identificador\"", "ERROR", "\"principal\"", "\")\"", "\"]\"", "\"(\"",
  "\"[\"", "\"=\"", "\":\"", "\",\"", "\".\"", "\"->\"", "\";\"",
  "\"enlazador de archivos\"", "\"ruta del archivo\"", "$accept", "super",
  "inicio", "principal", "declarations", "assign", "concat_links",
  "operaciones", "tipos", "operando", "arreglo", "instrucciones", "method",
  "method_call", "method_arguments", "concat_IDs_operations",
  "comparaciones", "comparacionesbooleanas", "ret", "rule_IF", "rule_ELSE",
  "rule_for", "rule_While", "tamano_arreglo", "OPERACIONES", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    37,    38,    39,    39,    40,    40,    41,    41,    41,
      42,    42,    42,    42,    42,    43,    43,    44,    44,    45,
      45,    45,    46,    46,    46,    46,    46,    47,    48,    48,
      48,    48,    48,    48,    48,    48,    48,    48,    48,    48,
      48,    49,    50,    51,    51,    52,    52,    52,    52,    53,
      53,    54,    54,    54,    54,    55,    55,    55,    56,    57,
      58,    59,    60,    60,    61,    61,    61,    61,    61
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     3,     2,     3,     3,     0,
       3,     3,     3,     3,     3,     3,     2,     3,     3,     1,
       1,     1,     1,     1,     2,     1,     2,     4,     3,     5,
       2,     2,     2,     2,     5,     3,     2,     3,     3,     3,
       0,     5,     2,     3,     2,     3,     3,     1,     1,     3,
       1,     3,     3,     3,     3,     2,     2,     2,     6,     5,
      10,     6,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       9,     0,     0,     0,     2,     4,     0,     0,     9,     0,
       0,     0,     9,    16,     1,     0,     6,     9,     3,     0,
      23,     0,    22,     0,    63,    62,    25,     0,    21,    20,
      22,    11,    10,    19,    14,     8,    15,     0,     0,     5,
       7,    13,    12,    24,    26,    64,    65,    66,    67,    68,
       0,     0,    27,     0,    42,    40,    17,    18,    44,    48,
      47,     0,     0,     0,     0,     0,     0,     0,     0,    40,
       0,     0,     0,     0,     0,    40,    40,    40,    40,     0,
       0,    43,     0,     0,     0,    56,    55,    57,    50,     0,
       0,    40,    36,    40,    40,    41,    40,    40,     0,    32,
      31,    30,    33,    46,    45,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    19,    38,    28,    39,
      37,    35,    40,     0,    40,    40,    54,    53,    52,    51,
      49,    40,    40,     0,     0,     0,     0,    34,    29,    59,
       0,    61,    58,     0,     0,    40,     0,    60
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    70,     8,   107,   108,    33,
      26,    72,    16,    73,    54,    61,    87,    88,    74,    75,
      76,    77,    78,    27,    50
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -78
static const yytype_int16 yypact[] =
{
      -2,    70,   -25,    40,   -78,   -78,    30,    16,    46,    25,
      19,   156,    46,    27,   -78,    59,     3,    46,   -78,   165,
     -78,    74,    61,    59,   125,   125,   -78,    69,   -78,   -78,
       1,   125,   -78,   125,   -78,   -78,   -78,    65,    71,   -78,
     -78,   125,   -78,   -78,   -78,   -78,   -78,   -78,   -78,   -78,
      19,    19,   -78,   136,   -78,    66,   -78,   -78,   -78,    48,
      72,    89,    85,    88,    91,   165,    93,    96,   172,    66,
      94,    24,   117,   116,   121,    17,    66,    66,    66,   165,
     165,   -78,   138,   165,   165,   178,   154,   -78,   155,   141,
     165,    66,   -78,    66,    66,   -78,    66,    66,   143,   -78,
     -78,   -78,   -78,   -78,   -78,    32,   134,   178,   154,   144,
     146,   165,   165,   165,   140,   145,   -78,   -78,   -78,   -78,
     -78,   -78,    66,   165,    66,    66,   125,   -78,   125,   -78,
     -78,    66,    66,   152,   149,   158,   163,   -78,   -78,   -78,
      19,   -78,   -78,    92,   125,    66,   177,   -78
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -78,   -78,   -78,    18,    15,     2,   190,    -6,    37,    -7,
       0,    60,   -78,    34,   -78,    33,   -77,   -78,   -78,   -78,
     -78,   -78,   -78,   -78,   -24
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -10
static const yytype_int16 yytable[] =
{
       9,    51,     7,    25,    24,    31,   109,   110,     9,    51,
       7,    13,     9,    41,     7,    -9,     9,     9,     7,     7,
       1,    62,    63,    64,    98,     1,    18,    35,    53,    10,
      20,    65,    40,     2,    39,    21,   130,    66,    67,    68,
      14,    22,    15,    56,    57,    34,   134,    59,    32,    38,
      17,    69,    23,    19,    19,    71,    42,    44,    94,    85,
      10,    11,     2,    45,    46,    47,    48,    49,     1,    71,
      62,    63,    64,    59,    59,    71,    71,    71,    71,    79,
      65,    37,     9,   116,   106,    43,    66,    67,    68,    10,
      60,    71,    53,    71,    71,    52,    71,    71,    10,    11,
      69,    55,    86,    80,    12,   126,   128,    45,    46,    47,
      48,    49,   103,   104,    81,    82,    60,    60,    83,   145,
      51,    84,    71,    89,    71,    71,    90,   115,    93,    92,
      95,    71,    71,   144,   143,    99,   100,   101,   102,    28,
      45,    46,    47,    48,    49,    71,    29,    20,   127,   129,
      96,   117,    21,   118,   119,    97,   120,   121,    22,    28,
     105,    58,   112,   114,   113,   123,    29,    20,    28,    23,
     122,   124,    21,   125,   131,    29,    20,   139,    30,   132,
     140,    21,   133,   141,   135,   136,   111,    22,   142,    23,
       0,   137,   138,    45,    46,    47,    48,    49,    23,    53,
      10,    11,   147,    36,     0,   146,    91
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-78)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       0,    25,     0,    10,    10,    11,    83,    84,     8,    33,
       8,    36,    12,    19,    12,    12,    16,    17,    16,    17,
      22,     4,     5,     6,     7,    22,     8,    12,    27,    28,
      11,    14,    17,    35,    16,    16,   113,    20,    21,    22,
       0,    22,    12,    50,    51,    11,   123,    53,    11,    15,
      34,    34,    33,    29,    29,    55,    19,    23,    34,    65,
      28,    29,    35,    15,    16,    17,    18,    19,    22,    69,
       4,     5,     6,    79,    80,    75,    76,    77,    78,    31,
      14,    22,    82,    90,    82,    11,    20,    21,    22,    28,
      53,    91,    27,    93,    94,    26,    96,    97,    28,    29,
      34,    30,    65,    31,    34,   111,   112,    15,    16,    17,
      18,    19,    79,    80,    25,    30,    79,    80,    30,    27,
     144,    30,   122,    30,   124,   125,    30,    90,    34,    69,
      13,   131,   132,   140,   140,    75,    76,    77,    78,     3,
      15,    16,    17,    18,    19,   145,    10,    11,   111,   112,
      34,    91,    16,    93,    94,    34,    96,    97,    22,     3,
      22,    25,     8,    22,     9,    31,    10,    11,     3,    33,
      27,    27,    16,    27,    34,    10,    11,    25,    22,    34,
      31,    16,   122,    25,   124,   125,     8,    22,    25,    33,
      -1,   131,   132,    15,    16,    17,    18,    19,    33,    27,
      28,    29,    25,    13,    -1,   145,    34
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    22,    35,    38,    39,    40,    41,    42,    43,    47,
      28,    29,    34,    36,     0,    12,    49,    34,    40,    29,
      11,    16,    22,    33,    44,    46,    47,    60,     3,    10,
      22,    44,    45,    46,    50,    41,    43,    22,    50,    40,
      41,    44,    45,    11,    50,    15,    16,    17,    18,    19,
      61,    61,    26,    27,    51,    30,    46,    46,    25,    44,
      45,    52,     4,     5,     6,    14,    20,    21,    22,    34,
      42,    47,    48,    50,    55,    56,    57,    58,    59,    31,
      31,    25,    30,    30,    30,    44,    45,    53,    54,    30,
      30,    34,    48,    34,    34,    13,    34,    34,     7,    48,
      48,    48,    48,    52,    52,    22,    42,    44,    45,    53,
      53,     8,     8,     9,    22,    45,    46,    48,    48,    48,
      48,    48,    27,    31,    27,    27,    44,    45,    44,    45,
      53,    34,    34,    48,    53,    48,    48,    48,    48,    25,
      31,    25,    25,    44,    46,    27,    48,    25
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
/* Line 1792 of yacc.c  */
#line 660 "gramaticas.cpp"
    {
	  string* root = new std::string("Programa:");
		arbol.ponerRaiz(cuenta,root);
		cuenta++;
		Nodo raiz = arbol.raiz();

		arbol.agregarHijo(arbol.raiz(),(yyvsp[(1) - (1)].nodo));
		for(Nodo tmp = (yyvsp[(1) - (1)].nodo); tmp != nodoNulo; tmp = tmp->HD)
		{
			tmp->padre = arbol.raiz();
			Nodo tmp2 = tmp->HD;
			if(tmp2 != nodoNulo) tmp2->HI = tmp;
			if(*tmp->tokenName == "*asignacion*")
			{
				arbol.agregarNodosDeListaATabla(tmp->asignacion, arbol.raiz());
			}
			if(tmp->addToTable)
			{
				arbol.agregarNodoATabla(tmp,arbol.raiz());
			}
		}
		arbol.fillTable();
		printTree();
		semanticAnalisisPreO(arbol);
		if(semanticERROR){
			exit(0);
		}
	}
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 691 "gramaticas.cpp"
    {(yyval.nodo) = (yyvsp[(2) - (2)].nodo);}
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 692 "gramaticas.cpp"
    {(yyval.nodo) = (yyvsp[(1) - (1)].nodo);}
    break;

  case 5:
/* Line 1792 of yacc.c  */
#line 697 "gramaticas.cpp"
    {
		(yyvsp[(2) - (3)].nodo)->HD = (yyvsp[(3) - (3)].nodo);
		(yyvsp[(3) - (3)].nodo)->HI = (yyvsp[(2) - (3)].nodo);
		if((yyvsp[(1) - (3)].nodo) != nodoNulo){
			Nodo i;
			for(i = (yyvsp[(1) - (3)].nodo); i->HD != nodoNulo; i = i->HD){
				//Iterates through the declaration Nodes.
				//'til it finds the last one different to null.
			}
			(yyval.nodo) = (yyvsp[(1) - (3)].nodo);
			i->HD = (yyvsp[(2) - (3)].nodo);
			(yyvsp[(2) - (3)].nodo)->HI = (yyval.nodo);
		} else {
			(yyval.nodo) = (yyvsp[(2) - (3)].nodo);
		}
	}
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 714 "gramaticas.cpp"
    {
		(yyval.nodo) = (yyvsp[(2) - (2)].nodo);
		if((yyvsp[(1) - (2)].nodo) != nodoNulo){
			Nodo i;
			for(i = (yyvsp[(1) - (2)].nodo); i->HD != nodoNulo; i = i->HD){
				//Iterates through the declaration Nodes.
				//'til it finds the last one different to null.
			}
			i->HD = (yyval.nodo);
			(yyval.nodo)->HI = (yyvsp[(1) - (2)].nodo);
			(yyval.nodo) = (yyvsp[(1) - (2)].nodo);
		}
	}
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 731 "gramaticas.cpp"
    {
		(yyval.nodo) = (yyvsp[(1) - (3)].nodo);
		if((yyvsp[(3) - (3)].nodo) != nodoNulo){
			(yyvsp[(1) - (3)].nodo)->HD = (yyvsp[(3) - (3)].nodo);
			(yyvsp[(3) - (3)].nodo)->HI = (yyval.nodo);
		}
	}
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 739 "gramaticas.cpp"
    {
		(yyval.nodo) = new Caja(cuenta++,(yyvsp[(1) - (3)].hilera),NULL,NULL);
		(yyval.nodo)->addToTable = true;
		if((yyvsp[(3) - (3)].nodo) != nodoNulo){
			(yyval.nodo)->HD = (yyvsp[(3) - (3)].nodo);
			(yyvsp[(3) - (3)].nodo)->HI = (yyval.nodo);
		}
	}
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 747 "gramaticas.cpp"
    {(yyval.nodo) = nodoNulo;}
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 752 "gramaticas.cpp"
    {
		(yyval.nodo) = new Caja(cuenta++, NULL, NULL, NULL);
		(yyval.nodo)->tokenName = new string("*asignacion*");
		Caja* temp = new Caja( cuenta++, (yyvsp[(2) - (3)].hilera), NULL, NULL );
		(yyval.nodo)->asignacion = new list<Caja*>();
		(yyval.nodo)->asignacion->push_front(temp);
		temp = new Caja( cuenta++, (yyvsp[(1) - (3)].hilera), NULL, NULL );
		temp->addToTable = true;
		temp->tipo = (yyvsp[(3) - (3)].nodo)->tipo;
		(yyval.nodo)->tipo = (yyvsp[(3) - (3)].nodo)->tipo;
		(yyval.nodo)->asignacion->push_front(temp);
		(yyval.nodo)->asignacion->push_back((yyvsp[(3) - (3)].nodo));
		// tipos se agrega desde su producción.
		}
    break;

  case 11:
/* Line 1792 of yacc.c  */
#line 767 "gramaticas.cpp"
    {
		(yyval.nodo) = new Caja(cuenta++, NULL, NULL, NULL);
		Caja* temp = new Caja( cuenta++, (yyvsp[(2) - (3)].hilera), NULL, NULL );
		(yyval.nodo)->tokenName = new string("*asignacion*");
		(yyval.nodo)->asignacion = (yyvsp[(3) - (3)].params);
		(yyval.nodo)->asignacion->push_front(temp);
		temp = new Caja( cuenta++, (yyvsp[(1) - (3)].hilera), NULL, NULL );
		temp->addToTable = true;
		temp->tipo = entero;
		list<Caja*>::iterator it = (yyvsp[(3) - (3)].params)->begin();
		(yyval.nodo)->asignacion->push_front(temp);
		(yyval.nodo)->tipo = entero;
	}
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 781 "gramaticas.cpp"
    {
		(yyval.nodo) = (yyvsp[(1) - (3)].nodo);
		(yyval.nodo)->addToTable = true;
		Caja* temp = new Caja( cuenta++, (yyvsp[(2) - (3)].hilera), NULL, NULL );
		(yyval.nodo)->tipo = (yyvsp[(3) - (3)].nodo)->tipo;
		(yyval.nodo)->asignacion = new list<Caja*>();
		(yyval.nodo)->asignacion->push_front(temp);
		(yyval.nodo)->asignacion->push_back((yyvsp[(3) - (3)].nodo));

	}
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 792 "gramaticas.cpp"
    {
		list<Caja*>::iterator it = (yyvsp[(3) - (3)].params)->begin();
		(yyval.nodo) = (yyvsp[(1) - (3)].nodo);
		(yyvsp[(1) - (3)].nodo)->addToTable = true;
		Caja* temp = new Caja( cuenta++, (yyvsp[(2) - (3)].hilera), NULL, NULL );
		(yyval.nodo)->asignacion->push_front(temp);
		(yyval.nodo)->asignacion = (yyvsp[(3) - (3)].params);
		(yyval.nodo)->tipo = entero;
	}
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 802 "gramaticas.cpp"
    {
		(yyval.nodo) = new Caja(cuenta++, NULL, NULL, NULL);
		Caja* temp = new Caja( cuenta++, (yyvsp[(2) - (3)].hilera), NULL, NULL );
		(yyval.nodo)->tokenName = new string("*asignacion*");
		(yyval.nodo)->asignacion = new list<Caja*>();
		(yyval.nodo)->asignacion->push_back((yyvsp[(3) - (3)].nodo));
		(yyval.nodo)->asignacion->push_front(temp);
		temp = new Caja( cuenta++, (yyvsp[(1) - (3)].hilera), NULL, NULL );
		temp->addToTable = true;
		temp->tipo = (yyvsp[(3) - (3)].nodo)->tipo;
		(yyval.nodo)->asignacion->push_front(temp);
	}
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 823 "gramaticas.cpp"
    {
		(yyval.params) = (yyvsp[(1) - (3)].params);

		Caja* temp = new Caja( cuenta++, (yyvsp[(2) - (3)].hilera), NULL, NULL );
		(yyval.params)->push_back(temp);

		(yyval.params)->push_back((yyvsp[(3) - (3)].nodo));
		if((yyvsp[(3) - (3)].nodo)->tipo == unknown){
			(yyvsp[(3) - (3)].nodo)->tipo = entero;
		}
	}
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 834 "gramaticas.cpp"
    {
		(yyval.params) = new list<Caja*>();

		(yyval.params)->push_back((yyvsp[(1) - (3)].nodo));

		Caja* temp = new Caja( cuenta++, (yyvsp[(2) - (3)].hilera), NULL, NULL );
		(yyval.params)->push_back(temp);

		(yyval.params)->push_back((yyvsp[(3) - (3)].nodo));
		if((yyvsp[(3) - (3)].nodo)->tipo == unknown){
			(yyvsp[(3) - (3)].nodo)->tipo = entero;
		}
		if((yyvsp[(1) - (3)].nodo)->tipo == unknown){
			(yyvsp[(1) - (3)].nodo)->tipo = entero;
		}
	}
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 853 "gramaticas.cpp"
    {(yyval.nodo) = (yyvsp[(1) - (1)].nodo);}
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 855 "gramaticas.cpp"
    {
		(yyval.nodo) = new Caja(cuenta++, (yyvsp[(1) - (1)].hilera),NULL,NULL);
		(yyval.nodo)->tipo = booleano;
	}
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 860 "gramaticas.cpp"
    {
		(yyval.nodo) = new Caja(cuenta++, (yyvsp[(1) - (1)].hilera),NULL,NULL);
		(yyval.nodo)->tipo = hilera;
	}
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 868 "gramaticas.cpp"
    {
		(yyval.nodo) = new Caja(cuenta++, (yyvsp[(1) - (1)].hilera),NULL,NULL);
		(yyval.nodo)->tipo = unknown;
	}
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 873 "gramaticas.cpp"
    {
		(yyval.nodo) = new Caja(cuenta++,NULL,NULL,NULL);
		(yyval.nodo)->tokenName = new string("*NULL*");
		(yyval.nodo)->tokenValue = (yyvsp[(1) - (1)].intVal);
		(yyval.nodo)->tipo = entero;
	}
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 880 "gramaticas.cpp"
    {
		int value = 0-(yyvsp[(2) - (2)].intVal);
		(yyval.nodo) = new Caja(cuenta++,NULL,NULL,NULL);
		(yyval.nodo)->tokenName = new string("*NULL*");
		(yyval.nodo)->tokenValue = value;
		(yyval.nodo)->tipo = entero;
	}
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 888 "gramaticas.cpp"
    {
		(yyval.nodo) = (yyvsp[(1) - (1)].nodo);
		(yyval.nodo)->tipo = unknown;
	}
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 893 "gramaticas.cpp"
    {
		(yyval.nodo) = (yyvsp[(2) - (2)].nodo);
		(yyval.nodo)->tipo = unknown;
	}
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 901 "gramaticas.cpp"
    {
		(yyval.nodo) = new Caja(cuenta++, (yyvsp[(1) - (4)].hilera), NULL, NULL);
		(yyval.nodo)->array = (yyvsp[(3) - (4)].params);
		arbol.agregarNodosDeListaATabla((yyvsp[(3) - (4)].params),(yyval.nodo));
	}
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 909 "gramaticas.cpp"
    {
		(yyval.nodo) = (yyvsp[(1) - (3)].nodo);
		(yyval.nodo)->HD = (yyvsp[(3) - (3)].nodo);
		if((yyvsp[(3) - (3)].nodo) != nodoNulo)
			(yyvsp[(3) - (3)].nodo)->HI = (yyval.nodo);
	}
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 915 "gramaticas.cpp"
    {
		(yyval.nodo) = new Caja(cuenta++, (yyvsp[(1) - (5)].hilera),(yyvsp[(5) - (5)].nodo),NULL);
		(yyval.nodo)->params = new list<Caja*>();
		(yyval.nodo)->params->push_back((yyvsp[(3) - (5)].nodo));
		if((yyvsp[(5) - (5)].nodo) != nodoNulo)
			(yyvsp[(5) - (5)].nodo)->HI = (yyval.nodo);
	}
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 922 "gramaticas.cpp"
    {
		(yyval.nodo) = (yyvsp[(1) - (2)].nodo);
		(yyval.nodo)->HD = (yyvsp[(2) - (2)].nodo);
		if((yyvsp[(2) - (2)].nodo) != nodoNulo)
			(yyvsp[(2) - (2)].nodo)->HI = (yyval.nodo);
	}
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 928 "gramaticas.cpp"
    {
		(yyval.nodo) = (yyvsp[(1) - (2)].nodo);
		(yyval.nodo)->HD->HD = (yyvsp[(2) - (2)].nodo);
		if((yyvsp[(2) - (2)].nodo) != nodoNulo)
			(yyvsp[(2) - (2)].nodo)->HI = (yyval.nodo);
	}
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 934 "gramaticas.cpp"
    {
		(yyval.nodo) = (yyvsp[(1) - (2)].nodo);
		(yyval.nodo)->HD = (yyvsp[(2) - (2)].nodo);
		if((yyvsp[(2) - (2)].nodo) != nodoNulo)
			(yyvsp[(2) - (2)].nodo)->HI = (yyval.nodo);
	}
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 940 "gramaticas.cpp"
    {
		(yyval.nodo) = (yyvsp[(1) - (2)].nodo);
		(yyval.nodo)->HD = (yyvsp[(2) - (2)].nodo);
		if((yyvsp[(2) - (2)].nodo) != nodoNulo)
			(yyvsp[(2) - (2)].nodo)->HI = (yyval.nodo);
	}
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 946 "gramaticas.cpp"
    {
		(yyval.nodo) = new Caja(cuenta++, (yyvsp[(1) - (5)].hilera),(yyvsp[(5) - (5)].nodo),NULL);
		(yyval.nodo)->params = new list<Caja*>();

		Caja* temp = new Caja( cuenta++, (yyvsp[(3) - (5)].hilera), NULL, NULL );
		(yyval.nodo)->params->push_back(temp);
		if((yyvsp[(5) - (5)].nodo) != nodoNulo)
			(yyvsp[(5) - (5)].nodo)->HI = (yyval.nodo);
	}
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 955 "gramaticas.cpp"
    {
		(yyval.nodo) = (yyvsp[(1) - (3)].nodo);
		(yyval.nodo)->HD = (yyvsp[(3) - (3)].nodo);
		if((yyvsp[(3) - (3)].nodo) != nodoNulo)
		{
			(yyvsp[(3) - (3)].nodo)->HI = (yyval.nodo);
		}

		pila.push((yyvsp[(1) - (3)].nodo));	// Guardo en pila el ret.
	}
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 965 "gramaticas.cpp"
    {
		(yyval.nodo) = (yyvsp[(2) - (2)].nodo);
	}
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 968 "gramaticas.cpp"
    {
		(yyval.nodo) = (yyvsp[(1) - (3)].nodo);
		(yyval.nodo)->HD = (yyvsp[(3) - (3)].nodo);
		if((yyvsp[(3) - (3)].nodo) != nodoNulo)
			(yyvsp[(3) - (3)].nodo)->HI = (yyval.nodo);
	}
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 975 "gramaticas.cpp"
    {
		(yyval.nodo) = new Caja( cuenta++, (yyvsp[(1) - (3)].hilera), (yyvsp[(3) - (3)].nodo), NULL );
		(yyval.nodo)->addToTable = true;
		(yyval.nodo)->HD = (yyvsp[(3) - (3)].nodo);
		if((yyvsp[(3) - (3)].nodo) != nodoNulo)
			(yyvsp[(3) - (3)].nodo)->HI = (yyval.nodo);
		}
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 982 "gramaticas.cpp"
    {
		(yyval.nodo)->addToTable = true;
		(yyval.nodo)->HD = (yyvsp[(3) - (3)].nodo);
		if((yyvsp[(3) - (3)].nodo) != nodoNulo)
			(yyvsp[(3) - (3)].nodo)->HI = (yyval.nodo);
		}
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 988 "gramaticas.cpp"
    {(yyval.nodo) = nodoNulo;}
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 994 "gramaticas.cpp"
    {
		(yyval.nodo) = (yyvsp[(2) - (5)].nodo);
		arbol.agregarNodosDeListaATabla((yyvsp[(2) - (5)].nodo)->params, (yyval.nodo));
		(yyval.nodo)->addToTable = true;
		if((yyvsp[(4) - (5)].nodo) != nodoNulo)
			{(yyval.nodo)->HMI = (yyvsp[(4) - (5)].nodo);}
		for(Nodo tmp = (yyvsp[(4) - (5)].nodo); tmp != nodoNulo; tmp = tmp->HD){
				tmp->padre = (yyval.nodo);
				if(*tmp->tokenName == "*asignacion*"){
					arbol.agregarNodosDeListaATabla(tmp->asignacion, (yyval.nodo));
				}
				if(tmp->addToTable){
					arbol.agregarNodoATabla(tmp,(yyval.nodo));
				}
			}

			if( pila.empty() == true )	// Si está vacío, el método no tiene returns.
			{
				(yyvsp[(2) - (5)].nodo)->tipo = t_void;						// Le agrego al método
			}
			else
			{
				Caja* tmp = pila.top();	// El que está en el tope.
				pila.pop();							// Me deshago de él.

				// Me detengo hasta encontrar un tipo conocido o vaciar la pila.
				while( tmp->tipo == unknown && pila.empty() == false )
				{
					tmp = pila.top();
					pila.pop();
				}
				(yyvsp[(2) - (5)].nodo)->tipo = tmp->tipo;					// Le agrego al método, si  todos fueron unknown, ese agrego.
			}

			// Vacío la pila para los otros.
			while( pila.empty() == false )
			{
				pila.pop();
			}
	}
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 1038 "gramaticas.cpp"
    {
		(yyval.nodo) = new Caja(cuenta++,(yyvsp[(1) - (2)].hilera),NULL,NULL);
		(yyval.nodo)->params = (yyvsp[(2) - (2)].params);
	}
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 1045 "gramaticas.cpp"
    {(yyval.params) = (yyvsp[(2) - (3)].params);}
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 1046 "gramaticas.cpp"
    {(yyval.params) = NULL;}
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 1051 "gramaticas.cpp"
    {
			(yyval.params) = new list<Caja*>();
			(yyvsp[(1) - (3)].nodo)->addToTable = true;
			(yyval.params)->push_front((yyvsp[(1) - (3)].nodo));
			arbol.mergeLists((yyval.params),(yyvsp[(3) - (3)].params));
			delete (yyvsp[(3) - (3)].params);
		}
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 1059 "gramaticas.cpp"
    {
			(yyval.params) = new list<Caja*>();
			arbol.mergeLists((yyvsp[(3) - (3)].params),(yyvsp[(1) - (3)].params));
			arbol.mergeLists((yyval.params),(yyvsp[(3) - (3)].params));
			delete (yyvsp[(1) - (3)].params);
			delete (yyvsp[(3) - (3)].params);
		}
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 1067 "gramaticas.cpp"
    {
			(yyval.params) = new list<Caja*>();
			(yyvsp[(1) - (1)].nodo)->addToTable = true;
			(yyval.params)->push_back((yyvsp[(1) - (1)].nodo));
		}
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 1073 "gramaticas.cpp"
    {
			(yyval.params) = new list<Caja*>();
			arbol.mergeLists((yyval.params),(yyvsp[(1) - (1)].params));
		}
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 1080 "gramaticas.cpp"
    {
		Caja* temp = new Caja( cuenta++, (yyvsp[(2) - (3)].hilera), NULL, NULL );
		(yyvsp[(1) - (3)].params)->push_back(temp);

		arbol.mergeLists((yyvsp[(1) - (3)].params),(yyvsp[(3) - (3)].params));
		(yyval.params) = (yyvsp[(1) - (3)].params);
	}
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 1087 "gramaticas.cpp"
    {(yyval.params) = (yyvsp[(1) - (1)].params);}
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 1092 "gramaticas.cpp"
    {
		(yyval.params) = new list<Caja*>();

		(yyval.params)->push_back((yyvsp[(1) - (3)].nodo));

		Caja* temp = new Caja( cuenta++, (yyvsp[(2) - (3)].hilera), NULL, NULL );
		(yyval.params)->push_back(temp);

		(yyval.params)->push_back((yyvsp[(3) - (3)].nodo));
	}
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 1103 "gramaticas.cpp"
    {
		(yyval.params) = new list<Caja*>();

		(yyval.params)->push_back((yyvsp[(1) - (3)].nodo));

		Caja* temp = new Caja( cuenta++, (yyvsp[(2) - (3)].hilera), NULL, NULL );
		(yyval.params)->push_back(temp);

		arbol.mergeLists((yyval.params),(yyvsp[(3) - (3)].params));	// Ojo que operaciones devuelve una lista.
	}
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 1114 "gramaticas.cpp"
    {
		(yyval.params) = (yyvsp[(1) - (3)].params);						// Obtengo la lista que devuele operaciones.

		Caja* temp = new Caja( cuenta++, (yyvsp[(2) - (3)].hilera), NULL, NULL );
		(yyval.params)->push_back(temp);

		(yyvsp[(1) - (3)].params)->push_back((yyvsp[(3) - (3)].nodo));
	}
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 1123 "gramaticas.cpp"
    {
		(yyval.params) = (yyvsp[(1) - (3)].params);

		Caja* temp = new Caja( cuenta++, (yyvsp[(2) - (3)].hilera), NULL, NULL );
		(yyval.params)->push_back(temp);

		arbol.mergeLists((yyval.params), (yyvsp[(3) - (3)].params));
	}
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 1134 "gramaticas.cpp"
    {
		(yyval.nodo) = new Caja(cuenta++,(yyvsp[(1) - (2)].hilera),NULL,NULL);
		(yyval.nodo)->params = new list<Caja*>();
		(yyval.nodo)->params->push_back((yyvsp[(2) - (2)].nodo));
		if(*(yyvsp[(2) - (2)].nodo)->tokenName != "*NULL*"){arbol.agregarNodoATabla((yyvsp[(2) - (2)].nodo),(yyval.nodo));};
		(yyval.nodo)->tipo = (yyvsp[(2) - (2)].nodo)->tipo;
	}
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 1141 "gramaticas.cpp"
    {
		(yyval.nodo) = new Caja(cuenta++,(yyvsp[(1) - (2)].hilera),NULL,NULL);
		(yyval.nodo)->operaciones = (yyvsp[(2) - (2)].params);
		arbol.agregarNodosDeListaATabla((yyvsp[(2) - (2)].params),(yyval.nodo));
		(yyval.nodo)->tipo = entero;
	}
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 1147 "gramaticas.cpp"
    {
		(yyval.nodo) = new Caja(cuenta++,(yyvsp[(1) - (2)].hilera),NULL,NULL);
		(yyval.nodo)->comparacion = (yyvsp[(2) - (2)].params);
		arbol.agregarNodosDeListaATabla((yyvsp[(2) - (2)].params),(yyval.nodo));
		(yyval.nodo)->tipo = booleano;
	}
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 1157 "gramaticas.cpp"
    {
		(yyval.nodo) = new Caja(cuenta++,(yyvsp[(1) - (6)].hilera),NULL,NULL);
		(yyval.nodo)->comparacion = (yyvsp[(3) - (6)].params);
		arbol.agregarNodosDeListaATabla((yyvsp[(3) - (6)].params),(yyval.nodo));
		(yyval.nodo)->HMI = (yyvsp[(5) - (6)].nodo);
		for(Nodo tmp = (yyvsp[(5) - (6)].nodo); tmp != nodoNulo; tmp = tmp->HD){
			tmp->padre = (yyval.nodo);
			if(*tmp->tokenName == "*asignacion*"){
				arbol.agregarNodosDeListaATabla(tmp->asignacion, (yyval.nodo));
			}
			if(tmp->addToTable){
				arbol.agregarNodoATabla(tmp,(yyval.nodo));
			}
		}
	}
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 1176 "gramaticas.cpp"
    {
		(yyval.nodo) = new Caja(cuenta++,(yyvsp[(2) - (5)].hilera),NULL,NULL);
		(yyval.nodo)->HMI = (yyvsp[(4) - (5)].nodo);
		for(Nodo tmp = (yyvsp[(4) - (5)].nodo); tmp != nodoNulo; tmp = tmp->HD){
			tmp->padre = (yyval.nodo);
			if(*tmp->tokenName == "*asignacion*"){
				arbol.agregarNodosDeListaATabla(tmp->asignacion, (yyval.nodo));
			}
			if(tmp->addToTable){
				arbol.agregarNodoATabla(tmp,(yyval.nodo));
			}
		}
		(yyvsp[(1) - (5)].nodo)->HD = (yyval.nodo);
		(yyval.nodo)->HI = (yyvsp[(1) - (5)].nodo);
		(yyval.nodo) = (yyvsp[(1) - (5)].nodo);
	}
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 1196 "gramaticas.cpp"
    {
			(yyval.nodo) = new Caja(cuenta++,(yyvsp[(1) - (10)].hilera),NULL,NULL);
			(yyval.nodo)->asignacion = (yyvsp[(3) - (10)].nodo)->asignacion;
			arbol.agregarNodosDeListaATabla((yyvsp[(3) - (10)].nodo)->asignacion,(yyval.nodo));
			arbol.agregarNodosDeListaATabla((yyvsp[(5) - (10)].params),(yyval.nodo));
			arbol.agregarNodosDeListaATabla((yyvsp[(7) - (10)].params),(yyval.nodo));
			(yyval.nodo)->comparacion = (yyvsp[(5) - (10)].params);
			(yyval.nodo)->operaciones = (yyvsp[(7) - (10)].params);
			(yyval.nodo)->HMI = (yyvsp[(9) - (10)].nodo);
			for(Nodo tmp = (yyvsp[(9) - (10)].nodo); tmp != nodoNulo; tmp = tmp->HD){
				tmp->padre = (yyval.nodo);
				if(*tmp->tokenName == "*asignacion*"){
					arbol.agregarNodosDeListaATabla(tmp->asignacion, (yyval.nodo));
				}
				if(tmp->addToTable){
					arbol.agregarNodoATabla(tmp,(yyval.nodo));
				}
			}
		}
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 1218 "gramaticas.cpp"
    {
		(yyval.nodo) = new Caja(cuenta++,(yyvsp[(1) - (6)].hilera),NULL,NULL);
		(yyval.nodo)->comparacion = (yyvsp[(3) - (6)].params);
		arbol.agregarNodosDeListaATabla((yyvsp[(3) - (6)].params),(yyval.nodo));
		(yyval.nodo)->HMI = (yyvsp[(5) - (6)].nodo);
		for(Nodo tmp = (yyvsp[(5) - (6)].nodo); tmp != nodoNulo; tmp = tmp->HD){
			tmp->padre = (yyval.nodo);
			if(*tmp->tokenName == "*asignacion*"){
				arbol.agregarNodosDeListaATabla(tmp->asignacion, (yyval.nodo));
			}
			if(tmp->addToTable){
				arbol.agregarNodoATabla(tmp,(yyval.nodo));
			}
		}
	}
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 1236 "gramaticas.cpp"
    {
		(yyval.params) = new list<Caja*>();
		(yyval.params)->push_back((yyvsp[(1) - (1)].nodo));
	}
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 1241 "gramaticas.cpp"
    {
		(yyval.params) = (yyvsp[(1) - (1)].params);
	}
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 1247 "gramaticas.cpp"
    {(yyval.hilera) = (yyvsp[(1) - (1)].hilera);}
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 1248 "gramaticas.cpp"
    {(yyval.hilera) = (yyvsp[(1) - (1)].hilera);}
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 1249 "gramaticas.cpp"
    {(yyval.hilera) = (yyvsp[(1) - (1)].hilera);}
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 1250 "gramaticas.cpp"
    {(yyval.hilera) = (yyvsp[(1) - (1)].hilera);}
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 1251 "gramaticas.cpp"
    {(yyval.hilera) = (yyvsp[(1) - (1)].hilera);}
    break;


/* Line 1792 of yacc.c  */
#line 2913 "gramaticas.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2055 of yacc.c  */
#line 1255 "gramaticas.cpp"

int main(int argc, char** argv) {
	if(argc > 1){
		yyin = fopen(argv[0],"r");
	} else {
		yyin = stdin;
	}
	yyparse();
	return 0;
}

void printError(string errormsg, char tipo){
	extern int yylineno;
	cout<< errormsg<<" en la linea: "<<yylineno<<"\n";
	if(tipo == 'a'){
		printf("El error es: %s\n",yytext);
		exit(-1);
	}
}

void yyerror(const char *s) {
	extern int yylineno;
	printf("\n%s   , en la linea %d\n",s,yylineno);
	exit(-1);
}
