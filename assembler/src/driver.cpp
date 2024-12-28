/**
 * @file driver.cpp
 * @author Kosta Vukicevic (107367925+stcksmsh@users.noreply.github.com)
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
#include "formatter.hpp"
#include "../../linker/inc/linker.hpp"
#include <string.h>

Assembler::Driver::~Driver()
{
   logger->logDebug("Driver object destroyed");
   delete (scanner);
   scanner = nullptr;
   delete (parser);
   parser = nullptr;
   for(STentry entry : symbol_table){
      STentry::STforward_ref *current = entry.forward_refs;
      while(current){
         STentry::STforward_ref *next = current->next;
         delete current;
         current = next;
      }
   }
}

void Assembler::Driver::parse(const std::string &filename)
{
   this->filename = filename;
   assert(filename.empty() == false);
   std::ifstream in_file(filename);
   if (!in_file.good())
   {
      logger->logError("Unable to open file '" + filename + "'");
      return;
   }
   parse_helper(in_file);
   if (logger->errorExists())
   {
      exit(EXIT_FAILURE);
   }
   return;
}

void Assembler::Driver::parse_helper(std::istream &stream)
{
   logger->logInfo("Parsing input file", filename);
   delete (scanner);
   try
   {
      scanner = new Assembler::Scanner(&stream);
   }
   catch (std::bad_alloc &ba)
   {
      logger->logError("Failed to allocate scanner: (" + std::string(ba.what()) + "), exiting!!");
      return;
   }

   delete (parser);
   try
   {
      parser = new Assembler::Parser((*scanner) /* scanner */,
                                     (*this) /* driver */);
   }
   catch (std::bad_alloc &ba)
   {
      logger->logError("Failed to allocate parser: (" + std::string(ba.what()) + "), exiting!!");
      return;
   }
   const int accept(0);
   if (parser->parse() != accept)
   {
      logger->logError("Parse failed, exiting!!");
   }

   /// For each EQU symbol, resolve the expression
   for (std::pair<std::string, EquStruct *> equ : equ_list)
   {
      logger->logInfo("Resolving EQU symbol '" + equ.first + "'");
      int32_t value = 0;
      EquStruct *seek = equ.second;
      if(seek->name == ""){
         value = seek->value;
      }else{
         STentry *entry = get_symbol(seek->name);
         if(!entry){
            logger->logError("Symbol '" + seek->name + "' not found", filename);
         }
         value = entry->offset;
      }
      char op;
      while(seek->next){
         op = seek->operation;
         seek = seek->next;
         int32_t nextValue = 0;
         if(seek->name == ""){
            nextValue = seek->value;
         }else{
            STentry *entry = get_symbol(seek->name);
            if(!entry){
               logger->logError("Symbol '" + seek->name + "' not found", filename);
            }
            nextValue = entry->offset;
         }
         switch(op){
            case '+':
               value += nextValue;
               break;
            case '-':
               value -= nextValue;
               break;
            default:
               logger->logError("Invalid operation '" + std::string(1, op) + "'", filename);
         }
      }
      STentry *symbol = get_symbol(equ.first);
      if(!symbol){
         logger->logError("Symbol '" + equ.first + "' not found", filename);
      }
      symbol->offset = value;
      symbol->is_defined = true;
      symbol->is_const = true;
      logger->logDebug("Resolved EQU symbol '" + equ.first + "' to 0x" + Formatter::format("{:08x}", value), filename);
      
      /// Now delete the EQU struct
      EquStruct *currentEqu = equ.second;
      while(currentEqu){
         EquStruct *next = currentEqu->next;
         delete currentEqu;
         currentEqu = next;
      }

      /// Now resolve all forward references
      STentry::STforward_ref *currentRef = symbol->forward_refs;
      symbol->forward_refs = nullptr;
      while (currentRef)
      {
         logger->logDebug("Resolving forward reference in section '" + currentRef->section + "' at offset 0x" + Formatter::format("{:x}", currentRef->offset), filename);
         add_relocation(equ.first, currentRef->section, currentRef->offset);
         STentry::STforward_ref *next = currentRef->next;
         delete currentRef;
         currentRef = next;
      }
   }


   for (std::string symbol : global_list)
   {
      STentry *entry = get_symbol(symbol);
      if (!entry || !entry->is_defined)
      {
         logger->logWarning("Symbol '" + symbol + "' which is declared as global is not defined");
      }
      if (entry)
      {
         entry->local = false;
      }
   }

   for (std::string symbol : extern_list)
   {
      STentry *entry = get_symbol(symbol);
      if (!entry)
      {
         logger->logWarning("Symbol '" + symbol + "' which is declared as extern is not used");
      }
      else
      {
         if(entry->is_defined){
            logger->logError("Symbol '" + symbol + "' which is declared as extern is defined");
         }
         entry->local = false;
      }
   }

   /// If there are any local symbols that are not defined, throw an error
   for(STentry entry : symbol_table){
      if(entry.local && !entry.is_defined){
         logger->logError("Local symbol '" + entry.name + "' is not defined", filename);
      }
   }

   return;
}

