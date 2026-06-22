; Fantasys32 - Snake simples
; Objetivo: comer a comida vermelha, crescer e evitar parede/corpo.
;
; Controles esperados pela VM:
;   KEY_UP=0, KEY_DOWN=1, KEY_LEFT=2, KEY_RIGHT=3, KEY_SPACE=4
; Se o mapeamento da VM ficar diferente, altere apenas os .equ KEY_*.

.data
.equ KEY_UP, 0
.equ KEY_DOWN, 1
.equ KEY_LEFT, 2
.equ KEY_RIGHT, 3
.equ KEY_SPACE, 4

.equ GRID_W, 32
.equ GRID_H, 24
.equ CELL, 10
.equ MAX_CELLS, 64

.equ PRETO, 0xFF000000
.equ VERDE, 0xFF00CC55
.equ VERDE_HEAD, 0xFF66FF99
.equ VERMELHO, 0xFFFF3030
.equ BRANCO, 0xFFFFFFFF
.equ FUNDO_GAMEOVER, 0xFF330000

snake_len: .var 3
dir_x: .var 1
dir_y: .var 0
head_x: .var 8
head_y: .var 12
food_x: .var 20
food_y: .var 12
score: .var 0

snake_x: .space MAX_CELLS
snake_y: .space MAX_CELLS

msg_game_over: .string "GAME OVER"
msg_restart: .string "ESPACO reinicia"

.text
START:
    CALL INIT_GAME

GAME_LOOP:
    CALL READ_INPUT
    CALL MOVE_SNAKE
    CALL DRAW_SCREEN
    MOVL R1, 7
    SLEEP R1
    JMP GAME_LOOP

; ---------------------------------------------------------------------------
; Inicializa variaveis e os tres primeiros segmentos da cobra.
; ---------------------------------------------------------------------------
INIT_GAME:
    MOVL R1, snake_len.l
    MOVH R1, snake_len.h
    MOVL R2, 3
    STORE R1, R2, 0

    MOVL R1, dir_x.l
    MOVH R1, dir_x.h
    MOVL R2, 1
    STORE R1, R2, 0

    MOVL R1, dir_y.l
    MOVH R1, dir_y.h
    MOVL R2, 0
    STORE R1, R2, 0

    MOVL R1, head_x.l
    MOVH R1, head_x.h
    MOVL R2, 8
    STORE R1, R2, 0

    MOVL R1, head_y.l
    MOVH R1, head_y.h
    MOVL R2, 12
    STORE R1, R2, 0

    MOVL R1, food_x.l
    MOVH R1, food_x.h
    MOVL R2, 20
    STORE R1, R2, 0

    MOVL R1, food_y.l
    MOVH R1, food_y.h
    MOVL R2, 12
    STORE R1, R2, 0

    MOVL R1, score.l
    MOVH R1, score.h
    MOVL R2, 0
    STORE R1, R2, 0

    ; snake_x = [8, 7, 6]
    MOVL R1, snake_x.l
    MOVH R1, snake_x.h
    MOVL R2, 8
    STORE R1, R2, 0
    MOVL R2, 7
    STORE R1, R2, 1
    MOVL R2, 6
    STORE R1, R2, 2

    ; snake_y = [12, 12, 12]
    MOVL R1, snake_y.l
    MOVH R1, snake_y.h
    MOVL R2, 12
    STORE R1, R2, 0
    STORE R1, R2, 1
    STORE R1, R2, 2
    RET

; ---------------------------------------------------------------------------
; Le as setas. A cobra nao pode inverter diretamente o sentido.
; ---------------------------------------------------------------------------
READ_INPUT:
    MOVL R2, KEY_UP
    GKEY R1, R2
    BNE R1, R0, TRY_UP

    MOVL R2, KEY_DOWN
    GKEY R1, R2
    BNE R1, R0, TRY_DOWN

    MOVL R2, KEY_LEFT
    GKEY R1, R2
    BNE R1, R0, TRY_LEFT

    MOVL R2, KEY_RIGHT
    GKEY R1, R2
    BNE R1, R0, TRY_RIGHT
    RET

TRY_UP:
    MOVL R3, dir_y.l
    MOVH R3, dir_y.h
    LOAD R3, R4, 0
    MOVL R5, 1
    BEQ R4, R5, END_READ_INPUT
    MOVL R3, dir_x.l
    MOVH R3, dir_x.h
    MOVL R4, 0
    STORE R3, R4, 0
    MOVL R3, dir_y.l
    MOVH R3, dir_y.h
    ADDI R4, R0, -1
    STORE R3, R4, 0
    RET

TRY_DOWN:
    MOVL R3, dir_y.l
    MOVH R3, dir_y.h
    LOAD R3, R4, 0
    ADDI R5, R0, -1
    BEQ R4, R5, END_READ_INPUT
    MOVL R3, dir_x.l
    MOVH R3, dir_x.h
    MOVL R4, 0
    STORE R3, R4, 0
    MOVL R3, dir_y.l
    MOVH R3, dir_y.h
    MOVL R4, 1
    STORE R3, R4, 0
    RET

