/* tabla.l
 * Análisis sintáxico de expresiones booleanas. 
 * Depende de tabla.y 
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

#include "arbol.h"				/* Tipo de dato Nodo */
#include "tabla.tab.h"			/* Macros de tokens */
extern int lineno;				/* Número de lineas */

%}

%%

 /* Espacios en blanco */
[ \t]		{ ; }

 /* Fin de archivo */
<<EOF>>		{ printf("\nHasta la próxima!\n");
			  return 0; }

salir		{ printf("Hasta la próxima!\n");
			  return 0; }

 /* LETRAS */
[A-Z]		{ yylval.letra = yytext[0];
			  return LETRA; }

 /* BICONDICIONAL */
\<\-\>		{ return T_BICONDICIONAL; }

 /* CONDICIONAL */
\-\>		{ return T_CONDICIONAL; }

 /* OR EXCLUSIVO */
xor			{ return T_XOR; }

 /* Saltos de linea */
\n		{ lineno++; 
		  return '\n'; }

 /* Todo lo demás */
.		{ return yytext[0]; }

%%

int yywrap () 
{
	return 1;
}

