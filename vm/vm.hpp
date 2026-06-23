#pragma once
#include <cstdint>
#include <string>
#include <SDL2/SDL.h>

// ============================================================================
// Constantes da arquitetura Fantasys32
// ============================================================================
constexpr uint32_t TAM_MEM       = 16 * 1024 * 1024; // 16 MB endereçáveis
constexpr int      NUM_REGS      = 16;
constexpr int      R0            = 0;   // Constante 0
constexpr int      SP            = 14;  // Stack Pointer
constexpr int      PC            = 15;  // Program Counter

// Mapa de memória
constexpr uint32_t FRAMEBUFFER_INICIO = 0x00FB4000;
constexpr uint32_t PILHA_BASE         = 0x00FFF000; // limite inferior da pilha

// Vídeo
constexpr int VIDEO_LARG = 320;
constexpr int VIDEO_ALT  = 240;
constexpr int FRAMEBUFFER_PIXELS = VIDEO_LARG * VIDEO_ALT;

// Clock da VM (conforme a especificação): 60 ciclos por segundo, executando
// 104 instruções por ciclo. A tela é redesenhada a cada ciclo (60x por segundo).
constexpr int FPS                = 60;   // ciclos por segundo
constexpr int INSTR_POR_FRAME    = 104;  // instruções por ciclo

// A classe representa todo o estado da máquina virtual.
class VM {
public:
    VM();
    ~VM();

    // Carrega um arquivo .bin (cabeçalho + dados + código) na memória.
    void carregarCodigo(const std::string& arqBin);

    // Configuração vinda da linha de comando.
    void setEscala(int e)      { escala = (e < 1 ? 1 : e); }
    void setSemSyscall(bool b) { semSyscall = b; }

    // Loop principal: cria a janela SDL e executa até HALT ou fechar a janela.
    void executar();

    // Utilidade de depuração.
    void imprimirRegistradores() const;

private:
    int32_t  regs[NUM_REGS];
    uint8_t* mem;
    bool     executando;

    // Configuração
    int  escala     = 1;
    bool semSyscall = false;

    // Estado de vídeo (SDL)
    SDL_Window*   janela    = nullptr;
    SDL_Renderer* renderer  = nullptr;
    SDL_Texture*  textura   = nullptr;

    // Estado de áudio (SDL)
    SDL_AudioDeviceID audioDev = 0;

    // Tempo / aleatoriedade
    uint32_t numFrames   = 0;   // frames renderizados (FRAMENUM)
    uint64_t dormirAteMs = 0;   // instante até o qual SLEEP pausa a execução
    uint32_t rngState    = 1;   // estado do gerador LCG (SRAND/RAND)

    // --- Núcleo ---
    void executarInstrucao();
    uint32_t lerMemoria(uint32_t endereco) const;
    void     escreverMemoria(uint32_t endereco, uint32_t valor);
    void     empilhar(uint32_t valor);
    uint32_t desempilhar();
    void erro(const std::string& msg, uint32_t endereco) const;
    void checarAlinhamento(uint32_t endereco) const;

    // --- Vídeo / SDL ---
    void inicializarSDL();
    void finalizarSDL();
    void processarEventos();
    void renderizar();
    uint32_t* framebuffer() { return (uint32_t*)(mem + FRAMEBUFFER_INICIO); }
    void pintarPixel(int x, int y, uint32_t cor);
    void desenharRect(int x, int y, int w, int h, uint32_t cor);
    void desenharSprite(int x, int y, int w, int h, uint32_t addr);
    void desenharChar(int x, int y, char ch, uint32_t cor);
    void desenharString(int x, int y, uint32_t addr, uint32_t cor);
    void desenharInt(int x, int y, int32_t valor, uint32_t cor);

    // --- E/S ---
    bool teclaPressionada(int id) const;
    void tocarSom(int freq, int ms, int forma);
    uint32_t proximoAleatorio(); // avança o LCG
};
