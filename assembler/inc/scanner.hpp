/**
 * @file scanner.hpp
 * @author Kosta Vukicevic (stcksmsh@gmail.com)
 * @brief Scanner class header file, allows C++ to interact with flex generated scanner
 * @version 0.1
 * @date 2024-05-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __SCANNER_HPP__
#define __SCANNER_HPP__

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.hh"
#include "location.hh"

namespace Assembler{

class Scanner : public yyFlexLexer{
public:
   
   Scanner(std::istream *in) : yyFlexLexer(in)
   {
   };
   virtual ~Scanner() {
   };

   //get rid of override virtual function warning
   using FlexLexer::yylex;

   virtual
   int yylex( Assembler::Parser::semantic_type * const lval, 
              Assembler::Parser::location_type *location );
   // YY_DECL defined in mc_lexer.l
   // Method body created by flex in mc_lexer.yy.cc


private:
   /* yyval ptr */
   Assembler::Parser::semantic_type *yylval = nullptr;
};

} /* end namespace MC */

#endif /* END __SCANNER_HPP__ */
