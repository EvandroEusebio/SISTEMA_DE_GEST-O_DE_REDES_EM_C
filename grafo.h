
#ifndef GRAFO_H
#define GRAFO_H

#include "dispositivo.h"



/**
 * Estrutura de adjacencia (aresta)
 *
 * Cada no da lista de adjacencia representa uma conexao
 * entre dois dispositivos. Forma uma lista ligada.
 */
typedef struct Adj {
    int destino;          /**< Indice do vertice de destino */
    struct Adj *prox;    
} Adj;

/**
 * Estrutura de vertice
 *
 * Cada vertice contem um dispositivo de rede e a lista
 * de adjacencias (dispositivos aos quais esta conectado).
 */
typedef struct {
    Dispositivo dispositivo;  /**< Dados do dispositivo de rede */
    Adj *listaAdj;            /**< Lista ligada de adjacencias */
} Vertice;

/**
 * Estrutura do Grafo
 *
 * Representa a topologia completa da rede.
 * Usa um array dinamico de vertices e listas de adjacencia.
 */
typedef struct Grafo {
    int numVertices;      /**< Numero atual de vertices no grafo */
    Vertice *vertices;    /**< Array dinamico de vertices */
} Grafo;

/* ======================== Gestao do Grafo ======================== */


Grafo *criarGrafo();

void liberarGrafo(Grafo *g);

/* ======================== Vertices ======================== */

void adicionarDispositivo(Grafo *g);

int procurarVertice(Grafo *g, int id);

/* ======================== Arestas (Conexoes) ======================== */

/**
 * Cria uma conexao entre dois dispositivos (por ID)
 *
 * Procura os dispositivos pelos seus IDs e cria uma conexao
 * nao-direcionada entre eles (aresta nos dois sentidos).
 *
 * g Ponteiro para o grafo
 * id1 ID do primeiro dispositivo
 * id2 ID do segundo dispositivo
 */
void criarConexao(Grafo *g, int id1, int id2);

/**
 * Adiciona uma conexao por indice dos vertices
 *
 * Cria uma conexao diretamente entre dois vertices
 * usando os seus indices no array.
 *
 * g Ponteiro para o grafo
 * origem Indice do vertice de origem
 * destino Indice do vertice de destino
 */
void adicionarConexao(Grafo *g, int origem, int destino);

/**
 * Verifica se existe conexao entre dois dispositivos
 *
 * Procura se ha uma aresta que liga os dois dispositivos.
 *
 * g Ponteiro para o grafo
 * id1 ID do primeiro dispositivo
 * id2 ID do segundo dispositivo
 * 1 se existe conexao, 0 caso contrario
 */
int existeConexao(Grafo *g, int id1, int id2);

/* ======================== Visualizacao ======================== */

/**
 * Mostra a topologia completa da rede
 *
 * Imprime a lista de adjacencia de cada vertice,
 * mostrando a estrutura completa do grafo.
 *
 * g Ponteiro para o grafo
 */
void mostrarRede(Grafo *g);

/* ======================== Caminhos ======================== */

/**
 * Encontra e apresenta a rota mais curta entre dois dispositivos
 *
 * Usa uma busca em largura (BFS) para encontrar o caminho com o
 * menor numero de saltos entre o dispositivo de origem e o de
 * destino (o grafo nao tem pesos nas arestas, por isso "mais curta"
 * significa "com menos conexoes intermedias").
 *
 * g Ponteiro para o grafo
 * idOrigem ID do dispositivo de origem
 * idDestino ID do dispositivo de destino
 */
void caminhoMaisCurto(Grafo *g, int idOrigem, int idDestino);

/* ======================== Menus ======================== */

/**
 * Menu de gestao de conexoes
 *
 * Apresenta opcoes para criar conexoes e visualizar a topologia.
 *
 * g Ponteiro para o grafo
 * tipoUsuario Perfil do utilizador autenticado (ver PERFIL_* em login.h)
 */
void menuConexoes(Grafo *g, int tipoUsuario);

#endif
