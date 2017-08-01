/* arbol.c 
 * Implementación de árbol de análisis sintáctico explícito 
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

#include "arbol.h"				/* Nodo */
#include "vector.h"				/* Vector, Contenedor */
#include <stdio.h>				/* fprintf */
#include <stdlib.h>				/* exit, malloc, free */
#include <math.h>				/* pow */
#include <string.h>				/* strcpy, strcat, strcmp */

/* Declaraciones privadas */
Nodo *reservar();
int obtener_variables (Nodo *raiz, Vector *expresiones, int *contador);
char *obtener_expresiones (Nodo *raiz, Vector *expresiones, int *num_espacios);
void asignar_variables(int *evaluacion, int num_variables);
int evaluar (Nodo *raiz, int *evaluacion, int *indice);
char *agregar_parentesis (char *original);

/* Variables globales */
char registro[26];				/* Controla las variables (y su orden) que 
								   aparecen en la expresión */
char evaluacion_var[26];		/* Varía para cada fila. Contiene los valores 
								   de la evaluación */

/* nueva_hoja (letra) 
 * Crea y regresa un nuevo nodo con la letra dada.
 * Al tratarse de una hoja, no se agrega nada como hijos */

Nodo *nueva_hoja (char letra)
{
	Nodo *resultado;			/* Nodo resultado */
	resultado = reservar();
	resultado->letra = letra;
	return resultado;
}

/* nuevo_nodo (izquierda, derecha, operacion)
 * Crea un nodo intermedio con los hijos y la operacion especificadas.
 * Corresponde a un modelo de cración de abajo hacia arriba (ascendente) */

Nodo *nuevo_nodo (Nodo *izq, Nodo *der, int operacion)
{
		Nodo *resultado;		/* Nodo resultado */
		resultado = reservar();
		resultado->izquierda = izq;
		resultado->derecha = der;
		resultado->operacion = operacion;
		return resultado;
}

/* imprimir_tabla (raiz) 
 * imprime la tabla de verdad de la expresión que haya bajo el nodo raiz. */

void imprimir_tabla (Nodo *raiz)
{
	int num_variables;			/* Contador de variables en expresión */
	int num_filas;				/* 2 ^ num_variables */
	Vector *expresiones;		/* Vector con cadenas de expresiones */         
	Contenedor *iterador;		/* Iterador para recorrer vector */
	char *expresion;			/* Expresión final */
	int *evaluacion;			/* Arreglo con evaluaciones */
	int lon_cadena = 1;			/* Número de caracteres de encabezado */
	int num_espacios;			/* Para separación en tabla */
	int variables_temp;			/* Para guardar índice en llamadas recursivas */
	int aux_contador = 1;
	
	/* Limpiar variables globales */
	for (int i=0; i<26; i++)
		registro[i] = evaluacion_var[i] = 0;
	
	/* Obtener encabezado y número de variables */
	expresiones = nuevo_vector();
	num_variables = obtener_variables(raiz, expresiones, &aux_contador);
	expresion = obtener_expresiones(raiz, expresiones, &lon_cadena);
	num_filas = pow(2, num_variables);
	
	/* Construir separador */
	char separador[lon_cadena + 1];
	for (int i=0; i<lon_cadena; i++) 
		separador[i] = '=';
	separador[lon_cadena] = 0;
	
	/* Imprimir encabezado */
	printf("%s\n| ", separador);
	iterador = expresiones->primero;
	while(iterador != NULL) {
		printf("%s | ", iterador->cadena);
		iterador = iterador->siguiente;
	}
	printf("\n%s\n", separador);
	
	/* Construir máscaras */
	int mascara[num_variables];
	for(int i=0; i<num_variables; i++)
		mascara[i] = pow(2, i);
	
	/* Realizar evaluaciones */
	evaluacion = malloc(expresiones->num_elementos * sizeof(int));
	for (int i=0; i<num_filas; i++) {
		for (int j=0; j<num_variables; j++) 
			evaluacion[j] = (0 != (i & mascara[num_variables - j - 1]));
		asignar_variables(evaluacion, num_variables);
		variables_temp = num_variables;
		evaluar(raiz, evaluacion, &variables_temp);
		iterador = expresiones->primero;
		printf("| ");
		for (int j=0; j<expresiones->num_elementos; j++) {
			num_espacios = strlen(iterador->cadena) - 1;
			for (int k=0; k<num_espacios; k++) printf(" ");
			printf("%c | ", (evaluacion[j]) ? 'V' : 'F' );
			iterador = iterador->siguiente;
		}
		printf("\n");
	}
	printf("%s\n", separador);
	
	/* Liberar memoria */
	free(expresion);
	free(evaluacion);
	liberar_vector(expresiones);
}

