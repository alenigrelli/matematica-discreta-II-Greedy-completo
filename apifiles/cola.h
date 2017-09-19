#ifndef _COLA_H
#define _COLA_H

#include "vertices.h"

typedef struct NodoSt *NodoP;
typedef struct ColaSt *Cola;

struct ColaSt {
    NodoP primero;  // puntero al primer nodo
    NodoP ultimo;   // puntero al ultimo nodo
};

struct NodoSt {
    VerticeSt dato;  // puntero al vertice encolado
    NodoP sig;      // puntero al siguiente nodo
};

/*
 * Crear cola de VerticeSt vacia. Destruirla luego de usarla.
 */
Cola crearCola();

/*
 * Devuelve True si no hay elementos en la cola.
 */
bool estaVacia(Cola c);

/*
 * Coloca a 'x' al final de la cola 'c'.
 */
void encolar(Cola c, VerticeSt x);

/*
 * Quita al primero de la cola 'c' y lo devuelve.
 */
VerticeSt decolar(Cola c);

/*
 * Libera la memoria. Devuelve NULL.
 */
Cola destruirCola(Cola c);

#endif
