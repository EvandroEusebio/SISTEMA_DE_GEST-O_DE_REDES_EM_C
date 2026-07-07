#include "login.h"
#include "persistencia.h"

int login(User *user)
{
    FILE *arquivo;
    char nome_usuario[MAX_USERNAME];
    char senha[MAX_SENHA];

    printf("SEJA BEM VINDO AO SISTEMA!\n");
    printf("NOME: ");
    scanf("%s", nome_usuario);
    printf("SENHA: ");
    scanf("%s", senha);

    // abre o arquivo de usuarios para leitura
    arquivo = fopen("usuarios.txt", "r");
    if(!arquivo)
    {
        printf("Erro ao abrir o Arquivo.\n");
        return (1);
    }

    while (fscanf(arquivo, "%s %s %d", user->usuario, user->senha, &user->tipo) == 3)
    {
        if (strcmp(user->usuario, nome_usuario) == 0 && strcmp(user->senha, senha) == 0)
        {
            definirUtilizadorAutenticado(user->usuario);
            printf("Login realizado com sucesso!\n");
            fclose(arquivo);
            return (0);
        }
    }

    printf("Usuário ou senha incorretos!\n");
    fclose(arquivo);
    return (1);
}

const char *nomePerfil(int tipo)
{
    switch (tipo)
    {
    case PERFIL_ADMINISTRADOR:
        return "Administrador";
    case PERFIL_TECNICO:
        return "Tecnico";
    case PERFIL_VISITANTE:
        return "Visitante";
    default:
        return "Desconhecido";
    }
}