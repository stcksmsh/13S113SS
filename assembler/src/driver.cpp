/**
 * @file driver.cpp
 * @author Kosta Vukicevic (stcksmsh@gmail.com)
 * @brief Driver class implementation file, used for parsing input files, main class of assembler
 * @version 0.1
 * @date 2024-05-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <cctype>
#include <fstream>
#include <cassert>
#include <stddef.h>

#include "driver.hpp"
#include "elf.hpp"

#include "colors.h"

Assembler::Driver::~Driver()
{
   delete(scanner);
   scanner = nullptr;
   delete(parser);
   parser = nullptr;
}

void 
Assembler::Driver::parse( const std::string &filename)
{
   assert( filename.empty() == false );
   std::ifstream in_file( filename );
   if( ! in_file.good() )
   {
      std::cerr << "Unable to open file: " << filename << "\n";
      exit( EXIT_FAILURE );
   }
   parse_helper( in_file );
   logger.print();
   if(logger.errorExists()){
      exit(EXIT_FAILURE);
   }
   return;
}

void 
Assembler::Driver::parse_helper( std::istream &stream )
{
   delete(scanner);
   try
   {
      scanner = new Assembler::Scanner( &stream );
   }
   catch( std::bad_alloc &ba )
   {
      std::cerr << "Failed to allocate scanner: (" <<
         ba.what() << "), exiting!!\n";
      exit( EXIT_FAILURE );
   }
   
   delete(parser); 
   try
   {
      parser = new Assembler::Parser( (*scanner) /* scanner */, 
                                  (*this) /* driver */ );
   }
   catch( std::bad_alloc &ba )
   {
      std::cerr << "Failed to allocate parser: (" << 
         ba.what() << "), exiting!!\n";
      exit( EXIT_FAILURE );
   }
   const int accept( 0 );
   if( parser->parse() != accept )
   {
      std::cerr << "Parse failed!!\n";
   }

   for(std::string symbol :  global_list){
      STentry *entry = get_symbol(symbol);
      if(!entry || !entry->is_defined){
         logger.logError("Symbol '" + symbol + "' which is declared as global is not defined");
      }
      if(entry){
         entry->local = false;
      }
   }

   for(std::string symbol : extern_list){
      STentry *entry = get_symbol(symbol);
      if(!entry){
         logger.logWarning("Symbol '" + symbol + "' which is declared as extern is not defined");
      }else{
         entry->local = false;
      }
   }

   return;
}

Assembler::Driver::STentry *
Assembler::Driver::insert_symbol( const std::string name, const bool is_defined, const bool is_section){
   std::string section = current_section_ref.get().name;
   uint32_t offset = current_section_ref.get().size;
   /// First check if the symbol is already in the table
   for(int i = 0; i < symbol_table.size(); i ++){
      if(symbol_table[i].name == name){
         /// If it is defined, throw an error
         if(symbol_table[i].is_defined){
            logger.logError("Symbol '" + name + "' is already defined in section '" + symbol_table[i].section + "' at offset " + std::to_string(symbol_table[i].offset));
         }
         /// Otherwise complete the entry
         symbol_table[i].section = section;
         symbol_table[i].offset = offset;
         symbol_table[i].is_defined = is_defined;
         if(is_defined){
            /// And resolve all existing forward references
            STentry::STforward_ref *current = symbol_table[i].forward_refs;
            symbol_table[i].forward_refs = nullptr;
            while(current){
               set_TEXT(symbol_table[i].offset, current->offset);            
               STentry::STforward_ref *next = current->next;
               delete current;
               current = next;
            }
         }
         return &symbol_table[i];
      }
   }
   /// If the symbol is not in the table, add it
   STentry new_entry;
   new_entry.name = name;
   new_entry.section = section;
   new_entry.offset = offset;
   new_entry.local = true;
   new_entry.is_defined = is_defined;
   new_entry.is_section = is_section;
   new_entry.forward_refs = nullptr;
   symbol_table.push_back(new_entry);
   return &symbol_table[symbol_table.size() - 1];
}

