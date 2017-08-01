/* vector.h
 * Archivo de cabecera de vector
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

#ifndef __VECTOR_H__
#define __VECTOR_H__

/* Estructuras 
 * El vector funciona como una lista doblemente
 * ligada de la cual se guardan apuntadores para
 * el primero y ultimo elementos. */

typedef struct Contenedor {
	char *cadena;
	struct Contenedor *siguiente;
	struct Contenedor *anterior;
} Contenedor;

typedef struct Vector {
	Contenedor *primero;
	Contenedor *ultimo;
	int num_elementos;
} Vector;

/* Declaraciones */
Vector *nuevo_vector();
void insertar(Vector *vector, char *cadena);
void liberar_vector (Vector *vector);

#endif

