#include "vm.hpp"
#include "fonte.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>

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
// Pilha (cresce para baixo). SP começa em TAM_MEM; o primeiro PUSH escreve em
// 0x00FFFFFC. Limite inferior é PILHA_BASE (0x00FFF000).
// ============================================================================
void VM::empilhar(uint32_t valor) {
    if (regs[SP] - 4 < (int32_t)PILHA_BASE) {
        erro("estouro de pilha (Stack Overflow)", regs[SP]);
    }
    regs[SP] -= 4;
    escreverMemoria((uint32_t)regs[SP], valor);
}

uint32_t VM::desempilhar() {
    if ((uint32_t)regs[SP] >= TAM_MEM) {
        erro("pilha vazia (Stack Underflow)", regs[SP]);
    }
    uint32_t valor = lerMemoria((uint32_t)regs[SP]);
    regs[SP] += 4;
    return valor;
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

    // Offset de 16 bits com sinal (desvios) e endereço de 26 bits (JMP/CALL).
    int32_t  off16  = (int16_t)(imm18 & 0xFFFF);
    uint32_t addr26 = instr & 0x03FFFFFF;

    // Campos do tipo S: ra=[25:22], rb=[21:18], rc=[17:14], rd=[13:10], re=[9:6].
    uint32_t ra = f22, rb = f18, rc = f14;
    uint32_t rd_S = (instr >> 10) & 0xF;
    uint32_t re = (instr >> 6) & 0xF;

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

        // ---- Controle de fluxo: desvios condicionais (tipo I) ----
        // Destino = PC (já incrementado) + offset16*4. Offset com sinal de 16 bits.
        case 0x11: if (regs[rs] == regs[rt]) regs[PC] += off16 * 4; break; // BEQ
        case 0x12: if (regs[rs] != regs[rt]) regs[PC] += off16 * 4; break; // BNE
        case 0x13: if (regs[rs] <  regs[rt]) regs[PC] += off16 * 4; break; // BLT
        case 0x14: if (regs[rs] >  regs[rt]) regs[PC] += off16 * 4; break; // BGT
        case 0x15: if (regs[rs] <= regs[rt]) regs[PC] += off16 * 4; break; // BLE
        case 0x16: if (regs[rs] >= regs[rt]) regs[PC] += off16 * 4; break; // BGE

        // ---- Saltos incondicionais (tipo J): endereço absoluto = addr26*4 ----
        case 0x17: // JMP
            regs[PC] = addr26 * 4;
            break;
        case 0x18: // CALL: empilha o endereço de retorno (PC já = próxima instrução)
            empilhar((uint32_t)regs[PC]);
            regs[PC] = addr26 * 4;
            break;

        // ---- Unárias e pilha (tipo U) ----
        case 0x19: // PUSH rd
            empilhar((uint32_t)regs[rd]);
            break;
        case 0x1A: // POP rd
            regs[rd] = (int32_t)desempilhar();
            break;
        case 0x1B: regs[rd] = regs[rd] + 1; break; // INC
        case 0x1C: regs[rd] = regs[rd] - 1; break; // DEC
        case 0x1D: regs[rd] = ~regs[rd];      break; // NOT
        case 0x1E: // RET: desempilha o endereço de retorno
            regs[PC] = (int32_t)desempilhar();
            break;

        // ---- Sistema e E/S (tipo S) ----
        case 0x1F: // RECT ra,rb,rc,rd,re -> x,y,w,h,color
            desenharRect(regs[ra], regs[rb], regs[rc], regs[rd_S], (uint32_t)regs[re]);
            break;
        case 0x20: // DSPRITE ra,rb,rc,rd,re -> x,y,w,h,endereço
            desenharSprite(regs[ra], regs[rb], regs[rc], regs[rd_S], (uint32_t)regs[re]);
            break;
        case 0x21: // CLEAR ra -> limpa a tela com a cor
            for (int i = 0; i < FRAMEBUFFER_PIXELS; i++) framebuffer()[i] = (uint32_t)regs[ra];
            break;
        case 0x22: // GKEY ra,rb -> ra = 1 se a tecla rb está pressionada
            regs[ra] = teclaPressionada(regs[rb]) ? 1 : 0;
            break;
        case 0x23: // PLAY ra,rb,rc -> freq, ms, forma de onda
            tocarSom(regs[ra], regs[rb], regs[rc]);
            break;
        case 0x24: // SLEEP ra -> pausa a execução por ra ms
            dormirAteMs = SDL_GetTicks64() + (uint32_t)regs[ra];
            break;
        case 0x25: // PSTR ra,rb,rc,rd -> x,y,endereço da string,color
            desenharString(regs[ra], regs[rb], (uint32_t)regs[rc], (uint32_t)regs[rd_S]);
            break;
        case 0x26: // PINT ra,rb,rc,rd -> x,y,valor,color
            desenharInt(regs[ra], regs[rb], regs[rc], (uint32_t)regs[rd_S]);
            break;
        case 0x27: // SYSCALL ra,rb,rc,rd,re (auxílio de depuração)
            if (!semSyscall) {
                switch ((uint32_t)regs[ra]) {
                    case 0: // imprime um registrador no terminal
                        std::printf("[SYSCALL] R%d = %d (0x%08X)\n",
                                    rb, regs[rb], (uint32_t)regs[rb]);
                        break;
                    case 1: { // imprime uma string do programa no terminal
                        uint32_t a = (uint32_t)regs[rb];
                        std::printf("[SYSCALL] ");
                        while (a < TAM_MEM && mem[a] != 0) std::putchar(mem[a++]);
                        std::putchar('\n');
                        break;
                    }
                    case 2: // imprime todos os registradores
                        imprimirRegistradores();
                        break;
                    default:
                        std::printf("[SYSCALL] código %d não tratado\n", regs[ra]);
                }
            }
            break;
        case 0x28: // SRAND ra -> semente do gerador
            rngState = (uint32_t)regs[ra];
            break;
        case 0x29: { // RAND ra,rb,rc -> ra = aleatório em [rb, rc]
            int32_t mn = regs[rb], mx = regs[rc];
            if (mx < mn) { int32_t t = mn; mn = mx; mx = t; }
            uint32_t intervalo = (uint32_t)(mx - mn) + 1;
            regs[ra] = mn + (int32_t)(proximoAleatorio() % intervalo);
            break;
        }
        case 0x2A: // FRAMENUM ra -> número de frames renderizados
            regs[ra] = (int32_t)numFrames;
            break;
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

// ============================================================================
// SDL: inicialização e finalização
// ============================================================================
void VM::inicializarSDL() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    janela = SDL_CreateWindow("Fantasys32",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        VIDEO_LARG * escala, VIDEO_ALT * escala, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED);

    // Textura no formato ARGB8888, igual ao framebuffer (uint32_t por pixel).
    textura = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING, VIDEO_LARG, VIDEO_ALT);

    // Áudio: 44100 Hz, 16 bits com sinal, mono. Usamos a fila (não-bloqueante).
    SDL_AudioSpec want;
    SDL_zero(want);
    want.freq     = 44100;
    want.format   = AUDIO_S16SYS;
    want.channels = 1;
    want.samples  = 2048;
    want.callback = nullptr; // modo de fila (SDL_QueueAudio)
    audioDev = SDL_OpenAudioDevice(nullptr, 0, &want, nullptr, 0);
    if (audioDev) SDL_PauseAudioDevice(audioDev, 0);
}

