import gdown
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import warnings

# evitando avisos para uma saída limpa
warnings.simplefilter(action='ignore', category=FutureWarning)

# fazendo download
url = 'https://drive.google.com/file/d/1V8hRpD_W7vuCh_0wAHE7ickXuMRMfAqU/view?usp=drive_link'
output_csv = 'resultados_ordenacao.csv'

print(f"Baixando o arquivo de dados de '{url}'...")
gdown.download(url=url, output=output_csv, quiet=False, fuzzy=True)
print(f"Arquivo salvo como '{output_csv}'.")

def plotar_graficos_essenciais(df):
    """Gera os gráficos Tempo vs. Tamanho, Comparações e Trocas."""
    print("\n--- Gerando Gráficos Essenciais ---")

    #  tempo vs. tamanho 
    sns.set_theme(style="whitegrid", palette="viridis")
    for tipo_dado in df['Tipo de Dado'].unique():
        plt.figure(figsize=(12, 8))
        subset = df[df['Tipo de Dado'] == tipo_dado]
        ax = sns.lineplot(data=subset, x='Tamanho', y='Tempo (ms)', hue='Algoritmo', style='Algoritmo', markers=True, linewidth=2.5)
        ax.set_yscale('log')
        plt.title(f'Tempo de Execução vs. Tamanho do Vetor ({tipo_dado})', fontsize=16)
        plt.ylabel('Tempo (ms) - Escala Logarítmica', fontsize=12)
        plt.savefig(f'grafico_tempo_vs_tamanho_{tipo_dado.lower().replace(" ", "_")}.png', dpi=300)
        plt.show()

    df_maior_tamanho = df[df['Tamanho'] == 10000]

    # comparações por algoritmo 
    plt.figure(figsize=(14, 8))
    sns.barplot(data=df_maior_tamanho, x='Algoritmo', y='Comparacoes', hue='Tipo de Dado', palette='plasma')
    plt.title('Número de Comparações por Algoritmo (N=10000)', fontsize=16)
    plt.ylabel('Comparações (em dezenas de milhões)', fontsize=12)
    plt.xticks(rotation=45, ha='right')
    plt.tight_layout()
    plt.savefig('grafico_comparacoes_N10000.png', dpi=300)
    plt.show()

    # trocas por algoritmo
    plt.figure(figsize=(14, 8))
    ax = sns.barplot(data=df_maior_tamanho, x='Algoritmo', y='Trocas', hue='Tipo de Dado', palette='magma')
    ax.set_yscale('log')
    plt.title('Número de Trocas por Algoritmo (N=10000)', fontsize=16)
    plt.ylabel('Trocas (Escala Logarítmica)', fontsize=12)
    plt.xticks(rotation=45, ha='right')
    plt.tight_layout()
    plt.savefig('grafico_trocas_N10000.png', dpi=300)
    plt.show()

def plotar_graficos_adicionais(df):
    """Gera os gráficos Tempo Final, Eficiência e Fator de Lentidão."""
    print("\n--- Gerando Gráficos Adicionais ---")

    sns.set_theme(style="whitegrid", palette="inferno")
    tamanho_maximo = 10000
    df_maior_tamanho = df[df['Tamanho'] == tamanho_maximo].copy()

    # comparação direta de tempo ---
    plt.figure(figsize=(14, 8))
    ax = sns.barplot(data=df_maior_tamanho, x='Algoritmo', y='Tempo (ms)', hue='Tipo de Dado')
    ax.set_yscale('log')
    plt.title(f'Tempo de Execução por Algoritmo (N={tamanho_maximo})', fontsize=16)
    plt.xticks(rotation=45, ha='right')
    plt.savefig('grafico_tempo_final_N10000.png', dpi=300)
    plt.show()

    # correlação trocas vs. comparações 
    plt.figure(figsize=(12, 8))
    sns.scatterplot(data=df_maior_tamanho, x='Comparacoes', y='Trocas', hue='Algoritmo', style='Tipo de Dado', s=200)
    plt.xscale('log')
    plt.yscale('log')
    plt.title(f'Eficiência Intrínseca: Trocas vs. Comparações (N={tamanho_maximo})', fontsize=16)
    plt.xlabel('Comparações (Escala Logarítmica)', fontsize=12)
    plt.ylabel('Trocas (Escala Logarítmica)', fontsize=12)
    plt.legend(title='Legenda', bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.tight_layout()
    plt.savefig('grafico_trocas_vs_comparacoes.png', dpi=300)
    plt.show()

    # desempenho normalizado (fator de lentidão) 
    min_tempos = df_maior_tamanho.groupby('Tipo de Dado')['Tempo (ms)'].transform('min')
    df_maior_tamanho['Fator Lentidao'] = df_maior_tamanho['Tempo (ms)'] / min_tempos

    plt.figure(figsize=(14, 8))
    ax = sns.barplot(data=df_maior_tamanho, x='Algoritmo', y='Fator Lentidao', hue='Tipo de Dado', palette='rocket')
    ax.set_yscale('log')
    ax.axhline(1, ls='--', color='black') 
    plt.title(f'Desempenho Normalizado (Fator de Lentidão vs. o Mais Rápido) (N={tamanho_maximo})', fontsize=16)
    plt.ylabel('Vezes Mais Lento (Escala Logarítmica)', fontsize=12)
    plt.xticks(rotation=45, ha='right')
    plt.tight_layout()
    plt.savefig('grafico_fator_lentidao.png', dpi=300)
    plt.show()

def main():
    """
    Função principal que carrega os dados e chama as funções de plotagem.
    """
    try:
        df_resultados = pd.read_csv(output_csv)
        print("\nArquivo de dados carregado com sucesso. Iniciando geração de gráficos...")

        plotar_graficos_essenciais(df_resultados)
        plotar_graficos_adicionais(df_resultados)

        print("\nProcesso de visualização de dados concluído com sucesso!")

    except FileNotFoundError:
        print(f"Erro: O arquivo '{output_csv}' não foi encontrado após a tentativa de download.")
        print("Verifique a URL e suas permissões de compartilhamento no Google Drive.")
    except Exception as e:
        print(f"Ocorreu um erro inesperado: {e}")

# executando a função principal!!!
if __name__ == '__main__':
    main()
