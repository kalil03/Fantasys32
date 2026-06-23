# Jogo - Snake Fantasys32

Arquivo principal: `jogo.asm`.

## Como funciona

O jogo usa uma grade de 32 x 24 casas. Cada casa tem 10 pixels, entao ocupa a tela
320 x 240 inteira. A cobra guarda suas posicoes em dois vetores:

- `snake_x`: coluna de cada pedaco da cobra.
- `snake_y`: linha de cada pedaco da cobra.

A cabeca fica sempre no indice 0. A cada rodada:

1. Le o teclado com `GKEY`.
2. Calcula a nova posicao da cabeca usando `dir_x` e `dir_y`.
3. Verifica colisao com parede e corpo.
4. Se comeu a comida, aumenta `snake_len` e incrementa `score`.
5. Desloca o corpo do fim para o inicio.
6. Desenha tudo com `CLEAR`, `RECT`, `PINT` e `PSTR`.

## Controles

O arquivo assume este mapeamento:

- `KEY_LEFT = 0`
- `KEY_RIGHT = 1`
- `KEY_UP = 2`
- `KEY_DOWN = 3`
- `KEY_SPACE = 4`

Se a VM usar outro mapeamento, altere apenas os `.equ KEY_*` no inicio de
`jogo.asm`.

## Montagem

Depois de compilar o assembler:

```sh
../assembler_src/assembler jogo.asm jogo.bin
```

No Windows, se o executavel estiver como `assembler.exe`:

```powershell
..\assembler_src\assembler.exe jogo.asm jogo.bin
```
