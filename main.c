#include "grafo.h"
#include "login.h"
#include "persistencia.h"

int main()
{
    User user;

    while (1)
    {
        /* Realizar o login do usuário */
        if (login(&user) != 0)
        {
            printf("Falha no login. Encerrando o programa.\n");
            return 1;
        }

        printf("Bem-vindo, %s! (Perfil: %s)\n", user.usuario, nomePerfil(user.tipo));

        /* Criar e inicializar o grafo da rede */
        Grafo *rede = criarGrafo();
        carregarRede(rede);

        /* Apresentar o menu principal */
        int fazerLogout = menu_dispositivo(rede, user.tipo);

        salvarRede(rede);
        liberarGrafo(rede);

        if (fazerLogout)
        {
            terminarSessao();
            printf("Sessao terminada.\n");
            continue;
        }

        break;
    }

    return 0;
}
