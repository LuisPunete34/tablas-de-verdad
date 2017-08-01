/* tabla.l
 * Ricardo Quezada Figueroa 
 * Análisis sintáxico de expresiones booleanas. 
 * Depende de tabla.y */

%{ 

#include "arbol.h"              /* Tipo de dato Nodo */
#include "tabla.tab.h"		/* Macros de tokens */
extern int lineno;              /* Número de lineas */

%}

%%

 /* Espacios en blanco */
[ \t]		{ ; }

 /* Fin de archivo */
<<EOF>>         { printf("\nHasta la próxima!\n");
                  return 0; }

salir           { printf("Hasta la próxima!\n");
                  return 0; }

 /* LETRAS */
[A-Z]		{
	yylval.letra = yytext[0];
	return LETRA;
}

 /* BICONDICIONAL */
\<\-\>          { return T_BICONDICIONAL; }

 /* CONDICIONAL */
\-\>            { return T_CONDICIONAL; }

 /* OR EXCLUSIVO */
xor             { return T_XOR; }

 /* Saltos de linea */
\n      { lineno++; 
          return '\n'; }

 /* Todo lo demás */
.		{ return yytext[0]; }

%%

int yywrap () 
{
        return 1;
}

