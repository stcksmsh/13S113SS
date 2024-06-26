/**
 * @file driver.hpp
 * @author Kosta Vukicevic (107367925+stcksmsh@users.noreply.github.com)
 * @brief Driver class header file, used for parsing input files, main class of assembler
 * @version 0.1
 * @date 2024-05-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __DRIVER_HPP__
#define __DRIVER_HPP__

#include <string>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <vector>

#include "scanner.hpp"
#include "parser.hh"
#include "logger.hpp"

namespace Assembler
{
   /// @brief The opcode enum
   enum Opcode
   {
      HALT = 0b00000,
      INT = 0b00001,
      IRET = 0b00010,
      CALL = 0b00011,
      RET = 0b00100,
      JMP = 0b00101,
      BEQ = 0b00110,
      BNE = 0b00111,
      BGT = 0b01000,
      PUSH = 0b01001,
      POP = 0b01010,
      XCHG = 0b01011,
      ADD = 0b01100,
      SUB = 0b01101,
      MUL = 0b01110,
      DIV = 0b01111,
      NOT = 0b10000,
      AND = 0b10001,
      OR = 0b10010,
      XOR = 0b10011,
      SHL = 0b10100,
      SHR = 0b10101,
      LD = 0b10110,
      ST = 0b10111,
      CSRRD = 0b11000,
      CSRWR = 0b11001,
   };

   /// @brief The modifier
   enum Modifier
   {
      LIT_DIR = 0b000,
      SYM_DIR = 0b001,
      LIT_IND = 0b010,
      SYM_IND = 0b011,
      REG_DIR = 0b100,
      REG_IND = 0b101,
      REG_LIT_IND = 0b110,
      REG_SYM_IND = 0b111,
   };

   /// @brief The driver class, used for parsing input files
   class Driver
   {
   private:
      struct STentry
      {
         std::string name;
         std::string section;
         uint32_t offset;
         bool local;
         bool is_defined;
         bool is_section;
         bool is_const = false;
         struct STforward_ref
         {
            std::string section;
            uint32_t offset;
            STforward_ref *next;
         } *forward_refs;
      };

   public:
      Driver(Logger *logger) : parser(nullptr), scanner(nullptr), logger(logger)
      {
         logger->logDebug("Driver object created");
      }

      virtual ~Driver();

      /// @brief Parse the input file, wrapper for `parse_helper`
      /// @param filename The name of the file to parse
      void parse(const std::string &filename);

      /// @brief Insert a symbol into the symbol table
      /// @param name The name of the symbol
      /// @param is_defined Whether the symbol is defined or not
      /// @return A pointer to the symbol table entry
      STentry *insert_symbol(const std::string name, const bool is_defined = true, const bool is_section = false, std::string section = "", const bool is_const = false);

      /// @brief Update a symbol in the symbol table (used for .data and .bss section symbols)
      /// @param name The name of the symbol
      /// @param section The section the symbol is in
      void update_symbol(const std::string name, std::string section);

      /// @brief Get a symbol from the symbol table
      /// @param name The name of the symbol
      /// @return A pointer to the symbol table entry (or nullptr if not found)
      STentry *get_symbol(const std::string name);

      /// @brief Add a forward reference to a symbol
      /// @param name The name of the symbol
      void forward_reference(const std::string name);

      /// @brief Add a relocation to the relocation list (the symbol is presumed to be defined)
      /// @param symbol The symbol to relocate
      /// @param section The section in which the symbol is used
      /// @param offset The offset in the section where the symbol is used
      void add_relocation(const std::string symbol, const std::string section = "", const uint32_t offset = 0);

      /// @brief Add an extern to the extern list
      void add_extern(const std::vector<std::string> &externs);

      /// @brief Add a global symbol to the symbol table
      void add_global(const std::vector<std::string> &globals);

      /// @brief Add an equ to the equ list
      void add_equ(const std::string &name, EquStruct *equ);

      /// @brief Create an ELF file from the parsed data
      /// @param filename The name of the file to create
      void create_shared_file(const std::string &filename);

      /// @brief Add a section to the program and make it the current section
      /// @param name The name of the section
      void add_section(std::string section_name);

   private:
      /// @brief Parse the input file
      /// @param stream The input file stream
      void parse_helper(std::istream &stream);

      /// @brief Append binary data to the TEXT section
      /// @param text What to append
      void append_TEXT(const uint32_t text);

      /// @brief Append binary data to the DATA section
      /// @param data What to append
      void append_DATA(const uint32_t data);

      /// @brief Extend the BSS section
      /// @param bss The size to extend the BSS section by
      void extend_BSS(const std::size_t bss);

      Assembler::Parser *parser = nullptr;
      Assembler::Scanner *scanner = nullptr;
      Logger *logger;

      struct Section
      {
         std::string name;
         std::size_t offset;
         std::size_t size;
      };

      std::vector<Section> section_list;

      int current_section_index = -1;

      friend class Assembler::Parser;
      friend class Assembler::Scanner;

      /// @brief The symbol table
      std::vector<STentry> symbol_table;

      /// @brief The extern list
      std::vector<std::string> extern_list;

      /// @brief The global list
      std::vector<std::string> global_list;

      /// @brief The equ list
      std::vector<std::pair<std::string, EquStruct *>> equ_list;
      
      /// @brief The relocation list
      struct Relocation {
         /// @brief The section in which the symbol is defined
         std::string src_section;
         /// @brief The offset in the section where the symbol is defined
         uint32_t src_offset;
         /// @brief The section where the symbol is used
         std::string dst_section;
         /// @brief The offset in the section where the symbol is used
         uint32_t dst_offset;
      };
      std::vector<Relocation> relocation_list;

      /* I will use a char vector to store the binary data */
      /// @brief TEXT section binary data
      std::vector<char> TEXT;

      /// @brief DATA section binary data
      std::vector<char> DATA;

      /// @brief BSS section size
      std::size_t BSS = 0;

      /// @brief The name of the file being parsed
      std::string filename;

   };

};

#endif // __DRIVER_HPP__