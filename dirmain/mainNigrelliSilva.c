#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Cthulhu.h"

struct MejorPasada {
        u32 *semilla;   // mejor orden en orden aleatorio
        u32 coloreo;    // mejor coloreo
        u32 ordenCG;    // veces ordenado con ChicoGrande
        u32 ordenGC;    // veces ordenado con GrandeChico
        u32 ordenR;     // veces ordenado con Revierte
        u32 ordenRAR;   // veces ordenado con ReordenAleatorioRestringido
};

void seleccionarOrden(NimheP G, struct MejorPasada *mejor) {
    int ordenrand = rand() & 15;  // numero aleatorio
    if (ordenrand < 8) {
        mejor->ordenCG++;
        ChicoGrande(G);
    } else if (8 <= ordenrand && ordenrand < 13) {
        mejor->ordenR++;
        Revierte(G);
    } else if (13 <= ordenrand && ordenrand < 15) {
        mejor->ordenGC++;
        GrandeChico(G);
    } else if (15 == ordenrand) {
        mejor->ordenRAR++;
        ReordenAleatorioRestringido(G);
    }
}

u32* ordenAleatorio(NimheP G) {
    u32 n = NumeroDeVertices(G);  // limite para el orden generado
    u32 *x = calloc(n, sizeof(u32));  // para guardar el orden
    u32 pos;  // indice

    // generar un orden aleatorio
    for (u32 i = 1; i < n; i++) {
        // posicion random
        pos = rand()%(n-1) + 1;
        // guardar el valor a intercambiar
        u32 temp = x[i];
        // intercambiar 'i' con 'pos'
        // si 'pos' no fue inicializado, asignar el indice
        // sinó asignar con lo que contenia
        x[i] = (x[pos] == 0) ? pos : x[pos];
        // lo mismo pero al reves
        x[pos] = (temp == 0) ? i : temp;
    }
    // una vez mas para el cero
    pos = rand()%n;
    x[0] = x[pos];
    x[pos] = 0;
    return x;
}

int main(void) {
    NimheP G = NuevoNimhe();  // cargar grafo
    struct MejorPasada mejor; // datos sobre las corridas de greedy
    u32 colores;  // coloreo
    srand(time(NULL));  // inicializar rand()

    if (G == NULL) {
        printf("Error en formato de entrada\n");
        return 1;
    }
    // inicializar mejor
    mejor.semilla = NULL;
    mejor.coloreo = NumeroDeVertices(G) + 1;
    mejor.ordenCG = 0;
    mejor.ordenGC = 0;
    mejor.ordenR = 0;
    mejor.ordenRAR = 0;

    // correr dos colores
    colores = Chidos(G);
    if (colores) {
        printf("Grafo Bipartito\n");
        DestruirNimhe(G);
        return 0;
    }
    printf("Grafo No Bipartito\n");

    // correr 10 ordenes aleatorios
    for (int i = 0; i < 10; i++) {
        // generar orden aleatorio
        u32* x = ordenAleatorio(G);
        // ordenar
        OrdenEspecifico(G, x);
        // correr greedy
        colores = Greedy(G);
        printf("coloreo aleatorio numero %u: %u colores\n", i, colores);
        // actualizar mejor
        if (colores < mejor.coloreo) {
            mejor.coloreo = colores;
            // swap x con semilla
            u32 *temp = mejor.semilla;
            mejor.semilla = x;
            x = temp;
        }
        if (x != NULL)
            free(x);
    }
    // correr un WelshPowell
    OrdenWelshPowell(G);
    colores = Greedy(G);
    if (colores <= mejor.coloreo) {
        mejor.coloreo = colores;
        free(mejor.semilla);
        mejor.semilla = NULL;
    }
    printf("\n coloreo con Greedy WelshPowell:%u colores\n", colores);

    // es 3 colores
    if (mejor.coloreo == 3) {
        printf("X(G)=3\n");
        DestruirNimhe(G);
        return 0;
    }

    // retomar la mejor pasada
    // si semilla = NULL es porque fue la ultima
    if (mejor.semilla != NULL) {
        OrdenEspecifico(G, mejor.semilla);
        colores = Greedy(G);
        free(mejor.semilla);
    }

    printf("\n====Comenzando Greedy iterando 1001 veces====\n\n");
    // correr 1000 veces greedy
    for (int i = 0; i < 1000; ++i) {
        seleccionarOrden(G, &mejor);
        mejor.coloreo = Greedy(G);
    }

    // correr un Revierte
    Revierte(G);
    mejor.coloreo = Greedy(G);
    mejor.ordenR++;

    // resultados
    printf("Mejor colores con Greedy iterado 1001 veces: %u colores\n",
         mejor.coloreo);
    printf("(%u CG, %u GC, %u R, %u RAR)\n", mejor.ordenCG, mejor.ordenGC,
            mejor.ordenR, mejor.ordenRAR);

    DestruirNimhe(G);
    return 0;
}
