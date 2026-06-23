# Escopo — Fantasys32 (VM + Jogo)

Documento de planejamento. **Sem implementação nesta fase** — serve para organizar
ideias e decidir o que faremos. Meta: código **simples, funcional e que eu entenda
100%** (haverá prova individual sobre o trabalho).

---

## 1. O que o trabalho pede

Duas entregas:

1. **VM** (Máquina Virtual) em **C++** — executa o `.bin` gerado pelo assembler.
2. **Jogo** em Assembly Fantasys32 — clone de um clássico (Snake, Space Invaders,
   Flappy Bird, Breakout, Frogger ou Combat).

O **assembler já foi fornecido** pelo professor (`assembler_src/`). Não precisamos
implementá-lo, só saber usá-lo (`.asm` → `.bin`).

Entrega final: `.zip` com pastas `vm/` e `jogo/` + `README_COMPILAR.pdf` + `README_USO.pdf`.

---

## 2. Especificação resumida (referência rápida)

- **Arquitetura:** RISC 32 bits, big-endian, inspirada em MIPS.
- **Registradores:** 16 × 32 bits. R0 = sempre 0. R14 = SP. R15 = PC.
- **Memória:** 16 MB. Palavras de 4 bytes, tudo alinhado a 4 (acesso desalinhado = erro).
  - `0x000000–0xFB3FFF`: código + dados
  - `0xFB4000–0xFFEFFF`: vídeo (framebuffer 320×240, ARGB)
  - `0xFFF000–0xFFFFFF`: pilha (4 KB), SP começa em 0x00FFFFFF e cresce p/ baixo
- **Vídeo:** 320×240, pixel ARGB `0xAARRGGBB`. Pixel (x,y) em `0xFB4000+(y*320+x)*4`.
- **Tempo:** 60 FPS, 104 instruções por frame.
- **Imediatos de endereço** (LOAD/STORE/JMP/CALL/branch) são em **palavras** → VM multiplica por 4.

### Formatos de instrução (32 bits)
| Tipo | 31-26 | 25-22 | 21-18 | 17-14 | 13-10 | 9-6 | 5-0 |
|---|---|---|---|---|---|---|---|
| R | opcode | rs | rt | rd | 0 | 0 | 0 |
| I | opcode | rs | rt | imediato 18 bits ||||
| J | opcode | deslocamento 26 bits |||||
| S | opcode | ra | rb | rc | rd | re | 0 |
| U | opcode | rd | 0 (22 bits) |||||

### Lista de instruções (opcodes)
- Aritm./lógica (R): `00 ADD`,`01 SUB`,`02 MUL`,`03 DIV`,`04 MOD`,`05 AND`,`06 OR`,
  `07 XOR`,`08 SHL`,`09 SHR`,`0A ROL`,`0B ROR`. Com sinal, overflow trunca em 32 bits. Shift mascara `&0x1F`.
- `0C ADDI` (I).
- Memória (I): `0D MOVL`,`0E MOVH`,`0F LOAD`,`10 STORE`.
- Fluxo: `11 BEQ`,`12 BNE`,`13 BLT`,`14 BGT`,`15 BLE`,`16 BGE` (I, relativo a PC+desloc*4),
  `17 JMP`,`18 CALL` (J, absoluto).
- Pilha/unárias (U): `19 PUSH`,`1A POP`,`1B INC`,`1C DEC`,`1D NOT`,`1E RET`.
- Sistema/E-S (S): `1F RECT`,`20 DSPRITE`,`21 CLEAR`,`22 GKEY`,`23 PLAY`,`24 SLEEP`,
  `25 PSTR`,`26 PINT`,`27 SYSCALL`,`28 SRAND`,`29 RAND`,`2A FRAMENUM`,`2B HALT`.

### Erros que abortam a VM
- Acesso desalinhado, endereço fora de `0x000000–0xFFFFFF`, stack overflow/underflow.

---

## 3. Decisões
- [x] **Jogo: Snake** (grid simples, cobra cresce, colisão com parede/corpo, comida).
- [x] **VM: portar o esqueleto C → C++** (reaproveita loader, big-endian e MOVL).
- [ ] "104 instruções por frame" vs "por segundo" — assumir **por frame** (confirmar c/ prof).
- [x] SDL2 (o exemplo `sdl.c` já é SDL2).

---

## 4. Plano da VM (`vm/`)

Portar o esqueleto C → C++ organizado. Módulos sugeridos:

- `main.cpp` — parsing de args (`--scale`, `--no-syscall`, `--help`), carrega `.bin`, roda loop.
- `vm.hpp/.cpp` — estado (regs, memória), fetch/decode/execute, erros.
- `video.cpp` — framebuffer, RECT/CLEAR/DSPRITE/PSTR/PINT, fonte bitmap.
- `audio.cpp` — PLAY (seno/quadrada/triangular/ruído), não-bloqueante.
- `input.cpp` — mapeamento das 16 teclas → GKEY.
- `Makefile`.