Assembler::Driver::STentry *
Assembler::Driver::insert_symbol(const std::string name, const bool is_defined, const bool is_section, std::string section, const bool is_const)
{
   logger->logInfo("Inserting symbol '" + name + "'", filename, scanner->lineno());
   uint32_t offset = 0;
   if(!section_list.empty() && section == ""){
      offset = section_list.back().size;
      section = section_list.back().name;
   }
   /// First check if the symbol is already in the table
   for (std::size_t i = 0; i < symbol_table.size(); i++)
   {
      if (symbol_table[i].name == name)
      {
         /// If it is defined, throw an error
         if (symbol_table[i].is_defined)
         {
            logger->logError("Symbol '" + name + "' is already defined in section '" + symbol_table[i].section + "' at offset 0x" + Formatter::format("{:x}", symbol_table[i].offset), filename, scanner->lineno());
         }
         /// Otherwise complete the entry
         symbol_table[i].section = section;
         symbol_table[i].offset = offset;
         symbol_table[i].is_defined = is_defined;
         if (is_defined)
         {
            logger->logDebug("Resolving forward references for symbol '" + name + "'", filename, scanner->lineno());
            /// And resolve all existing forward references
            STentry::STforward_ref *current = symbol_table[i].forward_refs;
            symbol_table[i].forward_refs = nullptr;
            while (current)
            {
               logger->logDebug("Resolving forward reference in section '" + current->section + "' at offset 0x" + Formatter::format("{:x}", current->offset), filename);
               add_relocation(name, current->section, current->offset);
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
   new_entry.is_const = is_const;
   new_entry.forward_refs = nullptr;
   symbol_table.push_back(new_entry);
   return &symbol_table[symbol_table.size() - 1];
}

void Assembler::Driver::update_symbol(std::string name, std::string section){
   logger->logInfo("Updating symbol '" + name + "' to section '" + section + "'", filename, scanner->lineno());
   for (std::size_t i = 0; i < symbol_table.size(); i++)
   {
      if (symbol_table[i].name == name)
      {
         if(symbol_table[i].is_section){
            logger->logError("Symbol '" + name + "' is a section and cannot be updated", filename, scanner->lineno());
            return;
         }
         uint32_t new_offset = (section == ".data") ? DATA.size() : BSS;
         logger->logInfo("Updating relocations for symbol '" + name + "' in section '" + section + "'", filename, scanner->lineno());
         for(std::size_t j = 0; j < relocation_list.size(); j++){
            if (relocation_list[j].src_section == symbol_table[i].section && relocation_list[j].src_offset == symbol_table[i].offset)
            {
               logger->logDebug("Updating relocation in section '" + relocation_list[j].dst_section + "' at offset 0x" + Formatter::format("{:x}", relocation_list[j].dst_offset) + " to point to section '" + section + "' at offset 0x" + Formatter::format("{:x}", symbol_table[i].offset), filename, scanner->lineno());
               relocation_list[j].src_section = section;
               relocation_list[j].src_offset = new_offset;
            }
         }
         symbol_table[i].offset = new_offset;
         symbol_table[i].section = section;
         return;
      }
   }
   logger->logError("Symbol '" + name + "' not found", filename, scanner->lineno());
}

Assembler::Driver::STentry *
Assembler::Driver::get_symbol(const std::string name)
{
   logger->logInfo("Getting symbol '" + name + "'");
   for (std::size_t i = 0; i < symbol_table.size(); i++)
   {
      if (symbol_table[i].name == name)
      {
         logger->logDebug("Found symbol '" + name + "' in section '" + symbol_table[i].section + "' at offset 0x" + Formatter::format("{:x}", symbol_table[i].offset));
         return &symbol_table[i];
      }
   }
   /// If the symbol is not in the table return nullptr
   logger->logDebug("Symbol '" + name + "' not found");
   return nullptr;
}

void Assembler::Driver::forward_reference(const std::string name)
{
   logger->logInfo("Forward referencing symbol '" + name + "' in section '" + section_list.back().name, filename, scanner->lineno());
   std::string section = section_list.back().name;
   std::size_t offset = section_list.back().size;
   for (std::size_t i = 0; i < symbol_table.size(); i++)
   {
      if (symbol_table[i].name == name)
      {
         logger->logDebug("Symbol '" + name + "' found in section '" + symbol_table[i].section + "' at offset 0x" + Formatter::format("{:x}", symbol_table[i].offset));
         STentry::STforward_ref *current = symbol_table[i].forward_refs;
         symbol_table[i].forward_refs = new STentry::STforward_ref;
         symbol_table[i].forward_refs->section = section;
         symbol_table[i].forward_refs->offset = offset;
         symbol_table[i].forward_refs->next = current;
         return;
      }
   }
   logger->logDebug("Symbol '" + name + "' not found, adding as undefined symbol");
   /// If the symbol is not in the table, add it as an undefined symbol
   STentry new_entry;
   new_entry.name = name;
   new_entry.section = "";
   new_entry.offset = 0;
   new_entry.local = true;
   new_entry.is_defined = false;
   new_entry.forward_refs = new STentry::STforward_ref;
   new_entry.forward_refs->offset = offset;
   new_entry.forward_refs->section = section;
   new_entry.forward_refs->next = nullptr;
   symbol_table.push_back(new_entry);
}

void Assembler::Driver::add_relocation(const std::string symbol, const std::string section , const uint32_t offset)
{
   if(section == ""){
      /// This means that the method was invoked from the parser, when it found a defined symbol
      logger->logDebug("Adding relocation for symbol '" + symbol + "' in section '" + section + "' at offset 0x" + Formatter::format("{:x}", offset), filename, scanner->lineno());
      std::string current_section = section_list.back().name;
      uint32_t current_offset = section_list.back().size;
      STentry *entry = get_symbol(symbol);
      uint32_t symbol_offset = entry->offset;
      std::string symbol_section = entry->section;
      Relocation new_relocation;
      new_relocation.src_section = symbol_section;
      new_relocation.src_offset = symbol_offset;
      new_relocation.dst_section = current_section;
      new_relocation.dst_offset = current_offset;

      relocation_list.push_back(new_relocation);
   }else{
      /// This means that the method was invoked when a symbols forward reference was resolved
      logger->logDebug("Adding relocation for symbol '" + symbol + "' in section '" + section + "' at offset 0x" + Formatter::format("{:x} while resolving its forward ref", offset), filename, scanner->lineno());
      Relocation new_relocation;
      STentry *entry = get_symbol(symbol);
      new_relocation.src_section = entry->section;
      new_relocation.src_offset = entry->offset;
      if(entry->is_const){
         new_relocation.src_section = "";
         new_relocation.src_offset = entry->offset;
      }
      new_relocation.dst_section = section;
      new_relocation.dst_offset = offset;
      relocation_list.push_back(new_relocation);
   }
}

void Assembler::Driver::add_extern(const std::vector<std::string> &externs)
{
   logger->logInfo("Adding extern symbols", filename, scanner->lineno());
   for (std::size_t i = 0; i < externs.size(); i++)
   {
      logger->logDebug("Adding extern symbol '" + externs[i] + "'");
      extern_list.push_back(externs[i]);
   }
}

void Assembler::Driver::add_global(const std::vector<std::string> &globals)
{
   logger->logInfo("Adding global symbols", filename, scanner->lineno());
   for (std::size_t i = 0; i < globals.size(); i++)
   {
      logger->logDebug("Adding global symbol '" + globals[i] + "'", filename);
      global_list.push_back(globals[i]);
   }
}

void Assembler::Driver::add_equ(const std::string &name, EquStruct *equ)
{
   logger->logInfo("Adding EQU symbol '" + name + "'");
   STentry *entry = insert_symbol(name, false);
   equ_list.push_back(std::pair<std::string, EquStruct *>(name, equ));
}

void Assembler::Driver::create_shared_file(const std::string &filename)
{
   logger->logInfo("Creating shared file '" + filename + "'\n----------------------------------------------------");
   std::ofstream out_file;
   out_file.open(filename);
   if (!out_file.good())
   {
      logger->logError("Unable to open file '" + filename + "'");
      return;
   }
   ELF elf(logger);
   if (logger->errorExists())
   {
      logger->logError("Errors encountered while creating ELF file, aborting");
      return;
   }
   if(DATA.size() > 0){
      elf.add_section(".data", DATA, Section_Header::Section_Type::SHT_PROGBITS, Section_Header_Flags::SHF_ALLOC | Section_Header_Flags::SHF_WRITE, 1);
      elf.add_symbol(".data", ".data", 0, true, true, true);
   }
   if(BSS > 0){
      elf.add_section(".bss", std::vector<char>(), Section_Header::Section_Type::SHT_NOBITS, Section_Header_Flags::SHF_ALLOC | Section_Header_Flags::SHF_WRITE, 1);
      elf.section_headers.back().size = BSS;
      elf.add_symbol(".bss", ".bss", 0, true, true, true);
   }
   for (std::size_t i = 0; i < section_list.size(); i++)
   {
      if(section_list[i].size == 0){
         logger->logWarning("Section '" + section_list[i].name + "' in file '" + this->filename + "' is empty, skipping");
         continue;
      }
      std::string name = section_list[i].name;
      std::size_t offset = section_list[i].offset;
      std::size_t size = section_list[i].size;
      std::vector<char> data = std::vector<char>(TEXT.begin() + offset, TEXT.begin() + offset + size);
      uint32_t flags = Section_Header_Flags::SHF_ALLOC | Section_Header_Flags::SHF_EXECINSTR;
      elf.add_section(name, data, Section_Header::Section_Type::SHT_PROGBITS, flags, 1);
   }

   for (std::size_t j = 0; j < symbol_table.size(); j++)
   {
      if(symbol_table[j].is_section){
         /// If the section size is 0, skip it
         bool empty = true;
         for(std::size_t i = 0; i < section_list.size(); i++){
            if(section_list[i].name == symbol_table[j].name){
               empty = section_list[i].size == 0;
               break;
            }
         }
         if(empty) continue; /// No need to log warning, as this case is already handled on line 339

      }
      elf.add_symbol(symbol_table[j].name, symbol_table[j].section, symbol_table[j].offset, symbol_table[j].local, symbol_table[j].is_section, symbol_table[j].is_const, !symbol_table[j].is_defined);
   }

   /// Add relocations for undefined symbols
   for (std::size_t j = 0; j < symbol_table.size(); j++)
   {
      STentry::STforward_ref *current = symbol_table[j].forward_refs;
      while (current)
      {
         elf.add_relocation(symbol_table[j].name, current->section, current->offset);
         current = current->next;
      }
   }

   /// Add relocations for defined ( used in sections other than where they are defined ) symbols
   for(int i = 0; i < relocation_list.size(); i ++){
      elf.add_relocation(relocation_list[i].src_section, relocation_list[i].dst_section, relocation_list[i].dst_offset, relocation_list[i].src_offset);
   }

   elf.createShared(out_file);
}

void Assembler::Driver::add_section(std::string section_name)
{
   logger->logInfo("Adding section '" + section_name + "'", filename, scanner->lineno());
   for (std::size_t i = 0; i < section_list.size(); i++)
   {
      if (section_list[i].name == section_name)
      {
         logger->logError("Section '" + section_name + "' already exists");
      }
   }
   Section new_section;
   new_section.name = section_name;
   new_section.offset = TEXT.size();
   new_section.size = 0;
   section_list.push_back(new_section);
   current_section_index = section_list.size() - 1;
   insert_symbol(section_name, true, true);
}

void Assembler::Driver::append_TEXT(const uint32_t text)
{
   logger->logDebug("Appending binary data to '" + section_list.back().name + "' section", filename, scanner->lineno());
   for (int i = 0; i < 4; i++)
   {  
      TEXT.push_back((text >> (i * 8)) & 0xFF);
   }
   section_list.back().size += 4;
}

void Assembler::Driver::append_DATA(const uint32_t data)
{
   logger->logDebug("Appending binary data to DATA section", filename, scanner->lineno());
   for (int i = 0; i < 4; i++)
   {
      DATA.push_back((data >> (i * 8)) & 0xFF);
   }
}

void Assembler::Driver::extend_BSS(const std::size_t bss)
{
   logger->logDebug("Extending BSS section by " + std::to_string(bss) + " bytes", filename, scanner->lineno());
   BSS += bss;
}
      