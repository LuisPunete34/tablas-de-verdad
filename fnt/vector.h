/* vector.h
 * Ricardo Quezada Figueroa 
 * Archivo de cabecera de vector */

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

