/* arbol.h 
 * Archivo de cabecera de estructura de árbol 
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

#ifndef __ARBOL_H__
#define __ARBOL_H__

/* Macros */
#define AND 1
#define OR 2
#define NOT 3
#define CONDICIONAL 4
#define BICONDICIONAL 5
#define XOR 6
#define PARENTESIS 7

/* Estructuras */
typedef struct Nodo {		/* Nodo de árbol */
	char letra;				/* Para representar preposiciones en las hojas */
	int operacion;			/* Macro que define operacion en elementos no-hoja */
	struct Nodo *izquierda;	/* Apuntador a nodo izquierdo */
	struct Nodo *derecha;	/* Apuntador a nodo derecho */
} Nodo;

/* Declaraciones */
Nodo *nueva_hoja (char letra);
Nodo *nuevo_nodo (Nodo *izq, Nodo *der, int operacion);
void imprimir_tabla (Nodo *raiz);
void liberar_arbol (Nodo *raiz);

#endif