### Ordem de implementação (incremental, testável a cada passo)
1. Loader `.bin` + fetch/decode/execute + R0=0 + checagem de alinhamento/limites.
2. Aritmética/lógica + MOVL/MOVH + LOAD/STORE.
3. Controle de fluxo (branches, JMP, CALL, RET) + pilha (PUSH/POP).
4. Janela SDL + framebuffer + CLEAR/RECT + loop 60 FPS / 104 instr.
5. Teclado (GKEY) + HALT + SLEEP + FRAMENUM.
6. PSTR/PINT (fonte bitmap) + DSPRITE.
7. RAND/SRAND (LCG) + PLAY (áudio) + SYSCALL (com `--no-syscall`).

> Já pronto no esqueleto: loader, `lerMemoria`/`escreverMemoria` (big-endian),
> `VM_ClearFramebuffer`, MOVL. Reaproveitar.

---

## 5. Plano do Jogo (`jogo/`)
- Escrever `jogo.asm` (seção `.data` obrigatória + `.text`).
- Montar com o assembler fornecido → `jogo.bin`.
- Usar só instruções básicas na lógica (SYSCALL só p/ debug; jogo deve rodar com `--no-syscall`).
- Precisa ter arte gráfica e som coerentes, mesmo simples.

---

## 6. Fases de Implementação

Cada fase entrega algo testável. Só passar para a próxima quando a atual funcionar.

### Fase 0 — Preparação ✅
- [x] Criar estrutura de pastas `vm/` e `jogo/`.
- [x] Portar esqueleto C → C++ (classe `VM` em `vm.hpp`/`vm.cpp`, `main.cpp`, Makefile g++ -std=c++17).
- [x] Assembler do professor compilado; `prog1.asm` montado → `jogo/prog1.bin` (bate com a spec).
- [x] VM compila limpa (`-Wall -Wextra`) e executa MOVL/HALT.

### Fase 1 — Núcleo da CPU (sem gráficos) ✅
- [x] Loader do `.bin` (cabeçalho + dados + código, big-endian).
- [x] Ciclo fetch → decode → execute.
- [x] R0 sempre 0; checagem de **alinhamento** e **limites de endereço**.
- [x] Instruções: aritmética/lógica (00–0C), MOVL/MOVH, LOAD/STORE, HALT.
- [x] Teste `teste_fase1.asm` validado com `./vm --debug` (registradores corretos).

> **Importante (convenção do assembler):** as posições de bits dos registradores
> diferem por formato! Tipo R: `[25:22]=rd, [21:18]=rs, [17:14]=rt` (1º operando é o
> destino). Tipo I: `[25:22]=rs, [21:18]=rt`. Em LOAD/STORE o 1º operando é o
> registrador-base e o 2º é o dado. A VM segue o assembler, não a tabela da spec.

### Fase 2 — Controle de fluxo e pilha ✅
- [x] Branches (BEQ…BGE), JMP, CALL, RET.
- [x] PUSH/POP/INC/DEC/NOT + checagem de stack overflow/underflow.
- [x] Teste `teste_fase2.asm`: loop com branch, PUSH/POP, CALL/RET, NOT (R3=30, SP balanceado).

### Fase 3 — Vídeo e loop principal
- Janela SDL2 + textura framebuffer + escala (`--scale`).
- Loop 60 FPS executando 104 instruções/frame.
- CLEAR, RECT; HALT encerra limpo.
- Teste: desenhar retângulos na tela.

### Fase 4 — Entrada e temporização
- Mapeamento das 16 teclas → GKEY; SLEEP; FRAMENUM.
- Teste: mover um retângulo com as setas (estilo `sdl.c`).

### Fase 5 — Texto, sprites e extras
- PSTR/PINT com fonte bitmap (`Px437Acer710CGA.png`), DSPRITE com transparência.
- RAND/SRAND (LCG), PLAY (áudio não-bloqueante), SYSCALL + `--no-syscall`.
- **VM completa.**

### Fase 6 — Jogo Snake (Assembly)
- `jogo.asm`: `.data` (grid, cobra, comida, cores) + `.text`.
- Lógica: movimento, crescer ao comer, colisão parede/corpo, RAND p/ comida.
- Render com RECT/CLEAR; placar com PINT; som com PLAY.
- Rodar com `--no-syscall` para garantir independência de syscalls.

### Fase 7 — Empacotamento e entrega
- README_COMPILAR.pdf, README_USO.pdf, `.zip` com `vm/` e `jogo/`.
- Testar compilação limpa no Linux (g++ + SDL2).

---

## 7. Entregáveis finais
- [ ] `vm/` (C++, Makefile, compila com g++ no Linux)
- [ ] `jogo/` (`.asm` + `.bin`)
- [ ] `README_COMPILAR.pdf` (dependências: g++, SDL2; comandos exatos)
- [ ] `README_USO.pdf` (opções da VM; controles e objetivo do jogo)
- [ ] `.zip` final
