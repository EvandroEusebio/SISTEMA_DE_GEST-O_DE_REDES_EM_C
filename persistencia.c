#include "persistencia.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(_WIN32)
#include <direct.h>
#define CRIAR_DIRETORIO(caminho) _mkdir(caminho)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define CRIAR_DIRETORIO(caminho) mkdir(caminho, 0777)
#endif

#define DIRETORIO_DADOS "data"
#define DIRETORIO_LOGS "data/logs"
#define DIRETORIO_RELATORIOS "data/relatorios"
#define FICHEIRO_DISPOSITIVOS "data/dispositivos.txt"
#define FICHEIRO_CONEXOES "data/conexoes.txt"
#define FICHEIRO_LOGS "data/logs/operacoes.log"
#define FICHEIRO_RELATORIO_ESTATS "data/relatorios/relatorio_estatistico.txt"
#define FICHEIRO_RELATORIO_ESTADOS "data/relatorios/relatorio_estados.txt"

static char utilizador_autenticado[MAX_USERNAME] = "sistema";

static void criarDiretorioSeNecessario(const char *diretorio)
{
    if (CRIAR_DIRETORIO(diretorio) == -1 && errno != EEXIST)
    {
        printf("Nao foi possivel criar o diretorio %s\n", diretorio);
    }
}

static void formatarDataHora(char *buffer, size_t tamanho)
{
    time_t agora = time(NULL);
    struct tm *tm_local = localtime(&agora);

    if (tm_local == NULL)
    {
        snprintf(buffer, tamanho, "data-desconhecida");
        return;
    }

    strftime(buffer, tamanho, "%Y-%m-%d %H:%M:%S", tm_local);
}

static void escreverCabecalhoRelatorio(FILE *saida, const char *titulo)
{
    char data_hora[32];
    formatarDataHora(data_hora, sizeof(data_hora));

    fprintf(saida, "\n==================================================\n");
    fprintf(saida, "%s\n", titulo);
    fprintf(saida, "Utilizador: %s\n", utilizador_autenticado);
    fprintf(saida, "Gerado em : %s\n", data_hora);
    fprintf(saida, "==================================================\n\n");
}

static void escreverDispositivo(FILE *saida, const Dispositivo *dispositivo)
{
    fprintf(saida, "ID: %d | Nome: %s | IP: %s | Tipo: %s | Estado: %s\n",
            dispositivo->id,
            dispositivo->nome,
            dispositivo->ip,
            dispositivo->tipo,
            dispositivo->estado);
}

static void adicionarVerticeCarregado(Grafo *g, const Dispositivo *dispositivo)
{
    Vertice *novos_vertices = (Vertice *)realloc(g->vertices, (g->numVertices + 1) * sizeof(Vertice));
    if (novos_vertices == NULL)
    {
        printf("Erro ao carregar dispositivos.\n");
        exit(1);
    }

    g->vertices = novos_vertices;
    g->vertices[g->numVertices].dispositivo = *dispositivo;
    g->vertices[g->numVertices].listaAdj = NULL;
    g->numVertices++;
}

static void adicionarConexaoSilenciosa(Grafo *g, int origem, int destino)
{
    if (origem < 0 || destino < 0 || origem >= g->numVertices || destino >= g->numVertices)
    {
        return;
    }

    Adj *novo = (Adj *)malloc(sizeof(Adj));
    if (novo == NULL)
    {
        return;
    }

    novo->destino = destino;
    novo->prox = g->vertices[origem].listaAdj;
    g->vertices[origem].listaAdj = novo;

    novo = (Adj *)malloc(sizeof(Adj));
    if (novo == NULL)
    {
        return;
    }

    novo->destino = origem;
    novo->prox = g->vertices[destino].listaAdj;
    g->vertices[destino].listaAdj = novo;
}

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

