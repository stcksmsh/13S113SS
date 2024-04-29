%option noyywrap

%{
	#undef yywrap
	#define yywrap() 1

	#include "parser.h"

	int lineno = 1; // initialize to 1
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

{BEGIN}{DOT}"global"		{ return GLOBAL; }
{BEGIN}{DOT}"extern"		{ return EXTERN; }
{BEGIN}{DOT}"section"		{ return SECTION; }
{BEGIN}{DOT}"word"			{ return WORD; }
{BEGIN}{DOT}"skip"			{ return SKIP; }
{BEGIN}{DOT}"ascii"			{ return ASCII; }
{BEGIN}{DOT}"equ"			{ return EQU; }
{BEGIN}{DOT}"end"			{ return END; }

"halt"            		{ return HALT; }
"int"             		{ return INT; }
"iret"            		{ return IRET; }
"call"            		{ return CALL; }
"ret"             		{ return RET; }
"jmp"             		{ return JMP; }
"beq"             		{ return BEQ; }
"bne"             		{ return BNE; }
"bgt"             		{ return BGT; }
"push"            		{ return PUSH; }
"pop"             		{ return POP; }
"xchg"            		{ return XCHG; }
"add"             		{ return ADD; }
"sub"             		{ return SUB; }
"mul"             		{ return MUL; }
"div"             		{ return DIV; }
"not"             		{ return NOT; }
"and"             		{ return AND; }
"or"              		{ return OR; }
"xor"             		{ return XOR; }
"shl"             		{ return SHL; }
"shr"             		{ return SHR; }
"ld"              		{ return LD; }
"st"              		{ return ST; }
"csrrd"           		{ return CSRRD; }

{LABEL}					{ return LABEL; }
{REGISTER}      		{ int regnum =atoi(yytext+2); if( regnum > 15 ) yyerror("Invalid register"); return REGISTER; }
{BIN_LITERAL}			{ return BIN_LITERAL; }
{OCT_LITERAL}			{ return OCT_LITERAL; }
{DEC_LITERAL}			{ return DEC_LITERAL; }
{HEX_LITERAL}			{ return HEX_LITERAL; }
{SYMBOL}				{ return SYMBOL; }
{COMMA}					{ return COMMA; }

"\n"					{ lineno += 1; }
[ \t\r\f]+				{ /* eat up whitespace */ }

.				{ yyerror("Unrecognized character"); }

%%

void yyerror(char *message){
	printf("Error: \"%s\" in line %d. Token = %s\n", message, lineno, yytext);
	exit(1);
}