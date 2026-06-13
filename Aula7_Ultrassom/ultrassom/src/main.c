#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include "pwm_z402.h"

// Configuração do PWM (Cálculos de 48MHz / PS_128)
#define TPM_MODULE_100MS 37500 
#define TPM_PULSE_10US   4     

// Vamos usar a API legada para capturar a porta GPIOA inteira
#define GPIO_PORT "GPIOA"
// O pino PTA1 que escolhemos para o Echo
#define ECHO_PIN  1       

static const struct device *gpio_dev;
static struct gpio_callback echo_cb_data;

// Variáveis de tempo protegidas para interrupção
volatile uint32_t tempo_inicio = 0;
volatile uint32_t tempo_pulso_us = 0;
volatile float distancia_cm = 0.0;

// Callback de interrupção
void echo_isr_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    // Verifica o estado diretamente do pino 1 na porta A
    int estado_pino = gpio_pin_get(dev, ECHO_PIN);
    
    if (estado_pino == 1) {
        // Borda de subida: início do sinal de eco
        tempo_inicio = k_cycle_get_32();
    } 
    else {
        // Borda de descida: fim do sinal de eco
        uint32_t tempo_fim = k_cycle_get_32();
        uint32_t ciclos = tempo_fim - tempo_inicio;
        
        // Conversão robusta de ciclos de clock para microssegundos
        tempo_pulso_us = ciclos / (sys_clock_hw_cycles_per_sec() / 1000000);
        
        // Cálculo de distância: tempo_us / 58
        distancia_cm = (float)tempo_pulso_us / 58.0f;
    }
}

int main(void)
{
    // 1. Inicialização do Pino de Echo via Zephyr API (Método Clássico)
    gpio_dev = device_get_binding(GPIO_PORT);
    if (!gpio_dev) {
        printk("Erro: Nao foi possivel encontrar a porta %s\n", GPIO_PORT);
        return 0;
    }
    
    // Configura o pino PTA1 como entrada
    gpio_pin_configure(gpio_dev, ECHO_PIN, GPIO_INPUT);
    
    // Configura a interrupção para ambas as bordas
    gpio_pin_interrupt_configure(gpio_dev, ECHO_PIN, GPIO_INT_EDGE_BOTH);
    
    // Associa o callback ao pino
    gpio_init_callback(&echo_cb_data, echo_isr_handler, BIT(ECHO_PIN));
    gpio_add_callback(gpio_dev, &echo_cb_data);

    // 2. Inicialização do Pino de Trigger via biblioteca obrigatória pwm_z402
    pwm_tpm_Init(TPM2, TPM_PLLFLL, TPM_MODULE_100MS, TPM_CLK, PS_128, EDGE_PWM);
    pwm_tpm_Ch_Init(TPM2, 0, TPM_PWM_H, GPIOB, 18);
    pwm_tpm_CnV(TPM2, 0, TPM_PULSE_10US);

    printk("Sistema de Ultrassom Inicializado com Sucesso!\n");

    // 3. Loop de exibição de dados
    for (;;)
    {
        printk("Distancia Medida: %.2f cm\n", distancia_cm);
        k_msleep(500); 
    }
    
    return 0;
}