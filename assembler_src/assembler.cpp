#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstdio>

#include "utils.h"
#include "assembler.h"
#include "ast.h"
#include "parser.tab.hpp" // Inclui o Parser C++
#include "instructions.h"

using namespace std;

extern std::unordered_map<std::string, DefInstrucao> dicInstrucoes;
extern std::vector<AsmLine> programLines;
extern FILE* yyin;

unordered_map<string, uint32_t> passada1(vector<AsmLine>& programLines) {
    uint32_t endereco_atual = 0;
    unordered_map<string, uint32_t> tabSimbolos;

    for (auto& linha : programLines){
        #ifdef DEBUG
        cout << "Montando linha " << linha << "\n";
        #endif
        linha.endereco = endereco_atual; // Atribui o endereço à linha
        // Se a linha atual já tiver uma label, inseri-la na tabela de símbolos
        if(!linha.label.empty()){
            if(tabSimbolos.find(linha.label) != tabSimbolos.end()){
                std::cerr << "Erro na linha " << linha.lineNumber
                          << ": Label " << linha.label << " redefinido!" << "\n";
                exit(1);
            }
            tabSimbolos[linha.label] = endereco_atual;
        }

        if(linha.type == LINE_INSTRUCTION){
            endereco_atual+=4;
        }
        else if(linha.type == LINE_DIRECTIVE){
            if(linha.mnemonic == ".var"){
                endereco_atual+=4;
            }
            else if(linha.mnemonic == ".space"){
                int nPalavras = 0;
                if((linha.args[0].type == ARG_DEC_NUMBER) || (linha.args[0].type == ARG_HEX_NUMBER)){
                    nPalavras = linha.args[0].numericValue;
                }

                if(linha.args[0].type == ARG_IDENTIFIER){
                    const auto& val = tabSimbolos.find(linha.args[0].stringValue);
                    if(val == tabSimbolos.end()){
                        std::cerr << "Erro na linha " << linha.lineNumber
                                  << ": Identificador " << linha.args[0].stringValue << " não definido!" << "\n";
                        exit(1);
                    }
                    nPalavras = val->second;
                }
                endereco_atual += (nPalavras * 4);
            }
            else if(linha.mnemonic == ".array"){
                endereco_atual += (linha.args.size() * 4);
                for(auto& arg : linha.args){
                    if((arg.type != ARG_DEC_NUMBER) && (arg.type != ARG_HEX_NUMBER) && (arg.type != ARG_IDENTIFIER)){
                        std::cerr << "Erro na linha " << linha.lineNumber
                                  << ": Argumento inválido para .array!" << "\n"
                                  << " Os argumentos devem ser números decimais, hexadecimais ou identificadores já definidos!" << "\n";
                        exit(1);
                    }
                }
            }
            else if(linha.mnemonic == ".equ"){
                // .equ SIMBOLO, VALOR
                // SIMBOLO o parser já sabe que é um identificador (string). linha.args[0]
                // VALOR pode ser decimal, hexadecimal ou identificador (já definido). em linha.args[1]
                int valor = 0;
                if((linha.args[1].type == ARG_DEC_NUMBER) || (linha.args[1].type == ARG_HEX_NUMBER)){
                    valor = linha.args[1].numericValue;
                }
                else if(linha.args[1].type == ARG_IDENTIFIER){
                    const auto& val = tabSimbolos.find(linha.args[1].stringValue);
                    if(val == tabSimbolos.end()){
                        std::cerr << "Erro na linha " << linha.lineNumber
                                  << ": Identificador " << linha.args[1].stringValue << " não definido!" << "\n";
                        exit(1);
                    }
                    valor = val->second;
                }
                else{
                    std::cerr << "Erro na linha " << linha.lineNumber
                              << ": Argumento inválido para .equ!" << "\n"
                              << " O segundo argumento deve ser um número decimal, hexadecimal ou um identificador já definido!" << "\n";
                    exit(1);
                }
                if(tabSimbolos.find(linha.args[0].stringValue) != tabSimbolos.end()){
                    std::cerr << "Erro na linha " << linha.lineNumber
                              << ": Símbolo " << linha.args[0].stringValue << " redefinido!" << "\n";
                    exit(1);
                }
                tabSimbolos[linha.args[0].stringValue] = valor;
            }
            else if (linha.mnemonic == ".string"){
                std::string texto = linha.args[0].stringValue;
                texto = texto.substr(1, texto.size() - 2);
                //verificar se todos os caracteres são imprimíveis (0x20 a 0x7E)
                for(char c : texto){
                    if(c < 32 || c > 126){
                        std::cerr << "Erro na linha " << linha.lineNumber
                                  << ": Caracter inválido na string!" << "\n"
                                  << " Apenas caracteres ASCII-padrão (32 a 126) são permitidos!\n";
                        exit(1);
                    }
                }
                int tamanhoReal = texto.size() + 1;
                int padding = (4-(tamanhoReal % 4)) % 4;
                endereco_atual += (tamanhoReal + padding);
            }
        }
    }
    return tabSimbolos;
}