/* liberar_arbol
 * Libera toda la memoria dinámica de la raiz dada hacia abajo*/

void liberar_arbol (Nodo *raiz)
{
	if (raiz->izquierda != NULL)
		liberar_arbol(raiz->izquierda);
	if (raiz->derecha != NULL)
		liberar_arbol(raiz->derecha);
	free(raiz);
}

/* reservar
 * Reserva memoria de manera dinámica necesaria para un nodo
 * (definido en cabecera). Inicializa valores en 0 y en NULL */

Nodo *reservar ()
{
	Nodo *resultado;                /* Apuntador a resultado */
	resultado = malloc(sizeof(Nodo));
	if(!resultado) {
		fprintf(stderr, "Error al reservar memoria.\n");
		exit(1);
	}
	
	resultado->letra = 0;
	resultado->operacion = 0;
	resultado->izquierda = NULL;
	resultado->derecha = NULL;
	
	return resultado;
}

/* num_variables obtener_variable (raiz)
 * Regresa el número de hojas del árbol 
 * Inserta en el vector de las expresiones las letras
 * de las proposiciones */

int obtener_variables (Nodo *raiz, Vector *expresiones, int *contador)
{
	int resultado = 0;
	if (raiz->izquierda != NULL)
		resultado += obtener_variables(raiz->izquierda, expresiones, contador);
	if (raiz->derecha != NULL)
		resultado += obtener_variables(raiz->derecha, expresiones, contador);
	
	if(raiz->letra != 0){
		if(!registro[raiz->letra - 'A']) {
			registro[raiz->letra - 'A'] = (*contador)++;
			char *letra = malloc(2 * sizeof(char));
			letra[0] = raiz->letra;
			letra[1] = 0;
			insertar(expresiones, letra);
			return 1;
		} else {
			return 0;
		}
	}
	
	return resultado;
}

/* char obtener_expresiones (raiz, expresiones, num_espacios)
 * Recorre el árbol recursivamente. Va agregando al vector 'expresiones' 
 * las cadenas correspondientes a cada nodo. 'num_espacios' se debe inicializar
 * en 0 desde afuera. Sirve como contador del espacio total de todas las 
 * expresiones */

