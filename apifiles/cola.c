#include "cola.h"

Cola crearCola() {
    return calloc(1, sizeof(struct ColaSt));
}

bool estaVacia(Cola c) {
    return (c->primero == NULL);
}

void encolar(Cola c, VerticeSt x) {
    NodoP temp = malloc(sizeof(struct NodoSt));  // nuevo nodo
    temp->dato = x;
    temp->sig = NULL;

    // la cola está vacia
    if (c->primero == NULL && c->ultimo == NULL) {
        c->primero = c->ultimo = temp;
        return;
    }

    // encolar al final
    c->ultimo->sig = temp;
    c->ultimo = temp;
}

VerticeSt decolar(Cola c) {
    NodoP temp = c->primero;  // nodo a liberar
    VerticeSt v = temp->dato;  // vertice a devolver

    if (c->primero == c->ultimo)
        c->primero = c->ultimo = NULL;
    else
        c->primero = c->primero->sig;

    // liberar nodo
    free(temp);
    return v;
}

Cola destruirCola(Cola c) {
    NodoP nodo = c->primero;  // nodo a liberar
    NodoP sig;  // siguiente nodo a liberar
    while (nodo != NULL) {
        sig = nodo->sig;

        // solo dejar de apuntar al vertice
        nodo->dato = NULL;
        nodo->sig = NULL;

        // liberar nodo
        free(nodo);
        nodo = sig;
    }
    // liberar la estructura
    free(c);
    return NULL;
}