void imprimirTabSimbolos(const unordered_map<string, uint32_t>& tabSimbolos){
    cout << "Tabela de Símbolos:\n";
    // imprimir alinhado, par.first com até 20 caracteres, seguido do valor em hexadecimal
    for(const auto& par : tabSimbolos){
        cout << std::left << std::setw(20) << par.first << " : 0x" << std::hex << std::uppercase << par.second << std::dec
                << " (" << par.second << ")\n";
    }
}

bool validarArgumentos(const AsmLine& linha, const DefInstrucao& def){
    if(linha.args.size() != def.assinatura.size()){
        std::cerr << "Erro na linha " << linha.lineNumber
                  << ": Número incorreto de argumentos para a instrução " << linha.mnemonic << "!" << "\n"
                  << " Esperado: " << def.assinatura.size() << ", Encontrado: " << linha.args.size() << "\n";
        return false;
    }
    for(size_t i = 0; i < linha.args.size(); ++i){
        bool imediatoEsperado = (def.assinatura[i] == ARG_DEC_NUMBER || def.assinatura[i] == ARG_HEX_NUMBER) &&
                                (linha.args[i].type == ARG_DEC_NUMBER || linha.args[i].type == ARG_HEX_NUMBER || linha.args[i].type == ARG_IDENTIFIER || linha.args[i].type == ARG_LABEL_REF_H || linha.args[i].type == ARG_LABEL_REF_L);

        if(linha.args[i].type != def.assinatura[i] && !imediatoEsperado){
            std::cerr << "Erro na linha " << linha.lineNumber
                      << ": Tipo incorreto para o argumento " << (i+1) << " da instrução " << linha.mnemonic << "!" << "\n"
                      << " Esperado: " << argTypeToString(def.assinatura[i]) << ", Encontrado: " << argTypeToString(linha.args[i].type) << "\n";
            return false;
        }
    }
    return true;
}

