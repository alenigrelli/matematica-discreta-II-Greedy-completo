#include "Cthulhu.h"
#include "cola.h"
#include "grafo.h"

/*
 * Tratar de colorear usando el algoritmo dos colores.
 * Devuelve 1 si 'G' es dos colores, 0 si no.
 */
int Chidos(NimheP G) {
    VerticeSt x = NULL;  // primer vertice a colorear
    VerticeSt p = NULL;  // primer vertice de la cola
    u32 j = 0;  // cantidad de vertices coloreados

    // limpiar colores
    for (u32 i = 0; i < G->totalV; i++)
        G->vertices[i].color = 0;

    while (j < G->totalV) {
        // elegir v no coloreado
        u32 k;  // indice
        for (k = 0; G->vertices[k].color != 0; k++) {}
        x = &G->vertices[k];

        // colorear x con 1
        x->color = 1;
        j++;

        // crear cola con x adentro
        Cola c = crearCola();
        encolar(c, x);
        while (!estaVacia(c)) {
            p = decolar(c);
            // para cada vecino de p
            for (u32 i = 0; i < p->grado; i++) {
                if (*p->vecinos[i] == 0) {
                    // encolar y colorear
                    encolar(c, (VerticeSt) p->vecinos[i]);
                    j++;
                    *p->vecinos[i] = 3 - p->color;
                } else if (*p->vecinos[i] == p->color) {
                    // no es dos colores
                    c = destruirCola(c);
                    return 0;
                }
            }
        }
        c = destruirCola(c);
    }
    return 1;
}

/*
 * Colorea a 'v' y devuelve el color usado.
 * 'tam' es una cota superior de los colores de los vecinos de 'v'.
 */
u32 colorearVertice(VerticeSt v, u32 tam) {
    // memoria auxiliar para marcar los colores de los vertices vecinos
    u32* colores = calloc(tam / sizeof(u32) + 1, sizeof(u32));

    // visitar todos sus vecinos
    // y marcarlos usando bitwise
    colores[0] = 1;  // no usar el color 0
    for (u32 i = 0; i < v->grado; i++) {
        // posicion en el arreglo
        u32 indice = *v->vecinos[i] >> 5;

        // marcar el bit correspondiente
        colores[indice] = colores[indice] | (1 << (*v->vecinos[i] & 31));
    }

    // buscar primer color libre
    // saltar de a 32 colores
    u32 i = 0;
    while (colores[i] == 0xffffffff) {
        i++;
    }

    // colores[i] tiene un color disponible
    register u32 mask = 1;  // para recorrer cada bit
    register u32 c = 32 * i;  // color que representa cada bit

    // buscar color libre
    while (colores[i] & mask) {
        mask = mask << 1;
        c++;
    }

    // pintar a v
    v->color = c;

    // liberar memoria auxiliar
    free(colores);
    return c;
}

/*
 * Colorear todos los vertices de 'G' usando el algoritmo greedy.
 * Devuelve la cantidad de colores usados.
 */
u32 Greedy(NimheP G) {
    VerticeSt v;  // vertice a colorear
    u32 n = 1;  // coloreo

    // limpiar los colores para correr greedy
    memset(G->colorV, 0, (G->deltaM + 1) * sizeof(u32));

    // limpiar colores viejos de los vertices
    for (u32 i = 0; i < G->totalV; i++)
        G->vertices[i].color = 0;

    // el primer vertice tiene color 1
    G->vertices[G->orden[0]].color = 1;
    G->colorV[1] = 1;

    // colorear
    for (u32 i = 1; i < G->totalV; i++) {
        v = &G->vertices[G->orden[i]];

        // v tiene un solo vecino
        if (v->grado == 1) {
            v->color = (*v->vecinos[0] == 1) + 1;
            G->colorV[v->color]++;
            if (n < v->color)  n = v->color;
            continue;
        }

        // v está conectados con todos
        if (v->grado == G->totalV - 1) {
            n++;
            v->color = n;
            G->colorV[n]++;
            continue;
        }

        u32 c = colorearVertice(v, n);
        G->colorV[c]++;

        // guardar color mas grande
        if (n < c)  n = c;
    }
    G->colorN = n;
    return G->colorN;
}
