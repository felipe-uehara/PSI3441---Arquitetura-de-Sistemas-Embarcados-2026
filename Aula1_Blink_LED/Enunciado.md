# Atividade - LEDs

Depois de implementar o exemplo [Exemplo Pisca LED RTOS](https://www.notion.so/Exemplo-Pisca-LED-RTOS-1f1a7f4c2a2b8142bf66ddc7dbf306f2?pvs=21)  e simplificar o código seguindo o [Pisca LED Simples (Zephyr 2.7.1)](https://www.notion.so/Pisca-LED-Simples-Zephyr-2-7-1-1f1a7f4c2a2b8139b342c0db216d9c12?pvs=21), atualize o Zephyr seguindo [Atualização para o Zephyr 4.2](https://www.notion.so/Atualiza-o-para-o-Zephyr-4-2-27aa7f4c2a2b802a9a3efac5c8691a84?pvs=21).

Agora modifique o código para fazer piscar os 3 LED como em um semáforo. 

- Antes de começar, leia o [Explicação Detalhada do Código](https://www.notion.so/Explica-o-Detalhada-do-C-digo-1f1a7f4c2a2b811d96b7d23da38ef96b?pvs=21)
- Utilize as informações dos pinos e portas contidas nos [Documentos de Referência](https://www.notion.so/Documentos-de-Refer-ncia-1f1a7f4c2a2b815988c9fe62cb1e7c44?pvs=21)
    - Procure documento de pinagem as portas e pinos dos LEDs
    - Verifique no documento do esquemático a conexão dos LEDs.
- Procure qual é o Alias dos portas A, B, C, D e E no Device Tree → zephyr.dts
- Utilize outras funções além do [Toggle](https://docs.zephyrproject.org/latest/doxygen/html/group__gpio__interface.html#gaabf948471d313ff19410f1741dd16957:~:text=static%20int%20gpio_pin_toggle,pin%C2%A0)) para ligar e desligar os LEDs.
- **Utilize uma máquina de estados para fazer a alteração dos LEDs.**

## Perguntas:

1. Por que os LEDs são Active Low (acendem quando se coloca 0 na saída)? 
2. Quais funções você usou para acender e apagar os LEDs?
3. Explique o que é o DeviceTree. 
4. Explique as abstrações feitas pelo Sistema Operacional. 

[Zephyr RTOS](https://www.notion.so/Zephyr-RTOS-1f1a7f4c2a2b80c28c5ee53fdfa0bcc1?pvs=21)