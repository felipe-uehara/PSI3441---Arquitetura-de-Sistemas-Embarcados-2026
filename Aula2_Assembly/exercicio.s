; X = (a+b)-(c+d) a ser armazenado em 0xFF0C

MOV R0, #1        ; a = 1
MOV R1, #2        ; b = 2
ADD R0, R0, R1    ; b(atual) = b(anterior) + a = 3
SUB R0, R0, #3     ; b(atual) = b(anterior) - c = 0
SUB R0, R0, #4     ; b(atual) = b(anterior) - d = -4
STR R0, [0xFF0C]  