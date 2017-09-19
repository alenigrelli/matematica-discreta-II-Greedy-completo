#include "hash.h"

// Aplicar la hash a 'x'. 't' es el tamaño de la tabla hash.
#define hash(x, t) ( (u32) ((x) * (2654435761)) % (t))

HashP crearHash(u32 tam) {
    HashP h = malloc(sizeof(struct HashSt));
    h->tam = tam;
    h->tabla = calloc(tam, sizeof(VerticeSt));
    return h;
}

void insertarVertice(HashP h, VerticeSt v) {
    // aplicar la hash al nombre del vertice 'v'
    u32 ind = (u32) hash(v->nombre, h->tam);

    // buscar un lugar vacio
    // 'ind' es un indice para recorrer el arreglo de la tabla
    while (h->tabla[ind] != NULL) {
        ind++;
        if (ind == h->tam) ind = 0;
    }

    // guardar el vertice en el arreglo
    h->tabla[ind] = v;
}

VerticeSt buscarVertice(HashP h, u32 valor) {
    // aplicar la hash a 'valor'
    u32 ind = (u32) hash(valor, h->tam);

    // buscar el vertice con el nombre 'valor'
    // 'i' es para no dar mas de una vuelta a la tabla
    // 'ind' es un indice para recorrer el arreglo de la tabla
    // parar si se dió una vuelta o hay un lugar vacio
    for (u32 i = 0; i < h->tam && h->tabla[ind] != NULL; i++) {
        if (h->tabla[ind]->nombre == valor)
            return h->tabla[ind];
        ind++;
        if (ind == h->tam) ind = 0;
    }

    // no se encontró
    return NULL;
}

HashP destruirHash(HashP h) {
    free(h->tabla);
    free(h);
    return NULL;
}
