#include "grafo.h"
int main()
{
    /* Criar e inicializar o grafo da rede */
    Grafo *rede = criarGrafo();

    /* Apresentar o menu principal */
    menu_dispositivo(rede);

    liberarGrafo(rede);

    return 0;
}