static void escreverRelatorioEstatistico(FILE *saida, const Grafo *g)
{
    int totalConexoes = 0;
    int totalAtivados = 0;
    int totalDesativados = 0;
    int totalManutencao = 0;
    int totalComputadores = 0;
    int totalServidores = 0;
    int totalRouters = 0;
    int totalSwitches = 0;
    int totalImpressoras = 0;

    escreverCabecalhoRelatorio(saida, "RELATORIO ESTATISTICO DA REDE");

    for (int i = 0; i < g->numVertices; i++)
    {
        const Dispositivo *dispositivo = &g->vertices[i].dispositivo;

        if (strcmp(dispositivo->estado, "Ativado") == 0)
        {
            totalAtivados++;
        }
        else if (strcmp(dispositivo->estado, "Desativado") == 0)
        {
            totalDesativados++;
        }
        else if (strcmp(dispositivo->estado, "Manutencao") == 0)
        {
            totalManutencao++;
        }

        if (strcmp(dispositivo->tipo, "Computador") == 0)
        {
            totalComputadores++;
        }
        else if (strcmp(dispositivo->tipo, "Servidor") == 0)
        {
            totalServidores++;
        }
        else if (strcmp(dispositivo->tipo, "Router") == 0)
        {
            totalRouters++;
        }
        else if (strcmp(dispositivo->tipo, "Switch") == 0)
        {
            totalSwitches++;
        }
        else if (strcmp(dispositivo->tipo, "Impressora") == 0)
        {
            totalImpressoras++;
        }

        Adj *adjacencia = g->vertices[i].listaAdj;
        while (adjacencia != NULL)
        {
            if (adjacencia->destino > i)
            {
                totalConexoes++;
            }
            adjacencia = adjacencia->prox;
        }
    }

    fprintf(saida, "Total de dispositivos : %d\n", g->numVertices);
    fprintf(saida, "Total de conexoes     : %d\n\n", totalConexoes);

    fprintf(saida, "Por estado:\n");
    fprintf(saida, "- Ativados   : %d\n", totalAtivados);
    fprintf(saida, "- Desativados: %d\n", totalDesativados);
    fprintf(saida, "- Manutencao : %d\n\n", totalManutencao);

    fprintf(saida, "Por tipo:\n");
    fprintf(saida, "- Computadores: %d\n", totalComputadores);
    fprintf(saida, "- Servidores  : %d\n", totalServidores);
    fprintf(saida, "- Routers     : %d\n", totalRouters);
    fprintf(saida, "- Switches    : %d\n", totalSwitches);
    fprintf(saida, "- Impressoras : %d\n", totalImpressoras);

    fprintf(saida, "\nDispositivos registados:\n");
    for (int i = 0; i < g->numVertices; i++)
    {
        escreverDispositivo(saida, &g->vertices[i].dispositivo);
    }

    fprintf(saida, "\nFim do relatorio estatistico.\n");
}

static void escreverRelatorioEstados(FILE *saida, const Grafo *g)
{
    escreverCabecalhoRelatorio(saida, "RELATORIO DE ESTADOS DOS DISPOSITIVOS");

    fprintf(saida, "ATIVADOS\n");
    for (int i = 0; i < g->numVertices; i++)
    {
        if (strcmp(g->vertices[i].dispositivo.estado, "Ativado") == 0)
        {
            escreverDispositivo(saida, &g->vertices[i].dispositivo);
        }
    }

    fprintf(saida, "\nDESATIVADOS\n");
    for (int i = 0; i < g->numVertices; i++)
    {
        if (strcmp(g->vertices[i].dispositivo.estado, "Desativado") == 0)
        {
            escreverDispositivo(saida, &g->vertices[i].dispositivo);
        }
    }

    fprintf(saida, "\nEM MANUTENCAO\n");
    for (int i = 0; i < g->numVertices; i++)
    {
        if (strcmp(g->vertices[i].dispositivo.estado, "Manutencao") == 0)
        {
            escreverDispositivo(saida, &g->vertices[i].dispositivo);
        }
    }

    fprintf(saida, "\nFim do relatorio de estados.\n");
}

void definirUtilizadorAutenticado(const char *usuario)
{
    if (usuario == NULL || usuario[0] == '\0')
    {
        strcpy(utilizador_autenticado, "sistema");
        return;
    }

    strncpy(utilizador_autenticado, usuario, MAX_USERNAME - 1);
    utilizador_autenticado[MAX_USERNAME - 1] = '\0';
}

const char *obterUtilizadorAutenticado(void)
{
    return utilizador_autenticado;
}

void garantirEstruturaPersistencia(void)
{
    criarDiretorioSeNecessario(DIRETORIO_DADOS);
    criarDiretorioSeNecessario(DIRETORIO_LOGS);
    criarDiretorioSeNecessario(DIRETORIO_RELATORIOS);
}

