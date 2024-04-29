%{
    #include <stdio.h>
    int yylex(void);
%}

/* define tokens */

%token GLOBAL EXTERN SECTION WORD SKIP ASCII EQU END
%token HALT INT IRET CALL RET JMP BEQ BNE BGE BLE BGT PUSH POP XCHG ADD SUB MUL DIV NOT AND OR XOR SHL SHR LD ST CSRRD
%token LABEL REGISTER BIN_LITERAL OCT_LITERAL HEX_LITERAL DEC_LITERAL SYMBOL COMMA
%%

program:
    /* empty */
    | program line
    ;

line:
    directive
    | instruction
    | LABEL instruction
    | LABEL
    ;

directive:
    GLOBAL symbol_list
    | EXTERN symbol_list
    | SECTION SYMBOL
    | WORD symbol_or_literal_list
    | SKIP literal
    | ASCII string
    | END
    ;

symbol_list:
    SYMBOL
    | symbol_list COMMA SYMBOL
    ;

symbol_or_literal_list:
    symbol_or_literal
    | symbol_or_literal_list COMMA symbol_or_literal
    ;

symbol_or_literal:
    SYMBOL
    | literal
    ;

string:
    SYMBOL
    | string SYMBOL
    ;    

literal:
    BIN_LITERAL
    | OCT_LITERAL
    | HEX_LITERAL
    | DEC_LITERAL
    ;


instruction:
    HALT
    | INT
    | IRET
    | CALL
    | RET
    | JMP
    | BEQ
    | BNE
    | BGE
    | BLE
    | BGT
    | PUSH
    | POP
    | XCHG
    | ADD
    | SUB
    | MUL
    | DIV
    | NOT
    | AND
    | OR
    | XOR
    | SHL
    | SHR
    | LD
    | ST
    | CSRRD
    ;

%%
int parse(int argc, char **argv)
{
	yyparse();
	return 0;
}