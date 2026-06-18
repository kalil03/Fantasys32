#include "instructions.h"

std::unordered_map<std::string, DefInstrucao> dicInstrucoes = {
    {"ADD",  {0x00, FORMAT_R, {ARG_REGISTER, ARG_REGISTER, ARG_REGISTER}}},
    {"SUB",  {0x01, FORMAT_R, {ARG_REGISTER, ARG_REGISTER, ARG_REGISTER}}},
    {"MUL",  {0x02, FORMAT_R, {ARG_REGISTER, ARG_REGISTER, ARG_REGISTER}}},
    {"DIV",  {0x03, FORMAT_R, {ARG_REGISTER, ARG_REGISTER, ARG_REGISTER}}},
    {"MOD",  {0x04, FORMAT_R, {ARG_REGISTER, ARG_REGISTER, ARG_REGISTER}}},
    {"AND",  {0x05, FORMAT_R, {ARG_REGISTER, ARG_REGISTER, ARG_REGISTER}}},
    {"OR",   {0x06, FORMAT_R, {ARG_REGISTER, ARG_REGISTER, ARG_REGISTER}}},
    {"XOR",  {0x07, FORMAT_R, {ARG_REGISTER, ARG_REGISTER, ARG_REGISTER}}},
    {"SHL",  {0x08, FORMAT_R, {ARG_REGISTER, ARG_REGISTER, ARG_REGISTER}}}, 
    {"SHR",  {0x09, FORMAT_R, {ARG_REGISTER, ARG_REGISTER, ARG_REGISTER}}}, 
    {"ROL",  {0x0A, FORMAT_R, {ARG_REGISTER, ARG_REGISTER, ARG_REGISTER}}}, 
    {"ROR",  {0x0B, FORMAT_R, {ARG_REGISTER, ARG_REGISTER, ARG_REGISTER}}}, 

    {"ADDI",  {0x0C, FORMAT_IMM18, {ARG_REGISTER, ARG_REGISTER, ARG_DEC_NUMBER}}}, 
    {"MOVL",  {0x0D, FORMAT_IMM16, {ARG_REGISTER, ARG_DEC_NUMBER}}}, 
    {"MOVH",  {0x0E, FORMAT_IMM16, {ARG_REGISTER, ARG_DEC_NUMBER}}},
    {"LOAD",  {0x0F, FORMAT_IMM18, {ARG_REGISTER, ARG_REGISTER, ARG_DEC_NUMBER}}}, 
    {"STORE", {0x10, FORMAT_IMM18, {ARG_REGISTER, ARG_REGISTER, ARG_DEC_NUMBER}}},

    {"BEQ",  {0x11, FORMAT_BRANCH, {ARG_REGISTER, ARG_REGISTER, ARG_IDENTIFIER}}}, 
    {"BNE",  {0x12, FORMAT_BRANCH, {ARG_REGISTER, ARG_REGISTER, ARG_IDENTIFIER}}}, 
    {"BLT",  {0x13, FORMAT_BRANCH, {ARG_REGISTER, ARG_REGISTER, ARG_IDENTIFIER}}}, 
    {"BGT",  {0x14, FORMAT_BRANCH, {ARG_REGISTER, ARG_REGISTER, ARG_IDENTIFIER}}}, 
    {"BLE",  {0x15, FORMAT_BRANCH, {ARG_REGISTER, ARG_REGISTER, ARG_IDENTIFIER}}}, 
    {"BGE",  {0x16, FORMAT_BRANCH, {ARG_REGISTER, ARG_REGISTER, ARG_IDENTIFIER}}}, 

    {"JMP",  {0x17, FORMAT_J, {ARG_IDENTIFIER}}},
    {"CALL", {0x18, FORMAT_J, {ARG_IDENTIFIER}}},

    {"PUSH", {0x19, FORMAT_U, {ARG_REGISTER}}},
    {"POP",  {0x1A, FORMAT_U, {ARG_REGISTER}}},
    {"INC",  {0x1B, FORMAT_U, {ARG_REGISTER}}},
    {"DEC",  {0x1C, FORMAT_U, {ARG_REGISTER}}},
    {"NOT",  {0x1D, FORMAT_U, {ARG_REGISTER}}},
    {"RET",  {0x1E, FORMAT_U0, {}}},

    {"RECT",     {0x1F, FORMAT_S5, {ARG_REGISTER, ARG_REGISTER, ARG_REGISTER, ARG_REGISTER, ARG_REGISTER}}},
    {"DSPRITE",  {0x20, FORMAT_S4, {ARG_REGISTER, ARG_REGISTER, ARG_REGISTER, ARG_REGISTER, ARG_REGISTER}}},
    {"CLEAR",    {0x21, FORMAT_S1, {ARG_REGISTER}}},
    {"GKEY",     {0x22, FORMAT_S2, {ARG_REGISTER, ARG_REGISTER}}},
    {"PLAY",     {0x23, FORMAT_S3, {ARG_REGISTER, ARG_REGISTER, ARG_REGISTER}}},
    {"SLEEP",    {0x24, FORMAT_S1, {ARG_REGISTER}}},
    {"PSTR",     {0x25, FORMAT_S4, {ARG_REGISTER, ARG_REGISTER, ARG_REGISTER, ARG_REGISTER}}},
    {"PINT",     {0x26, FORMAT_S4, {ARG_REGISTER, ARG_REGISTER, ARG_REGISTER, ARG_REGISTER}}},
    {"SYSCALL",  {0x27, FORMAT_S5, {ARG_REGISTER, ARG_REGISTER, ARG_REGISTER, ARG_REGISTER, ARG_REGISTER}}},
    {"SRAND",    {0x28, FORMAT_S1, {ARG_REGISTER}}},
    {"RAND",     {0x29, FORMAT_S3, {ARG_REGISTER, ARG_REGISTER, ARG_REGISTER}}},
    {"FRAMENUM", {0x2A, FORMAT_S1, {ARG_REGISTER}}},
    {"HALT",     {0x2B, FORMAT_S0, {}}}
};

