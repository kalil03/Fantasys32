# Como Compilar — Fantasys32

Este documento explica como compilar a máquina virtual (VM) e como gerar o
binário do jogo a partir do código Assembly. A avaliação é feita em Linux.

## Dependências

- **Compilador C++** com suporte a C++17 (`g++` ou `clang++`)
- **make**
- **SDL2** (biblioteca de desenvolvimento) — vídeo, teclado e áudio

### Instalação das dependências (Ubuntu/Debian)

```sh
sudo apt-get update
sudo apt-get install -y build-essential libsdl2-dev
```

> Outras distribuições: instale o pacote equivalente da SDL2 de desenvolvimento
> (ex.: `SDL2-devel` no Fedora, `sdl2` no Arch).

## 1. Compilar a VM

```sh
cd vm
make
```

Isso gera o executável `vm` dentro da pasta `vm/`. Para limpar os arquivos
gerados:

```sh
make clean
```

## 2. Compilar o Assembler (necessário para montar o jogo)

```sh
cd assembler_src
make
```

Isso gera o executável `assembler` dentro de `assembler_src/`.

## 3. Gerar o binário do jogo (.bin)

Com o assembler já compilado, monte o código Assembly do jogo:

```sh
./assembler_src/assembler jogo/jogo.asm jogo/jogo.bin
```

Pronto. Agora é possível executar o jogo na VM (ver `README_USO`).

## Resumo rápido

```sh
# a partir da raiz do projeto
cd assembler_src && make && cd ..
cd vm && make && cd ..
./assembler_src/assembler jogo/jogo.asm jogo/jogo.bin
./vm/vm --scale 2 jogo/jogo.bin
```
