%{
/* C++ string header, for string ops below */
#include <string>

/* Implementation of yyFlexScanner */ 
#include "scanner.hpp"
#include <typeinfo>
#undef  YY_DECL
#define YY_DECL int Assembler::Scanner::yylex( Assembler::Parser::semantic_type * const lval, Assembler::Parser::location_type *loc )

/* typedef to make the returns for the tokens shorter */
using token = Assembler::Parser::token;

/* define yyterminate as this instead of NULL */
#define yyterminate() return( token::END )

/* msvc2010 requires that we exclude this header file. */
#define YY_NO_UNISTD_H

/* update location on matching */
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

%}

%option debug
%option nodefault
%option yyclass="Assembler::Scanner"
%option noyywrap
%option c++

%%
%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
%}

[ \t]+					{ /* ignore whitespace */ }

"\.global"					{ return token::GLOBAL; }
"\.extern"					{ return token::EXTERN; }
"\.section"					{ return token::SECTION; }
"\.word"					{ return token::WORD; }
"\.skip"					{ return token::SKIP; }
"\.ascii"					{ return token::ASCII; }
"\.equ"						{ return token::EQU; }
"\.end"                  { return token::END; }

"halt"				{ return token::HALT; }
"int"				{ return token::INT; }
"iret"				{ return token::IRET; }
"call"				{ return token::CALL; }
"ret"				{ return token::RET; }
"jmp"				{ return token::JMP; }
"beq"				{ return token::BEQ; }
"bne"				{ return token::BNE; }
"bgt"				{ return token::BGT; }
"push"				{ return token::PUSH; }
"pop"				{ return token::POP; }
"xchg"				{ return token::XCHG; }
"add"				{ return token::ADD; }
"sub"				{ return token::SUB; }
"mul"				{ return token::MUL; }
"div"				{ return token::DIV; }
"not"				{ return token::NOT; }
"and"				{ return token::AND; }
"or"				{ return token::OR; }
"xor"				{ return token::XOR; }
"shl"				{ return token::SHL; }
"shr"				{ return token::SHR; }
"ld"				{ return token::LD; }
"st"				{ return token::ST; }
"csrrd"				{ return token::CSRRD; }
"csrwr"				{ return token::CSRWR; }


"["					{ return token::LBRACKET; }
"]"					{ return token::RBRACKET; }

"+"					{ return token::PLUS; }
"$"                 { return token::DOLLAR; }


","					{ return token::COMMA; }
"\n"				{ return token::EOL; }


"%r"[[:digit:]]+				{ 
                                    yylval->build<int>( atoi(yytext + 2) );
                                    return token::GPR;
                                }

"0x"[0-9a-fA-F]+				{
                                    yylval->build<uint32_t>( strtoll(yytext, NULL, 16) );
                                    return token::NUMBER;
                                }

"0b"[01]+						{   
                                    yylval->build<uint32_t>( strtoll(yytext, NULL, 2) );
                                    return token::NUMBER;
                                }

"0"[0-7]+						{   
                                    yylval->build<uint32_t>( strtoll(yytext, NULL, 8) );
                                    return token::NUMBER;
                                }

[1-9][0-9]*						{   
                                    yylval->build<uint32_t>( atoi(yytext) );
                                    return token::NUMBER;
                                }

[a-zA-Z_][a-zA-Z0-9_]*:		    {   
                                    yytext[strlen(yytext) - 1] = '\0';
                                    yylval->build<std::string>( yytext );
                                    return token::LABEL; 
                                }

[a-zA-Z_][a-zA-Z0-9_]*		    {   
                                    yylval->build<std::string>( yytext );
                                    return token::SYMBOL; 
                                }

%%