pair<BinHeader, vector<uint32_t>> passada2(const unordered_map<string, uint32_t>& tabSimbolos){
    vector<uint32_t> binario;
    enum SECAO { SECAO_DESCONHECIDA, SECAO_TEXT, SECAO_DATA };
    SECAO secao_atual = SECAO_DESCONHECIDA;

    uint32_t tam_dados = 0;

    for(const auto& linha : programLines){
        uint32_t instrucao = 0;

        if(linha.type == LINE_DIRECTIVE){
            if(linha.mnemonic == ".data"){
                secao_atual = SECAO_DATA;
                continue;
            }
            if(linha.mnemonic == ".text"){
                secao_atual = SECAO_TEXT;
                tam_dados = binario.size() * 4;
                continue;
            }

            if(secao_atual != SECAO_DATA){
                std::cerr << "Erro na linha " << linha.lineNumber
                            << ": Diretivas de dados podem estar apenas na seção .data!\n";
                exit(1);
            }
            
            #ifdef DEBUG
            cout << "Processando diretiva: " << linha << "\n";
            #endif

            if(linha.mnemonic == ".var"){
                if(linha.args[0].type == ARG_DEC_NUMBER || linha.args[0].type == ARG_HEX_NUMBER){
                    binario.push_back(linha.args[0].numericValue);
                }
                else if(linha.args[0].type == ARG_IDENTIFIER){
                    const auto& val = tabSimbolos.find(linha.args[0].stringValue);
                    if(val == tabSimbolos.end()){
                        std::cerr << "Erro na linha " << linha.lineNumber
                                  << ": Identificador " << linha.args[0].stringValue << " não definido!" << "\n";
                        exit(1);
                    }
                    binario.push_back(val->second);
                }
                else{
                    std::cerr << "Erro na linha " << linha.lineNumber
                              << ": Argumento inválido para .var!" << "\n"
                              << " O argumento deve ser um número decimal, hexadecimal ou um identificador já definido!" << "\n";
                    exit(1);
                }
                #ifdef DEBUG
                cout << "Diretiva .var processada, valor: " << binario.back() << "\n";
                #endif
            }

            if(linha.mnemonic == ".array"){
                for(auto& arg : linha.args){
                    if(arg.type == ARG_DEC_NUMBER || arg.type == ARG_HEX_NUMBER){
                        binario.push_back(arg.numericValue);
                    }
                    else if(arg.type == ARG_IDENTIFIER){
                        const auto& val = tabSimbolos.find(arg.stringValue);
                        if(val == tabSimbolos.end()){
                            std::cerr << "Erro na linha " << linha.lineNumber
                                      << ": Identificador " << arg.stringValue << " não definido!" << "\n";
                            exit(1);
                        }
                        binario.push_back(val->second);
                    }
                    else{
                        std::cerr << "Erro na linha " << linha.lineNumber
                                  << ": Argumento inválido para .array!" << "\n"
                                  << " Os argumentos devem ser números decimais, hexadecimais ou identificadores já definidos!" << "\n";
                        exit(1);
                    }
                }
                #ifdef DEBUG
                cout << "Diretiva .array processada, valores: ";
                for(size_t i = binario.size() - linha.args.size(); i < binario.size(); ++i){
                    cout << binario[i] << " ";
                }
                cout << "\n";
                #endif
            }

            if(linha.mnemonic == ".space"){
                int nPalavras = 0;
                if((linha.args[0].type == ARG_DEC_NUMBER) || (linha.args[0].type == ARG_HEX_NUMBER)){
                    nPalavras = linha.args[0].numericValue;
                }
                else if(linha.args[0].type == ARG_IDENTIFIER){
                    const auto& val = tabSimbolos.find(linha.args[0].stringValue);
                    if(val == tabSimbolos.end()){
                        std::cerr << "Erro na linha " << linha.lineNumber
                                  << ": Identificador " << linha.args[0].stringValue << " não definido!" << "\n";
                        exit(1);
                    }
                    nPalavras = val->second;
                }
                for(int i = 0; i < nPalavras; ++i){
                    binario.push_back(0); // Preenche o espaço com zeros
                }
                #ifdef DEBUG
                cout << "Diretiva .space processada, valores: ";
                for(size_t i = binario.size() - nPalavras; i < binario.size(); ++i){
                    cout << binario[i] << " ";
                }
                cout << "\n";
                #endif
            }

            if(linha.mnemonic == ".string"){
                std::string texto = linha.args[0].stringValue;
                // Remove as aspas do início e do fim
                texto = texto.substr(1, texto.size() - 2);
                
                //Aloca a string em blocos de 4 bytes. O Assembler deve garantir que a string seja terminada com um byte nulo (`0x00`) e que os bytes sejam organizados em formato big-endian, preenchendo os blocos de 4 bytes conforme necessário. Caso a string (juntamente com o byte nulo) não seja um múltiplo de 4 bytes, o Assembler deve preencher os bytes restantes do último bloco com zeros.

                std::vector<uint32_t> blocos;
                uint32_t blocoAtual = 0;
                int bytesNoBloco = 0;
                for(char c : texto){
                    blocoAtual = (blocoAtual << 8) | static_cast<uint8_t>(c);
                    bytesNoBloco++;

                    if(bytesNoBloco == 4){
                        blocos.push_back(blocoAtual);
                        blocoAtual = 0;
                        bytesNoBloco = 0;
                    }
                }

                // Adiciona o byte nulo ao final da string
                blocoAtual = (blocoAtual << 8) | 0x00;
                bytesNoBloco++;
                if(bytesNoBloco > 0){
                    blocoAtual <<= (8 * (4 - bytesNoBloco)); // Preenche os bytes restantes com zeros
                    blocos.push_back(blocoAtual);
                }
                for(uint32_t bloco : blocos){
                    binario.push_back(bloco);
                }

                #ifdef DEBUG
                cout << "Diretiva .string processada, blocos: ";
                for(size_t i = binario.size() - blocos.size(); i < binario.size(); ++i){
                    cout << "0x" << std::hex << std::uppercase << binario[i] << std::dec << " ";
                }
                cout << "\n";
                #endif
            }

        }

        if(linha.type == LINE_INSTRUCTION){
            if (secao_atual != SECAO_TEXT){
                std::cerr   << "Erro na linha " << linha.lineNumber 
                            << ": Instruções só podem ser definidas na seção .text!\n";
                exit(1);
            }
            const auto& defIt = dicInstrucoes.find(linha.mnemonic);
            if(defIt == dicInstrucoes.end()){
                std::cerr << "Erro na linha " << linha.lineNumber
                          << ": Instrução desconhecida: " << linha.mnemonic << "\n";
                exit(1);
            }
            const auto& def = defIt->second;

            if(!validarArgumentos(linha, def)){
                exit(1);
            }

            #ifdef DEBUG
            cout << "Codificando instrução: " << linha << "\n";
            #endif

            instrucao = codificarInstrucao(linha, def, tabSimbolos);

            #ifdef DEBUG
            // Imprime a instrução codificada em hexadecimal, com 8 dígitos e zeros à esquerda
            cout << "Instrução codificada: 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << instrucao << std::dec << "\n";
            #endif  

            binario.push_back(instrucao);
        }
    }

    return {BinHeader{tam_dados}, binario};
}

