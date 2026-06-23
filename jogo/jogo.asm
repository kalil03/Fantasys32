; ============================================================================
; Fantasys32 - Snake
; Objetivo: comer a comida vermelha, crescer e nao bater na parede/corpo.
;
; Controles (mapa de teclas da VM):
;   ESQUERDA=0, DIREITA=1, CIMA=2, BAIXO=3, ESPACO=4
;
; Desenho INCREMENTAL (sem CLEAR a cada passo): a cada movimento apaga-se
; apenas a cauda e desenha-se a nova cabeca -> imagem estavel, sem piscar.
; A velocidade e controlada por um contador (move a cada SPEED ciclos).
;
; Convencoes do assembler usadas aqui:
;   ADD/SUB/MUL Rd, Ra, Rb  -> Rd = Ra (op) Rb        (destino primeiro)
;   LOAD  base, dest, i     -> dest = Mem[base + i*4] (base primeiro)
;   STORE base, val,  i     -> Mem[base + i*4] = val  (base primeiro)
; ============================================================================

.data
.equ GW, 32
.equ GH, 24
.equ CELL, 10
.equ MAXLEN, 64
.equ SPEED, 8            ; move a cada 8 ciclos (~7-8 passos/seg)

.equ K_LEFT, 0
.equ K_RIGHT, 1
.equ K_UP, 2
.equ K_DOWN, 3
.equ K_SPACE, 4

.equ BG, 0xFF101018      ; fundo
.equ COR_BODY, 0xFF22CC44
.equ COR_HEAD, 0xFF88FF88
.equ COR_FOOD, 0xFFEE3030
.equ COR_TXT, 0xFFFFFFFF
.equ GAMEOVER_BG, 0xFF330000

len: .var 3
dx: .var 1
dy: .var 0
fx: .var 20
fy: .var 12
score: .var 0
tick: .var 0

; temporarios persistidos em memoria (sobrevivem aos CALLs de desenho)
nhx: .var 0              ; nova cabeca
nhy: .var 0
ohx: .var 0              ; cabeca antiga
ohy: .var 0
otx: .var 0              ; cauda antiga
oty: .var 0
grew: .var 0            ; 1 se cresceu neste passo

sx: .space MAXLEN
sy: .space MAXLEN

msg_title: .string "SNAKE"
msg_start: .string "ESPACO PARA COMECAR"
msg_go: .string "GAME OVER"
msg_pt: .string "PONTOS"
msg_rs: .string "APERTE ESPACO"

.text
START:
    CALL TELA_INICIAL       ; menu inicial: espera ESPACO
    CALL INIT

LOOP:
    CALL READ_INPUT

    ; --- controle de velocidade: so move quando tick chega em SPEED ---
    MOVL R1, tick.l
    MOVH R1, tick.h
    LOAD R1, R2, 0          ; R2 = tick
    INC R2
    MOVL R3, SPEED
    BLT R2, R3, GUARDA_TICK ; tick < SPEED -> ainda nao move
    MOVL R2, 0              ; reseta tick
    STORE R1, R2, 0
    CALL STEP
    JMP LOOP
GUARDA_TICK:
    STORE R1, R2, 0
    JMP LOOP

; ---------------------------------------------------------------------------
; TELA_INICIAL: menu de abertura. Mostra o titulo e espera ESPACO (start).
; ---------------------------------------------------------------------------
TELA_INICIAL:
    ; fundo
    MOVL R1, BG.l
    MOVH R1, BG.h
    CLEAR R1
    ; titulo "SNAKE" (5 chars = 80px -> x = (320-80)/2 = 120)
    MOVL R1, 120
    MOVL R2, 72
    MOVL R3, msg_title.l
    MOVH R3, msg_title.h
    MOVL R4, COR_HEAD.l
    MOVH R4, COR_HEAD.h
    PSTR R1, R2, R3, R4
    ; "ESPACO PARA COMECAR" (19 chars = 304px -> x = (320-304)/2 = 8)
    MOVL R1, 8
    MOVL R2, 132
    MOVL R3, msg_start.l
    MOVH R3, msg_start.h
    MOVL R4, COR_TXT.l
    MOVH R4, COR_TXT.h
    PSTR R1, R2, R3, R4
