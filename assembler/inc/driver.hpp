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

namespace Assembler
{

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
         struct STforward_ref
         {
            std::string section;
            uint32_t offset;
            STforward_ref *next;
         } *forward_refs;
      };

   public:
      Driver(Logger *logger) : parser(nullptr), scanner(nullptr), current_section_ref(section_list[0]), logger(logger)
      {
         logger->logDebug("Driver object created");
         /// Without this we get SIGSEGVd so it will stay here
         Section text_section = {"", 0, 0};
         current_section_ref = std::ref(text_section);
      }

      virtual ~Driver();

      /// @brief Parse the input file, wrapper for `parse_helper`
      /// @param filename The name of the file to parse
      void parse(const std::string &filename);

      /// @brief Insert a symbol into the symbol table
      /// @param name The name of the symbol
      /// @param is_defined Whether the symbol is defined or not
      /// @return A pointer to the symbol table entry
      STentry *insert_symbol(const std::string name, const bool is_defined = true, const bool is_section = false, std::string section = "");

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

      /// @brief Add an extern to the extern list
      void add_extern(const std::vector<std::string> &externs);

      /// @brief Add a global symbol to the symbol table
      void add_global(const std::vector<std::string> &globals);

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

      /// @brief Set a DWORD in the TEXT section
      /// @param text The binary data to set
      /// @param offset Where to set the data
      void set_TEXT(const uint32_t text, uint32_t offset);

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

      std::reference_wrapper<Section> current_section_ref;

      friend class Assembler::Parser;
      friend class Assembler::Scanner;

      /// @brief The symbol table
      std::vector<STentry> symbol_table;

      /// @brief The extern list
      std::vector<std::string> extern_list;

      /// @brief The global list
      std::vector<std::string> global_list;

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

#endif // __DRIVER_H__