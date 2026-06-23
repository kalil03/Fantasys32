# Fantasys32

Implementação de uma **máquina virtual (VM)** de 32 bits (arquitetura RISC inspirada
no MIPS) e de um **jogo** escrito em Assembly para essa máquina.

O projeto tem duas partes:

- **VM** (`vm/`) — escrita em C++. Carrega um arquivo `.bin` e executa as instruções,
  manipulando registradores, memória, vídeo, teclado e som.
- **Jogo** (`jogo/`) — clone de **Snake** em Assembly Fantasys32 (em desenvolvimento).

O **assembler** (`assembler_src/`, fornecido pelo professor) converte os programas
`.asm` em `.bin` que a VM executa.

## Estrutura do projeto

```
vm/             VM em C++ (main.cpp, vm.cpp, vm.hpp, Makefile)
jogo/           Jogo em Assembly (.asm) e seu binário montado
assembler_src/  Assembler (.asm -> .bin)
ESCOPO.md       Planejamento e fases de implementação
```

## Dependências

- Compilador C++ com suporte a C++17 (`g++` ou `clang++`)
- `make`
- SDL2 (necessária a partir da Fase 3, para vídeo/áudio/teclado)

No Ubuntu/Debian:

```sh
sudo apt install build-essential libsdl2-dev
```

## Como compilar

**Assembler** (uma vez, para gerar binários):

```sh
cd assembler_src
make
```

**VM:**

```sh
cd vm
make
```

## Como rodar

1. Monte um programa `.asm` em `.bin` com o assembler:

```sh
./assembler_src/assembler programa.asm programa.bin
```

2. Execute o `.bin` na VM:

```sh
./vm/vm programa.bin
```

### Opções da VM

| Opção             | Descrição                                            |
|-------------------|------------------------------------------------------|
| `--scale <fator>` | Fator de escala da janela (padrão: 1).               |
| `--no-syscall`    | Ignora a instrução `SYSCALL`.                        |
| `--debug`         | Imprime os registradores ao final da execução.       |
| `--help`          | Exibe a ajuda.                                        |

### Exemplo rápido (teste do núcleo da CPU)

```sh
cd vm
make
../assembler_src/assembler teste_fase1.asm teste_fase1.bin
./vm --debug teste_fase1.bin
```

## Estado atual

- [x] **Fase 0** — Estrutura do projeto e portabilidade para C++
- [x] **Fase 1** — Núcleo da CPU (aritmética/lógica, MOVL/MOVH, LOAD/STORE)
- [x] **Fase 2** — Controle de fluxo (branches, JMP/CALL/RET) e pilha
- [x] **Fase 3** — Vídeo (SDL2, framebuffer, loop 60 FPS, CLEAR/RECT)
- [x] **Fase 4** — Teclado (GKEY), SLEEP, FRAMENUM
- [x] **Fase 5** — Texto (PSTR/PINT), DSPRITE, RAND, PLAY, SYSCALL — **VM completa**
- [ ] **Fase 6** — Jogo Snake (ajustes e testes na tela real)

Detalhes em [`ESCOPO.md`](ESCOPO.md).
