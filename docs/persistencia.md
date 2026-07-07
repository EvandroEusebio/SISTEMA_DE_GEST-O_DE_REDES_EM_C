# Módulo de Persistência

Este projeto guarda os dados da rede em ficheiros de texto simples para permitir carregamento automático, registo de alterações e geração de relatórios.

## Ficheiros utilizados

- `data/dispositivos.txt` - lista de dispositivos, um por linha
- `data/conexoes.txt` - lista de conexões entre IDs de dispositivos
- `data/logs/operacoes.log` - histórico de operações executadas
- `data/relatorios/relatorio_estatistico.txt` - relatório estatístico da rede
- `data/relatorios/relatorio_estados.txt` - relatório por estado dos dispositivos

## Formato dos dados

Dispositivos:

```text
id|nome|ip|tipo|estado
```

Conexões:

```text
id_origem|id_destino
```

Logs:

```text
YYYY-MM-DD HH:MM:SS | utilizador | acao | detalhe
```

## Ciclo de funcionamento

1. O utilizador autentica-se no sistema.
2. O programa carrega dispositivos e conexões do disco.
3. Qualquer inclusão, atualização, remoção ou criação de conexão grava imediatamente o estado atualizado.
4. Cada operação relevante é registada no ficheiro de logs.
5. Os relatórios podem ser gerados a partir do menu principal e também ficam guardados em ficheiro.

## Observação técnica

As conexões são guardadas apenas uma vez por par de dispositivos, embora o grafo use lista de adjacência nos dois sentidos.