TELA_WAIT:
    MOVL R2, K_SPACE
    GKEY R1, R2
    BEQ R1, R0, TELA_WAIT   ; enquanto ESPACO nao for pressionado, espera
    RET

; ---------------------------------------------------------------------------
; INIT: estado inicial + desenho do quadro inicial (uma unica vez).
; ---------------------------------------------------------------------------
INIT:
    ; valores iniciais
    MOVL R1, len.l
    MOVH R1, len.h
    MOVL R2, 3
    STORE R1, R2, 0
    MOVL R1, dx.l
    MOVH R1, dx.h
    MOVL R2, 1
    STORE R1, R2, 0
    MOVL R1, dy.l
    MOVH R1, dy.h
    MOVL R2, 0
    STORE R1, R2, 0
    MOVL R1, fx.l
    MOVH R1, fx.h
    MOVL R2, 20
    STORE R1, R2, 0
    MOVL R1, fy.l
    MOVH R1, fy.h
    MOVL R2, 12
    STORE R1, R2, 0
    MOVL R1, score.l
    MOVH R1, score.h
    MOVL R2, 0
    STORE R1, R2, 0
    MOVL R1, tick.l
    MOVH R1, tick.h
    MOVL R2, 0
    STORE R1, R2, 0

    ; corpo inicial: (8,12),(7,12),(6,12)
    MOVL R1, sx.l
    MOVH R1, sx.h
    MOVL R2, 8
    STORE R1, R2, 0
    MOVL R2, 7
    STORE R1, R2, 1
    MOVL R2, 6
    STORE R1, R2, 2
    MOVL R1, sy.l
    MOVH R1, sy.h
    MOVL R2, 12
    STORE R1, R2, 0
    STORE R1, R2, 1
    STORE R1, R2, 2

    ; semente do gerador aleatorio
    MOVL R1, 12345
    SRAND R1

    ; fundo
    MOVL R1, BG.l
    MOVH R1, BG.h
    CLEAR R1

    ; desenha a cobra inicial (cabeca + 2 corpos)
    MOVL R1, 8
    MOVL R2, 12
    MOVL R3, COR_HEAD.l
    MOVH R3, COR_HEAD.h
    CALL DRAW_CELL
    MOVL R1, 7
    MOVL R2, 12
    MOVL R3, COR_BODY.l
    MOVH R3, COR_BODY.h
    CALL DRAW_CELL
    MOVL R1, 6
    MOVL R2, 12
    MOVL R3, COR_BODY.l
    MOVH R3, COR_BODY.h
    CALL DRAW_CELL

    ; desenha a comida
    MOVL R1, 20
    MOVL R2, 12
    MOVL R3, COR_FOOD.l
    MOVH R3, COR_FOOD.h
    CALL DRAW_CELL

    CALL DRAW_SCORE
    RET

; ---------------------------------------------------------------------------
; READ_INPUT: atualiza dx/dy. Nao permite inverter o sentido diretamente.
; ---------------------------------------------------------------------------
READ_INPUT:
    MOVL R2, K_LEFT
    GKEY R1, R2
    BNE R1, R0, IN_LEFT
    MOVL R2, K_RIGHT
    GKEY R1, R2
    BNE R1, R0, IN_RIGHT
    MOVL R2, K_UP
    GKEY R1, R2
    BNE R1, R0, IN_UP
    MOVL R2, K_DOWN
    GKEY R1, R2
    BNE R1, R0, IN_DOWN
    RET

IN_LEFT:
    ; se dx==1 (indo p/ direita) ignora
    MOVL R1, dx.l
    MOVH R1, dx.h
    LOAD R1, R2, 0
    MOVL R3, 1
    BEQ R2, R3, IN_FIM
    MOVL R2, 0
    DEC R2                  ; R2 = -1
    CALL SET_DIR_X
    RET
IN_RIGHT:
    MOVL R1, dx.l
    MOVH R1, dx.h
    LOAD R1, R2, 0
    MOVL R3, 0
    DEC R3                  ; R3 = -1
    BEQ R2, R3, IN_FIM
    MOVL R2, 1
    CALL SET_DIR_X
    RET
IN_UP:
    MOVL R1, dy.l
    MOVH R1, dy.h
    LOAD R1, R2, 0
    MOVL R3, 1
    BEQ R2, R3, IN_FIM
    MOVL R2, 0
    DEC R2                  ; R2 = -1
    CALL SET_DIR_Y
    RET
