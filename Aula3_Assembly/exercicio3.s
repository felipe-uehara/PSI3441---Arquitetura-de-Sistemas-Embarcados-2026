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
    ADD R3, R0, R1      
    ADD R3, R3, R2      

LOOP: 
    B LOOP              ; Loop infinito para finalizar a execução

; ---------------------------------------------------------
; Sub-rotina FUNC (Versão Original / Sem Otimização)
; ---------------------------------------------------------
FUNC:
    ; --- EMPILHAMENTO MANUAL (Salvando contexto) ---
    STR R0, [R13]       ; Guarda R0 no endereço apontado por R13 (0xFF8)
    SUB R13, R13, #4    ; Desce o ponteiro da pilha para 0xFF4
    STR R1, [R13]       ; Guarda R1 no endereço 0xFF4
    SUB R13, R13, #4    ; Desce o ponteiro da pilha para 0xFF0
    STR R2, [R13]       ; Guarda R2 no endereço 0xFF0
    SUB R13, R13, #4    ; Desce o ponteiro da pilha para 0xFEC

    ; --- ROTINA INTERNA ---
    ; Simulação de uso que sobrescreve os registradores originais
    MOV R0, #0
    MOV R1, #0
    MOV R2, #0

    ; --- DESEMPILHAMENTO MANUAL (Restaurando contexto) ---
    ; A restauração precisa ser feita na ordem inversa (LIFO)
    ADD R13, R13, #4    ; Sobe o ponteiro da pilha para 0xFF0
    LDR R2, [R13]       ; Lê a memória em 0xFF0 e devolve o valor original de R2
    ADD R13, R13, #4    ; Sobe o ponteiro da pilha para 0xFF4
    LDR R1, [R13]       ; Lê a memória em 0xFF4 e devolve o valor original de R1
    ADD R13, R13, #4    ; Sobe o ponteiro da pilha para 0xFF8
    LDR R0, [R13]       ; Lê a memória em 0xFF8 e devolve o valor original de R0
    
    ; Retorna para a rotina principal (MAIN)
    BX LR