#ifndef LOGIN_H
#define LOGIN_H

#define MAX_USERNAME 50
#define MAX_SENHA 50
#include <stdio.h>
#include <string.h>


typedef struct {
    char usuario[MAX_USERNAME];
    char senha[MAX_SENHA];
    int tipo;
} User;


int login(User *user);


#endif 