char *obtener_expresiones (Nodo *raiz, Vector *expresiones, int *num_espacios)
{
	char *parte_izquierda;          /* Cadena que regresa rama izquierda */
	char *parte_derecha;            /* Cadena que regresa rama derecha */
	char resultado[100];            /* Resultado que se propaga hacia arriba */
	
	if (raiz->izquierda)
		parte_izquierda = obtener_expresiones(raiz->izquierda, 
			expresiones, num_espacios);
	if (raiz->derecha)
		 parte_derecha = obtener_expresiones(raiz->derecha, 
			expresiones, num_espacios);
	
	if (raiz->operacion == PARENTESIS)
		return parte_izquierda;
	
	if (raiz->letra != 0) {
	        
		if(!evaluacion_var[raiz->letra - 'A']) {
			evaluacion_var[raiz->letra - 'A'] = 1;
			*num_espacios += 4;
		}
		sprintf(resultado, "%c", raiz->letra);
		return strdup(resultado);
	
	} else {
	
		if (raiz->operacion == NOT) {
			if (raiz->izquierda->operacion == PARENTESIS)
				parte_izquierda = agregar_parentesis(parte_izquierda);
			sprintf(resultado, "~%s", parte_izquierda);
	    } else {
			if (raiz->izquierda->operacion == PARENTESIS) 
				parte_izquierda = agregar_parentesis(parte_izquierda);
			if (raiz->derecha->operacion == PARENTESIS) 
				parte_derecha = agregar_parentesis(parte_derecha);
	
			if (raiz->operacion == AND)
				sprintf(resultado, "%s & %s", 
					parte_izquierda, parte_derecha);
			else if (raiz->operacion == OR) 
				sprintf(resultado, "%s v %s", 
					parte_izquierda, parte_derecha);
			else if (raiz->operacion == XOR)
				sprintf(resultado, "%s xor %s", 
					parte_izquierda, parte_derecha);
			else if (raiz->operacion == CONDICIONAL)
				sprintf(resultado, "%s -> %s", 
					parte_izquierda, parte_derecha);
			else if (raiz->operacion == BICONDICIONAL)
				sprintf(resultado, "%s <-> %s", 
					parte_izquierda, parte_derecha);
		}
	
		insertar(expresiones, strdup(resultado));
		*num_espacios += strlen(resultado) + 3;
		return strdup(resultado);
	
	}
}

/* asignar_variables (evaluacion, num_variables) 
 * Asigna la evaluacion (arregla evaluacion) al arreglo global
 * evaluacion_var segun el orden de aparición en la expresión
 * (se encuentra especificado en el arregle registro) */

void asignar_variables(int *evaluacion, int num_variables) 
{
	int contador = 1;
	for(int i=0; i<num_variables; i++)
		for (int j=0; j<26; j++)
			if(registro[j] == contador) {
				evaluacion_var[j] = evaluacion[i];
				contador++;
				break;
			}
}

/* int evaluar (raiz, evaluacion, num_variables, indice) 
 * Recorre el árbol recursivamente evaluando la expresión. Los valores de 
 * evaluación se encuentran en 'evaluacion_var', los resultados se guardan en la
 * se gunda parte de 'evaluacion'. 'indice' se inicializa fuera de la función 
 * con el valor de numero de variables, controla la escritura de los resultados 
 * sobre el arreglo. */

int evaluar (Nodo *raiz, int *evaluacion, int *indice)
{
	int resultado;			/* Resultado que se propaga hacia arriba */

	if (raiz->letra != 0)
		return evaluacion_var[raiz->letra - 'A'];

	if (raiz->operacion == PARENTESIS)
		return evaluar(raiz->izquierda, evaluacion, indice);

	if (raiz->operacion == NOT) 
		resultado = !evaluar(raiz->izquierda, evaluacion, indice);
	else 
		if (raiz->operacion == OR) 
			resultado = evaluar(raiz->izquierda, evaluacion, indice) | 
				evaluar(raiz->derecha, evaluacion, indice);
		else if (raiz->operacion == AND) 
			resultado = evaluar(raiz->izquierda, evaluacion, indice) &
				evaluar(raiz->derecha, evaluacion, indice);
		else {
			/* Operadores compuestos */
			int izq = evaluar(raiz->izquierda, evaluacion, indice);
			int der = evaluar(raiz->derecha, evaluacion, indice);
			if (raiz->operacion == XOR)
				resultado = (der & !izq) | (!der & izq);
			else if (raiz->operacion == CONDICIONAL)
				resultado = !izq | der;
			else
				resultado = (!der & !izq) | (der & izq);

		} 

	evaluacion[(*indice)++] = resultado;
}

/* char agregar_parentesis(cadena) 
 * Reserva memoria dinámica para una nueva cadena envuelta en
 * paréntesis. Libera la memoria de la cadena anterior. */

char *agregar_parentesis (char *original)
{
	int longitud = strlen(original);
	char *resultado = malloc(longitud + 3);
	resultado[0] = '(';
	for (int i=0; i<longitud; i++)
		resultado[i + 1] = original[i];
	resultado[longitud + 1] = ')';
	resultado[longitud + 2] = 0;
	free(original);
	return resultado;
}