TRY_LEFT:
    MOVL R3, dir_x.l
    MOVH R3, dir_x.h
    LOAD R3, R4, 0
    MOVL R5, 1
    BEQ R4, R5, END_READ_INPUT
    MOVL R3, dir_x.l
    MOVH R3, dir_x.h
    ADDI R4, R0, -1
    STORE R3, R4, 0
    MOVL R3, dir_y.l
    MOVH R3, dir_y.h
    MOVL R4, 0
    STORE R3, R4, 0
    RET

TRY_RIGHT:
    MOVL R3, dir_x.l
    MOVH R3, dir_x.h
    LOAD R3, R4, 0
    ADDI R5, R0, -1
    BEQ R4, R5, END_READ_INPUT
    MOVL R3, dir_x.l
    MOVH R3, dir_x.h
    MOVL R4, 1
    STORE R3, R4, 0
    MOVL R3, dir_y.l
    MOVH R3, dir_y.h
    MOVL R4, 0
    STORE R3, R4, 0
    RET

END_READ_INPUT:
    RET

; ---------------------------------------------------------------------------
; Calcula nova cabeca, testa colisao, move corpo e trata comida.
; ---------------------------------------------------------------------------
MOVE_SNAKE:
    MOVL R1, head_x.l
    MOVH R1, head_x.h
    LOAD R1, R2, 0
    MOVL R3, dir_x.l
    MOVH R3, dir_x.h
    LOAD R3, R4, 0
    ADD R2, R2, R4        ; R2 = novo x

    MOVL R1, head_y.l
    MOVH R1, head_y.h
    LOAD R1, R3, 0
    MOVL R4, dir_y.l
    MOVH R4, dir_y.h
    LOAD R4, R5, 0
    ADD R3, R3, R5        ; R3 = novo y

    BLT R2, R0, GAME_OVER
    BLT R3, R0, GAME_OVER
    MOVL R4, 31
    BGT R2, R4, GAME_OVER
    MOVL R4, 23
    BGT R3, R4, GAME_OVER

    ; Colisao com o corpo: compara novo ponto com cada segmento.
    MOVL R4, snake_len.l
    MOVH R4, snake_len.h
    LOAD R4, R5, 0        ; R5 = tamanho
    MOVL R6, 0            ; R6 = i

SELF_LOOP:
    BGE R6, R5, SELF_OK

    MOVL R7, 4
    MUL R8, R6, R7

    MOVL R9, snake_x.l
    MOVH R9, snake_x.h
    ADD R9, R9, R8
    LOAD R9, R10, 0
    BNE R10, R2, SELF_NEXT

    MOVL R9, snake_y.l
    MOVH R9, snake_y.h
    ADD R9, R9, R8
    LOAD R9, R10, 0
    BEQ R10, R3, GAME_OVER

SELF_NEXT:
    INC R6
    JMP SELF_LOOP

SELF_OK:
    ; Verifica se comeu.
    MOVL R6, food_x.l
    MOVH R6, food_x.h
    LOAD R6, R7, 0
    BNE R2, R7, NO_EAT
    MOVL R6, food_y.l
    MOVH R6, food_y.h
    LOAD R6, R7, 0
    BNE R3, R7, NO_EAT

    ; Comeu: aumenta o tamanho ate MAX_CELLS.
    MOVL R6, MAX_CELLS
    BGE R5, R6, AFTER_GROW
    INC R5
    MOVL R6, snake_len.l
    MOVH R6, snake_len.h
    STORE R6, R5, 0

AFTER_GROW:
    MOVL R6, score.l
    MOVH R6, score.h
    LOAD R6, R7, 0
    INC R7
    STORE R6, R7, 0

    ; PLACE_FOOD usa R1-R3. Guardamos a nova cabeca antes da chamada.
    MOVL R8, head_x.l
    MOVH R8, head_x.h
    STORE R8, R2, 0
    MOVL R8, head_y.l
    MOVH R8, head_y.h
    STORE R8, R3, 0
    CALL PLACE_FOOD
    MOVL R8, head_x.l
    MOVH R8, head_x.h
    LOAD R8, R2, 0
    MOVL R8, head_y.l
    MOVH R8, head_y.h
    LOAD R8, R3, 0
    JMP SHIFT_BODY

NO_EAT:
    ; Sem comida: tamanho permanece igual.

SHIFT_BODY:
    ; Desloca corpo do fim para o inicio: pos[i] = pos[i - 1].
    ADDI R6, R5, -1

SHIFT_LOOP:
    BLE R6, R0, WRITE_HEAD
    ADDI R7, R6, -1
    MOVL R8, 4
    MUL R9, R7, R8
    MUL R10, R6, R8

    MOVL R11, snake_x.l
    MOVH R11, snake_x.h
    ADD R12, R11, R9
    LOAD R12, R13, 0
    ADD R12, R11, R10
    STORE R12, R13, 0

    MOVL R11, snake_y.l
    MOVH R11, snake_y.h
    ADD R12, R11, R9
    LOAD R12, R13, 0
    ADD R12, R11, R10
    STORE R12, R13, 0

    DEC R6
    JMP SHIFT_LOOP

