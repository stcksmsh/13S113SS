%skeleton "lalr1.cc"
%language "c++"
%require  "3.2"
%define api.namespace {Assembler}
/**
 * bison 3.3.2 change
 * %define parser_class_name to this, updated
 * should work for previous bison versions as 
 * well. -jcb 24 Jan 2020
 */
%define api.parser.class {Parser}

%code requires{
   #include <cstdint>
   #include <string>
   namespace Assembler {
      class Driver;
      class Scanner;
      enum csr_type{
         STATUS,
         HANDLER,
         CAUSE
      };
      union instruction{
         struct{
            uint32_t Disp: 12,
                     RegC: 4,
                     RegB: 4,
                     RegA: 4,
                     MOD: 3,
                     OC: 5;
         } fields;
         uint32_t raw = 0;
      };
   }

// The following definitions is missing when %locations isn't used
   # ifndef YY_NULLPTR
   #  if defined __cplusplus && 201103L <= __cplusplus
   #   define YY_NULLPTR nullptr
   #  else
   #   define YY_NULLPTR 0
   #  endif
   # endif

}

%parse-param { Scanner  &scanner  }
%parse-param { Driver  &driver  }

%code{
   #include <iostream>
   #include <cstdint>
   #include <fstream>
   
   /* include for all driver functions */
   #include "driver.hpp"

   #undef yylex
   #define yylex scanner.yylex
}

%define api.value.type variant
%define parse.assert


/* Directive tokens */
%token	                           GLOBAL		"global_drct"
%token	                           EXTERN		"extern_drct"
%token	                           SECTION		"section_drct"
%token	                           WORD			"word_drct"
%token	                           SKIP			"skip_drct"
%token	                           ASCII			"ascii_drct"
%token	                           EQU			"equ_drct"
%token	                           END         "end_drct"

/* Instruction tokens */
%token	                           HALT			"halt_inst"
%token	                           INT			"int_inst"
%token	                           IRET			"iret_inst"
%token	                           CALL			"call_inst"
%token	                           RET			"ret_inst"
%token	                           JMP			"jmp_inst"
%token	                           BEQ			"beq_inst"
%token	                           BNE			"bne_inst"
%token	                           BGT			"bgt_inst"
%token	                           PUSH			"push_inst"
%token	                           POP			"pop_inst"
%token	                           XCHG			"xchg_inst"
%token	                           ADD			"add_inst"
%token	                           SUB			"sub_inst"
%token	                           MUL			"mul_inst"
%token	                           DIV			"div_inst"
%token	                           NOT			"not_inst"
%token	                           AND			"and_inst"
%token	                           OR			   "or_inst"
%token	                           XOR			"xor_inst"
%token	                           SHL			"shl_inst"
%token	                           SHR			"shr_inst"
%token	                           LD			   "ld_inst"
%token	                           ST			   "st_inst"
%token	                           CSRRD       "csrrd_inst"
%token	                           CSRWR       "csrwr_inst"

/* Other non-type tokens */
%token	                           COMMA       ","
%token	                           EOL         "eol"
%token	                           LBRACKET    "["
%token	                           RBRACKET    "]"
%token	                           PLUS        "+"
%token	                           DOLLAR      "$"

/* Type tokens */

%token <uint32_t>                   NUMBER      "literal"
%token <int>                        GPR         "general purpose register"
%token <Assembler::csr_type>        CSR         "control and status register"
%token <std::string>                SYMBOL      "symbol"
%token <std::string>                LABEL       "label"


/* Types */

%type < std::vector<std::string> >  symbol_list "symbol_list"
/*  */
%type < Assembler::instruction >   operand     "operand"

/* End of file token */

%locations


%%

program:
   /* empty */
   | program line
   | program END     { return 0; }
   ;

line:
     EOL
   | label EOL
   | directive EOL
   | label directive EOL
   | instruction EOL
   | label instruction EOL
   ;

