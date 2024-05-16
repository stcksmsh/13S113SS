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

Assembler::Driver::~Driver()
{
   delete(scanner);
   scanner = nullptr;
   delete(parser);
   parser = nullptr;
}

void 
Assembler::Driver::parse( const char * const filename )
{
   assert( filename != nullptr );
   std::ifstream in_file( filename );
   if( ! in_file.good() )
   {
      std::cerr << "Unable to open file: " << filename << "\n";
      exit( EXIT_FAILURE );
   }
   parse_helper( in_file );
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
   return;
}

Assembler::Driver::STentry *
Assembler::Driver::insert_symbol( const std::string name, const std::string section, const uint32_t offset, const bool is_defined ){
   /// First check if the symbol is already in the table
   for(int i = 0; i < symbol_table.size(); i ++){
      if(symbol_table[i].name == name){
         /// If it is defined, throw an error
         if(symbol_table[i].is_defined){
            std::cerr << "Symbol " << name << " is already defined in section " << symbol_table[i].section << " at offset " << symbol_table[i].offset << std::endl;
            exit(EXIT_FAILURE);
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
               std::cout << "Resolving forward reference for symbol '" << name << "' at offset " << current->offset << std::endl;
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
   new_entry.forward_refs = nullptr;
   symbol_table.push_back(new_entry);
   return &symbol_table[symbol_table.size() - 1];
}

Assembler::Driver::STentry *
Assembler::Driver::get_symbol( const std::string name, const uint32_t offset, bool do_reference ){
   for(int i = 0; i < symbol_table.size(); i++){
      if(symbol_table[i].name == name){
         return &symbol_table[i];
      }
   }
   /// If the symbol is not in the table return nullptr
   return nullptr;
}

void
Assembler::Driver::forward_reference( const std::string name, const std::string section, const uint32_t offset ){
   for(int i = 0; i < symbol_table.size(); i++){
      if(symbol_table[i].name == name){
         STentry::STforward_ref *current = symbol_table[i].forward_refs;
         symbol_table[i].forward_refs = new STentry::STforward_ref;
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
Assembler::Driver::append_TEXT( const uint32_t text ){
   for(int i = 0; i < 4; i++){
      /// TODO: check if this is correct
      TEXT.push_back((text >> (i * 8)) & 0xFF);
   }
}

void
Assembler::Driver::set_TEXT( const uint32_t text, uint32_t offset ){
   for(int i = 0; i < 4; i++){
      /// TODO: check if this is correct
      TEXT[offset + i] = (text >> (i * 8)) & 0xFF;
   }
}



/// @brief helper function to write raw binary data to the stream
std::ostream &write(std::ostream& stream, const void *data, size_t size){
   unsigned char *ptr = (unsigned char *)data;
   while(size--){
      stream.put(*ptr++);
   }
   return stream;
}

/// @brief Print the ELF binary file to the stream
/// @param stream The stream to write to
/// @return The stream after writing
std::ostream&
Assembler::Driver::output(std::ostream &stream){
   struct ELF_Header{
      /// @brief The magic number of the ELF file
      uint8_t magic[4] = { 0x7F, 'E', 'L', 'F' };
      /// @brief The class of the ELF file ( 1 for 32-bit )
      uint8_t class_ = 1;
      /// @brief The data encoding of the ELF file ( 1 for little-endian )
      uint8_t data = 1;
      /// @brief The version of the ELF file ( 1 for original )
      uint8_t version = 1;
      /// @brief The OS ABI of the ELF file ( 0 for System V )
      uint8_t os_abi = 0;
      /// @brief The ABI version of the ELF file ( undefined here, so basically extra padding )
      uint8_t abi_version = 0;
      /// @brief 7 padding bytes
      uint8_t padding[7] = {0};
      /// @brief The type of the ELF file ( 1 - relocatable, 2 - exec, 3 - shared )
      enum ELF_Type: uint16_t{
         /// @brief Relocatable file
         ET_REL = 0x1,
         /// @brief Executable file
         ET_EXEC = 0x2,
         /// @brief Shared object file
         ET_DYN = 0x3,
         /// @brief Core file
         ET_CORE = 0x4,
         /// @brief Processor-specific
         ET_LOPROC = 0xFF00,
         /// @brief Processor-specific
         ET_HIPROC = 0xFFFF
      } type = ELF_Type::ET_EXEC;
      /// @brief The machine architecture of the ELF file ( 3e for AMD x86-64 )
      uint16_t machine = 0x3e;
      /// @brief The version of the ELF file ( 1 for original )
      uint32_t version2 = 0x1;
      /// @brief The entry point of the ELF file
      uint32_t entry = 0x0;
      /// @brief The offset of the program header table (right after this one)
      uint32_t ph_offset = sizeof(ELF_Header);
      /// @brief The offset of the section header table
      uint32_t sh_offset = 0x0;
      /// @brief The flags of the ELF file (unused here, so 0x0)
      uint32_t flags = 0x0;
      /// @brief The size of this header
      uint16_t eh_size = sizeof(ELF_Header);
      /// @brief The size of a program header
      uint16_t ph_size = 0x20;
      /// @brief The number of program headers
      uint16_t ph_num = 0x1;
      /// @brief The size of a section header
      uint16_t sh_size = 0x0;
      /// @brief The number of section headers
      uint16_t sh_num = 0x0;
      /// @brief The index of the section header string table
      uint16_t sh_str_index = 0x0;
   } file_header;

   struct Program_Header{
      /// @brief The type of the program header ( 1 for loadable segment )
      enum Program_Type: uint32_t{
         /// @brief Unused program header
         PT_NULL = 0x0,
         /// @brief Loadable segment
         PT_LOAD = 0x1,
         /// @brief Dynamic linking information
         PT_DYNAMIC = 0x2,
         /// @brief Program interpreter
         PT_INTERP = 0x3,
         /// @brief Auxiliary information
         PT_NOTE = 0x4,
         /// @brief Reserved
         PT_SHLIB = 0x5,
         /// @brief Program header table
         PT_PHDR = 0x6,
         /// @brief Reserved
         PT_LOPROC = 0x70000000,
         /// @brief Reserved
         PT_HIPROC = 0x7FFFFFFF
      } type = Program_Type::PT_LOAD;
      /// @brief The offset of the segment in the file
      uint32_t offset = 0x0;
      /// @brief The virtual address of the segment in memory
      uint32_t vaddr = 0x0;
      /// @brief The physical address of the segment in memory (can be left as 0?)
      uint32_t paddr = 0x0;
      /// @brief The size of the segment in the file
      uint32_t filesz = 0x0;
      /// @brief The size of the segment in memory
      uint32_t memsz = 0x0;
      /// @brief The flags of the segment (1,2,4 are executable, writable, readable, ORed together)
      uint32_t flags = 0x5;
      /// @brief The alignment of the segment (0 or 1 for no alignment)
      uint32_t align = 0x0;
   } program_header;

   struct Section_Header{
      /// @brief The name of the section (index in the string table)
      uint32_t name = 0x0;
      /// @brief The type of the section
      enum Section_Type: uint32_t{
         /// @brief Unused section
         SHT_NULL = 0x0,
         /// @brief Section with data
         SHT_PROGBITS = 0x1,
         /// @brief Section containing symbol table
         SHT_SYMTAB = 0x2,
         /// @brief Section containing string table
         SHT_STRTAB = 0x3,
         /// @brief Section containing relocation entries
         SHT_RELA = 0x4,
         /// @brief Section containing hash table
         SHT_HASH = 0x5,
         /// @brief Section containing dynamic linking information
         SHT_DYNAMIC = 0x6,
         /// @brief Section containing notes
         SHT_NOTE = 0x7,
         /// @brief Section containing uninitialized data
         SHT_NOBITS = 0x8,
         /// @brief Section containing relocation entries with explicit addends
         SHT_REL = 0x9,
         /// @brief Section containing reserved range
         SHT_SHLIB = 0x0A,
         /// @brief Section containing dynamic linker symbol table
         SHT_DYNSYM = 0x0B,
         /// @brief Section containing array of constructors
         SHT_INIT_ARRAY = 0x0E,
         /// @brief Section containing array of destructors
         SHT_FINI_ARRAY = 0x0F,
         /// @brief Section containing pre-init values
         SHT_PREINIT_ARRAY = 0x10,
         /// @brief Section containing section group
         SHT_GROUP = 0x11,
         /// @brief Section containing extended section indices
         SHT_SYMTAB_SHNDX = 0x12,
         /// @brief Section containing number of reserved types
         SHT_NUM = 0x13,
         /// @brief Section with unknown type
         SHT_UNKNOWN = 0xFFFFFFFF
      } type = Section_Type::SHT_PROGBITS;
      /// @brief The flags of the section
      uint32_t flags = 0x0;
      /// @brief The virtual address of the section in memory
      uint32_t addr = 0x0;
      /// @brief The offset of the section in the file
      uint32_t offset = 0x0;
      /// @brief The size of the section in the file
      uint32_t size = 0x0;
      /// @brief The index of the section header table entry
      uint32_t link = 0x0;
      /// @brief The extra information about the section
      uint32_t info = 0x0;
      /// @brief The alignment of the section
      uint32_t addralign = 0x0;
      /// @brief The size of each entry in the section
      uint32_t entsize = 0x0;
   } section_header;

   /// First set the relevant data in the headers, then write everything to the stream

   /*  TODO: set headers  */

   /// Now write the file and program headers to the stream
   write(stream, (const void*)&file_header, sizeof(file_header));
   write(stream, (const void*)&program_header, sizeof(program_header));
   /// Then we write the entries
   /*  TODO: write data  */
   /// Lastly we write the section header to the stream
   write(stream, &section_header, sizeof(section_header));
   return stream;
}