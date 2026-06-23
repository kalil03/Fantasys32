# Como Usar — Fantasys32

Este documento explica as opções de linha de comando da VM e do assembler, e
como jogar o jogo (Snake).

## Assembler

Converte um arquivo `.asm` em um `.bin` que a VM executa.

```sh
./assembler_src/assembler <arquivo.asm> <arquivo.bin>
```

Exemplo:

```sh
./assembler_src/assembler jogo/jogo.asm jogo/jogo.bin
```

## Máquina Virtual (VM)

Sintaxe básica:

```sh
./vm/vm [opções] caminho/para/arquivo.bin
```

### Opções de linha de comando

| Opção             | Descrição                                                          |
|-------------------|-------------------------------------------------------------------|
| `--scale <fator>` | Fator de escala da janela. `2` resulta em 640x480. Padrão: `1`.   |
| `--no-syscall`    | Desativa a instrução `SYSCALL` (a VM a ignora).                   |
| `--debug`         | Imprime os registradores no terminal ao final da execução.        |
| `--help`          | Exibe a mensagem de ajuda.                                         |

**Exemplos:**

```sh
./vm/vm jogo/jogo.bin
./vm/vm --scale 2 jogo/jogo.bin
./vm/vm --scale 2 --no-syscall jogo/jogo.bin
```

> Para fechar a janela da VM, clique no botão de fechar ou pressione `ESC`.

## O Jogo: Snake

### Objetivo

Controle a cobra para comer a comida vermelha. A cada comida, a cobra cresce e
a pontuação aumenta. O jogo termina se a cobra bater na parede ou no próprio
corpo.

### Controles

| Tecla            | Ação                          |
|------------------|-------------------------------|
| Seta esquerda    | Move para a esquerda          |
| Seta direita     | Move para a direita           |
| Seta cima        | Move para cima                |
| Seta baixo       | Move para baixo               |
| Espaço           | Reinicia após o "GAME OVER"   |

A cobra não pode inverter o sentido diretamente (ex.: indo para a direita, não
vira para a esquerda no mesmo instante).

### Como executar

```sh
./vm/vm --scale 2 jogo/jogo.bin
```

> O jogo funciona normalmente mesmo com `--no-syscall`, pois sua lógica não
> depende da instrução `SYSCALL`.
