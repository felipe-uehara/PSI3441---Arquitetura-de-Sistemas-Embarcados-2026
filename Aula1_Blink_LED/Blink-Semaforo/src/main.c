#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

/* O Devicetree da KL25Z já tem atalhos (aliases) para os LEDs.
   Isso puxa a porta (GPIOB/D), o pino (18, 19, 1) e a flag de inversão (Active-Low) de uma só vez! */
static const struct gpio_dt_spec led_red = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec led_green = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec led_blue = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);

#define SLEEP_TIME_RED     3000    
#define SLEEP_TIME_YELLOW  1000    
#define SLEEP_TIME_GREEN   3000    

void main(void)
{
    // Verifica se os controladores de todos os LEDs estão prontos


    /* Configura os pinos como saída. 
       A flag INACTIVE garante que eles comecem desligados com segurança. */
    gpio_pin_configure_dt(&led_red, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&led_green, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&led_blue, GPIO_OUTPUT_INACTIVE);

    while (1) {
        /* IMPORTANTE: Como estamos usando a API "_dt", o Zephyr trata a lógica.
           1 = Ativo (Ligado) e 0 = Inativo (Desligado).
           O próprio Zephyr traduzirá seu "1" lógico para um "0" elétrico no hardware! */
           
        // Semáforo: Vermelho
        gpio_pin_set_dt(&led_red, 1);
        k_msleep(SLEEP_TIME_RED);
        gpio_pin_set_dt(&led_red, 0);

        // Semáforo: "Amarelo" (Usando o Azul)
        gpio_pin_set_dt(&led_blue, 1);
        k_msleep(SLEEP_TIME_YELLOW);
        gpio_pin_set_dt(&led_blue, 0);

        // Semáforo: Verde
        gpio_pin_set_dt(&led_green, 1);
        k_msleep(SLEEP_TIME_GREEN);
        gpio_pin_set_dt(&led_green, 0);
    }
}