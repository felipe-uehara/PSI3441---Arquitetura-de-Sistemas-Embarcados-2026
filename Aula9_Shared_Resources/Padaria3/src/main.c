#include <zephyr.h>
#include <sys/printk.h>

// estoque de paes
//volatile int saldo_vitrine = 0;

#define saldoMax 10 //máximo de 10 pães na vitrine

K_SEM_DEFINE(saldo_vitrine, 0, saldoMax);

// --- THREAD 1: PADEIRO (Produtor) ---
#define PRIORITY_PADEIRO 7 

void padeiro_thread_entry(void *p1, void *p2, void *p3) {
    while (1) {
        // Produção: Adiciona um pão
        k_sem_give(&saldo_vitrine);
        
        printk("[PADEIRO] Assou um pao! | Saldo na vitrine: %d\n", k_sem_count_get(&saldo_vitrine));
        
        // Regra: O padeiro produz a cada 1 segundo (1000 ms)
        
        k_msleep(1000); 
    }
}
K_THREAD_DEFINE(padeiro_tid, 1024, padeiro_thread_entry, NULL, NULL, NULL, PRIORITY_PADEIRO, 0, 0);


// --- THREAD 2: CLIENTE (Consumidor) ---
#define PRIORITY_CLIENTE 6

void cliente_thread_entry(void *p1, void *p2, void *p3) {
    while (1) {
        // Consumo: Tenta retirar um pão
        
        
       k_sem_take(&saldo_vitrine, K_FOREVER); 
        printk("   -> [CLIENTE] Comprou um pao. | Saldo na vitrine: %d\n", k_sem_count_get(&saldo_vitrine));
       
        // Regra: O cliente tenta retirar a cada 1,5 segundos (1500 ms)
       
        k_msleep(1500); 
    }
}
K_THREAD_DEFINE(cliente_tid, 1024, cliente_thread_entry, NULL, NULL, NULL, PRIORITY_CLIENTE, 0, 0);


// --- MAIN ---
void main(void) {    
    printk("\t----Padaria com Mutex----\n");      
}