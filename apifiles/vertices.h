#ifndef _VERTICES_H
#define _VERTICES_H

#include "Cthulhu.h"

struct VerticeSt {
    u32 color;      // color del vertice
    u32 nombre;     // nombre del vertice unico
    u32 grado;      // cantidad de vecinos
    u32 **vecinos;  // arrego de vecinos de tamaño 'grado'
};

/*
 * Crea un vertice con nombre 'nombre' y grado 1 en 'v'.
 * Devuelve el mismo puntero 'v'
 */
VerticeSt crearVertice(u32 nombre, VerticeSt v);

/*
 * Agrega vecinos mutuamente entre 'v' y 'w'.
 * Apuntar solo al campo 'color' del vecino.
 * Usar campo 'color' como indice para guardar el siguiente vecino.
 * Al terminar, deberia ser 'color' == 'grado'.
 */
void agregarVecinos(VerticeSt v, VerticeSt w);

/*
 * Libera recursos del vertice, pero no su estructura VerticeSt!
 */
void destruirVertice(VerticeSt v);

#endif
