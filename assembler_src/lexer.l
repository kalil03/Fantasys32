%{
#include <iostream>
#include <string>
#include <vector>
#include "ast.h"
#include "parser.tab.hpp"

using namespace yy;

#undef YY_DECL
#define YY_DECL int yylex(yy::parser::semantic_type* yylval)

// --- MÁGICA PARA CORRIGIR O LOOKAHEAD ---
// Variável global que guardará a linha exata em que a instrução começou
int current_stmt_line = 1;
bool start_of_line = true;

// O Flex executa essa macro ANTES de processar qualquer token
#define YY_USER_ACTION \
    if (yytext[0] == '\n') { \
        start_of_line = true; \
    } else if (yytext[0] != ' ' && yytext[0] != '\t' && yytext[0] != '\r') { \
        if (start_of_line) { \
            current_stmt_line = yylineno; \
            start_of_line = false; \
        } \
    }
%}

%option noyywrap
%option yylineno

DIGIT      [0-9]
HEX_DIGIT  [0-9a-fA-F]
LETTER     [a-zA-Z_]
IDENTIFIER {LETTER}({LETTER}|{DIGIT})*

%%

\n                      { return parser::token::TOK_NEWLINE; }
[ \t\r]+                { /* Ignora espaços em branco e tabulações */ }
;.*                     { /* Ignora comentários */ }

"\.data"                { return parser::token::TOK_DIR_DATA; }
"\.text"                { return parser::token::TOK_DIR_TEXT; }
"\.equ"                 { return parser::token::TOK_DIR_EQU; }
"\.var"                 { return parser::token::TOK_DIR_VAR; }
"\.array"               { return parser::token::TOK_DIR_ARRAY; }
"\.space"               { return parser::token::TOK_DIR_SPACE; }
"\.string"              { return parser::token::TOK_DIR_STRING; }

","                     { return parser::token::TOK_COMMA; }
":"                     { return parser::token::TOK_COLON; }

R{DIGIT}+               { yylval->emplace<std::string>(yytext); return parser::token::TOK_REGISTER; }
{IDENTIFIER}:           { yylval->emplace<std::string>(yytext); return parser::token::TOK_LABEL_DEF; }
{IDENTIFIER}\.h         { yylval->emplace<std::string>(yytext); return parser::token::TOK_LABEL_REF_H; }
{IDENTIFIER}\.l         { yylval->emplace<std::string>(yytext); return parser::token::TOK_LABEL_REF_L; }
{IDENTIFIER}            { yylval->emplace<std::string>(yytext); return parser::token::TOK_IDENTIFIER; }

0x{HEX_DIGIT}+          { yylval->emplace<std::string>(yytext); return parser::token::TOK_HEX_NUMBER; }
-?{DIGIT}+              { yylval->emplace<std::string>(yytext); return parser::token::TOK_DEC_NUMBER; }
\"([^\"\\]|\\.)*\"      { yylval->emplace<std::string>(yytext); return parser::token::TOK_STRING_LITERAL; }

<<EOF>>                 { return parser::token::TOK_EOF; }
.                       { /* Ignora caracteres inválidos ou trate o erro aqui */ }

%%