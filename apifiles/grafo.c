#include "Cthulhu.h"
#include "grafo.h"
#include "hash.h"
#include "vertices.h"
#include <time.h>

#define MAXDIMACS 80

/*
 * Limpiar toda la memoria alocada antes de devolver NULL.
 */
void limpiarCarga(NimheP G, HashP h, u32* carga) {
    destruirHash(h);
    free(carga);
    free(G->orden);
    free(G->vertices);
    free(G);
}


/*
 * Crea la estructura NimheSt.
 * Saltea los comentarios y guarda la cantidad de vertices y lados.
 * Devuelve el puntero a la estructura si todo anduvo bien.
 * Devuelve NULL si el input es incorrecto.
 */
NimheP crearGrafo() {
    NimheP G = malloc(sizeof(struct NimheSt));  // estructura del grafo
    char cadena[MAXDIMACS];  // para leer el input
    char *palabra;  // para dividir el input

    // saltar comentarios
    do {
        // leer input y separar la primera palabra
        palabra = fgets(cadena, MAXDIMACS, stdin);
        palabra = strtok(cadena, " ");
    } while (strncmp(palabra, "c", 1) == 0);

    // chequear que la primera linea no comentada empiece con "p"
    if (strcmp(palabra , "p") != 0) {
        free(G);
        return NULL;
    }

    // saltar la palabra edge
    if (strcmp(strtok(NULL, " "), "edge") != 0) {
        free(G);
        return NULL;
    }

    // leer vertices y lados
    G->totalV = atoi(strtok(NULL, " "));
    G->totalL = atoi(strtok(NULL, " "));

    // crear arreglo para el orden
    G->orden = malloc(G->totalV * sizeof(u32));

    // crear arreglo para vertices
    G->vertices = calloc(G->totalV, sizeof(struct VerticeSt));
    return G;
}

/*
 * Carga los lados y los vertices del input.
 * Devuelve el mismo 'G' si todo anduvo bien.
 * Devuelve NULL si hay un error en el input, en la cantidad de lados o
 * en el nombre de un vertice.
 */
NimheP cargarLados(NimheP G) {
    char cadena[MAXDIMACS];  // para leer el input
    char *palabra;  // para dividir el input
    u32 nombre1, nombre2;  // nombres de los vertices del lado
    VerticeSt vertice1, vertice2;  // punteros a los vertices del lado
    u32 pos = 0;  // indice sobre 'G->vertices'

    // tabla hash
    HashP h = crearHash(2 * G->totalV);

    // arreglo temporal para guardar lados
    u32 *carga = calloc(G->totalL * 2, sizeof(u32));

    // leer lados y crear vertices
    for (u32 i = 0; i < G->totalL * 2; i += 2) {
        // leer input y separar la primera palabra
        palabra = fgets(cadena, MAXDIMACS, stdin);
        palabra = strtok(cadena, " ");

        // chequear que la primera palabra sea "e"
        if (strcmp(palabra, "e") != 0) {
            limpiarCarga(G, h, carga);
            return NULL;
        }

        // leer nombres de los vertices
        nombre1 = strtoul(strtok(NULL, " "), NULL, 10);
        nombre2 = strtoul(strtok(NULL, " "), NULL, 10);

        // 'strtol()' puede fallar al tratar de convertir un numero muy grande
        // depende de la artitectura
        if (errno == ERANGE) {
            limpiarCarga(G, h, carga);
            return NULL;
        }

        // guardar lado
        carga[i] = nombre1;
        carga[i+1] = nombre2;

        // buscar o crear vertices si no lo leimos antes
        // si ya estaban aumentar su grado
        vertice1 = buscarVertice(h, nombre1);
        if (vertice1 == NULL) {
            vertice1 = crearVertice(nombre1, &G->vertices[pos]);
            insertarVertice(h, vertice1);
            pos++;
        } else {
            vertice1->grado++;
        }
        vertice2 = buscarVertice(h, nombre2);
        if (vertice2 == NULL) {
            vertice2 = crearVertice(nombre2, &G->vertices[pos]);
            insertarVertice(h, vertice2);
            pos++;
        } else {
            vertice2->grado++;
        }
    }

    // chequear que se hayan cargado la cantidad correcta de vertices.
    if (G->totalV != pos) {
        limpiarCarga(G, h, carga);
        return NULL;
    }

    // crear arreglos de vecinos en todos los vertices
    for (u32 i = 0; i < G->totalV; i++)
        G->vertices[i].vecinos = malloc(G->vertices[i].grado * sizeof(u32*));

    // conectar los vertices
    for (u32 i = 0; i < G->totalL * 2; i += 2) {
        vertice1 = buscarVertice(h, carga[i]);
        vertice2 = buscarVertice(h, carga[i+1]);
        agregarVecinos(vertice1, vertice2);
    }

    // destruir tabla hash y arreglo temporal de carga
    destruirHash(h);
    free(carga);

    // limpiar los colores de los vertices
    for (u32 i = 0; i < G->totalV; i++)
        G->vertices[i].color = 0;

    return G;
}

