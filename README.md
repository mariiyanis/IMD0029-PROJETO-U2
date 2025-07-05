# Análise Comparativa de Algoritmos de Ordenação

Este repositório contém o código-fonte e os resultados do trabalho prático da Unidade II para a disciplina IMD0029 - Estruturas de Dados Básicas I. O objetivo do projeto é realizar uma análise experimental e comparativa do desempenho de cinco algoritmos de ordenação (Bubble Sort, Insertion Sort, Merge Sort, Selection Sort e Merge Sort).

# Funcionalidades
O projeto é dividido em duas partes principais:
## Programa em C++ (main.cpp) implementando os algoritmos Bubble Sort, Insertion Sort, Selection Sort, Merge Sort e Quick Sort;
- Coleta 3 métricas de desempenho: Tempo de Execução (ms), Número de Comparações e Número de Trocas;
- Testa os algoritmos em 3 tipos de conjuntos de dados: Aleatórios, Quase Ordenados e Inversamente Ordenados;
- Gera um arquivo resultados_ordenacao.csv com todos os dados brutos coletados.

## Programa em Python (gerar_graficos.py):
- Lê o arquivo resultados_ordenacao.csv.
- Utiliza as bibliotecas Pandas, Matplotlib e Seaborn para gerar visualizações gráficas.
- Cria um dashboard consolidado e gráficos individuais para analisar a performance dos algoritmos.
- Salva os gráficos gerados como arquivos .png.

# Tecnologias Utilizadas
- C++;
- Python: pandas, matplotlib e seaborn.
