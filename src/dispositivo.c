/**
 * @file dispositivo.c
 * @brief Implementacao do modulo de Dispositivos de Rede
 *
 * Contem todas as funcoes para gestao de dispositivos:
 * criacao, listagem, pesquisa, atualizacao, remocao e menus.
 *
 * Os dados sao armazenados diretamente no grafo (vertices).
 */

#include "dispositivo.h"
#include "grafo.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ======================== Funcoes Auxiliares ======================== */

/**
 * @brief Valida se uma string esta no formato de IP valido
 *
 * Verifica se o IP tem 4 octetos (0-255) separados por pontos.
 *
 * @param ip String com o IP a validar
 * @return 1 se valido, 0 caso contrario
 */
static int validarIP(char ip[])
{
    int a, b, c, d;
    char extra;

    if (sscanf(ip, "%d.%d.%d.%d%c",
               &a, &b, &c, &d, &extra) != 4)
    {
        return 0;
    }

    if (a < 0 || a > 255) return 0;
    if (b < 0 || b > 255) return 0;
    if (c < 0 || c > 255) return 0;
    if (d < 0 || d > 255) return 0;

    return 1;
}

/* ======================== Pesquisa de Dispositivos ======================== */

/**
 * @brief Procura um dispositivo pelo ID dentro do grafo
 */
int procurarDispositivoPorID(struct Grafo *g, int id)
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

/**
 * @brief Procura um dispositivo pelo nome dentro do grafo
 */
