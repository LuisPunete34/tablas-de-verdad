/* vector.c
 * Ricardo Quezada Figueroa 
 * Implementación de vector
 * vim: set foldmethod=indent : */

#include "vector.h"             /* Contenedor, Vector */
#include <stdio.h>              /* fprintf */
#include <stdlib.h>             /* malloc, free */

/* Declaraciones privadas */
Contenedor *crear_contenedor();

/* Vector nuevo_vector
 * Reserva espacio para un nuevo vector.
 * Los apuntadores se inicializan en NULL */

Vector *nuevo_vector()
{
        Vector *resultado;                      /* Variable resultado */
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
       Contenedor *resultado;                   /* Variable resultado */
       Contenedor *auxiliar;                    /* Variable para movimientos */
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
        Contenedor *resultado;                  /* Variable resultado */
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

