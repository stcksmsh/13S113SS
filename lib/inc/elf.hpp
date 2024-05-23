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

#define write_to_stream(os, data) os.write(reinterpret_cast<char *>(&data), sizeof(data))

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
      uint32_t name_index;
   };

public:
   ELF()
   {
      /// Add the NULL section header
      Section_Header null_section;
      section_headers.push_back(null_section);

      /// Add the .text section header
      Section_Header text_section;
      text_section.type = Section_Header::Section_Type::SHT_PROGBITS;
      text_section.flags = Section_Header_Flags::SHF_ALLOC | Section_Header_Flags::SHF_EXECINSTR;
      text_section.addralign = 0x1;
      section_names.push_back(".text");
      section_headers.push_back(text_section);

      /// Add the .data section header
      Section_Header data_section;
      data_section.type = Section_Header::Section_Type::SHT_PROGBITS;
      data_section.flags = Section_Header_Flags::SHF_ALLOC | Section_Header_Flags::SHF_WRITE;
      data_section.addralign = 0x1;
      section_names.push_back(".data");
      section_headers.push_back(data_section);

      /// Add the .bss section header
      Section_Header bss_section;
      bss_section.type = Section_Header::Section_Type::SHT_NOBITS;
      bss_section.flags = Section_Header_Flags::SHF_ALLOC | Section_Header_Flags::SHF_WRITE;
      bss_section.addralign = 0x1;
      section_names.push_back(".bss");
      section_headers.push_back(bss_section);
   };
   ~ELF(){};

   /// @brief Serialize a shared object file
   /// @param os The output stream to serialize the shared object file to
   /// @return The output stream
   std::ostream &createShared(std::ostream &stream);

   /// @brief Deserialize the ELF file
   /// @param is The input stream to deserialize from
   /// @return The input stream
   std::istream &deserialize(std::istream &stream);

   /// @brief Add a section to the ELF file
   void add_section(const std::string name, const std::vector<uint8_t> data, const Section_Header::Section_Type type, const uint32_t flags, const uint32_t addralign);

   /// @brief Add a symbol to the ELF file
   void add_symbol(const std::string name, const std::string section, const uint32_t value, bool is_local = false, bool is_section = false);

   /// @brief Add a relocation entry to the ELF file
   void add_relocation(const std::string name, const std::string section, const uint32_t offset);

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
   std::vector<uint8_t> binary_data;
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

   /// @brief The relocation table of the ELF file
   std::vector<Symbol_Table_Entry> relocation_table;
};

#endif // __ELF_HPP__