int procurarDispositivoPorNome(struct Grafo *g, char nome[])
{
    for (int i = 0; i < g->numVertices; i++)
    {
        if (strcmp(g->vertices[i].dispositivo.nome, nome) == 0)
        {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Procura um dispositivo pelo IP dentro do grafo
 */
int procurarDispositivoPorIP(struct Grafo *g, char ip[])
{
    for (int i = 0; i < g->numVertices; i++)
    {
        if (strcmp(g->vertices[i].dispositivo.ip, ip) == 0)
        {
            return i;
        }
    }
    return -1;
}

/* ======================== Criacao de Dispositivo ======================== */

/**
 * @brief Cria um novo dispositivo a partir da entrada do utilizador
 *
 * Pede todos os dados ao utilizador, valida-os e retorna
 * um ponteiro para o dispositivo criado. O chamador deve
 * libertar a memoria apos usar.
 */
Dispositivo *criarDispositivo()
{
    Dispositivo *novo = (Dispositivo *)malloc(sizeof(Dispositivo));
    if (!novo)
    {
        printf("\nErro ao alocar memoria.\n");
        return NULL;
    }

    /* Pedir ID */
    printf("\nID: ");
    scanf("%d", &novo->id);

    /* Pedir nome */
    printf("Nome: ");
    scanf(" %[^\n]", novo->nome);

    /* Pedir e validar IP */
    do
    {
        printf("IP no formato (x.x.x.x): ");
        scanf("%s", novo->ip);

        if (!validarIP(novo->ip))
        {
            printf("\nIP com formato invalido.\n\n");
        }

    } while (!validarIP(novo->ip));

    /* Pedir tipo */
    int tipo_escolha;
    do
    {
        printf("\nTipo: ");
        printf("\n1-Computador\n2-Servidor\n3-Router\n4-Switch\n5-Impressora\n");
        scanf("%d", &tipo_escolha);
        switch (tipo_escolha)
        {
        case 1: strcpy(novo->tipo, "Computador"); break;
        case 2: strcpy(novo->tipo, "Servidor"); break;
        case 3: strcpy(novo->tipo, "Router"); break;
        case 4: strcpy(novo->tipo, "Switch"); break;
        case 5: strcpy(novo->tipo, "Impressora"); break;
        default: printf("Opcao invalida.\n");
        }
    } while (tipo_escolha < 1 || tipo_escolha > 5);

    /* Pedir estado */
    int estado;
    do
    {
        printf("Estado: ");
        printf("\n1-Ativado\n2-Desativado\n3-Manutencao\n");
        scanf("%d", &estado);
        switch (estado)
        {
        case 1: strcpy(novo->estado, "Ativado"); break;
        case 2: strcpy(novo->estado, "Desativado"); break;
        case 3: strcpy(novo->estado, "Manutencao"); break;
        default: printf("Estado invalido.\n");
        }
    } while (estado < 1 || estado > 3);

    printf("\nDispositivo Criado!\n");
    return novo;
}

/* ======================== Exibicao de Dispositivos ======================== */

/**
 * @brief Exibe os dados completos de um dispositivo
 */
void exibirDispositivo(Dispositivo d)
{
    printf("\n=== DISPOSITIVO ENCONTRADO ===\n");
    printf("ID: %d\n", d.id);
    printf("Nome: %s\n", d.nome);
    printf("IP: %s\n", d.ip);
    printf("Tipo: %s\n", d.tipo);
    printf("Estado: %s\n", d.estado);
    printf("==============================\n");
}

/**
 * @brief Lista todos os dispositivos registados no grafo
 */
void listarDispositivos(struct Grafo *g)
{
    if (g->numVertices == 0)
    {
        printf("\nNenhum dispositivo cadastrado.\n");
        return;
    }

    printf("\n===== LISTA DE DISPOSITIVOS =====\n");
    for (int i = 0; i < g->numVertices; i++)
    {
        printf("\nID: %d\n", g->vertices[i].dispositivo.id);
        printf("Nome: %s\n", g->vertices[i].dispositivo.nome);
        printf("IP: %s\n", g->vertices[i].dispositivo.ip);
        printf("Tipo: %s\n", g->vertices[i].dispositivo.tipo);
        printf("Estado: %s\n", g->vertices[i].dispositivo.estado);
        printf("==============================\n");
    }
}

/* ======================== Pesquisa via Menu ======================== */

/**
 * @brief Pesquisa e exibe um dispositivo por ID
 *
 * Pede o ID ao utilizador, procura no grafo e exibe o resultado.
 */
void pesquisarPorID(struct Grafo *g)
{
    if (g->numVertices == 0)
    {
        printf("\nNenhum dispositivo cadastrado.\n");
        return;
    }

    int id;
    printf("\nDigite o ID: ");
    scanf("%d", &id);

    int pos = procurarDispositivoPorID(g, id);
    if (pos == -1)
    {
        printf("\nDispositivo ainda nao cadastrado.\n");
        return;
    }

    exibirDispositivo(g->vertices[pos].dispositivo);
}

/**
 * @brief Pesquisa e exibe um dispositivo por IP
 *
 * Pede o IP ao utilizador, procura no grafo e exibe o resultado.
 */
void pesquisarPorIP(struct Grafo *g)
{
    char ip[20];
    printf("\nDigite o IP: ");
    scanf("%s", ip);

    int pos = procurarDispositivoPorIP(g, ip);
    if (pos == -1)
    {
        printf("\nIP errado ou dispositivo mau configurado.\n");
        return;
    }

    exibirDispositivo(g->vertices[pos].dispositivo);
}

/**
 * @brief Pesquisa e exibe um dispositivo por nome
 *
 * Pede o nome ao utilizador, procura no grafo e exibe o resultado.
 */
void pesquisarPorNome(struct Grafo *g)
{
    char nome[50];
    printf("\nDigite o nome: ");
    scanf(" %[^\n]", nome);

    int pos = procurarDispositivoPorNome(g, nome);
    if (pos == -1)
    {
        printf("\nDispositivo ainda nao cadastrado.\n");
        return;
    }

    exibirDispositivo(g->vertices[pos].dispositivo);
}

/* ======================== Atualizacao e Remocao ======================== */

/**
 * @brief Atualiza os dados de um dispositivo existente
 *
 * Permite alterar nome, IP e tipo. O estado nao e alterado.
 */
void atualizarDispositivo(struct Grafo *g, int id)
{
    int pos = procurarDispositivoPorID(g, id);

    if (pos == -1)
    {
        printf("\nDispositivo ainda nao cadastrado.\n");
        return;
    }

    /* Atualizar nome */
    printf("Novo nome: ");
    scanf(" %[^\n]", g->vertices[pos].dispositivo.nome);

    /* Atualizar e validar IP */
    do
    {
        printf("Novo IP: ");
        scanf("%s", g->vertices[pos].dispositivo.ip);

        if (!validarIP(g->vertices[pos].dispositivo.ip))
        {
            printf("\nIP com formato invalido.\n\n");
        }

    } while (!validarIP(g->vertices[pos].dispositivo.ip));

    /* Atualizar tipo */
    printf("Novo tipo: ");
    scanf(" %[^\n]", g->vertices[pos].dispositivo.tipo);

    printf("\nDispositivo atualizado com sucesso.\n");
}

/**
 * @brief Remove um dispositivo do grafo
 *
 * Remove o vertice e desloca os vertices seguintes.
 * Tambem realoca o array para libertar memoria.
 */
void removerDispositivo(struct Grafo *g, int id)
{
    int pos = procurarDispositivoPorID(g, id);

    if (pos == -1)
    {
        printf("\nDispositivo ainda nao cadastrado.\n");
        return;
    }

    /* Deslocar vertices para preencher o espaco */
    for (int i = pos; i < g->numVertices - 1; i++)
    {
        g->vertices[i] = g->vertices[i + 1];
    }

    g->numVertices--;

    /* Realocar array para libertar memoria */
    if (g->numVertices > 0)
    {
        g->vertices = (Vertice *)realloc(g->vertices, g->numVertices * sizeof(Vertice));
    }
    else
    {
        free(g->vertices);
        g->vertices = NULL;
    }

    printf("\nDispositivo removido com sucesso.\n");
}

/* ======================== Menus ======================== */

/**
 * @brief Menu de pesquisa de dispositivos
 *
 * Apresenta opcoes: pesquisar por nome, ID ou IP.
 */
void menu_pesquisar(struct Grafo *g)
{
    if (g->numVertices == 0)
    {
        printf("\nNenhum dispositivo cadastrado.\n");
        return;
    }

    int opcao;
    do
    {
        printf("\n===== PESQUISA DE DISPOSITIVOS =====\n");
        printf("1. Pesquisar por Nome\n");
        printf("2. Pesquisar por ID\n");
        printf("3. Pesquisar por IP\n");
        printf("0. Voltar\n");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1: pesquisarPorNome(g); break;
        case 2: pesquisarPorID(g); break;
        case 3: pesquisarPorIP(g); break;
        case 0: break;
        default: printf("\nPesquisa invalida.\n");
        }
    } while (opcao != 0);
}

/**
 * @brief Menu principal de gestao de dispositivos
 *
 * Apresenta o menu principal com todas as operacoes disponiveis.
 */
void menu_dispositivo(struct Grafo *rede)
{
    int op, id;

    do
    {
        printf("\n==========GESTAO DO DISPOSITIVO==========\n");
        printf("1. Adicionar dispositivo\n");
        printf("2. Listar dispositivos\n");
        printf("3. Pesquisar dispositivo\n");
        printf("4. Atualizar dispositivo\n");
        printf("5. Remover dispositivo\n");
        printf("6. Gerir conexoes\n");
        printf("0. Sair\n");
        printf("\n============================================\n");
        scanf("%d", &op);

        switch (op)
        {
        case 1:
            adicionarDispositivo(rede);
            break;
        case 2:
            listarDispositivos(rede);
            break;
        case 3:
            menu_pesquisar(rede);
            break;
        case 4:
            if (rede->numVertices == 0)
            {
                printf("\nNenhum dispositivo cadastrado.\n");
                break;
            }
            printf("\nInsira o ID do dispositivo: ");
            scanf("%d", &id);
            atualizarDispositivo(rede, id);
            break;
        case 5:
            if (rede->numVertices == 0)
            {
                printf("\nNenhum dispositivo cadastrado.\n");
                break;
            }
            printf("\nInsira o ID do dispositivo: ");
            scanf("%d", &id);
            removerDispositivo(rede, id);
            break;
        case 6:
            menuConexoes(rede);
            break;
        case 0:
            break;
        default:
            printf("\nOpcao invalida");
        }
    } while (op != 0);
}
