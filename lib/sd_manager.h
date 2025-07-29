#ifndef SD_MANAGER_H
#define SD_MANAGER_H

#include "ff.h" // Inclui a biblioteca FatFs
#include <stdbool.h> 

// --- Constantes ---
#define SD_SPI_PORT spi0
#define SD_MISO_PIN 16
#define SD_CS_PIN   17
#define SD_SCK_PIN  18
#define SD_MOSI_PIN 19

// --- Protótipos das Funções Públicas ---

void sd_init();
bool sd_mount();
void sd_unmount();
bool sd_open_file(FIL* fp, const char* filename);
void sd_write_line(FIL* fp, const char* line);
void sd_close_file(FIL* fp);

#endif // SD_MANAGER_H