void VM::finalizarSDL() {
    if (audioDev) SDL_CloseAudioDevice(audioDev);
    if (textura)  SDL_DestroyTexture(textura);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (janela)   SDL_DestroyWindow(janela);
    SDL_Quit();
}

// Processa eventos da janela (fechar, ESC) e atualiza o estado do teclado.
void VM::processarEventos() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) executando = false;
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) executando = false;
    }
}

// Copia o framebuffer para a tela.
void VM::renderizar() {
    SDL_UpdateTexture(textura, nullptr, framebuffer(), VIDEO_LARG * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, textura, nullptr, nullptr);
    SDL_RenderPresent(renderer);
    numFrames++;
}

// ============================================================================
// Loop principal: 60 FPS, executando até HALT ou fechar a janela.
// ============================================================================
void VM::executar() {
    inicializarSDL();

    const uint32_t msPorFrame = 1000 / FPS;
    while (executando) {
        uint64_t inicio = SDL_GetTicks64();

        processarEventos();

        // Executa um lote de instruções. Se SLEEP estiver ativo, para o lote
        // (mas continua renderizando) até o tempo de pausa acabar.
        for (int k = 0; k < INSTR_POR_FRAME && executando; k++) {
            if (SDL_GetTicks64() < dormirAteMs) break;
            executarInstrucao();
        }

        renderizar();

        // Controla o FPS.
        uint64_t duracao = SDL_GetTicks64() - inicio;
        if (duracao < msPorFrame) SDL_Delay(msPorFrame - duracao);
    }

    finalizarSDL();
}

// ============================================================================
// Desenho no framebuffer
// ============================================================================
void VM::pintarPixel(int x, int y, uint32_t cor) {
    // Pixel transparente (alfa 0) não altera o framebuffer.
    if (((cor >> 24) & 0xFF) == 0) return;
    if (x < 0 || x >= VIDEO_LARG || y < 0 || y >= VIDEO_ALT) return;
    framebuffer()[y * VIDEO_LARG + x] = cor;
}