int carregarRede(Grafo *g)
{
    FILE *ficheiro;
    char linha[512];
    int carregados = 0;

    if (g == NULL)
    {
        return 0;
    }

    garantirEstruturaPersistencia();

    ficheiro = fopen(FICHEIRO_DISPOSITIVOS, "r");
    if (ficheiro != NULL)
    {
        while (fgets(linha, sizeof(linha), ficheiro) != NULL)
        {
            Dispositivo dispositivo;

            if (sscanf(linha, "%d|%49[^|]|%19[^|]|%19[^|]|%19[^\n]",
                       &dispositivo.id,
                       dispositivo.nome,
                       dispositivo.ip,
                       dispositivo.tipo,
                       dispositivo.estado) == 5)
            {
                adicionarVerticeCarregado(g, &dispositivo);
                carregados++;
            }
        }

        fclose(ficheiro);
    }

    ficheiro = fopen(FICHEIRO_CONEXOES, "r");
    if (ficheiro != NULL)
    {
        while (fgets(linha, sizeof(linha), ficheiro) != NULL)
        {
            int id_origem;
            int id_destino;
            int origem;
            int destino;

            if (sscanf(linha, "%d|%d", &id_origem, &id_destino) == 2)
            {
                origem = procurarVertice(g, id_origem);
                destino = procurarVertice(g, id_destino);

                if (origem != -1 && destino != -1 && !verificarConexao(g, origem, destino))
                {
                    adicionarConexaoSilenciosa(g, origem, destino);
                }
            }
        }

        fclose(ficheiro);
    }

    return carregados;
}

int salvarRede(const Grafo *g)
{
    FILE *ficheiro;

    if (g == NULL)
    {
        return 1;
    }

    garantirEstruturaPersistencia();

    ficheiro = fopen(FICHEIRO_DISPOSITIVOS, "w");
    if (ficheiro == NULL)
    {
        return 1;
    }

    for (int i = 0; i < g->numVertices; i++)
    {
        fprintf(ficheiro, "%d|%s|%s|%s|%s\n",
                g->vertices[i].dispositivo.id,
                g->vertices[i].dispositivo.nome,
                g->vertices[i].dispositivo.ip,
                g->vertices[i].dispositivo.tipo,
                g->vertices[i].dispositivo.estado);
    }

    fclose(ficheiro);

    ficheiro = fopen(FICHEIRO_CONEXOES, "w");
    if (ficheiro == NULL)
    {
        return 1;
    }

    for (int i = 0; i < g->numVertices; i++)
    {
        Adj *adjacencia = g->vertices[i].listaAdj;

        while (adjacencia != NULL)
        {
            if (adjacencia->destino > i)
            {
                fprintf(ficheiro, "%d|%d\n",
                        g->vertices[i].dispositivo.id,
                        g->vertices[adjacencia->destino].dispositivo.id);
            }

            adjacencia = adjacencia->prox;
        }
    }

    fclose(ficheiro);
    return 0;
}

void registarOperacao(const char *acao, const char *detalhe)
{
    FILE *ficheiro;
    char data_hora[32];

    garantirEstruturaPersistencia();

    ficheiro = fopen(FICHEIRO_LOGS, "a");
    if (ficheiro == NULL)
    {
        return;
    }

    formatarDataHora(data_hora, sizeof(data_hora));
    fprintf(ficheiro, "%s | %s | %s | %s\n",
            data_hora,
            utilizador_autenticado,
            acao,
            detalhe != NULL ? detalhe : "sem detalhes");

    fclose(ficheiro);
}

void terminarSessao(void)
{
    registarOperacao("LOGOUT", "Sessao terminada");
    definirUtilizadorAutenticado(NULL);
}

void gerarRelatorioEstatisticoRede(const Grafo *g)
{
    FILE *ficheiro;

    garantirEstruturaPersistencia();
    escreverRelatorioEstatistico(stdout, g);

    ficheiro = fopen(FICHEIRO_RELATORIO_ESTATS, "w");
    if (ficheiro == NULL)
    {
        return;
    }

    escreverRelatorioEstatistico(ficheiro, g);
    fclose(ficheiro);
}

void gerarRelatorioEstadosDispositivos(const Grafo *g)
{
    FILE *ficheiro;

    garantirEstruturaPersistencia();
    escreverRelatorioEstados(stdout, g);

    ficheiro = fopen(FICHEIRO_RELATORIO_ESTADOS, "w");
    if (ficheiro == NULL)
    {
        return;
    }

    escreverRelatorioEstados(ficheiro, g);
    fclose(ficheiro);
}

void menu_relatorios(Grafo *g)
{
    int opcao;

    do
    {
        printf("\n===== RELATORIOS =====\n");
        printf("1. Relatorio estatistico da rede\n");
        printf("2. Relatorio de estados dos dispositivos\n");
        printf("0. Voltar\n");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            gerarRelatorioEstatisticoRede(g);
            break;
        case 2:
            gerarRelatorioEstadosDispositivos(g);
            break;
        case 0:
            break;
        default:
            printf("Opcao invalida.\n");
        }
    } while (opcao != 0);
}