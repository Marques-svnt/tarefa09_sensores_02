#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "buzzer.h"

#define BUZZER_PIN 21

static uint slice_num;

// Inicializa o PWM para o buzzer
void buzzer_init(void) {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 125.0f); // Divisor de clock para frequências audíveis
    pwm_init(slice_num, &config, false); // Começa desligado
}

// Toca um tom por uma duração específica e depois para
void buzzer_buzz(uint freq, int duration_ms) {
    if (freq == 0) return;

    uint32_t top = 1000000 / freq; // Baseado no clock de 1MHz (125MHz / 125)
    
    pwm_set_wrap(slice_num, top);
    pwm_set_chan_level(slice_num, PWM_CHAN_B, top / 2); // 50% duty cycle
    pwm_set_enabled(slice_num, true);
    sleep_ms(duration_ms);
    pwm_set_enabled(slice_num, false); // Desliga após a duração
}