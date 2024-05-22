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
#include "logger.hpp"

namespace Assembler{

class Driver{
private:
      struct STentry{
      std::string name;
      std::string section;
      uint32_t offset;
      bool local;
      bool is_defined;
      bool is_section;
      struct STforward_ref{
         std::string section;
         uint32_t offset;
         STforward_ref *next;
      } *forward_refs;
   };

public:
   Driver(int debugLevel = 0): parser(nullptr), scanner(nullptr), current_section_ref(section_list[0]), logger(false, debugLevel) {
      /// Without this we get SIGSEGVd so it will stay here
      Section text_section = {"", 0, 0};
      current_section_ref = std::ref(text_section);
   }

   virtual ~Driver();
   
   /// @brief Parse the input file, wrapper for `parse_helper`
   /// @param filename The name of the file to parse
   void parse( const std::string &filename );

   /// @brief Insert a symbol into the symbol table
   /// @param name The name of the symbol
   /// @param is_defined Whether the symbol is defined or not
   /// @return A pointer to the symbol table entry
   STentry *insert_symbol( const std::string name, const bool is_defined = true, const bool is_section = false );

   /// @brief Get a symbol from the symbol table
   /// @param name The name of the symbol
   /// @return A pointer to the symbol table entry (or nullptr if not found)
   STentry *get_symbol( const std::string name );

   /// @brief Add a forward reference to a symbol
   /// @param name The name of the symbol
   void forward_reference( const std::string name);
   
   /// @brief Add an extern to the extern list
   void add_extern( const std::vector<std::string> &externs);

   /// @brief Add a global symbol to the symbol table
   void add_global( const std::vector<std::string> &globals);

   /// @brief Create an ELF file from the parsed data
   /// @param filename The name of the file to create
   void create_shared_file( const std::string &filename );

   void print(std::ostream& stream){
      stream << "Symbol Table:" << std::endl;
      stream << "Symbol\t\t|\tSection\t\t|\tOffset\t\t|\tLocal/Global\t|\tDefined/Undefined" << std::endl;
      stream << "---------------------------------------------------------------------------------------------" << std::endl;
      for(int i = 0; i < symbol_table.size(); i++){
         stream << symbol_table[i].name << ( symbol_table[i].name.size() < 8 ? "\t\t|\t" : "\t|\t");
         stream << symbol_table[i].section << ( symbol_table[i].section.size() < 8 ? "\t\t|\t" : "\t|\t");
         stream << std::hex << symbol_table[i].offset << std::dec << ( symbol_table[i].offset < 0xffffff ? "\t\t|\t" : "\t|\t");
         stream << (symbol_table[i].local ? "local" : "global") << "\t\t|\t";
         stream << (symbol_table[i].is_defined ? "defined" : "undefined") << std::endl;
      }
      stream << std::endl << std::endl;
      stream << "Extern List:" << std::endl;
      for(int i = 0; i < extern_list.size(); i++){
         stream << " - " <<  extern_list[i] << std::endl;
      }
      stream << std::endl << std::endl;
      
      stream << "Sections:" << std::endl;
      stream << "Name\t|\tOffset\t|\tSize" << std::endl;
      stream << "--------------------------------" << std::endl;
      for(int i = 0; i < section_list.size(); i++){
         stream << section_list[i].name << "\t|\t" << section_list[i].offset << "\t|\t" << section_list[i].size << std::endl;
      }

      stream << std::endl << std::endl;
      stream << "TEXT:" << std::endl;
      stream << std::hex << "\t0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F" << std::endl;
      for(int i = 0; i < TEXT.size();){
         stream << std::hex << i << ":\t";
         for(int j = 0; j < 16; j++){
            if( i < TEXT.size() ){
               stream << std::hex << ((int)TEXT[i]&0XF) << (((int)TEXT[i]>>4)&0xF);
               stream << "  ";
            }else{
               stream << "--  ";
            }
            i++;
         }
         stream << std::endl;
      }

      stream << std::endl << std::endl;
      stream << "Undefined symbols:" << std::endl;
      for(int i = 0; i < symbol_table.size(); i++){
         if(!symbol_table[i].is_defined){
            stream << " - " << symbol_table[i].name << std::endl;
            if(symbol_table[i].forward_refs){
               STentry::STforward_ref *current = symbol_table[i].forward_refs;
               while(current){
                  stream << "\t- '" << current->section << "' " << current->offset << std::endl;
                  current = current->next;
               }
            }
         }
      }

   }

   /// @brief Add a section to the program and make it the current section
   /// @param name The name of the section
   void add_section( std::string section_name);

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
   Logger logger;

   struct Section{
      std::string name;
      std::size_t offset;
      std::size_t size;
   };

   std::vector<Section> section_list;

   std::reference_wrapper<Section> current_section_ref;

   friend class Assembler::Parser;
   friend class Assembler::Scanner;

   /// @brief The symbol table
   std::vector<STentry> symbol_table;

   /// @brief The extern list
   std::vector<std::string> extern_list;

   /// @brief The global list
   std::vector<std::string> global_list;

   /* I will use a uint8_t vector to store the binary data */
   /// @brief TEXT section binary data
   std::vector<uint8_t> TEXT;
};

};


#endif // __DRIVER_H__