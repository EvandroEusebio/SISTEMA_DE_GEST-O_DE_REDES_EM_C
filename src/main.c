#include "grafo.h"
#include "login.h"

int main()
{
    User user;

    /* Realizar o login do usuário */
    if (login(&user) != 0)
    {
        printf("Falha no login. Encerrando o programa.\n");
        return 1;
    }
    else
    {
        printf("Bem-vindo, %s!\n", user.usuario);
        /* Criar e inicializar o grafo da rede */
        Grafo *rede = criarGrafo();
        /* Apresentar o menu principal */
        menu_dispositivo(rede);

        liberarGrafo(rede);
    }

    return 0;
}
