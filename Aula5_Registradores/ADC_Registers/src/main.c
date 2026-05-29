#include "MKL25Z4.h"

#define blue_led    1
#define green_led   19
#define adc_channel 8
#define adc_upper_lim 3000
#define adc_down_lim  1000

/*Enunciado: Escrever o código, sem utilização do Processo Expert,
para:
1) Fazer uma aquisição analógica;
2) Acender o LED azul quando o valor for próximo de 3.3 V e o
LED verde quando o valor for próximo de 0 V. 
*/

void init(){
    //Habilitar o clock para as Portas B e D
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;

    //Configurar o MUX dos pinos para GPIO
    PORTB->PCR[green_led] = PORT_PCR_MUX(1);
    PORTD->PCR[blue_led]  = PORT_PCR_MUX(1);
    
    //Setar a direção dos pinos como SAÍDA
    PTB->PDDR |= (1 << green_led);
    PTD->PDDR |= (1 << blue_led);
    
    //Iniciar com os LEDs apagados (Lógica Invertida: 1 = apagado)
    PTB->PSOR = (1 << green_led);
    PTD->PSOR = (1 << blue_led);

    //Habilitar o clock para o módulo ADC0
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;

    //Configuração do ADC:  MODE = 01 (Resolução de 12 bits)
    //                      ADIV = 00 (Divisor de clock = 1, leitura rápida)
    ADC0->CFG1 = ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(0);
    

    //Trigger por software
    ADC0->SC2 = 0;
}


void delayMs(int n){
    volatile int i;
    volatile int j;
    for (i = 0; i < n; i++) 
        for (j = 0; j < 7000; j++) {}
}

uint16_t adcRead(uint8_t canal){
    // Escrever o número do canal no registrador SC1A inicia a conversão!
    ADC0->SC1[0] = ADC_SC1_ADCH(canal);
    
    // Esperar a conversão terminar
    // O bit COCO (Conversion Complete) vai a 1 quando a leitura está pronta
    while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK)) {
        // Fica travado aqui por alguns microssegundos esperando...
    }
    
    // Retornar o resultado da conversão (0 a 4095)
    return ADC0->R[0];
}

int main(void) {
    init();

    uint16_t analogValue = 0;

    while(1){

        analogValue = adcRead(adc_channel);

        if(analogValue > adc_upper_lim){
            PTD->PCOR = (1 << blue_led);   // LIGA o LED Azul   (0)
            PTB->PSOR = (1 << green_led);  // APAGA o LED Verde (1)
        }

        if(analogValue < adc_down_lim){
            PTD->PSOR = (1 << blue_led);   // desliga o LED Azul  
            PTB->PCOR = (1 << green_led);  // liga o LED Verde 
        }

        else{
            PTD->PSOR = (1 << blue_led);
            PTB->PSOR = (1 << green_led);
        }
    }   
    return 0;
}