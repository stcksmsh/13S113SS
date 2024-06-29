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
%option yylineno
%option nodefault
%option yyclass="Assembler::Scanner"
%option noyywrap
%option c++

%%
%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
%}

[ \t]+					{ /* ignore whitespace */ }
"#"[^\n]*				{ /* ignore comments */ }

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
"-"					{ return token::MINUS; }
"$"                 { return token::DOLLAR; }


","					{ return token::COMMA; }
"\n"+				{ loc->lines( yyleng ); return token::EOL; }


"%r"[[:digit:]]+				{ 
                                    yylval->build<uint8_t>( atoi(yytext + 2) );
                                    return token::GPR;
                                }

"%sp"                           { yylval->build<uint8_t>( 14 ); return token::GPR; }
"%pc"                           { yylval->build<uint8_t>( 15 ); return token::GPR; }

"%status"                       { yylval->build<Assembler::csr_type>( STATUS ); return token::CSR; }
"%handler"                      { yylval->build<Assembler::csr_type>( HANDLER ); return token::CSR; }
"%cause"                        { yylval->build<Assembler::csr_type>( CAUSE ); return token::CSR; }

"0x"[0-9a-fA-F]+				{
                                    yylval->build<int32_t>( strtoll(yytext, NULL, 16) );
                                    return token::NUMBER;
                                }

"0b"[01]+						{   
                                    yylval->build<int32_t>( strtoll(yytext, NULL, 2) );
                                    return token::NUMBER;
                                }

"0"[0-7]+						{   
                                    yylval->build<int32_t>( strtoll(yytext, NULL, 8) );
                                    return token::NUMBER;
                                }

[1-9][0-9]*						{   
                                    yylval->build<int32_t>( atoi(yytext) );
                                    return token::NUMBER;
                                }

"0"								{   
                                    yylval->build<int32_t>( 0 );
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

"\""[[:print:]]*"\""            {   
                                    yytext[strlen(yytext) - 1] = '\0';
                                    std::string str = yytext + 1;

                                    /// We need to 'build' the string, meaning we need to take care of escape sequences
                                    std::string parsed_str = "";

                                    for( size_t i = 0; i < str.size(); i++ )
                                    {
                                        if( str[i] == '\\' )
                                        {
                                            i++;
                                            switch( str[i] )
                                            {
                                                case 'n': parsed_str += '\n'; break;
                                                case 't': parsed_str += '\t'; break;
                                                case 'r': parsed_str += '\r'; break;
                                                case '0': parsed_str += '\0'; break;
                                                case '\\': parsed_str += '\\'; break;
                                                case '"': parsed_str += '"'; break;
                                                default: parsed_str += str[i]; break;
                                            }
                                        }
                                        else
                                        {
                                            parsed_str += str[i];
                                        }
                                    }
                                    /// Add a null terminator
                                    parsed_str += '\0';

                                    yylval->build<std::string>( parsed_str );



                                    return token::STRING; 
                                }

.                               { std::cerr << "Unrecognized token: " << yytext << " at line " << loc->end.line << " character " << loc->end.column << std::endl; exit(1); } 
%%
