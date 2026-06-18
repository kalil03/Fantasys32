; Teste da Fase 1: aritmética/lógica, MOVL/MOVH, STORE/LOAD, HALT
.data
valor: .var 0          ; uma palavra para testar STORE/LOAD

.text
START:
    MOVL R1, 10         ; R1 = 10
    MOVL R2, 3          ; R2 = 3
    ADD  R3, R1, R2     ; R3 = 13
    SUB  R4, R1, R2     ; R4 = 7
    MUL  R5, R1, R2     ; R5 = 30
    DIV  R6, R1, R2     ; R6 = 3
    MOD  R7, R1, R2     ; R7 = 1

    ; Constrói 0x12345678 em R8 com MOVL + MOVH
    MOVL R8, 0x5678
    MOVH R8, 0x1234     ; R8 = 0x12345678

    ; STORE/LOAD: grava R8 em 'valor' e relê em R9
    MOVL R10, valor.l
    MOVH R10, valor.h   ; R10 = endereço de 'valor'
    STORE R10, R8, 0    ; Mem[R10] = R8   (rs=base, rt=valor)
    LOAD  R10, R9, 0    ; R9 = Mem[R10]   (rs=base, rt=destino) = 0x12345678

    HALT
