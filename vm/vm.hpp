#pragma once
#include <cstdint>
#include <string>

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
constexpr uint32_t FRAMEBUFFER_FIM    = 0x00FFEFFF;
constexpr uint32_t PILHA_TOPO         = 0x00FFFFFF; // SP inicial
constexpr uint32_t PILHA_BASE         = 0x00FFF000; // limite inferior da pilha

// Vídeo
constexpr int VIDEO_LARG = 320;
constexpr int VIDEO_ALT  = 240;
constexpr int FRAMEBUFFER_PIXELS = VIDEO_LARG * VIDEO_ALT;

// A classe representa todo o estado da máquina virtual.
class VM {
public:
    VM();
    ~VM();

    // Carrega um arquivo .bin (cabeçalho + dados + código) na memória.
    void carregarCodigo(const std::string& arqBin);

    // Executa uma única instrução (fetch -> decode -> execute).
    void executarInstrucao();

    // Utilidades de depuração.
    void imprimirRegistradores() const;

    // Indica se a VM deve continuar executando (HALT zera isto).
    bool rodando() const { return executando; }

private:
    int32_t  regs[NUM_REGS];
    uint8_t* mem;
    bool     executando;

    // Acesso à memória (big-endian), já com checagem de alinhamento/limites.
    uint32_t lerMemoria(uint32_t endereco) const;
    void     escreverMemoria(uint32_t endereco, uint32_t valor);

    // Erros que abortam a execução.
    void erro(const std::string& msg, uint32_t endereco) const;
    void checarAlinhamento(uint32_t endereco) const;
};
