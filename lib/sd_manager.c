#include "sd_manager.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "f_util.h"
#include "hardware/rtc.h"

static FATFS fs;

void sd_init() {
    // Inicializa o SPI com uma velocidade de clock baixa para garantir compatibilidade
    spi_init(SD_SPI_PORT, 1000 * 1000); // 1MHz
    
    // Configura a função dos pinos para SPI
    gpio_set_function(SD_MISO_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SD_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SD_MOSI_PIN, GPIO_FUNC_SPI);
    
    // O pino Chip Select (CS) é um GPIO normal, não uma função de hardware do SPI
    gpio_init(SD_CS_PIN);
    gpio_set_dir(SD_CS_PIN, GPIO_OUT);
    gpio_put(SD_CS_PIN, 1); // Desseleciona o cartão por padrão (nível alto)

    rtc_init();
}

bool sd_mount() {
    FRESULT fr = f_mount(&fs, "", 1);
    if (FR_OK != fr) {
        printf("ERRO: f_mount falhou: %s (%d)\n", FRESULT_str(fr), fr);
        return false;
    }
    printf("INFO: Cartao SD montado com sucesso.\n");
    return true;
}

void sd_unmount() {
    f_unmount("");
    printf("INFO: Cartao SD desmontado.\n");
}

bool sd_open_file(FIL* fp, const char* filename) {
    FRESULT fr = f_open(fp, filename, FA_OPEN_APPEND | FA_WRITE);
    if (FR_OK != fr) {
        printf("ERRO: f_open falhou: %s (%d)\n", FRESULT_str(fr), fr);
        return false;
    }
    return true;
}

void sd_write_line(FIL* fp, const char* line) {
    int rc = f_printf(fp, "%s\n", line);
    if (rc < 0) {
        printf("ERRO: f_printf falhou.\n");
    }
}

void sd_close_file(FIL* fp) {
    FRESULT fr = f_close(fp);
    if (FR_OK != fr) {
        printf("ERRO: f_close falhou: %s (%d)\n", FRESULT_str(fr), fr);
    }
}