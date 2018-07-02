#include "ArbolC++.h"

#include <iostream>
#include <list>
#include <string>

int mov_registro(Caja* operando);
void mov_EAX(int operando);
void jmp_condicionado(Caja* jmp, std::string etiqueta);
void jmp_normal(std::string etiqueta);
void cmp_registros(int registro_1, int registro_2);
void escribir_etiqueta(std::string etiqueta);
std::string generar_etiqueta();

int main()
{
    return 0;
}

/*
 * Formato de la lista: operando comparacion operando (AND/OR opcionales si es cadena de comparaciones).
 * Por ejemplo: 5 == x
 * *iterador = '5'
 * *(++iterador) = '=='
 * *(++iterador) = 'x'
 */
void comparaciones_plantilla(list<Caja*>* lista_comparaciones)
{
    std::list<Caja*>::iterator it_operando1 = lista_comparaciones->begin();
    std::list<Caja*>::iterator it_comparacion = ++it_operando1;
    std::list<Caja*>::iterator it_operando2 = ++it_comparacion;
    std::list<Caja*>::iterator it_pivote = ++it_operando2;

    std::string etiqueta_true;
	std::string etiqueta_false;
	
	bool primero = true;
	bool or_variable;

    int registro_1;
    int registro_2;

    // Con la lista me muevo de 3 en 3 por cada comparación.
    do
    {
        // Para entender todo esto, ver los ejemplos del cuaderno.

        registro_1 = mov_registro( *it_operando1 );         // Primer operando.
        registro_2 = mov_registro( *it_operando2 );         // Segundo operando.

        cmp_registros(registro_1, registro_2);              // Escribir el compare de los registros.

        // Manejar las etiquetas.

        etiqueta = generar_etiqueta();                      // Etiqueta del true.
        jmp_condicionado(*it_comparacion, etiqueta);        // Escribo el jump hacia el true.
		
		if( primero == true )
		{
			mov_EAX(0);                                         // Escribo el Mov EAX, 0 (por si es false).
			
			escribir_etiqueta(etiqueta);                       	// Escribo la etiqueta del true.
			mov_EAX(1);                                         // true.
			primero = false;
		}
		else // no primero :)
		{
			if( or_variable == true )
			{

			}
			else
			{
				
			}
				
		}

        // Si hay AND/OR, continúo, si no, termino.
		if(it_pivote != lista_comparaciones->end())
		{
			// Reconozco por medio del bool, cuándo es OR o AND.
			if( (*it_pivote)->tokenName->compare("||") )
			{
				or_variable = true;
			}
			else
			{
				or_variable = false;
			}
			it_operando1++;
			it_operando1++;
			it_operando1++;
			it_operando1++;
			it_comparacion = it_operando1++;
			it_operando2 = it_comparacion++;
			it_pivote = it_operando2++;
		}
    }while( it_pivote != lista_comparaciones->end() )
}

/*
 * Escribe un mov registro, operando y retorna el número del registro
 * donde guardó el operando.
*/
int mov_registro(Caja* operando)
{
    // Aquí tengo que hacer el procedimiento para obtener un registro libre.
    int registro;

    // Aquí guardo en el archivo el código en MIPS del mov.

    return registro;
}

// Para devolver resultados.
void mov_EAX(int operando)
{
    // Aquí tengo que hacer el procedimiento para obtener un registro libre.
    int registro;

    // Aquí guardo en el archivo el código en MIPS del mov.

    return registro;
}

void cmp_registros(int registro_1, int registro_2)
{

}

std::string generar_etiqueta()
{
    return "etiqueta";
}

void jmp_condicionado(Caja* jmp, std::string etiqueta)
{

}