IN_DOWN:
    MOVL R1, dy.l
    MOVH R1, dy.h
    LOAD R1, R2, 0
    MOVL R3, 0
    DEC R3                  ; R3 = -1
    BEQ R2, R3, IN_FIM
    MOVL R2, 1
    CALL SET_DIR_Y
    RET
IN_FIM:
    RET

; Define dx=R2, dy=0
SET_DIR_X:
    MOVL R1, dx.l
    MOVH R1, dx.h
    STORE R1, R2, 0
    MOVL R1, dy.l
    MOVH R1, dy.h
    MOVL R2, 0
    STORE R1, R2, 0
    RET
; Define dy=R2, dx=0
SET_DIR_Y:
    MOVL R1, dy.l
    MOVH R1, dy.h
    STORE R1, R2, 0
    MOVL R1, dx.l
    MOVH R1, dx.h
    MOVL R2, 0
    STORE R1, R2, 0
    RET

; ---------------------------------------------------------------------------
; STEP: um movimento da cobra (logica + desenho incremental).
; ---------------------------------------------------------------------------
STEP:
    ; nova cabeca = cabeca + dir
    MOVL R1, sx.l
    MOVH R1, sx.h
    LOAD R1, R2, 0          ; R2 = head x
    MOVL R1, dx.l
    MOVH R1, dx.h
    LOAD R1, R3, 0
    ADD R2, R2, R3          ; R2 = novo x
    MOVL R1, nhx.l
    MOVH R1, nhx.h
    STORE R1, R2, 0

    MOVL R1, sy.l
    MOVH R1, sy.h
    LOAD R1, R4, 0          ; head y
    MOVL R1, dy.l
    MOVH R1, dy.h
    LOAD R1, R5, 0
    ADD R4, R4, R5          ; R4 = novo y
    MOVL R1, nhy.l
    MOVH R1, nhy.h
    STORE R1, R4, 0

    ; colisao com paredes
    BLT R2, R0, GAME_OVER
    BLT R4, R0, GAME_OVER
    MOVL R6, 31
    BGT R2, R6, GAME_OVER
    MOVL R6, 23
    BGT R4, R6, GAME_OVER

    ; colisao com o corpo (ignora o ultimo segmento, que vai sair)
    MOVL R1, len.l
    MOVH R1, len.h
    LOAD R1, R7, 0          ; R7 = len
    MOVL R8, 1
    SUB R9, R7, R8          ; R9 = len-1
    MOVL R6, 0             ; i
COL_LOOP:
    BGE R6, R9, COL_OK
    MOVL R8, 4
    MUL R10, R6, R8        ; offset i
    MOVL R1, sx.l
    MOVH R1, sx.h
    ADD R11, R1, R10
    LOAD R11, R12, 0       ; sx[i]
    BNE R12, R2, COL_NEXT
    MOVL R1, sy.l
    MOVH R1, sy.h
    ADD R11, R1, R10
    LOAD R11, R12, 0       ; sy[i]
    BEQ R12, R4, GAME_OVER
COL_NEXT:
    INC R6
    JMP COL_LOOP
