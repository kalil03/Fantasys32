#include "vm.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>

// ============================================================================
// Construtor / destrutor
// ============================================================================
VM::VM() {
    for (int i = 0; i < NUM_REGS; i++) {
        regs[i] = 0;
    }
    // A pilha cresce para baixo. A spec menciona SP inicial 0x00FFFFFF, mas esse
    // valor não é múltiplo de 4 e quebraria o alinhamento no primeiro PUSH
    // (SP-=4 -> 0x00FFFFFB). Usamos TAM_MEM (0x01000000): é alinhado e o primeiro
    // PUSH escreve em 0x00FFFFFC, dentro da região de pilha (0xFFF000–0xFFFFFF).
    regs[SP] = TAM_MEM;

    mem = new uint8_t[TAM_MEM];
    std::memset(mem, 0, TAM_MEM);

    executando = true;
}

VM::~VM() {
    delete[] mem;
}

// ============================================================================
// Carga do arquivo .bin
// Formato: [4 bytes cabeçalho = endereço inicial do código][dados][código]
// ============================================================================
void VM::carregarCodigo(const std::string& arqBin) {
    FILE* file = std::fopen(arqBin.c_str(), "rb");
    if (!file) {
        std::fprintf(stderr, "Erro: não foi possível abrir o arquivo '%s'\n", arqBin.c_str());
        std::exit(EXIT_FAILURE);
    }

    // Tamanho do arquivo
    std::fseek(file, 0, SEEK_END);
    long tamArq = std::ftell(file);
    std::rewind(file);

    if (tamArq < 4) {
        std::fprintf(stderr, "Erro: arquivo '%s' inválido (menor que o cabeçalho)\n", arqBin.c_str());
        std::fclose(file);
        std::exit(EXIT_FAILURE);
    }

    // Lê o cabeçalho de 4 bytes (big-endian) = endereço inicial do código
    uint32_t inicioCodigo = 0;
    if (std::fread(&inicioCodigo, sizeof(uint32_t), 1, file) != 1) {
        std::fprintf(stderr, "Erro: falha ao ler o cabeçalho de '%s'\n", arqBin.c_str());
        std::fclose(file);
        std::exit(EXIT_FAILURE);
    }
    inicioCodigo = __builtin_bswap32(inicioCodigo); // big-endian -> host little-endian

    // Lê o restante (dados + código) a partir do endereço 0 da memória
    long tamCorpo = tamArq - 4;
    if (std::fread(mem, 1, tamCorpo, file) != (size_t)tamCorpo) {
        std::fprintf(stderr, "Erro: falha ao ler o corpo de '%s'\n", arqBin.c_str());
        std::fclose(file);
        std::exit(EXIT_FAILURE);
    }
    std::fclose(file);

    regs[PC] = inicioCodigo; // PC inicial
}

// ============================================================================
// Tratamento de erros e checagens
// ============================================================================
void VM::erro(const std::string& msg, uint32_t endereco) const {
    std::fprintf(stderr, "Erro de execução: %s (endereço 0x%08X, PC=0x%08X)\n",
                 msg.c_str(), endereco, (uint32_t)regs[PC]);
    std::exit(EXIT_FAILURE);
}

void VM::checarAlinhamento(uint32_t endereco) const {
    if (endereco % 4 != 0) {
        erro("acesso desalinhado (Alignment Error)", endereco);
    }
    if (endereco > 0x00FFFFFF) {
        erro("endereço inválido (fora dos limites)", endereco);
    }
}

// ============================================================================
// Acesso à memória (big-endian)
// ============================================================================
uint32_t VM::lerMemoria(uint32_t endereco) const {
    checarAlinhamento(endereco);
    return (mem[endereco]     << 24) |
           (mem[endereco + 1] << 16) |
           (mem[endereco + 2] <<  8) |
            mem[endereco + 3];
}

void VM::escreverMemoria(uint32_t endereco, uint32_t valor) {
    checarAlinhamento(endereco);
    mem[endereco]     = (valor >> 24) & 0xFF;
    mem[endereco + 1] = (valor >> 16) & 0xFF;
    mem[endereco + 2] = (valor >>  8) & 0xFF;
    mem[endereco + 3] =  valor        & 0xFF;
}

