#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include "ast.h"

enum FormatoInstrucao {
    FORMAT_R, // rd, rs, rt
    FORMAT_IMM16, // rt, rs, imm16
    FORMAT_IMM18, // rt, rs, imm18
    FORMAT_BRANCH, // rs, rt, label
    FORMAT_J,    // addr26
    FORMAT_U,    // Unico operando registrador
    FORMAT_U0,    // RET
    FORMAT_S5,    // ra, rb, rc, rd, re (instruções de Sistema e E/S com 5 argumentos registradores)
    FORMAT_S4,    // ra, rb, rc, rd (instruções de Sistema e E/S com 4 argumentos registradores)
    FORMAT_S3,     // ra, rb, rc (instruções de Sistema e E/S com 3 argumentos registradores)
    FORMAT_S2,     // ra, rb (instruções de Sistema e E/S com 2 argumentos registradores)
    FORMAT_S1,      // ra (instrução de Sistema e E/S com 1 argumento registrador)
    FORMAT_S0       // Instrução de Sistema e E/S sem argumentos
};

struct DefInstrucao {
    uint32_t opcode;
    FormatoInstrucao formato;
    std::vector<ArgType> assinatura;
};

uint32_t codificarInstrucao(const AsmLine& linha, const DefInstrucao& def, const std::unordered_map<std::string, uint32_t>& tabSimbolos);