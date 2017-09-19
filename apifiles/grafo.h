#ifndef _GRAFO_H
#define _GRAFO_H

#include "vertices.h"

u32 *col;                        // variable global de colorV
struct VerticeSt *ver;           // variable global de vertices

struct NimheSt {
    struct VerticeSt *vertices;  // todos los vertices
    u32 *orden;                  // orden de los vertices
    u32 *colorV;                 // cantidad de vertices de cada color
    u32 colorN;                  // mejor coloreo
    u32 totalV;                  // numero total de vertices
    u32 totalL;                  // numero total de lados
    u32 deltaM;                  // el mayor de los grados
};

#endif
