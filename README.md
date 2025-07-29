# Datalogger de Movimento com IMU (MPU6050) e Raspberry Pi Pico


Este projeto consiste no desenvolvimento de um sistema embarcado portátil (datalogger) para captura de dados de movimento. Utilizando um Raspberry Pi Pico, o sistema lê continuamente os dados de um sensor IMU MPU6050 (acelerômetro e giroscópio de 3 eixos) e os armazena em um cartão MicroSD.

Cada sessão de gravação é salva em um arquivo .csv individual com um nome único contendo data e hora, garantindo a rastreabilidade dos dados. O dispositivo possui uma interface de usuário intuitiva com um display OLED para feedback de status, um LED RGB para indicações visuais rápidas e um buzzer para alertas sonoros. O controle é feito por botões físicos para iniciar/parar a gravação e gerenciar o cartão SD.

Para a análise dos dados, um script em Python foi desenvolvido para ler múltiplos arquivos de log, processá-los e gerar visualizações gráficas de forma automatizada.

##  Funcionamento

- Captura de Dados Inerciais: Leitura contínua de dados do acelerômetro (eixos X, Y, Z) e giroscópio (eixos X, Y, Z).

- Armazenamento Confiável: Gravação dos dados em formato .csv em um cartão MicroSD, utilizando a biblioteca FatFs.
Nomenclatura Dinâmica: Geração de arquivos com nomes únicos baseados em data e hora (log_DD-MM-AAAA_HH-MM-SS.csv), permitindo múltiplas sessões de gravação sem sobrescrever dados.

- Interface de Usuário Completa: Feedback em tempo real ao usuário através de:

 - Display OLED: Exibe o status atual do sistema (Inicializando, Pronto, Gravando, Salvo, Erro, etc.).

 - LED RGB: Indica o estado de operação com um código de cores intuitivo (Verde para pronto, Vermelho para gravando, Azul para salvando, etc.).

 - Buzzer: Emite alertas sonoros para confirmar ações importantes (início e fim de gravação).

- Controle por Botões Físicos: Operação total do dispositivo via botões para Iniciar/Parar a gravação e Montar/Desmontar o cartão SD com segurança.

- Análise de Dados Automatizada: Script em Python que localiza todos os arquivos de log em uma pasta, processa os dados e gera gráficos individuais para cada sessão de captura.

## Tecnologias e Ferramentas Utilizadas
# Hardware
- Microcontrolador: Raspberry Pi Pico

- Sensor: IMU MPU6050 (Acelerômetro + Giroscópio)

- Armazenamento: Módulo Leitor de Cartão MicroSD

- Display: OLED SSD1306 128x64 (I2C)

- Feedback Visual/Sonoro: LED RGB e Buzzer

- Controle: 2x Push Buttons

# Software & Firmware
- Linguagem (Firmware): C

- SDK: Raspberry Pi Pico SDK

- Bibliotecas C: FatFs (sistema de arquivos), ssd1306 (driver do display)

- Linguagem (Análise): Python 3

- Bibliotecas Python: Pandas, Matplotlib

- Ambiente de Desenvolvimento: Visual Studio Code