COL_OK:
    ; guarda cabeca antiga (sx[0],sy[0])
    MOVL R1, sx.l
    MOVH R1, sx.h
    LOAD R1, R2, 0
    MOVL R1, ohx.l
    MOVH R1, ohx.h
    STORE R1, R2, 0
    MOVL R1, sy.l
    MOVH R1, sy.h
    LOAD R1, R2, 0
    MOVL R1, ohy.l
    MOVH R1, ohy.h
    STORE R1, R2, 0

    ; guarda cauda antiga (sx[len-1],sy[len-1])
    MOVL R1, len.l
    MOVH R1, len.h
    LOAD R1, R7, 0
    MOVL R8, 1
    SUB R9, R7, R8
    MOVL R8, 4
    MUL R10, R9, R8
    MOVL R1, sx.l
    MOVH R1, sx.h
    ADD R11, R1, R10
    LOAD R11, R12, 0
    MOVL R1, otx.l
    MOVH R1, otx.h
    STORE R1, R12, 0
    MOVL R1, sy.l
    MOVH R1, sy.h
    ADD R11, R1, R10
    LOAD R11, R12, 0
    MOVL R1, oty.l
    MOVH R1, oty.h
    STORE R1, R12, 0

    ; grew = 0 por padrao
    MOVL R1, grew.l
    MOVH R1, grew.h
    MOVL R2, 0
    STORE R1, R2, 0

    ; comeu? nova cabeca == comida
    MOVL R1, fx.l
    MOVH R1, fx.h
    LOAD R1, R5, 0
    MOVL R1, nhx.l
    MOVH R1, nhx.h
    LOAD R1, R2, 0
    BNE R2, R5, FEZ_SHIFT
    MOVL R1, fy.l
    MOVH R1, fy.h
    LOAD R1, R6, 0
    MOVL R1, nhy.l
    MOVH R1, nhy.h
    LOAD R1, R4, 0
    BNE R4, R6, FEZ_SHIFT

    ; --- COMEU ---
    MOVL R1, score.l
    MOVH R1, score.h
    LOAD R1, R7, 0
    INC R7
    STORE R1, R7, 0
    ; som curto
    MOVL R1, 880
    MOVL R2, 50
    MOVL R3, 1
    PLAY R1, R2, R3
    ; cresce se houver espaco
    MOVL R1, len.l
    MOVH R1, len.h
    LOAD R1, R7, 0
    MOVL R8, MAXLEN
    BGE R7, R8, SEM_CRESCER
    INC R7
    STORE R1, R7, 0
    MOVL R1, grew.l
    MOVH R1, grew.h
    MOVL R2, 1
    STORE R1, R2, 0
SEM_CRESCER:
    ; nova comida + desenho da comida + placar
    CALL PLACE_FOOD
    MOVL R5, fx.l
    MOVH R5, fx.h
    LOAD R5, R1, 0
    MOVL R5, fy.l
    MOVH R5, fy.h
    LOAD R5, R2, 0
    MOVL R3, COR_FOOD.l
    MOVH R3, COR_FOOD.h
    CALL DRAW_CELL
    CALL DRAW_SCORE

FEZ_SHIFT:
    ; desloca o corpo: para i de len-1 ate 1: s[i] = s[i-1]
    MOVL R1, len.l
    MOVH R1, len.h
    LOAD R1, R7, 0
    MOVL R8, 1
    SUB R6, R7, R8         ; i = len-1
SHIFT:
    BLE R6, R0, WRITE_HEAD
    MOVL R8, 1
    SUB R9, R6, R8         ; i-1
    MOVL R8, 4
    MUL R10, R6, R8        ; off i
    MUL R11, R9, R8        ; off i-1
    MOVL R1, sx.l
    MOVH R1, sx.h
    ADD R12, R1, R11
    LOAD R12, R13, 0
    ADD R12, R1, R10
    STORE R12, R13, 0
    MOVL R1, sy.l
    MOVH R1, sy.h
    ADD R12, R1, R11
    LOAD R12, R13, 0
    ADD R12, R1, R10
    STORE R12, R13, 0
    DEC R6
    JMP SHIFT

WRITE_HEAD:
    MOVL R1, nhx.l
    MOVH R1, nhx.h
    LOAD R1, R2, 0
    MOVL R1, sx.l
    MOVH R1, sx.h
    STORE R1, R2, 0
    MOVL R1, nhy.l
    MOVH R1, nhy.h
    LOAD R1, R2, 0
    MOVL R1, sy.l
    MOVH R1, sy.h
    STORE R1, R2, 0

    ; --- desenho incremental (em rajada, sem CLEAR) ---
    ; apaga a cauda antiga se nao cresceu
    MOVL R1, grew.l
    MOVH R1, grew.h
    LOAD R1, R5, 0
    BNE R5, R0, PULA_APAGA
    MOVL R5, otx.l
    MOVH R5, otx.h
    LOAD R5, R1, 0
    MOVL R5, oty.l
    MOVH R5, oty.h
    LOAD R5, R2, 0
    MOVL R3, BG.l
    MOVH R3, BG.h
    CALL DRAW_CELL
PULA_APAGA:
    ; cabeca antiga vira corpo
    MOVL R5, ohx.l
    MOVH R5, ohx.h
    LOAD R5, R1, 0
    MOVL R5, ohy.l
    MOVH R5, ohy.h
    LOAD R5, R2, 0
    MOVL R3, COR_BODY.l
    MOVH R3, COR_BODY.h
    CALL DRAW_CELL
    ; nova cabeca
    MOVL R5, nhx.l
    MOVH R5, nhx.h
    LOAD R5, R1, 0
    MOVL R5, nhy.l
    MOVH R5, nhy.h
    LOAD R5, R2, 0
    MOVL R3, COR_HEAD.l
    MOVH R3, COR_HEAD.h
    CALL DRAW_CELL
    RET