WRITE_HEAD:
    MOVL R6, snake_x.l
    MOVH R6, snake_x.h
    STORE R6, R2, 0
    MOVL R6, snake_y.l
    MOVH R6, snake_y.h
    STORE R6, R3, 0

    MOVL R6, head_x.l
    MOVH R6, head_x.h
    STORE R6, R2, 0
    MOVL R6, head_y.l
    MOVH R6, head_y.h
    STORE R6, R3, 0
    RET

; ---------------------------------------------------------------------------
; Reposiciona a comida de modo simples e deterministico.
; food_x = (food_x + 7) mod 32, food_y = (food_y + 5) mod 24.
; ---------------------------------------------------------------------------
PLACE_FOOD:
    MOVL R1, food_x.l
    MOVH R1, food_x.h
    LOAD R1, R2, 0
    ADDI R2, R2, 7
    MOVL R3, 31
    BLE R2, R3, FOOD_X_OK
    ADDI R2, R2, -32
FOOD_X_OK:
    STORE R1, R2, 0

    MOVL R1, food_y.l
    MOVH R1, food_y.h
    LOAD R1, R2, 0
    ADDI R2, R2, 5
    MOVL R3, 23
    BLE R2, R3, FOOD_Y_OK
    ADDI R2, R2, -24
FOOD_Y_OK:
    STORE R1, R2, 0
    RET

; ---------------------------------------------------------------------------
; Desenha fundo, comida, cobra e pontuacao.
; ---------------------------------------------------------------------------
DRAW_SCREEN:
    MOVL R1, PRETO.l
    MOVH R1, PRETO.h
    CLEAR R1

    ; Comida.
    MOVL R1, food_x.l
    MOVH R1, food_x.h
    LOAD R1, R2, 0
    MOVL R3, CELL
    MUL R2, R2, R3

    MOVL R1, food_y.l
    MOVH R1, food_y.h
    LOAD R1, R4, 0
    MUL R4, R4, R3

    MOVL R1, CELL
    MOVL R5, VERMELHO.l
    MOVH R5, VERMELHO.h
    RECT R2, R4, R1, R1, R5

    ; Cobra.
    MOVL R6, snake_len.l
    MOVH R6, snake_len.h
    LOAD R6, R7, 0
    MOVL R6, 0

DRAW_SNAKE_LOOP:
    BGE R6, R7, DRAW_SCORE
    MOVL R8, 4
    MUL R9, R6, R8

    MOVL R10, snake_x.l
    MOVH R10, snake_x.h
    ADD R10, R10, R9
    LOAD R10, R2, 0
    MOVL R11, CELL
    MUL R2, R2, R11

    MOVL R10, snake_y.l
    MOVH R10, snake_y.h
    ADD R10, R10, R9
    LOAD R10, R3, 0
    MUL R3, R3, R11

    MOVL R4, CELL
    BEQ R6, R0, DRAW_HEAD
    MOVL R5, VERDE.l
    MOVH R5, VERDE.h
    RECT R2, R3, R4, R4, R5
    INC R6
    JMP DRAW_SNAKE_LOOP

DRAW_HEAD:
    MOVL R5, VERDE_HEAD.l
    MOVH R5, VERDE_HEAD.h
    RECT R2, R3, R4, R4, R5
    INC R6
    JMP DRAW_SNAKE_LOOP

DRAW_SCORE:
    MOVL R1, 260
    MOVL R2, 6
    MOVL R3, score.l
    MOVH R3, score.h
    LOAD R3, R3, 0
    MOVL R4, BRANCO.l
    MOVH R4, BRANCO.h
    PINT R1, R2, R3, R4
    RET

; ---------------------------------------------------------------------------
; Tela final. Espaco reinicia a partida.
; ---------------------------------------------------------------------------
GAME_OVER:
    ; A colisao chega aqui sem RET de MOVE_SNAKE. Resetar SP evita acumular
    ; enderecos de retorno antigos quando o jogador reinicia muitas vezes.
    MOVL R14, 0
    MOVH R14, 0x0100

    MOVL R1, FUNDO_GAMEOVER.l
    MOVH R1, FUNDO_GAMEOVER.h
    CLEAR R1

    MOVL R1, 118
    MOVL R2, 96
    MOVL R3, msg_game_over.l
    MOVH R3, msg_game_over.h
    MOVL R4, BRANCO.l
    MOVH R4, BRANCO.h
    PSTR R1, R2, R3, R4

    MOVL R1, 96
    MOVL R2, 116
    MOVL R3, msg_restart.l
    MOVH R3, msg_restart.h
    MOVL R4, BRANCO.l
    MOVH R4, BRANCO.h
    PSTR R1, R2, R3, R4

WAIT_RESTART:
    MOVL R2, KEY_SPACE
    GKEY R1, R2
    BEQ R1, R0, WAIT_RESTART
    CALL INIT_GAME
    JMP GAME_LOOP
