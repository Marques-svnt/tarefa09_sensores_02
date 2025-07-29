#include <stdio.h>
#include <string.h>

// Includes módulos de hardware
#include "lib/imu_driver.h"
#include "lib/sd_manager.h"
#include "lib/ui_manager.h"
#include "lib/buzzer.h"

// Includes do Pico SDK
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/gpio.h"
#include "hardware/rtc.h"

// --- Pinos dos Botões ---
#define BOTAO_START_STOP_PIN 5 // Botão A
#define BOTAO_MOUNT_PIN 6      // Botão B

// --- Máquina de Estados ---
typedef enum
{
    ESTADO_INICIALIZANDO,
    ESTADO_AGUARDANDO,
    ESTADO_GRAVANDO,
    ESTADO_SD_DESMONTADO,
    ESTADO_ERRO
} SystemState;

// --- Variáveis Globais ---
volatile SystemState estado_atual = ESTADO_INICIALIZANDO;
volatile bool botao_start_stop_pressionado = false;
volatile bool botao_mount_pressionado = false;

FIL fil; // Objeto de arquivo global para o datalog

void set_rtc_from_compile_time()
{

    char date_str[] = __DATE__;
    char time_str[] = __TIME__;

    int day, year, month, hour, min, sec;
    char month_str[4];

    // Parse da string de data
    sscanf(date_str, "%s %d %d", month_str, &day, &year);

    // Converte o nome do mês para número
    if (strcmp(month_str, "Jan") == 0)
        month = 1;
    else if (strcmp(month_str, "Feb") == 0)
        month = 2;
    else if (strcmp(month_str, "Mar") == 0)
        month = 3;
    else if (strcmp(month_str, "Apr") == 0)
        month = 4;
    else if (strcmp(month_str, "May") == 0)
        month = 5;
    else if (strcmp(month_str, "Jun") == 0)
        month = 6;
    else if (strcmp(month_str, "Jul") == 0)
        month = 7;
    else if (strcmp(month_str, "Aug") == 0)
        month = 8;
    else if (strcmp(month_str, "Sep") == 0)
        month = 9;
    else if (strcmp(month_str, "Oct") == 0)
        month = 10;
    else if (strcmp(month_str, "Nov") == 0)
        month = 11;
    else if (strcmp(month_str, "Dec") == 0)
        month = 12;

    // Parse da string de hora
    sscanf(time_str, "%d:%d:%d", &hour, &min, &sec);

    datetime_t t = {
        .year = (int16_t)year,
        .month = (int8_t)month,
        .day = (int8_t)day,
        .dotw = 0,
        .hour = (int8_t)hour,
        .min = (int8_t)min,
        .sec = (int8_t)sec};

    // Ajusta o RTC do Pico
    rtc_set_datetime(&t);
    printf("INFO: RTC ajustado para a hora da compilação: %s %s\n", __DATE__, __TIME__);
}

// --- Tratamento de Interrupção dos Botões ---
void gpio_irq_handler(uint gpio, uint32_t events)
{
    // Lógica de Debounce simples para evitar múltiplas leituras
    static uint32_t last_irq_time = 0;
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    if (current_time - last_irq_time < 250)
    { // Ignora interrupções em menos de 250ms
        return;
    }
    last_irq_time = current_time;

    if (gpio == BOTAO_START_STOP_PIN)
    {
        botao_start_stop_pressionado = true;
    }
    else if (gpio == BOTAO_MOUNT_PIN)
    {
        botao_mount_pressionado = true;
    }
}

