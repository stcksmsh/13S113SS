%skeleton "lalr1.cc"
%language "c++"
%require  "3.8"
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
      enum csr_type: char{
         STATUS = 0,
         HANDLER = 1,
         CAUSE = 2
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
      struct extended_instruction{
         instruction inst;
         uint32_t extension;
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
%token	                           MINUS       "-"
%token	                           DOLLAR      "$"

/* Type tokens */

%token <int32_t>                    NUMBER      "literal"
%token <uint8_t>                    GPR         "general purpose register"
%token <Assembler::csr_type>        CSR         "control and status register"
%token <std::string>                SYMBOL      "symbol"
%token <std::string>                LABEL       "label"
%token <std::string>                STRING       "ascii"


/* Types */

%type <std::vector<std::string>>    symbol_list "symbol_list"

%type <std::vector<uint32_t>>       symbol_or_literal_list "symbol_or_literal_list"

%type <Assembler::extended_instruction>      operand     "operand"

%type <uint32_t>                    expression  "expression"

%type <std::string>                 label   "label_type"
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
   | directive EOL
   | label directive EOL
   | instruction EOL
   | label instruction EOL
   | bss_directive EOL
   | data_directive EOL
   ;

instruction:
     HALT                               { Assembler::instruction inst; inst.fields.OC = Opcode::HALT; driver.append_TEXT(inst.raw); }
   | INT                                { Assembler::instruction inst; inst.fields.OC = Opcode::INT; driver.append_TEXT(inst.raw); }
   | IRET                               { Assembler::instruction inst; inst.fields.OC = Opcode::IRET; driver.append_TEXT(inst.raw); }
   | CALL operand                       { $2.inst.fields.OC = Opcode::CALL; 
                                          driver.append_TEXT($2.inst.raw); 
                                          if(($2.inst.fields.MOD & 0b100) == 0){
                                             driver.append_TEXT($2.extension);
                                          }
                                        }
   | RET                                { Assembler::instruction inst; inst.fields.OC = Opcode::RET; driver.append_TEXT(inst.raw); }
   | JMP operand                        { $2.inst.fields.OC = Opcode::JMP; 
                                          driver.append_TEXT($2.inst.raw);
                                          if(($2.inst.fields.MOD & 0b100) == 0){
                                             driver.append_TEXT($2.extension);
                                          }
                                        }
   | BEQ GPR COMMA GPR COMMA operand    { $6.inst.fields.OC = Opcode::BEQ; $6.inst.fields.RegA = $2; $6.inst.fields.RegB = $4; 
                                          driver.append_TEXT($6.inst.raw);
                                          if(($6.inst.fields.MOD & 0b100) == 0){
                                             driver.append_TEXT($6.extension);
                                          }
                                        }
   | BNE GPR COMMA GPR COMMA operand    { $6.inst.fields.OC = Opcode::BNE; $6.inst.fields.RegA = $2; $6.inst.fields.RegB = $4; 
                                          driver.append_TEXT($6.inst.raw);
                                          if(($6.inst.fields.MOD & 0b100) == 0){
                                             driver.append_TEXT($6.extension);
                                          }
                                        }
   | BGT GPR COMMA GPR COMMA operand    { $6.inst.fields.OC = Opcode::BGT; $6.inst.fields.RegA = $2; $6.inst.fields.RegB = $4; 
                                          driver.append_TEXT($6.inst.raw);
                                          if(($6.inst.fields.MOD & 0b100) == 0){
                                             driver.append_TEXT($6.extension);
                                          }
                                        }
   | PUSH GPR                           { Assembler::instruction inst; inst.fields.OC = Opcode::PUSH; inst.fields.RegA = $2; driver.append_TEXT(inst.raw); }
   | POP GPR                            { Assembler::instruction inst; inst.fields.OC = Opcode::POP; inst.fields.RegA = $2; driver.append_TEXT(inst.raw); }
   | XCHG GPR COMMA GPR                 { Assembler::instruction inst; inst.fields.OC = Opcode::XCHG; inst.fields.RegA = $2; inst.fields.RegB = $4; driver.append_TEXT(inst.raw); }
   | ADD GPR COMMA GPR                  { Assembler::instruction inst; inst.fields.OC = Opcode::ADD; inst.fields.RegA = $2; inst.fields.RegB = $4; driver.append_TEXT(inst.raw); }
   | SUB GPR COMMA GPR                  { Assembler::instruction inst; inst.fields.OC = Opcode::SUB; inst.fields.RegA = $2; inst.fields.RegB = $4; driver.append_TEXT(inst.raw); }
   | MUL GPR COMMA GPR                  { Assembler::instruction inst; inst.fields.OC = Opcode::MUL; inst.fields.RegA = $2; inst.fields.RegB = $4; driver.append_TEXT(inst.raw); }
   | DIV GPR COMMA GPR                  { Assembler::instruction inst; inst.fields.OC = Opcode::DIV; inst.fields.RegA = $2; inst.fields.RegB = $4; driver.append_TEXT(inst.raw); }
   | NOT GPR                            { Assembler::instruction inst; inst.fields.OC = Opcode::NOT; inst.fields.RegA = $2; driver.append_TEXT(inst.raw); }
   | AND GPR COMMA GPR                  { Assembler::instruction inst; inst.fields.OC = Opcode::AND; inst.fields.RegA = $2; inst.fields.RegB = $4; driver.append_TEXT(inst.raw); }
   | OR GPR COMMA GPR                   { Assembler::instruction inst; inst.fields.OC = Opcode::OR; inst.fields.RegA = $2; inst.fields.RegB = $4; driver.append_TEXT(inst.raw); }
   | XOR GPR COMMA GPR                  { Assembler::instruction inst; inst.fields.OC = Opcode::XOR; inst.fields.RegA = $2; inst.fields.RegB = $4; driver.append_TEXT(inst.raw); }
   | SHL GPR COMMA GPR                  { Assembler::instruction inst; inst.fields.OC = Opcode::SHL; inst.fields.RegA = $2; inst.fields.RegB = $4; driver.append_TEXT(inst.raw); }
   | SHR GPR COMMA GPR                  { Assembler::instruction inst; inst.fields.OC = Opcode::SHR; inst.fields.RegA = $2; inst.fields.RegB = $4; driver.append_TEXT(inst.raw); }
   | LD operand COMMA GPR               { $2.inst.fields.OC = Opcode::LD; $2.inst.fields.RegA = $4; 
                                          driver.append_TEXT($2.inst.raw);
                                          if(($2.inst.fields.MOD & 0b100) == 0){
                                             driver.append_TEXT($2.extension);
                                          }
                                        }
   | ST GPR COMMA operand               { $4.inst.fields.OC = Opcode::ST; $4.inst.fields.RegA = $2;
                                          driver.append_TEXT($4.inst.raw);
                                          if(($4.inst.fields.MOD & 0b100) == 0){
                                             driver.append_TEXT($4.extension);
                                          }
                                        }
   | CSRRD CSR COMMA GPR                { Assembler::instruction inst; inst.fields.OC = Opcode::CSRRD; inst.fields.RegA = $2; inst.fields.RegB = $4; driver.append_TEXT(inst.raw); }
   | CSRWR GPR COMMA CSR                { Assembler::instruction inst; inst.fields.OC = Opcode::CSRWR; inst.fields.RegA = $2; inst.fields.RegB = $4; driver.append_TEXT(inst.raw); }
   ;

operand:  
     DOLLAR NUMBER                      { $$.inst.fields.MOD = Modifier::LIT_DIR; $$.extension = $2; }
   | NUMBER                             { $$.inst.fields.MOD = Modifier::LIT_IND; $$.extension = $1; }
   | DOLLAR SYMBOL                      { $$.inst.fields.MOD = Modifier::SYM_DIR;
                                          Driver::STentry *entry = driver.get_symbol($2);
                                          if(!entry || !entry->is_defined){
                                             driver.forward_reference($2);
                                             $$.extension = 0;
                                          }else{
                                             if(!entry->is_const){
                                                driver.add_relocation($2);
                                             } 
                                             $$.extension = entry->offset;
                                          }
                                        }
   | SYMBOL                             { $$.inst.fields.MOD = Modifier::SYM_IND;
                                          Driver::STentry *entry = driver.get_symbol($1);
                                          if(!entry || !entry->is_defined){
                                             driver.forward_reference($1);
                                             $$.extension = 0;
                                          }else{
                                             if(!entry->is_const){
                                                driver.add_relocation($1);
                                             } 
                                             $$.extension = entry->offset;
                                          }
                                        }
   | GPR                                { $$.inst.fields.MOD = Modifier::REG_DIR; $$.inst.fields.RegC = $1; }
   | LBRACKET GPR RBRACKET              { $$.inst.fields.MOD = Modifier::REG_IND; $$.inst.fields.RegC = $2; }
   | LBRACKET GPR PLUS NUMBER RBRACKET  { $$.inst.fields.MOD = Modifier::REG_LIT_IND; $$.inst.fields.RegC = $2;
                                          if(($4 > (1<<12-1)) || $4 < 0-(1<<12)){
                                             driver.logger->logError("Literal value is out of range for REG_LIT_IND addressing", driver.filename, @3.begin.line);
                                          }
                                          else{
                                             $$.inst.fields.Disp = $4;
                                          }
                                        }
   | LBRACKET GPR PLUS SYMBOL RBRACKET  { $$.inst.fields.MOD = Modifier::REG_SYM_IND; $$.inst.fields.RegC = $2;
                                          Driver::STentry *entry = driver.get_symbol($4);
                                          if(!entry || !entry->is_defined){
                                             driver.logger->logError("Symbol '" + $4 + "' is not defined at assembly time, which is required for REG_SYM_IND addressing", driver.filename, @3.begin.line);
                                          }
                                          else if(entry->offset > 1<<12-1 || entry->offset < -(1<<12)){
                                             driver.logger->logError("Symbol '" + $4 + "' is out of range for REG_SYM_IND addressing", driver.filename, @3.begin.line);
                                          }
                                          else{

                                             if(!entry->is_const){
                                                driver.add_relocation($4);
                                             }
                                             $$.inst.fields.Disp = entry->offset;
                                          }
                                        }
   ;


directive:
     GLOBAL symbol_list                 { driver.add_global($2); }
   | EXTERN symbol_list                 { driver.add_extern($2); }
   | SECTION SYMBOL                     { driver.add_section($2); }
   | EQU SYMBOL COMMA expression        { driver.insert_symbol($2, true, false, "", true);
                                          driver.get_symbol($2)->offset = $4;
                                        }
   ;

data_directive:
     WORD symbol_or_literal_list        { for(uint32_t val : $2) driver.append_DATA(val); }
   | label WORD symbol_or_literal_list  { driver.update_symbol($1, ".data");
                                          for(uint32_t val : $3) driver.append_DATA(val);
                                        }
   | ASCII STRING                       { for(char c : $2) driver.DATA.push_back(c); }
   | label ASCII STRING                 { driver.update_symbol($1, ".data");
                                          for(char c : $3) driver.DATA.push_back(c);
                                        }
   ;

bss_directive:
      SKIP NUMBER                       { driver.extend_BSS($2); }
   | label SKIP NUMBER                  { driver.update_symbol($1, ".bss");
                                          driver.extend_BSS($3);
                                        }
   ;

symbol_list:
     SYMBOL                             { $$ = std::vector<std::string>(); $$.push_back($1); }
   | symbol_list COMMA SYMBOL           { $$ = $1; $$.push_back($3); }
   ;

symbol_or_literal_list:
     NUMBER                             { $$ = std::vector<uint32_t>(); $$.push_back($1); }
   | SYMBOL                             { $$ = std::vector<uint32_t>();
                                          Driver::STentry *entry = driver.get_symbol($1);
                                          $$.push_back(driver.get_symbol($1)->offset);
                                          
                                        }
   | symbol_or_literal_list COMMA NUMBER{ $$ = $1; $$.push_back($3); }
   | symbol_or_literal_list COMMA SYMBOL{ $$ = $1;
                                          Driver::STentry *entry = driver.get_symbol($3);
                                          if(!entry){
                                             driver.logger->logError("Undefined symbol", driver.filename, @3.begin.line);
                                          }
                                          $$.push_back(driver.get_symbol($3)->offset); }

label:
   LABEL                                { $$ = $1; driver.insert_symbol($1); }
   | label EOL
   ;

expression:
     NUMBER                             { $$ = $1; }
   | expression PLUS NUMBER             { $$ = $1 + $3; if($$ < $1) Parser::error(@2, "Overflow occured"); }
   | expression MINUS NUMBER            { $$ = $1 - $3; if($$ > $1) Parser::error(@2, "Underflow occured"); }

%%

void 
Assembler::Parser::error( const location_type &l, const std::string &err_message )
{
   driver.logger->logError(err_message, driver.filename, l.begin.line);
   // std::cerr << "Error: " << err_message << " at " << l << std::endl;

   /*


   */
}
