#include "grafo.h"
#include "persistencia.h"
#include "login.h"
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

    if (procurarDispositivoPorID(g, d->id) != -1)
    {
        printf("\nJa existe um dispositivo com esse ID.\n");
        free(d);
        return;
    }

    if (procurarDispositivoPorIP(g, d->ip) != -1)
    {
        printf("\nJa existe um dispositivo com esse IP.\n");
        free(d);
        return;
    }

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

        {
            char detalhe[256];
            snprintf(detalhe, sizeof(detalhe), "ID=%d | Nome=%s | IP=%s | Tipo=%s | Estado=%s",
                     d->id,
                     d->nome,
                     d->ip,
                     d->tipo,
                     d->estado);
            registarOperacao("INCLUSAO", detalhe);
        }

        salvarRede(g);

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


static int verificarConexao(Grafo *g, int origem, int destino)
{
    Adj *aux = g->vertices[origem].listaAdj;

    while (aux != NULL)
    {
        if (aux->destino == destino)
        {
            return 1;
        }
        aux = aux->prox;
    }

    return 0;
}

void criarConexao(Grafo *g, int id1, int id2)
{
    int v1 = procurarVertice(g, id1);
    int v2 = procurarVertice(g, id2);

    if (v1 == -1 || v2 == -1)
    {
        printf("\nDispositivo nao encontrado.\n");
        return;
    }

    if (v1 == v2)
    {
        printf("\nNao e permitido criar uma conexao com o mesmo dispositivo.\n");
        return;
    }

    if (verificarConexao(g, v1, v2))
    {
        printf("\nConexao ja existente.\n");
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

    {
        char detalhe[256];
        snprintf(detalhe, sizeof(detalhe), "ID1=%d | ID2=%d", id1, id2);
        registarOperacao("CONEXAO", detalhe);
    }

    salvarRede(g);

    printf("\nConexao criada com sucesso.\n");
}

void adicionarConexao(Grafo *g, int origem, int destino)
{
    if (g == NULL || origem < 0 || destino < 0 || origem >= g->numVertices || destino >= g->numVertices)
    {
        printf("\nIndices invalidos.\n");
        return;
    }

    if (origem == destino)
    {
        printf("\nNao e permitido criar uma conexao com o mesmo dispositivo.\n");
        return;
    }

    if (verificarConexao(g, origem, destino))
    {
        printf("\nConexao ja existente.\n");
        return;
    }

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

    {
        char detalhe[256];
        snprintf(detalhe, sizeof(detalhe), "ORIGEM=%d | DESTINO=%d",
                 g->vertices[origem].dispositivo.id,
                 g->vertices[destino].dispositivo.id);
        registarOperacao("CONEXAO", detalhe);
    }

    salvarRede(g);
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
        printf("%d - %s [%s] -> ",
               i,
               g->vertices[i].dispositivo.nome,
               g->vertices[i].dispositivo.tipo );

        Adj *aux = g->vertices[i].listaAdj;

        while (aux != NULL)
        {
            printf("%s [%s] -> ",
                   g->vertices[aux->destino].dispositivo.nome,
                   g->vertices[aux->destino].dispositivo.tipo);
            aux = aux->prox;
        }

        printf("\n");
    }
}

/* ======================== Caminhos ======================== */

void caminhoMaisCurto(Grafo *g, int idOrigem, int idDestino)
{
    if (g == NULL || g->numVertices == 0)
    {
        printf("\nGrafo vazio.\n");
        return;
    }

    int origem = procurarVertice(g, idOrigem);
    int destino = procurarVertice(g, idDestino);

    if (origem == -1 || destino == -1)
    {
        printf("\nDispositivo nao encontrado.\n");
        return;
    }

    int n = g->numVertices;
    int *visitado = (int *)calloc(n, sizeof(int));
    int *anterior = (int *)malloc(n * sizeof(int));
    int *fila = (int *)malloc(n * sizeof(int));

    if (visitado == NULL || anterior == NULL || fila == NULL)
    {
        printf("\nErro ao alocar memoria.\n");
        free(visitado);
        free(anterior);
        free(fila);
        return;
    }

    for (int i = 0; i < n; i++)
    {
        anterior[i] = -1;
    }

    int inicioFila = 0;
    int fimFila = 0;

    visitado[origem] = 1;
    fila[fimFila++] = origem;

    /* Busca em largura (BFS): garante o menor numero de saltos */
    while (inicioFila < fimFila)
    {
        int atual = fila[inicioFila++];

        if (atual == destino)
        {
            break;
        }

        Adj *aux = g->vertices[atual].listaAdj;
        while (aux != NULL)
        {
            if (!visitado[aux->destino])
            {
                visitado[aux->destino] = 1;
                anterior[aux->destino] = atual;
                fila[fimFila++] = aux->destino;
            }
            aux = aux->prox;
        }
    }

    if (!visitado[destino])
    {
        printf("\nNao existe rota entre os dispositivos %d e %d.\n", idOrigem, idDestino);
    }
    else
    {
        int *caminho = (int *)malloc(n * sizeof(int));
        int tamanho = 0;

        if (caminho == NULL)
        {
            printf("\nErro ao alocar memoria.\n");
        }
        else
        {
            int atual = destino;
            while (atual != -1)
            {
                caminho[tamanho++] = atual;
                atual = anterior[atual];
            }

            printf("\n===== ROTA MAIS CURTA =====\n");
            printf("Numero de saltos: %d\n", tamanho - 1);
            printf("Caminho: ");
            for (int i = tamanho - 1; i >= 0; i--)
            {
                printf("%s (ID %d)", g->vertices[caminho[i]].dispositivo.nome,
                       g->vertices[caminho[i]].dispositivo.id);
                if (i > 0)
                {
                    printf(" -> ");
                }
            }
            printf("\n");

            free(caminho);
        }
    }

    free(visitado);
    free(anterior);
    free(fila);
}

/* ======================== Menus ======================== */

void menuConexoes(Grafo *g, int tipoUsuario)
{
    int op;
    int id1, id2;

    do
    {
        printf("\n===== CONEXOES =====\n");
        printf("1. Criar conexao (por ID)\n");
        printf("2. Adicionar conexao (por indice)\n");
        printf("3. Mostrar topologia\n");
        printf("4. Encontrar rota mais curta (por ID)\n");
        printf("0. Voltar\n");

        scanf("%d", &op);

        switch (op)
        {
        case 1:
            if (tipoUsuario == PERFIL_VISITANTE)
            {
                printf("\nAcesso negado. O perfil Visitante nao pode criar conexoes.\n");
                break;
            }
            printf("\nID do primeiro dispositivo: ");
            scanf("%d", &id1);
            printf("ID do segundo dispositivo: ");
            scanf("%d", &id2);
            criarConexao(g, id1, id2);
            break;

        case 2:
            if (tipoUsuario == PERFIL_VISITANTE)
            {
                printf("\nAcesso negado. O perfil Visitante nao pode criar conexoes.\n");
                break;
            }
            printf("\nIndice do primeiro vertice: ");
            scanf("%d", &id1);
            printf("Indice do segundo vertice: ");
            scanf("%d", &id2);
            adicionarConexao(g, id1, id2);
            break;

        case 3:
            mostrarRede(g);
            break;

        case 4:
            printf("\nID do dispositivo de origem: ");
            scanf("%d", &id1);
            printf("ID do dispositivo de destino: ");
            scanf("%d", &id2);
            caminhoMaisCurto(g, id1, id2);
            break;
        }

    } while (op != 0);
}
