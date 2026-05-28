/*
Escreva um programa para fazer o LED verde piscar
com 2 segundo de período usando registradores
– Sequência do programa
• (1) Habilitar clock da porta B;
• (2) Configurar Pino 19 (Pin Control Register);
• (3) Setar a direção do Pino;
• (4) Habilitar saída;
• (5) Função de espera;
• (6) Desabilitar saída;
• (7) Função de espera;
• Repetir passos (4)-(7). 

Exemplo:
Define o endereço do registrador SIM_SCGC5: #define SIM_SCGC5 (*((volatile unsigned int*)0x40048038))

Função de espera:
void delayMs (int n); // Função: Espera n milisegundos 
    //esta função depende do clock default do microcontrolador. Para o KL25Z a frequência é
    21 MHz aproximadamente. O valor do contador deverá ser ajustado para se conseguir o
    tempo de espera desejado. 

void delayMs (int n) {
int i;
int j;
for (i = 0; i < n; i++)
for (j = 0; j < 7000; j++) {}
}
*/

//Video do codigo rodando na placa frescale FRDM-KL25Z: https://youtube.com/shorts/ZPgjSx5nheY

//Definições dos pinos e endereços
#define SIM_SCGC5 (*((volatile unsigned int*)0x40048038)) //define o endereço do registrador SIM_SCGC5 apontando para o endereço 0x40048038 

#define PORTB_PCR19 (*((volatile unsigned int*)0x4004A04C)) //define o endereço 0x4004A04C para o LED verde da placa freescale


//Definição das funções (input/output GPIO) dos pinos e dos registradores
#define PTB_PDDR     (*((volatile unsigned int*)0x400FF054)) //define a direção do pino do portB
#define PTB_PSOR     (*((volatile unsigned int*)0x400FF044)) //habilita a escrita do pino do portB, fazendo o SET
#define PTB_PCOR     (*((volatile unsigned int*)0x400FF048)) //habilita a escrita do pino do portB, fazendo seu RESET



void delayMs(int n){
    volatile int i;
    volatile int j;
    for (i = 0; i < n; i++) 
        for (j = 0; j < 7000; j++) {}
}

int main(void) {
    SIM_SCGC5 |= (1 << 10); //escreve 1 no bit10 (que representa o PortB) e zero nos demais bits
    
    PORTB_PCR19 = (1 << 8); //configura o pino 19 do portB como GPIO e seta o bit 8(PE - Pull Enable) como '1', habilitando um resistor interno de pull-up/down para entrada digitais
    

    PTB_PDDR |= (1 << 19); //seta a direção do portB, escrevendo no bit19='1' (saída) e '0' nos demais (input)
    
    PTB_PSOR |= (1 << 19); //seta o estado do bit19 como '1', apagando o led por conta da lógica invertida (3v3->resistor->LED->GPIO)

    while (1) {
        PTB_PCOR |= (1 << 19);//reseta o estado do bit19 como '0', ligando o led
        
        delayMs(1000);

        PTB_PSOR |= (1 << 19);

        delayMs(1000);
    }
    return 0;
}