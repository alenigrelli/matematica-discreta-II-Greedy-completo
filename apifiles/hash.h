#ifndef _HASH_H
#define _HASH_H

#include "vertices.h"

typedef struct HashSt *HashP;

struct HashSt {
    u32 tam;         // tamaño del arreglo
    VerticeSt *tabla;  // arreglo de VerticeSt
};

/*
 * Crear tabla hash vacia con el tamaño 'tam'.
 * Destruir luego de usarla.
 */
HashP crearHash(u32 tam);

/*
 * Inserta el vertice 'valor' en la tabla hash 'h'.
 * No insertar mas vertice que el tamaño de la tabla hash.
 */
void insertarVertice(HashP h, VerticeSt v);

/*
 * Devuelve el vertice con el nombre 'valor' si se encuentra en tabla hash 'h'.
 * Devuelve NULL si no se encuentra.
 */
VerticeSt buscarVertice(HashP h, u32 valor);

/*
 * Libera la memoria. Devuelve NULL.
 */
HashP destruirHash(HashP h);

#endif