NimheP NuevoNimhe() {
    // crear estructura inicial
    NimheP G = crearGrafo();
    if (G == NULL)
        return NULL;

    // cargar vertices y lados
    G = cargarLados(G);
    if (G == NULL)
        return NULL;

    // calcular 'deltaM' y llenar 'orden'
    G->deltaM = G->totalV;
    for (u32 i = 0; i < G->totalV; i++) {
        G->orden[i] = i;
        if (G->deltaM < G->vertices[i].grado)
            G->deltaM = G->vertices[i].grado;
    }

    // cosas para el coloreo
    G->colorV = calloc(G->deltaM + 2, sizeof(u32));
    G->colorN = G->deltaM + 1;

    // inicializar semilla
    srand(time(NULL));

    // setear variables globales para el reordenamiento
    col = G->colorV;
    ver = G->vertices;
    return G;
}

u32 NumeroDeVertices(NimheP G) {
    return G->totalV;
}

u32 NumeroDeLados(NimheP G) {
    return G->totalL;
}

u32 NumeroVerticesDeColor(NimheP G, u32 i) {
    return G->colorV[i];
}

u32 ImprimirVerticesDeColor(NimheP G, u32 i) {
    u32 total = G->colorV[i];  // total de vertices de color 'i'
    VerticeSt v;  // para recorrer los vertices

    if (total == 0) {
        printf("No hay vertices de color %u\n", i);
        return 0;
    }

    printf("Vertices de Color %u: ", i);
    // recorrer todos los vertices buscando los que tienen color 'i'
    for (u32 j = 0, conteo = 0; conteo < total; j++) {
        v = &G->vertices[G->orden[j]];
        if (v->color == i) {
            conteo++;
            printf("%u%s", v->nombre, conteo == total ? ".\n" : ",");
        }
    }
    return total;
}

u32 CantidadDeColores(NimheP G) {
    return G->colorN;
}

VerticeSt IesimoVerticeEnElOrden(NimheP G, u32 i) {
    // chequear que 'i' no sobrepase la cantidad de vertices
    assert(i < G->totalV);
    return &G->vertices[G->orden[i]];
}

VerticeSt IesimoVecino(NimheP G, VerticeSt x, u32 i) {
    // chequear que 'i' no sobrepase el grado del vertice 'x'
    assert(i <= x->grado);
    // como vecinos[i] apunta al parametro color del vecino i
    // con el casteo, apunta a todo el vertice
    return (VerticeSt) x->vecinos[i];
}

int DestruirNimhe(NimheP G) {
    // liberar recursos de los vertices
    for (u32 i = 0; i < G->totalV; i++)
        destruirVertice(&G->vertices[i]);

    // liberar las estructuras de todos los vertices
    free(G->vertices);

    // liberar todo lo demas
    free(G->orden);
    free(G->colorV);
    free(G);

    // siempre devolver 1
    return 1;
}
