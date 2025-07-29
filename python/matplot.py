import pandas as pd
import matplotlib.pyplot as plt
import os
import glob

# --- Configurações ---
PASTA_DE_LOGS = 'logs'
PASTA_OUTPUT = 'output'
TAXA_DE_AMOSTRAGEM_HZ = 50

# --- Início do Script ---

print(f"--- Analisador de Dados do Datalogger ---")

# --- CONSTRUÇÃO ROBUSTA DOS CAMINHOS (A PARTE QUE FALTAVA) ---
# 1. Encontra o diretório absoluto onde o próprio script está salvo
diretorio_script = os.path.dirname(os.path.abspath(__file__))

# 2. Cria os caminhos absolutos para as pastas de logs e output, baseados na localização do script
caminho_absoluto_logs = os.path.join(diretorio_script, PASTA_DE_LOGS)
caminho_absoluto_output = os.path.join(diretorio_script, PASTA_OUTPUT)

# 3. Cria a pasta de output usando o caminho absoluto, se ela não existir
os.makedirs(caminho_absoluto_output, exist_ok=True)
print(f"Gráficos serão salvos na pasta: '{caminho_absoluto_output}'")

# 4. Procura os arquivos CSV no caminho absoluto
print(f"Procurando arquivos .csv na pasta: '{caminho_absoluto_logs}'...")
padrao_busca = os.path.join(caminho_absoluto_logs, '*.csv')
lista_de_arquivos = glob.glob(padrao_busca)

# Verifica se algum arquivo foi encontrado
if not lista_de_arquivos:
    print(f"\n[ERRO] Nenhum arquivo .csv encontrado no caminho esperado!")
    print("Verifique se a pasta 'logs' realmente existe ao lado do seu script e contém os arquivos.")
    exit()

print(f"Encontrados {len(lista_de_arquivos)} arquivos de log. Gerando gráficos individuais...")

# --- Loop Principal: Processa cada arquivo individualmente ---
for arquivo_csv in lista_de_arquivos:
    print(f"\nProcessando arquivo: {arquivo_csv}...")
    
    df = pd.read_csv(arquivo_csv)
    df['tempo_s'] = df['numero_amostra'] / TAXA_DE_AMOSTRAGEM_HZ
    
    nome_base = os.path.basename(arquivo_csv)
    nome_sem_extensao = os.path.splitext(nome_base)[0]
    
    # --- Gráfico de Aceleração ---
    plt.style.use('seaborn-v0_8-whitegrid')
    plt.figure(figsize=(12, 6))
    plt.plot(df['tempo_s'], df['accel_x'], label='Aceleração X')
    plt.plot(df['tempo_s'], df['accel_y'], label='Aceleração Y')
    plt.plot(df['tempo_s'], df['accel_z'], label='Aceleração Z')
    plt.title(f'Acelerômetro - {nome_base}', fontsize=16)
    plt.xlabel('Tempo (s)', fontsize=12)
    plt.ylabel('Valor Bruto do Sensor', fontsize=12)
    plt.legend()
    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    plt.tight_layout()
    
    nome_arquivo_acel = f"acel_{nome_sem_extensao}.png"
    caminho_salvar_acel = os.path.join(caminho_absoluto_output, nome_arquivo_acel)
    plt.savefig(caminho_salvar_acel)
    plt.close()
    print(f" -> Gráfico de acelerômetro salvo como: '{caminho_salvar_acel}'")

    # --- Gráfico de Giroscópio ---
    plt.figure(figsize=(12, 6))
    plt.plot(df['tempo_s'], df['giro_x'], label='Giroscópio X')
    plt.plot(df['tempo_s'], df['giro_y'], label='Giroscópio Y')
    plt.plot(df['tempo_s'], df['giro_z'], label='Giroscópio Z')
    plt.title(f'Giroscópio - {nome_base}', fontsize=16)
    plt.xlabel('Tempo (s)', fontsize=12)
    plt.ylabel('Valor Bruto do Sensor', fontsize=12)
    plt.legend()
    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    plt.tight_layout()

    nome_arquivo_giro = f"giro_{nome_sem_extensao}.png"
    caminho_salvar_giro = os.path.join(caminho_absoluto_output, nome_arquivo_giro)
    plt.savefig(caminho_salvar_giro)
    plt.close()
    print(f" -> Gráfico de giroscópio salvo como: '{caminho_salvar_giro}'")

print("\n--- Análise concluída! ---")