/**
 * @file elf.hpp
 * @author Kosta Vukicevic (stcksmsh@gmail.com)
 * @brief ELF class header file, used for working with ELF files (serialization, deserialization)
 * @version 0.1
 * @date 2024-05-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __ELF_HPP__
#define __ELF_HPP__

/// This header has structure definitions (headers, etc.) and field values for the ELF file format

#include <iostream>
#include <cstdint>
#include <cstddef>
#include <vector>

#include "elf_structs.hpp"
#include "logger.hpp"

#define write_to_stream(os, data) os.write(reinterpret_cast<char *>(&data), sizeof(data))
namespace Assembler
{
   class Driver;
};

class ELF
{
public:
   /// Entry in the symbol table
   struct Symbol_Table_Entry
   {
      std::string name;
      std::string section;
      uint32_t value;
      bool is_local;
      bool is_section;
      bool is_const;
      bool is_extern;
      uint32_t name_index;
   };

public:
   ELF(Logger *logger) : logger(logger)
   {
      /// Add the NULL section header
      Section_Header null_section;
      section_names.push_back("");
      section_headers.push_back(null_section);

      /// Add the NULL symbol table entry
      Symbol_Table_Entry null_symbol;
      null_symbol.name = "";
      null_symbol.section = "";
      null_symbol.value = 0;
      null_symbol.is_local = true;
      null_symbol.is_section = false;
      null_symbol.name_index = 0;
      symbol_table.push_back(null_symbol);
   };
   ~ELF(){};

   /// @brief Create a shared object file
   /// @param os The output stream to serialize the shared object file to
   /// @return The output stream
   std::ostream &createShared(std::ostream &stream);

   /// @brief Create an executable file
   /// @param stream The output stream to serialize the executable file to
   /// @return The output stream
   std::ostream &createExecutable(std::ostream &stream); 

   /// @brief Set the address of a section
   /// @param section The name of the section
   /// @param address The address to set
   void setSectionAddress(const std::string section, const uint32_t address);

   /// @brief Read the ELF object from a file
   /// @param is The input stream to deserialize from
   /// @return The input stream
   std::istream &readFromStream(std::istream &stream);

   /// @brief Add a section to the ELF file
   void add_section(const std::string name, const std::vector<char> data, const Section_Header::Section_Type type, const uint32_t flags, const uint32_t addralign);

   /// @brief Add a symbol to the ELF file
   void add_symbol(const std::string name, const std::string section, const uint32_t value, bool is_local = false, bool is_section = false, bool is_const = false, bool is_extern = false);

   /// @brief Add a relocation entry to the ELF file
   void add_relocation(const std::string name, const std::string section, const uint32_t offset, uint32_t addend = 0);

private:
   /* The format will be as follows:
    *
    *    ELF Header
    *    Program Header
    *    Data
    *       TEXT Section
    *       Section Header String Table
    *    Section Headers
    *       NULL header
    *       Individual section headers
    */

   /// @brief The ELF header of the ELF file
   ELF_Header file_header;

   /// @brief The binary data of the ELF file (what was written in the source file)
   std::vector<char> binary_data;
   /// @brief The program headers of the ELF file
   std::vector<Program_Header> program_headers;
   /// @brief The section headers of the ELF file
   /// Mandatory sections are: NULL, .text, .data, .bss, which are then followed by user defined sections
   /// and after those sections come the .symtab, .strtab and .shstrtab sections
   std::vector<Section_Header> section_headers;
   /// @brief The names of the sections (in order)
   std::vector<std::string> section_names;

   /// @brief The symbol table of the ELF file
   std::vector<Symbol_Table_Entry> symbol_table;

   struct Relocation_Entry
   {
      std::string section;
      std::string name;
      uint32_t offset;
      uint32_t addend;
      
   };

   /// @brief The relocation table of the ELF file
   std::vector<Relocation_Entry> relocation_table;

   /// @brief The logger object
   Logger *logger;


   friend class Assembler::Driver;
   friend class Linker;
};

#endif // __ELF_HPP__