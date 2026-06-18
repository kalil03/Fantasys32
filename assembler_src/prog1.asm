; Exemplo de programa que desenha um retângulo e 
; espera pela tecla ESPAÇO ser pressionada para continuar

.data
.equ SPACE_KEYCODE, 0x04
.equ AZUL, 0xFF0000FF
.equ BRANCO, 0xFFFFFFFF
.equ PRETO, 0xFF000000
.equ RETANGULO_LARG, 8
.equ RETANGULO_ALT, 8
msg: .string "Pressione ESPACO para continuar..."

.text
START:
    ; Limpa a tela
    MOVL R1, PRETO.l         ; color = preto (parte baixa)
    MOVH R1, PRETO.h         ; color = preto (parte alta)
    CLEAR R1

    ; Desenha um retângulo azul no centro da tela
    MOVL R1, 160            ; x = 160
    MOVL R2, 120            ; y = 120
    MOVL R3, RETANGULO_LARG ; w = 8
    MOVL R4, RETANGULO_ALT  ; h = 8
    MOVL R5, AZUL.l         ; color = azul (parte baixa)
    MOVH R5, AZUL.h         ; color = azul (parte alta)
    RECT R1, R2, R3, R4, R5

    ; Imprime a mensagem
    MOVL R1, 10            ; x = 160
    MOVL R2, 10            ; y = 120
    MOVL R3, msg.l          ; endereço da string (parte baixa)
    MOVH R3, msg.h          ; endereço da string (parte alta)
    MOVL R4, BRANCO.l         ; color = branco (ARGB)
    MOVH R4, BRANCO.h         ; color = branco (ARGB)
    PSTR R1, R2, R3, R4    

WAIT_KEY:
    ; Espera até que a tecla ESPAÇO seja pressionada
    MOVL R2, SPACE_KEYCODE  ; keyID = ESPAÇO
    GKEY R1, R2             ; R1 = 1 se ESPAÇO estiver pressionado, 
                            ; 0 caso contrário
    BEQ R1, R0, WAIT_KEY    ; Se R1 == 0, continua esperando

    ; Limpa a tela com azul após a tecla ser pressionada
    MOVL R1, AZUL.l         ; color = azul (parte baixa)
    MOVH R1, AZUL.h         ; color = azul (parte alta)
    CLEAR R1                ; Limpa a tela com azul

END: ; Fica em loop infinito
    BEQ R0, R0, END
