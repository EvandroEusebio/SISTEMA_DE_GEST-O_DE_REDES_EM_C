#ifndef GRAFO_H
#define GRAFO_H
#include "Dispositivos.h"

//LISTA DE ADJACENCIA
typedef struct Adj {
    int destino;
    struct Adj *prox;
} Adj;

//   VERTICE
typedef struct {
    Dispositivo dispositivo;
    Adj *listaAdj;
} Vertice;

//GRAFO
typedef struct Grafo {
    int numVertices;
    Vertice *vertices;
} Grafo;

Grafo *criarGrafo();
void adicionarDispositivo(Grafo *g);
void adicionarConexao(Grafo *g, int origem, int destino);
void criarConexao(Grafo *g, int id1, int id2);
void mostrarRede(Grafo *g);
void menuConexoes(Grafo *g);
void liberarGrafo(Grafo *g);
int procurarVertice(Grafo *g, int id);
int existeConexao(Grafo *g, int id1, int id2);

#endif
