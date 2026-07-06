# Sistema de Gestão de Redes em C

Sistema de gestão de topologia de redes, desenvolvido em C como exercício prático de **grafos** e **estruturas de dados**.

## Funcionalidades

- **Autenticação de utilizador** — sistema de login com validação de credenciais através de ficheiro
- **Gestão de dispositivos** — criar, listar, pesquisar (por nome, ID ou IP), atualizar e remover dispositivos de rede
- **Gestão de conexões** — criar ligações entre dispositivos e visualizar a topologia da rede
- **Validação de IP** — validação automática de endereços IPv4 no formato x.x.x.x
- **Tipos de dispositivo** — Computador, Servidor, Router, Switch, Impressora
- **Estados** — Ativado, Desativado, Manutenção
- **Persistência automática** — dados da rede carregados no arranque e guardados após alterações
- **Sistema de logs** — registo de inclusão, atualização, remoção e conexões
- **Relatórios** — estatísticas da rede e dispositivos por estado

## Estrutura do Projeto

```
├── include/
│   ├── dispositivo.h    # Struct Dispositivo e declarações de funções
│   ├── grafo.h          # Structs Grafo, Vertice, Adj e declarações de funções
│   └── login.h          # Struct User e declaração da função de login
├── src/
│   ├── main.c           # Ponto de entrada (login + menu principal)
│   ├── dispositivo.c    # Implementação da gestão de dispositivos
│   ├── grafo.c          # Implementação do grafo (vértices e arestas)
│   ├── login.c          # Autenticação de utilizador
│   └── persistencia.c   # Persistência, logs e relatórios
├── data/                # Ficheiros gerados em execução
├── docs/                # Documentação complementar
├── usuarios.txt         # Ficheiro de credenciais (user senha tipo)
├── output/              # Executável gerado
└── Makefile
```

## Estruturas de Dados

- **Grafo** — array dinâmico de vértices com listas de adjacência
- **Vertice** — contém um `Dispositivo` e uma lista ligada de adjacências
- **Adj** — nó da lista de adjacência (aresta não-direcionada)
- **User** — dados de autenticação: utilizador, senha e tipo de perfil

## Sistema de Login

O utilizador deve autenticar-se antes de aceder ao sistema. As credenciais são armazenadas no ficheiro `usuarios.txt` com o formato:

```
username senha tipo
```

Credencial padrão: `admin` / `1234` (tipo 1)

## Persistência e Relatórios

Ao iniciar, o programa carrega automaticamente os dados de `data/dispositivos.txt` e `data/conexoes.txt`.

Qualquer inclusão, atualização, remoção ou criação de conexão é gravada de forma automática e registada em `data/logs/operacoes.log`.

Os relatórios são gerados pelo menu interno de relatórios e também guardados em `data/relatorios/`:

- `relatorio_estatistico.txt`
- `relatorio_estados.txt`

Os ficheiros usam formato texto separado por `|`, o que facilita leitura manual e manutenção.

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