#include <iostream>
#include <vector>
#include <cstring>
#include <string>

#include "ast.h"
#include "parser.tab.hpp" // Inclui o Parser C++
#include "assembler.h"

extern std::vector<AsmLine> programLines; 

int main(int argc, char** argv) {
    // Checa opções de ajuda primeiro

    std::string debug_info = "Compilado sem opções de depuração.";

    #ifdef DEBUG
    debug_info = "Compilado com opções de depuração. Informações detalhadas serão exibidas durante a montagem.";
    #endif

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--help") == 0) {
            std::cout << "===Fantasys32 Assembler===\n\n"
                      << "Uso: " << argv[0] << " <arquivo.asm> <arquivo.bin>\n\n"
                      << "Monta o arquivo de entrada <arquivo.asm> para <arquivo.bin>.\n\n"
                      << "Opções:\n"
                      << "  -h, --help    Mostra esta ajuda e sai\n\n"
                      << debug_info << "\n";
            return 0;
        }
    }

    if (argc < 3) {
        std::cerr << "Uso correto: " << argv[0] << " <arquivo.asm> <arquivo.bin>" << std::endl;
        return 1;
    }

    if(strcmp(argv[1], argv[2]) == 0){
        std::cerr << "Erro: O arquivo de entrada e o arquivo de saída não podem ser o mesmo!" << std::endl;
        return 1;
    }

    montar(argv[1], argv[2]);

    return 0;
}