#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip> // Para formatar o hexadecimal
#include <cstdint> // Para uint32_t

enum LineType {
    LINE_INSTRUCTION,
    LINE_DIRECTIVE,
    LINE_LABEL_ONLY
};

enum ArgType {
    ARG_REGISTER,
    ARG_DEC_NUMBER,
    ARG_HEX_NUMBER,
    ARG_IDENTIFIER,
    ARG_LABEL_REF_H,
    ARG_LABEL_REF_L,
    ARG_STRING_LITERAL // Usado APENAS pela diretiva .string
};

std::string argTypeToString(ArgType type);

struct Argument {
    ArgType type;
    
    // O "Fat Node": campos para armazenar o valor já mastigado
    uint32_t numericValue; 
    std::string stringValue;

    friend std::ostream& operator<<(std::ostream& os, const Argument& arg) {
        switch(arg.type) {
            case ARG_REGISTER:      
                os << "R" << arg.numericValue << "[REG]"; 
                break;
            case ARG_DEC_NUMBER:    
                // Imprime como int32_t para exibir negativos corretamente no debug
                os << static_cast<int32_t>(arg.numericValue) << "[DEC]"; 
                break;
            case ARG_HEX_NUMBER:    
                os << "0x" << std::hex << std::uppercase << arg.numericValue << std::dec << "[HEX]"; 
                break;
            case ARG_IDENTIFIER:    
                os << arg.stringValue << "[ID]"; 
                break;
            case ARG_LABEL_REF_H:   
                os << arg.stringValue << "[REF_H]"; 
                break;
            case ARG_LABEL_REF_L:   
                os << arg.stringValue << "[REF_L]"; 
                break;
            case ARG_STRING_LITERAL:
                os << arg.stringValue << "[STR]"; 
                break;
        }
        return os;
    }
};

struct AsmLine {
    LineType type;
    std::string label;
    std::string mnemonic;
    std::vector<Argument> args; 
    int lineNumber;
    uint32_t endereco = 0; // Endereço da instrução ou dado, preenchido na Passada 1

    friend std::ostream& operator<<(std::ostream& os, const AsmLine& line) {
        os << "Linha " << line.lineNumber << " (Endereço: 0x" << std::hex << line.endereco << std::dec << ") | ";
        
        switch(line.type) {
            case LINE_LABEL_ONLY:       os << "[ROTULO] "; break;
            case LINE_DIRECTIVE:   os << "[DIRET] "; break;
            case LINE_INSTRUCTION: os << "[INSTR] "; break;
        }

        if (!line.label.empty()) os << line.label << ": ";
        if (!line.mnemonic.empty()) os << line.mnemonic;

        for (size_t i = 0; i < line.args.size(); ++i) {
            os << (i == 0 ? " " : ", ") << line.args[i];
        }
        
        return os;
    }
};

#endif