#include "vm.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>

// Imprime a mensagem de ajuda das opções de linha de comando.
static void mostrarAjuda(const char* prog) {
    std::printf(
        "Uso: %s [opções] caminho/para/arquivo.bin\n\n"
        "Opções:\n"
        "  --scale <fator>   Fator de escala da janela (padrão: 1).\n"
        "  --no-syscall      Ignora a execução da instrução SYSCALL.\n"
        "  --debug           Imprime os registradores ao final da execução.\n"
        "  --help            Exibe esta mensagem de ajuda.\n",
        prog);
}

int main(int argc, char** argv) {
    int         scale       = 1;
    bool        noSyscall   = false;
    bool        debug       = false;
    std::string arqBin;

    // Parsing simples dos argumentos de linha de comando.
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "--help") == 0) {
            mostrarAjuda(argv[0]);
            return EXIT_SUCCESS;
        } else if (std::strcmp(argv[i], "--scale") == 0 && i + 1 < argc) {
            scale = std::atoi(argv[++i]);
            if (scale < 1) scale = 1;
        } else if (std::strcmp(argv[i], "--no-syscall") == 0) {
            noSyscall = true;
        } else if (std::strcmp(argv[i], "--debug") == 0) {
            debug = true;
        } else {
            arqBin = argv[i]; // assume que é o caminho do .bin
        }
    }

    if (arqBin.empty()) {
        std::fprintf(stderr, "Erro: nenhum arquivo .bin informado.\n\n");
        mostrarAjuda(argv[0]);
        return EXIT_FAILURE;
    }

    VM vm;
    vm.setEscala(scale);
    vm.setSemSyscall(noSyscall);
    vm.carregarCodigo(arqBin);

    vm.executar();

    if (debug) {
        vm.imprimirRegistradores();
    }

    return EXIT_SUCCESS;
}
