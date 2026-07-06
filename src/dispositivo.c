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
#include "persistencia.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ======================== Funcoes Auxiliares ======================== */

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

static void lerLinhaTexto(char *buffer, size_t tamanho)
{
    int caractere;

    while ((caractere = getchar()) != '\n' && caractere != EOF)
    {
    }

    if (fgets(buffer, tamanho, stdin) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
    else if (tamanho > 0)
    {
        buffer[0] = '\0';
    }
}

/* ======================== Pesquisa de Dispositivos ======================== */

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

Dispositivo *criarDispositivo()
{
    Dispositivo *novo = (Dispositivo *)malloc(sizeof(Dispositivo));
    if (!novo)
    {
        printf("\nErro ao alocar memoria.\n");
        return NULL;
    }

    printf("\nID: ");
    scanf("%d", &novo->id);

    printf("Nome: ");
    lerLinhaTexto(novo->nome, sizeof(novo->nome));

    do
    {
        printf("IP no formato (x.x.x.x): ");
        scanf("%s", novo->ip);

        if (!validarIP(novo->ip))
        {
            printf("\nIP com formato invalido.\n\n");
        }

    } while (!validarIP(novo->ip));

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

void pesquisarPorNome(struct Grafo *g)
{
    char nome[50];
    printf("\nDigite o nome: ");
    lerLinhaTexto(nome, sizeof(nome));

    int pos = procurarDispositivoPorNome(g, nome);
    if (pos == -1)
    {
        printf("\nDispositivo ainda nao cadastrado.\n");
        return;
    }

    exibirDispositivo(g->vertices[pos].dispositivo);
}

/* ======================== Atualizacao e Remocao ======================== */

void atualizarDispositivo(struct Grafo *g, int id)
{
    int pos = procurarDispositivoPorID(g, id);
    char detalhe[256];

    if (pos == -1)
    {
        printf("\nDispositivo ainda nao cadastrado.\n");
        return;
    }

    printf("Novo nome: ");
    lerLinhaTexto(g->vertices[pos].dispositivo.nome, sizeof(g->vertices[pos].dispositivo.nome));

    do
    {
        printf("Novo IP: ");
        scanf("%s", g->vertices[pos].dispositivo.ip);

        if (!validarIP(g->vertices[pos].dispositivo.ip))
        {
            printf("\nIP com formato invalido.\n\n");
        }

    } while (!validarIP(g->vertices[pos].dispositivo.ip));

    for (int i = 0; i < g->numVertices; i++)
    {
        if (i != pos && strcmp(g->vertices[i].dispositivo.ip, g->vertices[pos].dispositivo.ip) == 0)
        {
            printf("\nJa existe um dispositivo com esse IP.\n");
            return;
        }
    }

    int tipo;
    do
    {
        printf("Novo tipo: ");
        printf("\n1-Computador\n2-Servidor\n3-Router\n4-Switch\n5-Impressora\n");
        scanf("%d", &tipo);
        switch (tipo)
        {
        case 1: strcpy(g->vertices[pos].dispositivo.tipo, "Computador"); break;
        case 2: strcpy(g->vertices[pos].dispositivo.tipo, "Servidor"); break;
        case 3: strcpy(g->vertices[pos].dispositivo.tipo, "Router"); break;
        case 4: strcpy(g->vertices[pos].dispositivo.tipo, "Switch"); break;
        case 5: strcpy(g->vertices[pos].dispositivo.tipo, "Impressora"); break;
        default: printf("Opcao invalida.\n");
        }
    } while (tipo < 1 || tipo > 5);

    int estado;
    do
    {
        printf("Novo estado: ");
        printf("\n1-Ativado\n2-Desativado\n3-Manutencao\n");
        scanf("%d", &estado);
        switch (estado)
        {
        case 1: strcpy(g->vertices[pos].dispositivo.estado, "Ativado"); break;
        case 2: strcpy(g->vertices[pos].dispositivo.estado, "Desativado"); break;
        case 3: strcpy(g->vertices[pos].dispositivo.estado, "Manutencao"); break;
        default: printf("Estado invalido.\n");
        }
    } while (estado < 1 || estado > 3);

    snprintf(detalhe, sizeof(detalhe), "ID=%d | Nome=%s | IP=%s | Tipo=%s | Estado=%s",
             g->vertices[pos].dispositivo.id,
             g->vertices[pos].dispositivo.nome,
             g->vertices[pos].dispositivo.ip,
             g->vertices[pos].dispositivo.tipo,
             g->vertices[pos].dispositivo.estado);

    registarOperacao("ATUALIZACAO", detalhe);
    salvarRede(g);

    printf("\nDispositivo atualizado com sucesso.\n");
}

void removerDispositivo(struct Grafo *g, int id)
{
    int pos = procurarDispositivoPorID(g, id);
    Dispositivo removido;
    char detalhe[256];

    if (pos == -1)
    {
        printf("\nDispositivo ainda nao cadastrado.\n");
        return;
    }

    removido = g->vertices[pos].dispositivo;

    for (int i = 0; i < g->numVertices; i++)
    {
        if (i == pos)
        {
            continue;
        }

        Adj *anterior = NULL;
        Adj *atual = g->vertices[i].listaAdj;

        while (atual != NULL)
        {
            if (atual->destino == pos)
            {
                Adj *remover = atual;

                if (anterior == NULL)
                {
                    g->vertices[i].listaAdj = atual->prox;
                }
                else
                {
                    anterior->prox = atual->prox;
                }

                atual = atual->prox;
                free(remover);
                continue;
            }

            if (atual->destino > pos)
            {
                atual->destino--;
            }

            anterior = atual;
            atual = atual->prox;
        }
    }

    while (g->vertices[pos].listaAdj != NULL)
    {
        Adj *temp = g->vertices[pos].listaAdj;
        g->vertices[pos].listaAdj = temp->prox;
        free(temp);
    }

    for (int i = pos; i < g->numVertices - 1; i++)
    {
        g->vertices[i] = g->vertices[i + 1];
    }

    g->numVertices--;

    if (g->numVertices > 0)
    {
        g->vertices = (Vertice *)realloc(g->vertices, g->numVertices * sizeof(Vertice));
    }
    else
    {
        free(g->vertices);
        g->vertices = NULL;
    }

    snprintf(detalhe, sizeof(detalhe), "ID=%d | Nome=%s | IP=%s | Tipo=%s | Estado=%s",
             removido.id,
             removido.nome,
             removido.ip,
             removido.tipo,
             removido.estado);

    registarOperacao("REMOCAO", detalhe);
    salvarRede(g);

    printf("\nDispositivo removido com sucesso.\n");
}

/* ======================== Menus ======================== */

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

int menu_dispositivo(struct Grafo *rede)
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
        printf("7. Relatorios\n");
        printf("8. Logout\n");
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
        case 7:
            menu_relatorios(rede);
            break;
        case 8:
            return 1;
        case 0:
            return 0;
        default:
            printf("\nOpcao invalida");
        }
    } while (op != 0);

    return 0;
}