Assembler::Driver::STentry *
Assembler::Driver::get_symbol( const std::string name ){
   for(int i = 0; i < symbol_table.size(); i++){
      if(symbol_table[i].name == name){
         return &symbol_table[i];
      }
   }
   /// If the symbol is not in the table return nullptr
   return nullptr;
}

void
Assembler::Driver::forward_reference( const std::string name){
   std::string section = current_section_ref.get().name;
   std::size_t offset = current_section_ref.get().size;
   for(int i = 0; i < symbol_table.size(); i++){
      if(symbol_table[i].name == name){
         STentry::STforward_ref *current = symbol_table[i].forward_refs;
         symbol_table[i].forward_refs = new STentry::STforward_ref;
         symbol_table[i].forward_refs->section = section;
         symbol_table[i].forward_refs->offset = offset;
         symbol_table[i].forward_refs->next = current;
         return;
      }
   }
   /// If the symbol is not in the table, add it as an undefined symbol
   STentry new_entry;
   new_entry.name = name;
   new_entry.section = section;
   new_entry.offset = 0;
   new_entry.local = true;
   new_entry.is_defined = false;
   new_entry.forward_refs = new STentry::STforward_ref;
   new_entry.forward_refs->offset = offset;
   new_entry.forward_refs->section = section;
   new_entry.forward_refs->next = nullptr;
   symbol_table.push_back(new_entry);
}

void
Assembler::Driver::add_extern( const std::vector<std::string> &externs ){
   for(int i = 0; i < externs.size(); i++){
      extern_list.push_back(externs[i]);
   }
}

void
Assembler::Driver::add_global( const std::vector<std::string> &globals ){
   for(int i = 0; i < globals.size(); i++){
      global_list.push_back(globals[i]);
   }
}

void
Assembler::Driver::create_shared_file( const std::string &filename ){
   std::ofstream out_file;
   out_file.open(filename);
   if( ! out_file.good() )
   {
      std::cerr << "Unable to open file: " << filename << "\n";
      exit( EXIT_FAILURE );
   }
   ELF elf;
   for(int i = 0; i < section_list.size(); i++){
      std::string name = section_list[i].name;
      std::size_t offset = section_list[i].offset;
      std::size_t size = section_list[i].size;
      std::vector<uint8_t> data = std::vector<uint8_t>(TEXT.begin() + offset, TEXT.begin() + offset + size);

      uint32_t flags = Section_Header_Flags::SHF_ALLOC | Section_Header_Flags::SHF_EXECINSTR;      
      elf.add_section(name, data, Section_Header::Section_Type::SHT_PROGBITS, flags, 1);

      for(int j = 0; j < symbol_table.size(); j++){
         if(symbol_table[j].section == name){
            elf.add_symbol(symbol_table[j].name, name, symbol_table[j].offset, symbol_table[j].local, symbol_table[j].is_section);
         }
      }

      for(int j = 0; j < symbol_table.size(); j++){
         if(symbol_table[j].section == name && !symbol_table[j].is_defined){
            STentry::STforward_ref *current = symbol_table[j].forward_refs;
            while(current){
               elf.add_relocation(symbol_table[j].name, current->section, current->offset);
               current = current->next;
            }
         }
      }

   }

   elf.createShared(out_file);
}

void
Assembler::Driver::add_section(std::string section_name){
   for(int i = 0; i < section_list.size(); i++){
      if(section_list[i].name == section_name){
         logger.logError("Section '" + section_name + "' already exists");
      }
   }
   Section new_section;
   new_section.name = section_name;
   new_section.offset = TEXT.size();
   new_section.size = 0;
   section_list.push_back(new_section);
   current_section_ref = std::ref(section_list[section_list.size() - 1]);
   insert_symbol(section_name, true, true);
}

void
Assembler::Driver::append_TEXT( const uint32_t text ){
   for(int i = 0; i < 4; i++){
      /// TODO: check if this is correct
      TEXT.push_back((text >> (i * 8)) & 0xFF);
   }
   current_section_ref.get().size += 4;
}

void
Assembler::Driver::set_TEXT( const uint32_t text, uint32_t offset ){
   for(int i = 0; i < 4; i++){
      /// TODO: check if this is correct
      TEXT[offset + i] = (text >> (i * 8)) & 0xFF;
   }
}
