; Inicialização do Stack Pointer
LDR R13, =0xFF8

MAIN:
    ; Carga dos valores iniciais nos registradores
    LDR R0, =0x125
    LDR R1, =0x144
    MOV R2, #0x56

    ; Chamada da sub-rotina FUNC
    ; O endereço de retorno é salvo automaticamente no LR
    BL FUNC

    ; Operações matemáticas após o retorno da sub-rotina
    ; Como o contexto foi preservado, R0, R1 e R2 mantêm os valores iniciais
    ADD R3, R0, R1      ; R3 = 0x125 + 0x144 = 0x269
    ADD R3, R3, R2      ; R3 = 0x269 + 0x56 = 0x2BF

LOOP: 
    B LOOP              ; Loop infinito para travar a execução no final


FUNC:
    ; Salva os valores de R0, R1 e R2 na pilha em uma única instrução
    PUSH {R0, R1, R2}   

    ; Simulação de rotina interna que destrói os valores dos registradores
    MOV R0, #0
    MOV R1, #0
    MOV R2, #0

    ; Restaura os valores originais de R0, R1 e R2 a partir da pilha
    POP {R0, R1, R2}    
    
    ; Retorna para a rotina principal (MAIN)
    BX LR