void VM::desenharRect(int x, int y, int w, int h, uint32_t cor) {
    for (int j = 0; j < h; j++)
        for (int i = 0; i < w; i++)
            pintarPixel(x + i, y + j, cor);
}

void VM::desenharSprite(int x, int y, int w, int h, uint32_t addr) {
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            uint32_t cor = lerMemoria(addr + (uint32_t)((j * w + i) * 4));
            pintarPixel(x + i, y + j, cor);
        }
    }
}

// Desenha um caractere 8x8 escalado para 16x16 (cada pixel vira um bloco 2x2).
void VM::desenharChar(int x, int y, char ch, uint32_t cor) {
    unsigned char c = (unsigned char)ch;
    if (c >= 'a' && c <= 'z') c -= 0x20;        // minúscula -> maiúscula
    if (c < 0x20 || c > 0x5F) c = 0x20;          // fora da fonte -> espaço
    const uint8_t* glifo = FONTE8X8[c - 0x20];
    for (int linha = 0; linha < 8; linha++) {
        for (int col = 0; col < 8; col++) {
            if (glifo[linha] & (0x80 >> col)) {
                // bloco 2x2 para escalar 8x8 -> 16x16
                pintarPixel(x + col * 2,     y + linha * 2,     cor);
                pintarPixel(x + col * 2 + 1, y + linha * 2,     cor);
                pintarPixel(x + col * 2,     y + linha * 2 + 1, cor);
                pintarPixel(x + col * 2 + 1, y + linha * 2 + 1, cor);
            }
        }
    }
}

void VM::desenharString(int x, int y, uint32_t addr, uint32_t cor) {
    // String termina em byte nulo. Cada caractere ocupa 16 pixels de largura.
    for (int i = 0; addr + i < TAM_MEM; i++) {
        char ch = (char)mem[addr + i];
        if (ch == 0) break;
        desenharChar(x + i * 16, y, ch, cor);
    }
}

void VM::desenharInt(int x, int y, int32_t valor, uint32_t cor) {
    char buf[16];
    std::snprintf(buf, sizeof(buf), "%d", valor);
    for (int i = 0; buf[i] != 0; i++) {
        desenharChar(x + i * 16, y, buf[i], cor);
    }
}

// ============================================================================
// E/S: teclado, som e aleatoriedade
// ============================================================================
bool VM::teclaPressionada(int id) const {
    // Mapeamento das 16 teclas (conforme a especificação).
    static const SDL_Scancode mapa[16] = {
        SDL_SCANCODE_LEFT,  SDL_SCANCODE_RIGHT, SDL_SCANCODE_UP,    SDL_SCANCODE_DOWN,
        SDL_SCANCODE_SPACE, SDL_SCANCODE_RETURN,SDL_SCANCODE_N,     SDL_SCANCODE_M,
        SDL_SCANCODE_A,     SDL_SCANCODE_S,     SDL_SCANCODE_D,     SDL_SCANCODE_W,
        SDL_SCANCODE_Q,     SDL_SCANCODE_E,     SDL_SCANCODE_C,     SDL_SCANCODE_V
    };
    if (id < 0 || id > 15) return false;
    const Uint8* estado = SDL_GetKeyboardState(nullptr);
    return estado[mapa[id]] != 0;
}

// Gera a forma de onda pedida e a envia para a fila de áudio (não-bloqueante).
void VM::tocarSom(int freq, int ms, int forma) {
    if (!audioDev || freq <= 0 || ms <= 0) return;
    if (ms > 5000) ms = 5000; // limita a duração

    const int taxa = 44100;
    int nAmostras = taxa * ms / 1000;
    std::vector<int16_t> amostras(nAmostras);
    const double amp = 0.25 * 32767.0;

    for (int i = 0; i < nAmostras; i++) {
        double t = (double)i / taxa;
        double fase = 2.0 * M_PI * freq * t;
        double v = 0.0;
        switch (forma) {
            case 0: v = std::sin(fase); break;                       // SINE
            case 1: v = (std::sin(fase) >= 0 ? 1.0 : -1.0); break;   // SQUARE
            case 2: v = std::asin(std::sin(fase)) * (2.0 / M_PI); break; // TRIANGLE
            case 3: v = ((double)rand() / RAND_MAX) * 2.0 - 1.0; break;   // NOISE
            default: v = std::sin(fase);
        }
        amostras[i] = (int16_t)(v * amp);
    }

    // Substitui qualquer som anterior.
    SDL_ClearQueuedAudio(audioDev);
    SDL_QueueAudio(audioDev, amostras.data(), nAmostras * sizeof(int16_t));
}

// Gerador linear congruente (LCG): mesma semente -> mesma sequência.
uint32_t VM::proximoAleatorio() {
    rngState = rngState * 1664525u + 1013904223u;
    return rngState;
}
