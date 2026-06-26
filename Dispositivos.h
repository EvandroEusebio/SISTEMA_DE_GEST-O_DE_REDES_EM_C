#ifndef DISPOSITIVO_H
#define DISPOSITIVO_H
#define MAX_DISPOSITIVOS 100

struct Grafo;

typedef struct {
    int id;
    char nome[50];
    char ip[20];
    char tipo[20];
    char estado[20];
} Dispositivo;

Dispositivo *criarDispositivo();
void listarDispositivos(struct Grafo *g);
int procurarDispositivoPorID(struct Grafo *g, int id);
int procurarDispositivoPorIP(struct Grafo *g, char ip[]);
int procurarDispositivoPorNome(struct Grafo *g, char nome[]);
void pesquisarPorID(struct Grafo *g);
void pesquisarPorIP(struct Grafo *g);
void pesquisarPorNome(struct Grafo *g);
void removerDispositivo(struct Grafo *g, int id);
void atualizarDispositivo(struct Grafo *g, int id);
void exibirDispositivo(Dispositivo d);
void menu_dispositivo(struct Grafo *rede);
void menu_pesquisar(struct Grafo *g);

#endif
