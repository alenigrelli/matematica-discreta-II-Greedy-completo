/*
 *
 * Proyecto Discreta II - 2016
 *          Parte 1
 *
 * Alejandro Jose Nigrelli -> e-mail: alejandro.nigrelli@gmail.com
 * Alejandro Ismael Silva  -> e-mail: alex.sai95@gmail.com
 *
 */

#ifndef _CTHULHU_H
#define _CTHULHU_H

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct VerticeSt *VerticeSt;
typedef struct NimheSt *NimheP;
typedef uint32_t u32;

// color.c
int Chidos(NimheP G);
u32 Greedy(NimheP G);

// grafo.c
NimheP NuevoNimhe();
u32 NumeroDeVertices(NimheP G);
u32 NumeroDeLados(NimheP G);
u32 NumeroVerticesDeColor(NimheP G, u32 i);
u32 ImprimirVerticesDeColor(NimheP G, u32 i);
u32 CantidadDeColores(NimheP G);
VerticeSt IesimoVerticeEnElOrden(NimheP G, u32 i);
VerticeSt IesimoVecino(NimheP, VerticeSt x, u32 i);
int DestruirNimhe(NimheP G);

// orden.c
void OrdenNatural(NimheP G);
void OrdenWelshPowell(NimheP G);
void ReordenAleatorioRestringido(NimheP G);
void GrandeChico(NimheP G);
void ChicoGrande(NimheP G);
void Revierte(NimheP G);
void OrdenEspecifico(NimheP G, u32* x);

// vertices.c
u32 ColorDelVertice(VerticeSt x);
u32 GradoDelVertice(VerticeSt x);
u32 NombreDelVertice(VerticeSt x);
void ImprimirVecinosDelVertice(VerticeSt x, NimheP G);

#endif
