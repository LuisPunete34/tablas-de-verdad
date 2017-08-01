/* vector.c
 * Implementación de vector
 * vim: set foldmethod=indent :
 *
 * Copyright (c) 2017 Ricardo Quezada
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * */

#include "vector.h"				/* Contenedor, Vector */
#include <stdio.h>				/* fprintf */
#include <stdlib.h>				/* malloc, free */

/* Declaraciones privadas */
Contenedor *crear_contenedor();

/* Vector nuevo_vector
 * Reserva espacio para un nuevo vector.
 * Los apuntadores se inicializan en NULL */

Vector *nuevo_vector()
{
	Vector *resultado;			/* Variable resultado */
	resultado = malloc(sizeof(Vector));
	if (!resultado) {
		fprintf(stderr, "Memoria insuficiente al crear vector");
		exit(1);
	}
	resultado->primero = NULL;
	resultado->ultimo = NULL;
	resultado->num_elementos = 0;
	return resultado;
}

/* insertar (vector, cadena) 
 * Inserta un nuevo elemento al final del vevtor especficado */

void insertar(Vector *vector, char *cadena)
{
	Contenedor *resultado;					/* Variable resultado */
	Contenedor *auxiliar;					/* Variable para movimientos */
	resultado = crear_contenedor();
	resultado->cadena = cadena;

	if (vector->primero == NULL) {
		vector->primero = resultado;
		vector->ultimo = resultado;
	} else {
		auxiliar = vector->ultimo;
		vector->ultimo = resultado;
		resultado->anterior = auxiliar;
		auxiliar->siguiente = resultado;
	}

	vector->num_elementos++;
}

/* liberar_vector (vector) 
 * Libera la memoria reservada para el vector especificada. */

void liberar_vector(Vector *vector)
{
	Contenedor *it, *aux;
	aux = it = vector->primero;
	while(aux != NULL) {
		aux = aux->siguiente;
		free(it->cadena);
		free(it);
		it = aux;
	}
	free(vector);
}

/* Contenedor crear_contenedor
 * Reserva espacio para un contenedos.
 * Los apuntadores se inicializan en NULL y la caden en "" */

Contenedor *crear_contenedor()
{
	Contenedor *resultado;				/* Variable resultado */
	resultado = malloc(sizeof(Contenedor));
	if (!resultado) {
		fprintf(stderr, "Memoria insuficiente al crear Contenedor");
		exit(1);
	}
	resultado->cadena = "";
	resultado->siguiente = NULL;
	resultado->anterior = NULL;
	return resultado;
}

