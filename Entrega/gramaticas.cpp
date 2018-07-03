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
	struct Caja* nodo;
	struct Simbolos* simbolo;
	std::list<Caja*>* params;
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


%type <nodo> declarations
%type <nodo> assign
%type <nodo> rule_IF
%type <params> comparaciones;
%type <nodo> instrucciones;
%type <nodo> tipos
%type <params> operaciones
%type <nodo> rule_for
%type <nodo> rule_While
%type <nodo> rule_ELSE
%type <nodo> ret
%type <nodo> method_call
%type <nodo> method
%type <params> method_arguments
%type <params> concat_IDs_operations
%type <nodo> operando
%type <nodo> arreglo
%type <hilera> OPERACIONES
%type <params> comparacionesbooleanas
%type <params> tamano_arreglo
%type <nodo> inicio
%type <nodo> principal

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
	
// ToDo: hacer la concatenación de parámetros.  Verificar que todo funciona.
varios_parametros:

	;

//This is where we end our suffering.
%%
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
