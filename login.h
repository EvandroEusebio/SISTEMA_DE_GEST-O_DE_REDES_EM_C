#ifndef LOGIN_H
#define LOGIN_H

#define MAX_USERNAME 50
#define MAX_SENHA 50
#include <stdio.h>
#include <string.h>

/* Perfis de utilizador (campo "tipo" em usuarios.txt) */
#define PERFIL_ADMINISTRADOR 1
#define PERFIL_TECNICO       2
#define PERFIL_VISITANTE     3

typedef struct {
    char usuario[MAX_USERNAME];
    char senha[MAX_SENHA];
    int tipo;
} User;


int login(User *user);

/**
 * Devolve o nome do perfil correspondente ao tipo de utilizador
 *
 * tipo Valor do campo "tipo" do utilizador (1, 2 ou 3)
 * Nome do perfil ("Administrador", "Tecnico", "Visitante" ou "Desconhecido")
 */
const char *nomePerfil(int tipo);

#endif 