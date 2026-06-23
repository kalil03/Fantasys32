; Teste das Fases 3/4/5: CLEAR, RECT, PSTR, PINT, RAND e HALT.
.data
.equ PRETO, 0xFF000000
.equ AZUL, 0xFF0000FF
.equ BRANCO, 0xFFFFFFFF
msg: .string "FANTASYS32 OK"
.text
START:
    ; Limpa a tela de preto
    MOVL R1, PRETO.l
    MOVH R1, PRETO.h
    CLEAR R1

    ; Retângulo azul no centro
    MOVL R1, 140
    MOVL R2, 100
    MOVL R3, 40
    MOVL R4, 40
    MOVL R5, AZUL.l
    MOVH R5, AZUL.h
    RECT R1, R2, R3, R4, R5

    ; Texto branco
    MOVL R1, 10
    MOVL R2, 10
    MOVL R3, msg.l
    MOVH R3, msg.h
    MOVL R4, BRANCO.l
    MOVH R4, BRANCO.h
    PSTR R1, R2, R3, R4

    ; Imprime um número (123) abaixo
    MOVL R1, 10
    MOVL R2, 40
    MOVL R3, 123
    MOVL R4, BRANCO.l
    MOVH R4, BRANCO.h
    PINT R1, R2, R3, R4

    ; Número aleatório em [10,20] -> R7 (testa SRAND/RAND)
    MOVL R1, 42
    SRAND R1
    MOVL R2, 10
    MOVL R3, 20
    RAND R7, R2, R3

    HALT
