/**
 * @file driver.hpp
 * @author Kosta Vukicevic (stcksmsh@gmail.com)
 * @brief Driver class header file, used for parsing input files, main class of assembler
 * @version 0.1
 * @date 2024-05-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */



#ifndef __DRIVER_H__
#define __DRIVER_H__


#include <string>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <vector>

#include "scanner.hpp"
#include "parser.hh"

namespace Assembler{

class Driver{
private:
      struct STentry{
      std::string name;
      std::string section;
      uint32_t offset;
      bool local;
      bool is_defined;
      struct STforward_ref{
         uint32_t offset;
         STforward_ref *next;
      } *forward_refs;
   };

public:
   Driver(): parser(nullptr), scanner(nullptr), symbol_table() {}

   virtual ~Driver();
   
   /// @brief Parse the input file, wrapper for `parse_helper`
   /// @param filename Name of the file to parse
   void parse( const char * const filename );

   /// @brief Insert a symbol into the symbol table
   /// @param name The name of the symbol
   /// @param section The name of the section the symbol is in
   /// @param offset The offset of the symbol in the section
   /// @param is_defined Whether the symbol is defined or not
   /// @return A pointer to the symbol table entry
   STentry *insert_symbol( const std::string name, const std::string section, const uint32_t offset, const bool is_defined = true);

   /// @brief Get a symbol from the symbol table
   /// @param name The name of the symbol
   /// @return A pointer to the symbol table entry (or nullptr if not found)
   STentry *get_symbol( const std::string name, const uint32_t offset, bool do_reference = true);

   /// @brief Add a forward reference to a symbol
   /// @param name The name of the symbol
   /// @param section Which section the symbol is in
   /// @param offset The offset of the reference
   void forward_reference( const std::string name, const std::string section, const uint32_t offset);
   
   void add_extern( const std::vector<std::string> &externs);

   void print(){
      std::cout << "Symbol Table:" << std::endl;
      std::cout << "Symbol\t\t|\tSection\t|\tOffset\t|\tLocal/Global\t|\tDefined/Undefined" << std::endl;
      std::cout << "---------------------------------------------------------------------------------------------" << std::endl;
      for(int i = 0; i < symbol_table.size(); i++){
      std::cout << symbol_table[i].name << "\t\t|\t" << symbol_table[i].section << "\t|\t" << symbol_table[i].offset << "\t|\t" << (symbol_table[i].local ? "local" : "global") << \
         "\t\t|\t" << (symbol_table[i].is_defined ? "defined" : "undefined") << std::endl;
      }
      std::cout << std::endl << std::endl;
      std::cout << "Extern List:" << std::endl;
      for(int i = 0; i < extern_list.size(); i++){
         std::cout << " - " <<  extern_list[i] << std::endl;
      }
      std::cout << std::endl << std::endl;
      std::cout << "TEXT Section:" << std::endl;
      std::cout << std::hex << "\t0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F" << std::endl;
      for(int i = 0; i < TEXT.size();){
         std::cout << std::hex << i << ":\t";
         for(int j = 0; j < 16; j++){
            if( i < TEXT.size() ){
               std::cout << std::hex << ((int)TEXT[i]&0XF) << (((int)TEXT[i]>>4)&0xF);
               std::cout << "  ";
            }else{
               std::cout << "--  ";
            }
            i++;
         }
         std::cout << std::endl;
      }
   }

   /// @brief Output the ELF binary file
   /// @param stream The output file stream
   /// @return The output file stream
   std::ostream& output(std::ostream &stream);

private:
   /// @brief Parse the input file
   /// @param stream The input file stream
   void parse_helper( std::istream &stream );

   /// @brief Set a DWORD in the TEXT section
   /// @param text The binary data to set
   /// @param offset Where to set the data
   void set_TEXT( const uint32_t text, uint32_t offset );

   /// @brief Append binary data to the TEXT section
   /// @param text What to append
   void append_TEXT( const uint32_t text );

   Assembler::Parser  *parser  = nullptr;
   Assembler::Scanner *scanner = nullptr;

   std::string current_section = "";

   friend class Assembler::Parser;
   friend class Assembler::Scanner;

   std::vector<STentry> symbol_table;

   std::vector<std::string> extern_list;

   /* I will use char vectors to store the binary data */
   /// @brief RODATA section binary data
   std::vector<char> RODATA;
   /// @brief DATA section binary data
   std::vector<char> DATA;
   /// @brief TEXT section binary data
   std::vector<char> TEXT;
   /// @brief BSS section binary data
   std::vector<char> BSS;
};

};


#endif // __DRIVER_H__