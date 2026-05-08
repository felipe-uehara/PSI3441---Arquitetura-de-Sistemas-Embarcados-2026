#include <zephyr.h>             // Funções básicas do Zephyr (ex: k_msleep)
#include <device.h>             // API  para obter e usar dispositivos
#include <drivers/gpio.h>       // API para controle de GPIO

#define LED_PORT       "GPIO_1" // Nome do controlador GPIO (label no .pio\build\frdm_kl25z\zephyr\zephyr.dts)
#define RGB_RED          18       // Pino PTB18 onde está o LED vermelho
#define RGB_GREEN        19       // Pino PTB18 onde está o LED verde
#define RGB_BLUE         13       // Pino PTB18 onde está o LED azul
#define SLEEP_TIME_MS  500    // Intervalo de piscar (milissegundos)

void main(void)
{

}