#include "Cthulhu.h"
#include "vertices.h"

VerticeSt crearVertice(u32 nombre, VerticeSt v) {
    v->nombre = nombre;
    v->grado = 1;
    return v;
}

void agregarVecinos(VerticeSt v, VerticeSt w) {
    // v->w
    v->vecinos[v->color] = &w->color;
    v->color++;
    // w->v
    w->vecinos[w->color] = &v->color;
    w->color++;
}

u32 ColorDelVertice(VerticeSt x) {
    return x->color;
}

u32 GradoDelVertice(VerticeSt x) {
    return x->grado;
}

u32 NombreDelVertice(VerticeSt x) {
    return x->nombre;
}

void ImprimirVecinosDelVertice(VerticeSt x, NimheP G) {
    VerticeSt w;  // vecino de 'x'

    for (u32 i = 0; i < x->grado; i++) {
        // vecinos[i] apunta al color del vertice vecino
        // con el casteo apunta a toda la estructura
        w = (VerticeSt) x->vecinos[i];
        printf("%u%s", w->nombre, i == x->grado - 1 ? ".\n" : ",");
    }
}

void destruirVertice(VerticeSt v) {
    // liberar solo el arreglo de vecinos
    free(v->vecinos);
}