uint32_t htonl2(uint32_t v) {
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    return v;
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #if defined(_MSC_VER)
        return _byteswap_ulong(v); // Intrínseco do MSVC
    #else
        return __builtin_bswap32(v); // Intrínseco do GCC/Clang
    #endif
#else
    // Fallback manual se o compilador não ajudar
    return ((v >> 24) & 0xff) | ((v << 8) & 0xff0000) |
           ((v >> 8) & 0xff00) | ((v << 24) & 0xff000000);
#endif
}

void montar(const char* arqEntrada, const char* arqSaida){

    yy::parser parser;

    FILE* file = fopen(arqEntrada, "r");
    if (!file) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo '" << arqEntrada << "'." << std::endl;
        exit(1);
    }

    yyin = file;

    std::cout << "Montando programa: " << arqEntrada << "\n";

    if(parser.parse() == 0){
        #ifdef DEBUG
        cout    << "Parser executado com sucesso! Foram lidas " << programLines.size()
                << " linhas não-vazias!\n";
        #endif
    }
    else{
        std::cerr << "Erro na análise sintática.\n";
        exit(1);
    }

    fclose(file);

    //passada 1: ler o arquivo, montar a AST, resolver equates e labels, calcular endereços
    auto tabSimbolos = passada1(programLines);

    #ifdef DEBUG
    imprimirTabSimbolos(tabSimbolos);
    #endif

    //passada 2: gerar o código de máquina usando a AST e as informações coletadas na passada 1
    auto binario = passada2(tabSimbolos);

    // Escrever o arquivo binário
    FILE* outFile = fopen(arqSaida, "wb");
    if (!outFile) {
        std::cerr << "Erro: Nao foi possivel criar o arquivo '" << arqSaida << "'." << std::endl;
        exit(1);        
    }

    // escrever o cabeçalho
    BinHeader header = binario.first;
    uint32_t dataSize_be = htonl2(header.dataSize); // Converte para big-endian
    fwrite(&dataSize_be, sizeof(uint32_t), 1, outFile);

    // escrever os dados e as instruções em formato big-endian
    for(uint32_t palavra : binario.second){
        uint32_t palavra_be = htonl2(palavra); // Converte para big-endian
        fwrite(&palavra_be, sizeof(uint32_t), 1, outFile);
    }

    fclose(outFile);

    std::cout << "Arquivo binário gerado com sucesso: " << arqSaida << "\n";

    return;
}   

