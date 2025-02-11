#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_GPIO 22  // Pino do servomotor
#define PWM_FREQ 50    // Frequência de 50Hz

// Função para calcular o nível de PWM baseado no tempo ativo (em µs)
uint16_t calcular_nivel_pwm(uint slice, float tempo_us) {
    float clock_freq = 125000000.0;  // Frequência padrão do RP2040 
    float div = 64.0;                // Divisor de clock para PWM
    float wrap = (clock_freq / (div * PWM_FREQ)) - 1;
    return (uint16_t)((tempo_us / 20000.0) * wrap);
}

// Configuração do PWM no pino do servo
void configurar_pwm() {
    gpio_set_function(SERVO_GPIO, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(SERVO_GPIO);
    
    pwm_set_clkdiv(slice, 64.0);  // Define o divisor de clock
    pwm_set_wrap(slice, (125000000 / (64 * PWM_FREQ)) - 1);
    pwm_set_enabled(slice, true);
}

// Função para definir o ângulo do servo
void definir_angulo(float tempo_us) {
    uint slice = pwm_gpio_to_slice_num(SERVO_GPIO);
    pwm_set_gpio_level(SERVO_GPIO, calcular_nivel_pwm(slice, tempo_us));
}

// Movimento suave entre 0° e 180°
void movimentacao_suave() {
    for (float tempo_us = 500; tempo_us <= 2400; tempo_us += 5) {
        definir_angulo(tempo_us);
        sleep_ms(10);
    }
    for (float tempo_us = 2400; tempo_us >= 500; tempo_us -= 5) {
        definir_angulo(tempo_us);
        sleep_ms(10);
    }
}

int main() {
    stdio_init_all();
    configurar_pwm();

    while (true) {
        definir_angulo(2400); // 180°
        sleep_ms(5000);

        definir_angulo(1470); // 90°
        sleep_ms(5000);

        definir_angulo(500); // 0°
        sleep_ms(5000);

        movimentacao_suave(); // Movimentação entre 0° e 180°
    }
}
