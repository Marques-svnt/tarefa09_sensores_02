#include "ui_manager.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "font.h"
#include "buzzer.h"

// Estrutura do display, estática para ser usada apenas neste arquivo.
static ssd1306_t disp;

void ui_init()
{
    // Inicializa I2C para o Display
    i2c_init(UI_DISP_I2C_PORT, 400 * 1000);
    gpio_set_function(UI_DISP_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(UI_DISP_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(UI_DISP_SDA_PIN);
    gpio_pull_up(UI_DISP_SCL_PIN);
    ssd1306_init(&disp, 128, 64, false, UI_DISP_ADDR, UI_DISP_I2C_PORT);
    ssd1306_config(&disp);

    // Inicializa os GPIOs do LED como saída
    gpio_init(UI_LED_R_PIN);
    gpio_init(UI_LED_G_PIN);
    gpio_init(UI_LED_B_PIN);
    gpio_set_dir(UI_LED_R_PIN, GPIO_OUT);
    gpio_set_dir(UI_LED_G_PIN, GPIO_OUT);
    gpio_set_dir(UI_LED_B_PIN, GPIO_OUT);

    buzzer_init();
}

void ui_update_display(const char *line1, const char *line2)
{
    ssd1306_fill(&disp, false);
    if (line1)
    {
        ssd1306_draw_string(&disp, (char *)line1, 5, 10);
    }
    if (line2)
    {
        ssd1306_draw_string(&disp, (char *)line2, 5, 30);
    }
    ssd1306_send_data(&disp);
}

void ui_set_led_state(UILedState state)
{
    bool r = 0, g = 0, b = 0;
    switch (state)
    {
    case UI_LED_INIT:
        r = 1;
        g = 1;
        b = 0;
        break;
    case UI_LED_READY:
        r = 0;
        g = 1;
        b = 0;
        break;
    case UI_LED_RECORDING:
        r = 1;
        g = 0;
        b = 0;
        break;
    case UI_LED_SAVING:
        r = 0;
        g = 0;
        b = 1;
        break;
    case UI_LED_ERROR:
        r = 1;
        g = 0;
        b = 1;
        break;
    case UI_LED_OFF:
    default:
        r = 0;
        g = 0;
        b = 0;
        break;
    }
    gpio_put(UI_LED_R_PIN, r);
    gpio_put(UI_LED_G_PIN, g);
    gpio_put(UI_LED_B_PIN, b);
}

void ui_beep(int count)
{
    for (int i = 0; i < count; ++i)
    {
        buzzer_buzz(880, 70);
        if (count > 1 && i < count - 1)
        {
            sleep_ms(70);
        }
    }
}

void ui_flash_led(UILedState state, int count) {
    for (int i = 0; i < count; i++) {
        ui_set_led_state(state);
        sleep_ms(50); // LED aceso por 50ms
        ui_set_led_state(UI_LED_OFF);
        sleep_ms(50); // LED apagado por 50ms
    }
}