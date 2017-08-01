/* tabla.y
 * Análisis sintáctico de expresiones booleanas. 
 * Usa yacc para construir un árbol explicito y después
 * evaluar todas las posibilidades y construir la tabla
 * de verdad. El analizador léxico está en tabla.l 
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

%{

#include "arbol.h"				/* nodo, nuevo_nodo, nueva_hoja */
#include <stdio.h>				/* printf, fprintf */
#include <stdlib.h>				/* exit */
#include <ctype.h>				/* getchar */
#include <setjmp.h>				/* setjmp, longjmp */

/* Declaraciones */
int yylex ();
void yyerror (char *mensaje);
void imprimir_notacion ();
void peligro (char *mensaje, char *detalle);
void error_ejecucion (char *mensaje, char *detalle);

%}

%union {						/* Tipos de datos de la pila de yacc */
	char letra;					/* Caso de hojas. Proposiciones */
	Nodo* nodo;					/* Caso de nodos. Apuntadores a árbol */ 
}

%token <letra> LETRA			/* Token para representar proposiciones */
%token <letra> T_BICONDICIONAL	/* Token para bicondicionales <-> */
%token <letra> T_CONDICIONAL	/* Token para condicionales -> */
%token <letra> T_XOR			/* Token para or exclusivo xor */
%type <nodo> expresion			/* Tipo de dato de no terminal expresión */

/* Asociatividad y precedencia */
%left 'v' T_XOR
%left '&' T_CONDICIONAL T_BICONDICIONAL    
%left NEGACION

%%

lista:			/* Nada */
		|		lista '\n'
		|		lista expresion '\n'
					{ imprimir_tabla($2); 
					  liberar_arbol($2); 
					  printf("\nExpresión> "); }
		|			  lista error '\n'                        
					{ yyerrok; 
					  printf("\nExpresión> "); }
		;

expresion:		LETRA
					{ $$ = nueva_hoja($1); }
		|		expresion '&' expresion
					{ $$ = nuevo_nodo($1, $3, AND); }
		|		expresion 'v' expresion
					{ $$ = nuevo_nodo($1, $3, OR); }
		|		expresion T_XOR expresion
					{ $$ = nuevo_nodo($1, $3, XOR); }
		|		expresion T_CONDICIONAL expresion
					{ $$ = nuevo_nodo($1, $3, CONDICIONAL); }
		|		expresion T_BICONDICIONAL expresion
					{ $$ = nuevo_nodo($1, $3, BICONDICIONAL); }
		|		'~' expresion %prec NEGACION
					{ $$ = nuevo_nodo($2, NULL, NOT); }
		|		'(' expresion ')'
					{ $$ = nuevo_nodo($2, NULL, PARENTESIS); }
		;

%%

jmp_buf inicio;				/* Para recuperación despues de errores */
char *progname;				/* Para mensajes de error */
int lineno = 1;				/* Contador de lineas. Usado en mensajes de error */

/* main (argumentos)
 * Entra inmediatamente al ciclo de yyparse */

int main (int argc, char *argv[])
{
	progname = argv[0];
	printf("Creación de tablas de verdad a partir de expresiones booleanas.\n");
	imprimir_notacion();
	setjmp(inicio);
	printf("Expresión> ");
	yyparse();
	return 0;
}

/* imprimir_notacion
 * imprime la notación usada */

void imprimir_notacion ()
{
	printf("'salir' para terminar ejecución\n");
	printf("==> OR 'v'\n==> AND '&'\n==> NOT '~'\n==> XOR 'xor'\n");
	printf("==> CONDICIONAL '->'\n==> BICONDICIONAL '<->'\n\n");
}

void yyerror (char *mensaje)
{
	peligro(mensaje, (char *) 0);
}

/* peligro (mensaje, detalle)
 * Imprime el mensaje y el detalle en el flujo de errores */

void peligro (char *mensaje, char *detalle)
{
	fprintf(stderr, "%s: %s. ", progname, mensaje);
	if (detalle) 
		fprintf(stderr, "%s. ", detalle);
	fprintf(stderr, "En línea %d.\n", lineno);
}

/* error_ejecucion (mensaje, detalle) 
 * Llama a la funcion peligro para imprimir mensajes y hace
 * un salto al inicio de la ejecución */

void error_ejecucion (char *mensaje, char *detalle)
{
	peligro(mensaje, detalle);
	longjmp(inicio, 0);
}

