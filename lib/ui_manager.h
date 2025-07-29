#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "ssd1306.h" // Precisamos do tipo ssd1306_t
#include <stdint.h>

// --- Constantes dos Pinos de UI ---
#define UI_DISP_I2C_PORT i2c1
#define UI_DISP_SDA_PIN  14
#define UI_DISP_SCL_PIN  15
#define UI_DISP_ADDR     0x3C

#define UI_LED_R_PIN 13
#define UI_LED_B_PIN 12
#define UI_LED_G_PIN 11

typedef enum {
    UI_LED_OFF,
    UI_LED_INIT,
    UI_LED_READY,
    UI_LED_RECORDING,
    UI_LED_SAVING, 
    UI_LED_ERROR 
} UILedState;

// --- Protótipos das Funções Públicas ---

void ui_init();
void ui_update_display(const char* line1, const char* line2);
void ui_set_led_state(UILedState state);
void ui_beep(int count);
void ui_flash_led(UILedState state, int count);

#endif // UI_MANAGER_H