// ============================================================================
// Ciclo de execução de uma instrução (fetch -> decode -> execute)
//
// Fases implementadas:
//   Fase 1: aritmética/lógica (00–0C), MOVL/MOVH (0D/0E), LOAD/STORE (0F/10), HALT.
//   (Fase 2 adicionará controle de fluxo e pilha; demais nas fases seguintes.)
// ============================================================================
void VM::executarInstrucao() {
    uint32_t instr = lerMemoria(regs[PC]);
    regs[PC] += 4;

    // Decodificação dos campos por posição de bits. ATENÇÃO: o assembler usa
    // posições diferentes conforme o formato:
    //   Tipo R: [25:22]=rd, [21:18]=rs, [17:14]=rt   (1º operando é o destino)
    //   Tipo I: [25:22]=rs, [21:18]=rt               (imediato nos 18 bits baixos)
    uint32_t opcode = (instr >> 26) & 0x3F;
    uint32_t f22    = (instr >> 22) & 0xF;  // rd (tipo R) / rs (tipo I)
    uint32_t f18    = (instr >> 18) & 0xF;  // rs (tipo R) / rt (tipo I)
    uint32_t f14    = (instr >> 14) & 0xF;  // rt (tipo R)
    uint32_t imm18  =  instr & 0x3FFFF;     // tipo I (18 bits)

    // Apelidos por formato, para clareza nas operações.
    uint32_t rd = f22, rs_R = f18, rt_R = f14; // tipo R
    uint32_t rs = f22, rt = f18;               // tipo I

    // Imediato de 18 bits com sinal (para ADDI/LOAD/STORE).
    int32_t imm18s = (imm18 & 0x20000) ? (int32_t)(imm18 | 0xFFFC0000)
                                       : (int32_t)imm18;

    switch (opcode) {
        // ---- Aritmética (com sinal, overflow trunca em 32 bits) ----
        case 0x00: regs[rd] = regs[rs_R] + regs[rt_R]; break; // ADD
        case 0x01: regs[rd] = regs[rs_R] - regs[rt_R]; break; // SUB
        case 0x02: regs[rd] = regs[rs_R] * regs[rt_R]; break; // MUL
        case 0x03: // DIV
            if (regs[rt_R] == 0) erro("divisão por zero", regs[PC] - 4);
            regs[rd] = regs[rs_R] / regs[rt_R];
            break;
        case 0x04: // MOD
            if (regs[rt_R] == 0) erro("módulo por zero", regs[PC] - 4);
            regs[rd] = regs[rs_R] % regs[rt_R];
            break;

        // ---- Lógica (bit a bit, sem sinal) ----
        case 0x05: regs[rd] = regs[rs_R] & regs[rt_R]; break; // AND
        case 0x06: regs[rd] = regs[rs_R] | regs[rt_R]; break; // OR
        case 0x07: regs[rd] = regs[rs_R] ^ regs[rt_R]; break; // XOR

        // ---- Deslocamentos (lógicos, amount mascarado em 0x1F) ----
        case 0x08: // SHL
            regs[rd] = (int32_t)((uint32_t)regs[rs_R] << (regs[rt_R] & 0x1F));
            break;
        case 0x09: // SHR (lógico)
            regs[rd] = (int32_t)((uint32_t)regs[rs_R] >> (regs[rt_R] & 0x1F));
            break;
        case 0x0A: { // ROL
            uint32_t v = (uint32_t)regs[rs_R];
            uint32_t s = regs[rt_R] & 0x1F;
            regs[rd] = (int32_t)(s == 0 ? v : ((v << s) | (v >> (32 - s))));
            break;
        }
        case 0x0B: { // ROR
            uint32_t v = (uint32_t)regs[rs_R];
            uint32_t s = regs[rt_R] & 0x1F;
            regs[rd] = (int32_t)(s == 0 ? v : ((v >> s) | (v << (32 - s))));
            break;
        }

        // ---- ADDI (tipo I): rt = rs + imm ----
        case 0x0C: regs[rt] = regs[rs] + imm18s; break;

        // ---- Movimentação ----
        case 0x0D: // MOVL rt, imm16  (zera os bits superiores)
            regs[rt] = imm18 & 0xFFFF;
            break;
        case 0x0E: // MOVH rt, imm16  (preserva os bits inferiores)
            regs[rt] = (regs[rt] & 0x0000FFFF) | ((imm18 & 0xFFFF) << 16);
            break;

        // ---- Memória ----
        case 0x0F: { // LOAD rt, rs, imm18  -> rt = Mem[rs + imm18*4]
            uint32_t ender = (uint32_t)regs[rs] + (uint32_t)(imm18s * 4);
            regs[rt] = (int32_t)lerMemoria(ender);
            break;
        }
        case 0x10: { // STORE rt, rs, imm18 -> Mem[rs + imm18*4] = rt
            uint32_t ender = (uint32_t)regs[rs] + (uint32_t)(imm18s * 4);
            escreverMemoria(ender, (uint32_t)regs[rt]);
            break;
        }

        // ---- Sistema ----
        case 0x2B: // HALT
            executando = false;
            break;

        default:
            std::fprintf(stderr,
                "Erro: instrução 0x%08X (opcode 0x%02X) não implementada (PC=0x%08X)\n",
                instr, opcode, (uint32_t)regs[PC] - 4);
            std::exit(EXIT_FAILURE);
    }

    regs[R0] = 0; // R0 é sempre 0
}

// ============================================================================
// Depuração
// ============================================================================
void VM::imprimirRegistradores() const {
    std::printf("Registradores:\n");
    for (int i = 0; i < NUM_REGS; i++) {
        std::printf("R%-2d: 0x%08X\n", i, (uint32_t)regs[i]);
    }
}
