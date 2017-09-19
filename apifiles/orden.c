#include "Cthulhu.h"
#include "grafo.h"

// segun nombre, creciente
int compararVertices(const void *v1, const void *v2) {
    const u32 *c1 = v1;
    const u32 *c2 = v2;
    return (ver[(*c1)].nombre - ver[(*c2)].nombre);
}

// segun grados, decreciente
int compararGrados(const void *v1, const void *v2) {
    const u32 *c1 = v1;
    const u32 *c2 = v2;
    return (ver[(*c2)].grado - ver[(*c1)].grado);
}

// segun cantidad de vertices de ese color, decreciente
int compararColoresGra(const void *v1, const void *v2) {
    const u32 *c1 = v1;
    const u32 *c2 = v2;
    if ((col[ver[(*c2)].color] - col[ver[(*c1)].color]) == 0)
        return ver[(*c1)].color - ver[(*c2)].color;
    return (col[ver[(*c2)].color] - col[ver[(*c1)].color]);
}

// segun cantidad de vertices de ese color, creciente
int compararColoresChi(const void *v1, const void *v2) {
    const u32 *c1 = v1;
    const u32 *c2 = v2;
    if ((col[ver[(*c1)].color] - col[ver[(*c2)].color]) == 0) {
        return ver[(*c1)].color - ver[(*c2)].color;
    }
    return (col[ver[(*c1)].color] - col[ver[(*c2)].color]);
}

// segun color, decreciente
int compararColoresRev(const void *v1, const void *v2) {
    const u32 *c1 = v1;
    const u32 *c2 = v2;
    return (ver[(*c2)].color - ver[(*c1)].color);
}


void OrdenNatural(NimheP G) {
    qsort(G->orden, G->totalV, sizeof(u32), compararVertices);
}

void OrdenWelshPowell(NimheP G) {
    qsort(G->orden, G->totalV, sizeof(u32), compararGrados);
}

// ordenar los vertices segun su color
// el orden de los colores es aleatorio
void ReordenAleatorioRestringido(NimheP G) {
    u32 *c = G->colorV;  // para generar el orden aleatorio
    u32 n = G->colorN + 1;  // limite para el orden generado
    u32 pos;  // indice

    // generar una cantidad de vertices de cada color aleatorio
    for (u32 i = 1; i < n; i++) {
        // posicion random
        pos = rand()%(n-1) + 1;
        // guardar el valor a intercambiar
        u32 temp = c[i];
        // intercambiar 'i' con 'pos'
        // si 'pos' no fue inicializado, asignar el indice
        // sinó asignar con lo que contenia
        c[i] = (c[pos] == 0) ? pos : c[pos];
        // lo mismo pero al reves
        c[pos] = (temp == 0) ? i : temp;
    }
    // ordenar de alguna forma que dependa de 'col'
    qsort(G->orden, G->totalV, sizeof(u32), compararColoresChi);
}

// ordenar primero los vertices del color con mas vertices
void GrandeChico(NimheP G) {
    qsort(G->orden, G->totalV, sizeof(u32), compararColoresGra);
}

// ordenar primero los vertices del color con menos vertices
void ChicoGrande(NimheP G) {
    qsort(G->orden, G->totalV, sizeof(u32), compararColoresChi);
}

// ordenar primero los vertices del color mas grande
void Revierte(NimheP G) {
    qsort(G->orden, G->totalV, sizeof(u32), compararColoresRev);
}

// ordenar segun 'x'
void OrdenEspecifico(NimheP G, u32 *x) {
    bool *b = calloc(G->totalV, sizeof(bool));  // para marcar numeros del orden
    // calloc inicializa en false

    // chequear imagen de x
    // x[i] debe ser menor estricto a la cantidad de vertices
    // b guarda true si ya leimos ese numero
    for (u32 i = 0; i < G->totalV; i++) {
        if (G->totalV <= x[i] || b[x[i]]) {
            free(b);
            return;
        }
        b[x[i]] = true;
    }
    // x es correcto. Liberar b
    free(b);

    // ordenar
    OrdenNatural(G);

    // guardar el nuevo orden en un nuevo arreglo
    u32 *ord = malloc(G->totalV * sizeof(u32));

    // llenar 'ord' segun los indices de 'x'
    for (u32 i = 0; i < G->totalV; i++)
        ord[i] = G->orden[x[i]];

    // liberar el viejo orden
    free(G->orden);

    // asignar el nuevo orden
    G->orden = ord;
}