; ---------------------------------------------------------------------------
; PLACE_FOOD: sorteia nova posicao da comida.
; ---------------------------------------------------------------------------
PLACE_FOOD:
    MOVL R2, 0
    MOVL R3, 31
    RAND R4, R2, R3
    MOVL R1, fx.l
    MOVH R1, fx.h
    STORE R1, R4, 0
    MOVL R2, 0
    MOVL R3, 23
    RAND R4, R2, R3
    MOVL R1, fy.l
    MOVH R1, fy.h
    STORE R1, R4, 0
    RET

; ---------------------------------------------------------------------------
; DRAW_CELL: desenha uma celula 10x10. R1=coluna, R2=linha, R3=cor.
; ---------------------------------------------------------------------------
DRAW_CELL:
    MOVL R4, CELL
    MUL R5, R1, R4         ; x = coluna*10
    MUL R6, R2, R4         ; y = linha*10
    MOVL R7, CELL          ; largura/altura
    RECT R5, R6, R7, R7, R3
    RET

; ---------------------------------------------------------------------------
; DRAW_SCORE: limpa a area e imprime a pontuacao no canto superior esquerdo.
; ---------------------------------------------------------------------------
DRAW_SCORE:
    MOVL R1, 4
    MOVL R2, 4
    MOVL R3, 60
    MOVL R4, 16
    MOVL R5, BG.l
    MOVH R5, BG.h
    RECT R1, R2, R3, R4, R5
    MOVL R1, score.l
    MOVH R1, score.h
    LOAD R1, R6, 0
    MOVL R1, 4
    MOVL R2, 4
    MOVL R4, COR_TXT.l
    MOVH R4, COR_TXT.h
    PINT R1, R2, R6, R4
    RET

; ---------------------------------------------------------------------------
; GAME_OVER: tela final; ESPACO reinicia.
; ---------------------------------------------------------------------------
GAME_OVER:
    ; reseta a pilha (descarta retornos antigos) para reiniciar limpo
    MOVL R14, 0
    MOVH R14, 0x0100
    ; som grave
    MOVL R1, 140
    MOVL R2, 300
    MOVL R3, 1
    PLAY R1, R2, R3
    ; fundo vermelho escuro
    MOVL R1, GAMEOVER_BG.l
    MOVH R1, GAMEOVER_BG.h
    CLEAR R1
    ; "GAME OVER" (9 chars = 144px -> x = (320-144)/2 = 88)
    MOVL R1, 88
    MOVL R2, 64
    MOVL R3, msg_go.l
    MOVH R3, msg_go.h
    MOVL R4, COR_TXT.l
    MOVH R4, COR_TXT.h
    PSTR R1, R2, R3, R4
    ; "PONTOS" (6 chars = 96px -> x = (320-96)/2 = 112)
    MOVL R1, 112
    MOVL R2, 104
    MOVL R3, msg_pt.l
    MOVH R3, msg_pt.h
    MOVL R4, COR_TXT.l
    MOVH R4, COR_TXT.h
    PSTR R1, R2, R3, R4
    ; valor da pontuacao (centralizado p/ 1-2 digitos)
    MOVL R1, score.l
    MOVH R1, score.h
    LOAD R1, R6, 0
    MOVL R1, 148
    MOVL R2, 128
    MOVL R4, COR_TXT.l
    MOVH R4, COR_TXT.h
    PINT R1, R2, R6, R4
    ; "APERTE ESPACO" (13 chars = 208px -> x = (320-208)/2 = 56)
    MOVL R1, 56
    MOVL R2, 168
    MOVL R3, msg_rs.l
    MOVH R3, msg_rs.h
    MOVL R4, COR_TXT.l
    MOVH R4, COR_TXT.h
    PSTR R1, R2, R3, R4
WAIT_RESTART:
    MOVL R2, K_SPACE
    GKEY R1, R2
    BEQ R1, R0, WAIT_RESTART
    CALL INIT
    JMP LOOP
