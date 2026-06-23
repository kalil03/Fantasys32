; Teste da Fase 2: loop com branch, PUSH/POP, CALL/RET, INC/DEC/NOT
.data
.equ DUMMY, 0

.text
START:
    ; Soma 1+2+3+4+5 em R3 usando um loop (R1 = contador, R2 = limite)
    MOVL R1, 1
    MOVL R2, 5
    MOVL R3, 0
LOOP:
    ADD  R3, R3, R1     ; R3 += R1
    INC  R1             ; R1++
    BLE  R1, R2, LOOP   ; enquanto R1 <= 5, repete   -> R3 = 15

    ; Testa PUSH/POP (deve preservar e restaurar R3)
    PUSH R3
    MOVL R3, 999
    POP  R3             ; R3 volta a ser 15

    ; Testa CALL/RET: a sub-rotina dobra R3
    CALL DOBRA          ; R3 = 30

    ; Testa NOT (R4 = ~0 = 0xFFFFFFFF)
    MOVL R4, 0
    NOT  R4

    HALT

DOBRA:
    ADD R3, R3, R3      ; R3 = R3 * 2
    RET
