#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <drivers/sensor.h>
#include <drivers/adc.h>
#include <sys/printk.h>
#include <stdlib.h>

// --- ESTADOS DO SISTEMA ---
typedef enum {
    MODO_ADC, //somente o ADC do pino PTB0
    MODO_COMPLETO //ADC com o Acelerometro da placa
} modo_exibicao_t;

volatile modo_exibicao_t modo_atual = MODO_ADC;

// --- CONFIGURAÇÃO DA INTERRUPÇÃO DO BOTÃO ---
static struct gpio_callback button_cb_data;

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
    if (modo_atual == MODO_ADC) {
        modo_atual = MODO_COMPLETO;
        printk("\n>> MODO COMPLETO ATIVADO <<\n");
    } else {
        modo_atual = MODO_ADC;
        printk("\n>> MODO ADC ATIVADO <<\n");
    }
}

// --- THREAD DO ACELERÔMETRO ---
//prioridade igual a do ADC para não concorrer com ele
#define PRIORITY_ACCEL 7
void accel_thread_entry(void *p1, void *p2, void *p3) {
    const struct device *accel_dev = DEVICE_DT_GET_ANY(nxp_fxos8700);
    struct sensor_value accel[3];

    if (!device_is_ready(accel_dev)) {
        printk("Erro: Acelerometro nao encontrado ou nao esta pronto!\n");
        return;
    }

    while (1) {
        if (modo_atual == MODO_COMPLETO) {
            sensor_sample_fetch(accel_dev);
            sensor_channel_get(accel_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
            
    
            const char *sign_x = (accel[0].val1 == 0 && accel[0].val2 < 0) ? "-" : "";
            const char *sign_y = (accel[1].val1 == 0 && accel[1].val2 < 0) ? "-" : "";
            const char *sign_z = (accel[2].val1 == 0 && accel[2].val2 < 0) ? "-" : "";

            printk("[ACCEL] X: %s%d.%06d | Y: %s%d.%06d | Z: %s%d.%06d\n", 
                   sign_x, accel[0].val1, abs(accel[0].val2),
                   sign_y, accel[1].val1, abs(accel[1].val2),
                   sign_z, accel[2].val1, abs(accel[2].val2));
        }
        k_msleep(1000); 
    }
}
K_THREAD_DEFINE(accel_tid, 1024, accel_thread_entry, NULL, NULL, NULL, PRIORITY_ACCEL, 0, 0);

// --- THREAD DO ADC ---
#define PRIORITY_ADC 7

void adc_thread_entry(void *p1, void *p2, void *p3) {
    // Busca o ADC de forma garantida pelo Device Tree
    const struct device *adc_dev = DEVICE_DT_GET(DT_NODELABEL(adc0));

    if (!device_is_ready(adc_dev)) {
        printk("Erro: Dispositivo ADC nao encontrado!\n");
        return;
    }

    struct adc_channel_cfg channel_cfg = {
        .gain             = ADC_GAIN_1,
        .reference        = ADC_REF_INTERNAL,
        .acquisition_time = ADC_ACQ_TIME_DEFAULT,
        .channel_id       = 8, 
        .differential     = 0
    };
    adc_channel_setup(adc_dev, &channel_cfg);

    int16_t sample_buffer;
    struct adc_sequence sequence = {
        .channels    = BIT(8),
        .buffer      = &sample_buffer,
        .buffer_size = sizeof(sample_buffer),
        .resolution  = 12,
    };

    while (1) {
        int err = adc_read(adc_dev, &sequence);
        if (err < 0) {
            printk("Erro ao ler ADC: %d\n", err);
        } else {
            printk("[ADC] Valor do Pino A0: %d\n", sample_buffer);
        }
        k_msleep(500); 
    }
}
K_THREAD_DEFINE(adc_tid, 1024, adc_thread_entry, NULL, NULL, NULL, PRIORITY_ADC, 0, 0);

// --- MAIN ---
void main(void) {
    // Busca a Porta A de forma garantida pelo Device Tree (Pino PTA12 / D3)
    const struct device *button_dev = DEVICE_DT_GET(DT_NODELABEL(gpioa));

    if (!device_is_ready(button_dev)) { 
        printk("Erro: Porta GPIOA nao encontrada ou nao pronta!\n");
        return; 
    }

    // Configura o pino 12 (D3) como entrada, com pull-up interno e interrupção na borda de descida
    gpio_pin_configure(button_dev, 12, GPIO_INPUT | GPIO_PULL_UP | GPIO_ACTIVE_LOW);
    gpio_pin_interrupt_configure(button_dev, 12, GPIO_INT_EDGE_TO_ACTIVE);
    
    // Liga a função de callback ao pino
    gpio_init_callback(&button_cb_data, button_pressed, BIT(12));
    gpio_add_callback(button_dev, &button_cb_data);

    printk("\n======================================================\n");
    printk("Sistema Inicializado (Zephyr 2.7).\n");
    printk("Pressione o botao no pino D3 (PTA12) para alternar.\n");
    printk("======================================================\n");
}