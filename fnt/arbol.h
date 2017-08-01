/* arbol.h 
 * Ricardo Quezada Figueroa 
 * Archivo de cabecera de estructura de árbol */

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
typedef struct Nodo {                   /* Nodo de árbol */
        char letra;                     /* Para representar preposiciones en las hojas */
        int operacion;                  /* Macro que define operacion en elementos no-hoja */
        struct Nodo *izquierda;         /* Apuntador a nodo izquierdo */
        struct Nodo *derecha;           /* Apuntador a nodo derecho */
} Nodo;

/* Declaraciones */
Nodo *nueva_hoja (char letra);
Nodo *nuevo_nodo (Nodo *izq, Nodo *der, int operacion);
void imprimir_tabla (Nodo *raiz);
void liberar_arbol (Nodo *raiz);

#endif

