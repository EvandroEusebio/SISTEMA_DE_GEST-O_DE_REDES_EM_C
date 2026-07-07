#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "grafo.h"
#include "login.h"

void definirUtilizadorAutenticado(const char *usuario);
const char *obterUtilizadorAutenticado(void);

void garantirEstruturaPersistencia(void);

int carregarRede(Grafo *g);
int salvarRede(const Grafo *g);

void registarOperacao(const char *acao, const char *detalhe);
void terminarSessao(void);

void gerarRelatorioEstatisticoRede(const Grafo *g);
void gerarRelatorioEstadosDispositivos(const Grafo *g);
void menu_relatorios(Grafo *g);

#endif