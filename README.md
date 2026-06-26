# Sistema de Gestão de Redes em C

Sistema de gestão de topologia de redes, desenvolvido em C como exercício prático de **grafos** e **estruturas de dados**.

## Funcionalidades

- **Gestão de dispositivos** — criar, listar, pesquisar (por nome, ID ou IP), atualizar e remover dispositivos de rede
- **Gestão de conexões** — criar ligações entre dispositivos e visualizar a topologia da rede
- **Validação de IP** — validação automática de endereços IPv4 no formato x.x.x.x
- **Tipos de dispositivo** — Computador, Servidor, Router, Switch, Impressora
- **Estados** — Ativado, Desativado, Manutenção

## Estrutura do Projeto

```
├── include/
│   ├── dispositivo.h    # Struct Dispositivo e declarações de funções
│   └── grafo.h          # Structs Grafo, Vertice, Adj e declarações de funções
├── src/
│   ├── main.c           # Ponto de entrada
│   ├── dispositivo.c    # Implementação da gestão de dispositivos
│   └── grafo.c          # Implementação do grafo (vértices e arestas)
├── output/              # Executável gerado
└── Makefile
```

## Estruturas de Dados

- **Grafo** — array dinâmico de vértices com listas de adjacência
- **Vertice** — contém um `Dispositivo` e uma lista ligada de adjacências
- **Adj** — nó da lista de adjacência (aresta não-direcionada)

## Como Compilar e Executar

```bash
make          # Compilar o projeto
make run      # Compilar e executar
make clean    # Limpar ficheiros gerados
```

## Requisitos

- GCC
- Make
- Linux / Unix (ou WSL no Windows)

## Licença

MIT License — Copyright (c) 2026 Evandro Eusébio, Paulo Gaspar, Fernando Alves