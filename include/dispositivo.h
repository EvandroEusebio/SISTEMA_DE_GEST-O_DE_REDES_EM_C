#ifndef DISPOSITIVO_H
#define DISPOSITIVO_H

struct Grafo;

typedef struct
{
    int id;
    char nome[50];
    char ip[20];
    char tipo[20];
    char estado[20];
} Dispositivo;

Dispositivo *criarDispositivo();
void exibirDispositivo(Dispositivo d);
void listarDispositivos(struct Grafo *g);

int procurarDispositivoPorID(struct Grafo *g, int id);

int procurarDispositivoPorNome(struct Grafo *g, char nome[]);

int procurarDispositivoPorIP(struct Grafo *g, char ip[]);

void pesquisarPorID(struct Grafo *g);

void pesquisarPorIP(struct Grafo *g);

void pesquisarPorNome(struct Grafo *g);

void atualizarDispositivo(struct Grafo *g, int id);

void removerDispositivo(struct Grafo *g, int id);

void menu_pesquisar(struct Grafo *g);

int menu_dispositivo(struct Grafo *rede);

#endif
