#include "ast.h"

std::string argTypeToString(ArgType type){
    switch(type){
        case ARG_REGISTER: return "registrador";
        case ARG_DEC_NUMBER: return "número decimal";
        case ARG_HEX_NUMBER: return "número hexadecimal";
        case ARG_IDENTIFIER: return "identificador";
        case ARG_LABEL_REF_H: return "referência de rótulo (parte alta)";
        case ARG_LABEL_REF_L: return "referência de rótulo (parte baixa)";
        case ARG_STRING_LITERAL: return "literal de string";
        default: return "UNKNOWN_ARG_TYPE";
    }
}