uint32_t codificarInstrucao(const AsmLine& linha, const DefInstrucao& def, const std::unordered_map<std::string, uint32_t>& tabSimbolos){

    uint32_t binario = ((def.opcode & 0x3F) << 26); // opcode ocupa os 6 bits mais significativos

    switch (def.formato){
        case FORMAT_R:
            // Formato R: opcode(6) | rd(4) | rs(4) | rt(4) | unused(14)
            binario |= (linha.args[0].numericValue & 0xF) << 22; // rd
            binario |= (linha.args[1].numericValue & 0xF) << 18; // rs
            binario |= (linha.args[2].numericValue & 0xF) << 14; // rt
            break;

        case FORMAT_IMM16:
        case FORMAT_IMM18:
        {
            // Formato I: opcode(6) | rs(4) | rt(4) | imm(18)

            if (def.formato == FORMAT_IMM16){
                binario |= (linha.args[0].numericValue & 0xF) << 18; // rt
            }
            else{
                // Para FORMAT_IMM18, o primeiro argumento é rs e o segundo é rt
                binario |= (linha.args[0].numericValue & 0xF) << 22; // rs
                binario |= (linha.args[1].numericValue & 0xF) << 18; // rt
            }
            
            int immPos = def.assinatura.size() - 1;

            uint32_t imm = 0;
            if(linha.args[immPos].type == ARG_DEC_NUMBER || linha.args[immPos].type == ARG_HEX_NUMBER){
                imm = linha.args[immPos].numericValue;
            }
            else if(linha.args[immPos].type == ARG_IDENTIFIER || linha.args[immPos].type == ARG_LABEL_REF_H || linha.args[immPos].type == ARG_LABEL_REF_L){
                const auto& val = tabSimbolos.find(linha.args[immPos].stringValue);
                if(val == tabSimbolos.end()){
                    std::cerr << "Erro na linha " << linha.lineNumber
                                << ": Identificador " << linha.args[immPos].stringValue << " não definido!" << "\n";
                    exit(1);
                }
                imm = val->second;
                if (linha.args[immPos].type == ARG_LABEL_REF_H) {
                    imm = (imm >> 16) & 0xFFFF; // parte alta
                } else if (linha.args[immPos].type == ARG_LABEL_REF_L) {
                    imm = imm & 0xFFFF; // parte baixa
                }
            }
            else{
                std::cerr << "Erro na linha " << linha.lineNumber
                            << ": Argumento imediato inválido!" << "\n"
                            << " O argumento imediato deve ser um número decimal, hexadecimal ou um identificador já definido!" << "\n";
                exit(1);
            }

            if(def.formato == FORMAT_IMM16){
                imm &= 0xFFFF; // imediato de 16 bits
            }
            else{
                imm &= 0x3FFFF; // imediato de 18 bits
            }

            binario |= imm; // imm ocupa os 18 bits menos significativos
            
            break;
        }

        case FORMAT_BRANCH:
        {        
            // Formato Branch: opcode(6) | rs(4) | rt(4) | zeroes(2) | offset(16)
            binario |= (linha.args[0].numericValue & 0xF) << 22; // rs
            binario |= (linha.args[1].numericValue & 0xF) << 18; // rt
            int16_t offset = 0;
         
            const auto& val = tabSimbolos.find(linha.args[2].stringValue);
            if(val == tabSimbolos.end()){
                std::cerr << "Erro na linha " << linha.lineNumber
                            << ": Rótulo " << linha.args[2].stringValue << " não definido!" << "\n";
                exit(1);
            }
            uint32_t enderecoDestino = val->second;
            int64_t distancia = (static_cast<int64_t>(enderecoDestino) - static_cast<int64_t>(linha.endereco + 4))/4;
            if (distancia < -32768 || distancia > 32767) {
                std::cerr << "Erro na linha " << linha.lineNumber
                            << ": Rótulo " << linha.args[2].stringValue << " está muito distante para ser alcançado com um offset de 16 bits!" << "\n";
                exit(1);
            }
            #ifdef DEBUG
            std::cout << "distancia para o rótulo " << linha.args[2].stringValue << ": " << distancia << " palavras\n";
            #endif
            // O offset é calculado em relação à próxima instrução (PC-relative)
            // o salto pode ser positivo (para frente) ou negativo (para trás), 
            // então usamos int16_t para permitir ambos os casos. Lembrando que o 
            // offset é dado em número de palavras (4 bytes), então dividimos por 4.
            offset = static_cast<int16_t>((enderecoDestino - (linha.endereco + 4)) / 4);
        
            binario |= (offset & 0xFFFF); // offset ocupa os 16 bits menos significativos
            break;
        }

        case FORMAT_J:
        {
            // Formato J: opcode(6) | address(26)
            uint32_t enderecoDestino = 0;
            const auto& val = tabSimbolos.find(linha.args[0].stringValue);
            if(val == tabSimbolos.end()){
                std::cerr << "Erro na linha " << linha.lineNumber
                            << ": Rótulo " << linha.args[0].stringValue << " não definido!" << "\n";
                exit(1);
            }
            enderecoDestino = val->second;
            //o endereço é um endereço absoluto, mas em palavras.
            uint32_t address = enderecoDestino / 4; // converte de byte para palavra
            if (address > 0x3FFFFFF) { // verifica se cabe em 26 bits
                std::cerr << "Erro na linha " << linha.lineNumber
                            << ": Endereço " << linha.args[0].stringValue << " é muito grande para ser codificado em 26 bits!" << "\n";
                exit(1);
            }
            binario |= (address & 0x3FFFFFF); // address ocupa os 26 bits menos significativos
            break;
        }
        case FORMAT_U:
        case FORMAT_U0:
        {
            // Formato U: opcode(6) | rd(4) | zeroes(22)
            if (def.formato == FORMAT_U) {
                binario |= (linha.args[0].numericValue & 0xF) << 22; // rd
            }
            break;
        }

        case FORMAT_S5:
        case FORMAT_S4:
        case FORMAT_S3:
        case FORMAT_S2:
        case FORMAT_S1:
        case FORMAT_S0:
        {
            // Formato S: opcode(6) | ra(4) | rb(4) | rc(4) | rd(4) | re(4) | zeroes(6)
            int nRegs = def.assinatura.size(); // número de argumentos que são registradores
            if (nRegs > 0) binario |= (linha.args[0].numericValue & 0xF) << 22; // ra
            if (nRegs > 1) binario |= (linha.args[1].numericValue & 0xF) << 18; // rb
            if (nRegs > 2) binario |= (linha.args[2].numericValue & 0xF) << 14; // rc
            if (nRegs > 3) binario |= (linha.args[3].numericValue & 0xF) << 10; // rd
            if (nRegs > 4) binario |= (linha.args[4].numericValue & 0xF) << 6; // re
            break;
        }

    }

    return binario;
}
