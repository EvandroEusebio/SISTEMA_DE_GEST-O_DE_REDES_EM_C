#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>

/* ======================== Gestao do Grafo ======================== */

Grafo *criarGrafo()
{
    Grafo *g = (Grafo *)malloc(sizeof(Grafo));
    if (!g)
    {
        printf("Falha ao criar o GRAFO!\n");
        exit(1);
    }

    g->numVertices = 0;
    g->vertices = NULL;
    printf("GRAFO CRIADO COM SUCESSO!\n");
    return g;
}

void liberarGrafo(Grafo *g)
{
    if (g == NULL) return;

    for (int i = 0; i < g->numVertices; i++)
    {
        Adj *aux = g->vertices[i].listaAdj;

        while (aux != NULL)
        {
            Adj *temp = aux;
            aux = aux->prox;
            free(temp);
        }
    }
    free(g->vertices);
    free(g);
}

/* ======================== Vertices ======================== */

void adicionarDispositivo(Grafo *g)
{
    Dispositivo *d = criarDispositivo();
    if (d == NULL)
        return;

    /* Aumentar o array de vertices */
    g->numVertices++;
    g->vertices = (Vertice *)realloc(g->vertices, g->numVertices * sizeof(Vertice));

    if (!g->vertices)
    {
        printf("Erro ao alocar memoria para vertices.\n");
        exit(1);
    }
    g->vertices[g->numVertices - 1].dispositivo = *d;
    g->vertices[g->numVertices - 1].listaAdj = NULL;

    free(d);
    printf("Dispositivo adicionado ao grafo com sucesso!\n");
}


int procurarVertice(Grafo *g, int id)
{
    for (int i = 0; i < g->numVertices; i++)
    {
        if (g->vertices[i].dispositivo.id == id)
        {
            return i;
        }
    }
    return -1;
}

/* ======================== Arestas (Conexoes) ======================== */


void criarConexao(Grafo *g, int id1, int id2)
{
    int v1 = procurarVertice(g, id1);
    int v2 = procurarVertice(g, id2);

    if (v1 == -1 || v2 == -1)
    {
        printf("\nDispositivo nao encontrado.\n");
        return;
    }

    /* Aresta de v1 para v2 */
    Adj *novo = (Adj *)malloc(sizeof(Adj));
    novo->destino = v2;
    novo->prox = g->vertices[v1].listaAdj;
    g->vertices[v1].listaAdj = novo;

    /* Aresta de v2 para v1 (nao-direcionado) */
    novo = (Adj *)malloc(sizeof(Adj));
    novo->destino = v1;
    novo->prox = g->vertices[v2].listaAdj;
    g->vertices[v2].listaAdj = novo;

    printf("\nConexao criada com sucesso.\n");
}

void adicionarConexao(Grafo *g, int origem, int destino)
{
    /* Aresta de origem para destino */
    Adj *novo = (Adj *)malloc(sizeof(Adj));
    novo->destino = destino;
    novo->prox = g->vertices[origem].listaAdj;
    g->vertices[origem].listaAdj = novo;

    /* Aresta de destino para origem (nao-direcionado) */
    Adj *novo2 = (Adj *)malloc(sizeof(Adj));
    novo2->destino = origem;
    novo2->prox = g->vertices[destino].listaAdj;
    g->vertices[destino].listaAdj = novo2;
}

int existeConexao(Grafo *g, int id1, int id2)
{
    int v1 = procurarVertice(g, id1);
    int v2 = procurarVertice(g, id2);

    if (v1 == -1 || v2 == -1)
        return 0;

    Adj *aux = g->vertices[v1].listaAdj;

    while (aux != NULL)
    {
        if (aux->destino == v2)
            return 1;
        aux = aux->prox;
    }

    return 0;
}

/* ======================== Visualizacao ======================== */


void mostrarRede(Grafo *g)
{
    printf("\n===== TOPOLOGIA DA REDE =====\n\n");

    for (int i = 0; i < g->numVertices; i++)
    {
        printf("%d - %s -> ",
               i,
               g->vertices[i].dispositivo.nome);

        Adj *aux = g->vertices[i].listaAdj;

        while (aux != NULL)
        {
            printf("%s ",
                   g->vertices[aux->destino].dispositivo.nome);
            aux = aux->prox;
        }

        printf("\n");
    }
}

/* ======================== Menus ======================== */

void menuConexoes(Grafo *g)
{
    int op;
    int id1, id2;

    do
    {
        printf("\n===== CONEXOES =====\n");
        printf("1. Criar conexao (por ID)\n");
        printf("2. Adicionar conexao (por indice)\n");
        printf("3. Mostrar topologia\n");
        printf("0. Voltar\n");

        scanf("%d", &op);

        switch (op)
        {
        case 1:
            printf("\nID do primeiro dispositivo: ");
            scanf("%d", &id1);
            printf("ID do segundo dispositivo: ");
            scanf("%d", &id2);
            criarConexao(g, id1, id2);
            break;

        case 2:
            printf("\nIndice do primeiro vertice: ");
            scanf("%d", &id1);
            printf("Indice do segundo vertice: ");
            scanf("%d", &id2);
            adicionarConexao(g, id1, id2);
            break;

        case 3:
            mostrarRede(g);
            break;
        }

    } while (op != 0);
}