instruction:
     HALT                               { Assembler::instruction inst; inst.fields.OC = 0b00000; driver.append_TEXT(inst.raw); }
   | INT                                { Assembler::instruction inst; inst.fields.OC = 0b00001; driver.append_TEXT(inst.raw); }
   | IRET                               { Assembler::instruction inst; inst.fields.OC = 0b00010; driver.append_TEXT(inst.raw); }
   | CALL operand                       { $2.fields.OC = 0b00011; driver.append_TEXT($2.raw); std::cout << std::hex << $2.raw << std::endl; }
   | RET                                { Assembler::instruction inst; inst.fields.OC = 0b00100; driver.append_TEXT(inst.raw); }
   | JMP operand                        { $2.fields.OC = 0b00101; driver.append_TEXT($2.raw); }
   | BEQ GPR COMMA GPR COMMA operand    { $6.fields.OC = 0b00110; $6.fields.RegA = $2; $6.fields.RegB = $4; driver.append_TEXT($6.raw); }
   | BNE GPR COMMA GPR COMMA operand    { $6.fields.OC = 0b00111; $6.fields.RegA = $2; $6.fields.RegB = $4; driver.append_TEXT($6.raw); }
   | BGT GPR COMMA GPR COMMA operand    { $6.fields.OC = 0b01000; $6.fields.RegA = $2; $6.fields.RegB = $4; driver.append_TEXT($6.raw); }
   | PUSH GPR                           { Assembler::instruction inst; inst.fields.OC = 0b01001; inst.fields.RegA = $2; driver.append_TEXT(inst.raw); }
   | POP GPR                            { Assembler::instruction inst; inst.fields.OC = 0b01010; inst.fields.RegA = $2; driver.append_TEXT(inst.raw); }
   | XCHG GPR COMMA GPR                 { Assembler::instruction inst; inst.fields.OC = 0b01011; inst.fields.RegA = $2; inst.fields.RegB = $4; driver.append_TEXT(inst.raw); }
   | ADD GPR COMMA GPR                  { Assembler::instruction inst; inst.fields.OC = 0b01100; inst.fields.RegA = $2; inst.fields.RegB = $4; driver.append_TEXT(inst.raw); }
   | SUB GPR COMMA GPR                  { Assembler::instruction inst; inst.fields.OC = 0b01101; inst.fields.RegA = $2; inst.fields.RegB = $4; driver.append_TEXT(inst.raw); }
   | MUL GPR COMMA GPR                  { Assembler::instruction inst; inst.fields.OC = 0b01110; inst.fields.RegA = $2; inst.fields.RegB = $4; driver.append_TEXT(inst.raw); }
   | DIV GPR COMMA GPR                  { Assembler::instruction inst; inst.fields.OC = 0b01111; inst.fields.RegA = $2; inst.fields.RegB = $4; driver.append_TEXT(inst.raw); }
   | NOT GPR                            { Assembler::instruction inst; inst.fields.OC = 0b10000; inst.fields.RegA = $2; driver.append_TEXT(inst.raw); }
   | AND GPR COMMA GPR                  { Assembler::instruction inst; inst.fields.OC = 0b10001; inst.fields.RegA = $2; inst.fields.RegB = $4; driver.append_TEXT(inst.raw); }
   | OR GPR COMMA GPR                   { Assembler::instruction inst; inst.fields.OC = 0b10010; inst.fields.RegA = $2; inst.fields.RegB = $4; driver.append_TEXT(inst.raw); }
   | XOR GPR COMMA GPR                  { Assembler::instruction inst; inst.fields.OC = 0b10011; inst.fields.RegA = $2; inst.fields.RegB = $4; driver.append_TEXT(inst.raw); }
   | SHL GPR COMMA GPR                  { Assembler::instruction inst; inst.fields.OC = 0b10100; inst.fields.RegA = $2; inst.fields.RegB = $4; driver.append_TEXT(inst.raw); }
   | SHR GPR COMMA GPR                  { Assembler::instruction inst; inst.fields.OC = 0b10101; inst.fields.RegA = $2; inst.fields.RegB = $4; driver.append_TEXT(inst.raw); }
   | LD operand COMMA GPR               { $2.fields.OC = 0b10110; $2.fields.RegA = $4; driver.append_TEXT($2.raw); }
   | ST GPR COMMA operand               { $4.fields.OC = 0b10111; $4.fields.RegA = $2; driver.append_TEXT($4.raw); }
   | CSRRD CSR COMMA GPR                { Assembler::instruction inst; inst.fields.OC = 0b11000; inst.fields.RegA = $2; driver.append_TEXT(inst.raw); }
   | CSRWR GPR COMMA CSR                { Assembler::instruction inst; inst.fields.OC = 0b11001; inst.fields.RegA = $2; driver.append_TEXT(inst.raw); }
   ;

operand:  
     DOLLAR NUMBER                      { $$.fields.MOD = 0b000; $$.fields.Disp = $2; }
   | NUMBER                             { $$.fields.MOD = 0b001; $$.fields.Disp = $1; }
   | DOLLAR SYMBOL                      { $$.fields.MOD = 0b010; /* TODO: check if symbol is defined etc. */$$.fields.Disp = driver.get_symbol($2, driver.TEXT.size(), true)->offset; }
   | SYMBOL                             { $$.fields.MOD = 0b011; /* TODO: check if symbol is defined etc. */$$.fields.Disp = driver.get_symbol($1, driver.TEXT.size(), true)->offset;}
   | GPR                                { $$.fields.MOD = 0b100; $$.fields.RegC = $1; }
   | LBRACKET GPR RBRACKET              { $$.fields.MOD = 0b101; $$.fields.RegC = $2; }
   | LBRACKET GPR PLUS NUMBER RBRACKET  { $$.fields.MOD = 0b110; $$.fields.RegC = $2; $$.fields.Disp = $4; }
   | LBRACKET GPR PLUS SYMBOL RBRACKET  { $$.fields.MOD = 0b111; $$.fields.RegC = $2; /* TODO: check if symbol is defined etc. */$$.fields.Disp = driver.get_symbol($4, driver.TEXT.size(), true)->offset;}
   ;


directive:
     GLOBAL symbol_list                 {  for(std::string s : $2){
                                              Assembler::Driver::STentry *entry = driver.get_symbol(s, driver.TEXT.size(), false);
                                              if(!entry){
                                                 entry = driver.insert_symbol(s, driver.current_section, driver.TEXT.size(), false);
                                              }
                                              entry->local = false;
                                           }
                                        }
   | EXTERN symbol_list                 { driver.add_extern($2); }
   | SECTION SYMBOL                     { driver.current_section = $2;
                                          driver.insert_symbol($2, driver.current_section, driver.TEXT.size());
                                        }
   | SKIP NUMBER                        { for(int i = 0; i < $2; i++) driver.TEXT.push_back(0); }
   ;

symbol_list:
   SYMBOL                               { $$ = std::vector<std::string>(); $$.push_back($1); }
   | symbol_list COMMA SYMBOL           { $$ = $1; $$.push_back($3); }
   ;

label:
   LABEL                                { /* TODO: change offset to section offset */driver.insert_symbol($1, driver.current_section, driver.TEXT.size()); }
   ;
%%

void 
Assembler::Parser::error( const location_type &l, const std::string &err_message )
{
   std::cerr << "Error: " << err_message << " at " << l << std::endl;
}
