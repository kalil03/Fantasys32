%language "c++"

%{
#include <iostream>
#include <string>
#include <vector>
#include "ast.h"

extern int current_stmt_line;
std::vector<AsmLine> programLines;
%}

%code {
    extern int yylex(yy::parser::semantic_type* yylval);
}

%code requires {
    #include "ast.h"
    #include <string>
    #include <vector>
}

%define api.value.type variant

%token <std::string> TOK_LABEL_DEF TOK_IDENTIFIER TOK_REGISTER 
%token <std::string> TOK_HEX_NUMBER TOK_DEC_NUMBER TOK_STRING_LITERAL
%token <std::string> TOK_LABEL_REF_H TOK_LABEL_REF_L
%token TOK_DIR_DATA TOK_DIR_TEXT TOK_DIR_EQU TOK_DIR_VAR TOK_DIR_ARRAY TOK_DIR_SPACE TOK_DIR_STRING
%token TOK_COMMA TOK_COLON TOK_NEWLINE TOK_EOF

%type <AsmLine> line instruction directive
%type <std::vector<Argument>> arg_list
%type <Argument> argument 
%type <std::string> label_decl

%% 

program:
    lines TOK_EOF { return 0; }
    ;

lines:
    lines line_entry
    | line_entry
    ;

line_entry:
    line TOK_NEWLINE { programLines.push_back($1); }
    | TOK_NEWLINE    { /* Ignora linhas em branco */ }
    ;

label_decl:
    TOK_LABEL_DEF {
        $$ = $1;
        $$.pop_back(); // Remove o ':'
    }
    | TOK_IDENTIFIER TOK_COLON {
        $$ = $1;
    }
    ;

line:
    label_decl instruction {
        $$ = $2;
        $$.label = $1;
    }
    | label_decl directive {
        $$ = $2;
        $$.label = $1;
    }
    | label_decl {
        $$.type = LINE_LABEL_ONLY;
        $$.label = $1;
        $$.lineNumber = current_stmt_line;
    }
    | instruction { $$ = $1; }
    | directive   { $$ = $1; }
    ;

instruction:
    TOK_IDENTIFIER arg_list {
        $$.type = LINE_INSTRUCTION;
        $$.mnemonic = $1;
        $$.args = $2;
        $$.lineNumber = current_stmt_line;
    }
    | TOK_IDENTIFIER {
        $$.type = LINE_INSTRUCTION;
        $$.mnemonic = $1;
        $$.lineNumber = current_stmt_line;
    }
    ;

directive:
    TOK_DIR_DATA {
        $$.type = LINE_DIRECTIVE;
        $$.mnemonic = ".data";
        $$.lineNumber = current_stmt_line;
    }
    | TOK_DIR_TEXT {
        $$.type = LINE_DIRECTIVE;
        $$.mnemonic = ".text";
        $$.lineNumber = current_stmt_line;
    }
    | TOK_DIR_EQU TOK_IDENTIFIER TOK_COMMA argument {
        $$.type = LINE_DIRECTIVE;
        $$.mnemonic = ".equ";
        $$.args.push_back(Argument{ARG_IDENTIFIER, 0, $2}); 
        $$.args.push_back($4);
        $$.lineNumber = current_stmt_line;
    }
    | TOK_DIR_VAR argument {
        $$.type = LINE_DIRECTIVE;
        $$.mnemonic = ".var";
        $$.args.push_back($2); 
        $$.lineNumber = current_stmt_line;
    }
    | TOK_DIR_ARRAY arg_list {
        $$.type = LINE_DIRECTIVE;
        $$.mnemonic = ".array";
        $$.args = $2;          
        $$.lineNumber = current_stmt_line;
    }
    | TOK_DIR_SPACE argument {
        $$.type = LINE_DIRECTIVE;
        $$.mnemonic = ".space";
        $$.args.push_back($2); 
        $$.lineNumber = current_stmt_line;
    }
    | TOK_DIR_STRING TOK_STRING_LITERAL {
        $$.type = LINE_DIRECTIVE;
        $$.mnemonic = ".string";
        $$.args.push_back(Argument{ARG_STRING_LITERAL, 0, $2});
        $$.lineNumber = current_stmt_line;
    }
    ;

arg_list:
    argument { $$.push_back($1); }
    | arg_list TOK_COMMA argument {
        $$ = $1;
        $$.push_back($3);
    }
    ;

argument:
    TOK_REGISTER { 
        uint32_t val = std::stoul($1.substr(1));
        $$ = Argument{ARG_REGISTER, val, ""}; 
    }
    | TOK_DEC_NUMBER { 
        uint32_t val = static_cast<uint32_t>(std::stoll($1));
        $$ = Argument{ARG_DEC_NUMBER, val, ""}; 
    }
    | TOK_HEX_NUMBER { 
        uint32_t val = std::stoul($1, nullptr, 16);
        $$ = Argument{ARG_HEX_NUMBER, val, ""}; 
    }
    | TOK_IDENTIFIER { 
        $$ = Argument{ARG_IDENTIFIER, 0, $1}; 
    }
    | TOK_LABEL_REF_H { 
        std::string x = $1.substr(0, $1.size() - 2); // Remove o ".h"
        $$ = Argument{ARG_LABEL_REF_H, 0, x}; 
    }
    | TOK_LABEL_REF_L { 
        std::string x = $1.substr(0, $1.size() - 2); // Remove o ".l"
        $$ = Argument{ARG_LABEL_REF_L, 0, x}; 
    }
    ;

%%

void yy::parser::error(const std::string& msg) {
    std::cerr << "Erro sintatico na linha " << current_stmt_line << ": " << msg << std::endl;
}