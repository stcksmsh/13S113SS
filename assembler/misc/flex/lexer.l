%option noyywrap

%{
	#undef yywrap
	#define yywrap() 1

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	int lineno = 1; // initialize to 1
	void ret_print(char *token_type);
	void yyerror();
%}


alpha 			[a-zA-Z]
num 			[0-9]
bin				[01]
oct				[0-7]
hex				[0-9a-fA-F]
alnum 			{alpha}|{num}
ws 				[ \t]+

BEGIN				^
DOT                	\.
COMMA              	,


COMMENT 			#[{BEGIN}\r\n]+
HEX_LITERAL			0x{hex}+
OCT_LITERAL			0{oct}+
DEC_LITERAL			{num}+
BIN_LITERAL			0b{bin}+
SYMBOL 				{alpha}+{alnum}*
REGISTER			$r[0-9]+

LABEL            	{SYMBOL}:

%%

{COMMENT}			{ /* ignore comments */ }

{BEGIN}{DOT}"global"		{ ret_print("GLOBAL"); }
{BEGIN}{DOT}"extern"		{ ret_print("EXTERN"); }
{BEGIN}{DOT}"section"		{ ret_print("SECTION"); }
{BEGIN}{DOT}"word"			{ ret_print("WORD"); }
{BEGIN}{DOT}"skip"			{ ret_print("SKIP"); }
{BEGIN}{DOT}"ascii"			{ ret_print("ASCII"); }
{BEGIN}{DOT}"equ"			{ ret_print("EQU"); }
{BEGIN}{DOT}"end"			{ ret_print("END"); }

"halt"            		{ ret_print("HALT"); }
"int"             		{ ret_print("INT"); }
"iret"            		{ ret_print("IRET"); }
"call"            		{ ret_print("CALL"); }
"ret"             		{ ret_print("RET"); }
"jmp"             		{ ret_print("JMP"); }
"beq"             		{ ret_print("BEQ"); }
"bne"             		{ ret_print("BNE"); }
"bgt"             		{ ret_print("BGT"); }
"push"            		{ ret_print("PUSH"); }
"pop"             		{ ret_print("POP"); }
"xchg"            		{ ret_print("XCHG"); }
"add"             		{ ret_print("ADD"); }
"sub"             		{ ret_print("SUB"); }
"mul"             		{ ret_print("MUL"); }
"div"             		{ ret_print("DIV"); }
"not"             		{ ret_print("NOT"); }
"and"             		{ ret_print("AND"); }
"or"              		{ ret_print("OR"); }
"xor"             		{ ret_print("XOR"); }
"shl"             		{ ret_print("SHL"); }
"shr"             		{ ret_print("SHR"); }
"ld"              		{ ret_print("LD"); }
"st"              		{ ret_print("ST"); }
"csrrd"           		{ ret_print("CSRRD"); }

{LABEL}					{ ret_print("LABEL"); }
{REGISTER}      		{ int regnum =atoi(yytext+2); if( regnum > 15 ) yyerror("Invalid register"); ret_print("REGISTER"); }
{BIN_LITERAL}			{ ret_print("BIN_LITERAL"); }
{OCT_LITERAL}			{ ret_print("OCT_LITERAL"); }
{DEC_LITERAL}			{ ret_print("DEC_LITERAL"); }
{HEX_LITERAL}			{ ret_print("HEX_LITERAL"); }
{SYMBOL}				{ ret_print("SYMBOL"); }
{COMMA}					{ ret_print("COMMA"); }

"\n"					{ lineno += 1; }
[ \t\r\f]+				{ /* eat up whitespace */ }

.				{ yyerror("Unrecognized character"); }

%%

void ret_print(char *token_type){
	printf("yytext: %s\ttoken: %s\tlineno: %d\n", yytext, token_type, lineno);
}

void yyerror(char *message){
	printf("Error: \"%s\" in line %d. Token = %s\n", message, lineno, yytext);
	exit(1);
}

int parse(int argc, char *argv[]){
	yyin = fopen(argv[1], "r");
	yylex();
	fclose(yyin);
	return 0;
}