// --- Função Principal ---
int main()
{
    stdio_init_all();
    sleep_ms(2000); // Um tempo para o terminal serial conectar
    printf("INFO: Iniciando Datalogger...\n");

    // Inicializa todos os módulos de hardware
    ui_init();
    imu_init();
    sd_init();

    set_rtc_from_compile_time();

    // Configuração dos Botões com Pull-up interno
    gpio_init(BOTAO_START_STOP_PIN);
    gpio_set_dir(BOTAO_START_STOP_PIN, GPIO_IN);
    gpio_pull_up(BOTAO_START_STOP_PIN);

    gpio_init(BOTAO_MOUNT_PIN);
    gpio_set_dir(BOTAO_MOUNT_PIN, GPIO_IN);
    gpio_pull_up(BOTAO_MOUNT_PIN);

    gpio_set_irq_enabled_with_callback(BOTAO_START_STOP_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BOTAO_MOUNT_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    unsigned int contador_amostras = 0;
    const char *NOME_ARQUIVO = "datalog.csv";

    // Loop infinito da aplicação
    while (1)
    {
        switch (estado_atual)
        {

        case ESTADO_INICIALIZANDO:
            ui_set_led_state(UI_LED_INIT);
            ui_update_display("Datalogger", "Montando SD...");

            if (sd_mount())
            {
                estado_atual = ESTADO_AGUARDANDO;
            }
            else
            {
                estado_atual = ESTADO_ERRO;
            }
            break;

        case ESTADO_AGUARDANDO:
            ui_set_led_state(UI_LED_READY);
            ui_update_display("Pronto", "B1: GRAVAR");

            if (botao_start_stop_pressionado)
            {
                botao_start_stop_pressionado = false;

                // --- LÓGICA DE NOME DE ARQUIVO DINÂMICO ---
                char nome_arquivo_dinamico[30];
                datetime_t t_now;

                // Pega a data e hora atuais do RTC
                rtc_get_datetime(&t_now);

                // Formata a string do nome do arquivo
                sprintf(nome_arquivo_dinamico, "log_%02d-%02d-%d_%02d-%02d-%02d.csv",
                        t_now.day, t_now.month, t_now.year,
                        t_now.hour, t_now.min, t_now.sec);

                printf("INFO: Nome do arquivo de log: %s\n", nome_arquivo_dinamico);

                ui_beep(1);

                if (sd_open_file(&fil, nome_arquivo_dinamico))
                { // Usa o novo nome
                    contador_amostras = 0;
                    if (f_size(&fil) == 0)
                    {
                        sd_write_line(&fil, "numero_amostra,accel_x,accel_y,accel_z,giro_x,giro_y,giro_z");
                    }
                    estado_atual = ESTADO_GRAVANDO;
                }
                else
                {
                    estado_atual = ESTADO_ERRO;
                }
            }
            if (botao_mount_pressionado)
            {
                botao_mount_pressionado = false;
                sd_unmount(); // Chama a função para desmontar

                // Atualiza a UI para o novo estado
                estado_atual = ESTADO_SD_DESMONTADO;
            }
            break;
        case ESTADO_GRAVANDO:
            ui_set_led_state(UI_LED_RECORDING);

            char buffer_display[20];
            sprintf(buffer_display, "Amostra: %u", contador_amostras);
            ui_update_display("Gravando...", buffer_display);

            int16_t accel[3],
                gyro[3];
            imu_read_raw(accel, gyro);

            char csv_line[100];
            sprintf(csv_line, "%u,%d,%d,%d,%d,%d,%d", contador_amostras, accel[0], accel[1], accel[2], gyro[0], gyro[1], gyro[2]);

            ui_set_led_state(UI_LED_SAVING);
            sd_write_line(&fil, csv_line);
            ui_set_led_state(UI_LED_RECORDING); // Volta para vermelho

            contador_amostras++;

            if (botao_start_stop_pressionado)
            {
                botao_start_stop_pressionado = false;

                ui_update_display("Finalizando...", "Salvando dados...");
                ui_set_led_state(UI_LED_SAVING);
                sd_close_file(&fil);
                ui_flash_led(UI_LED_SAVING, 3);
                ui_beep(2);

                ui_set_led_state(UI_LED_READY);
                ui_update_display("Dados Salvos!", "");
                sleep_ms(2000);
                estado_atual = ESTADO_AGUARDANDO;
            }
            break;

        case ESTADO_SD_DESMONTADO:
            ui_set_led_state(UI_LED_OFF); // LED desligado indica SD desmontado
            ui_update_display("SD Desmontado", "Pressione B2 Montar");

            if (botao_mount_pressionado)
            {
                botao_mount_pressionado = false;

                estado_atual = ESTADO_INICIALIZANDO;
            }
            break;

        case ESTADO_ERRO:
            ui_set_led_state(UI_LED_ERROR);
            ui_update_display("ERRO FATAL", "Verifique o SD");
            ui_set_led_state(UI_LED_OFF);
            sleep_ms(250);
            ui_set_led_state(UI_LED_ERROR);
            sleep_ms(250);
            break;
        }
        sleep_ms(10);
    }
    return 0;
}