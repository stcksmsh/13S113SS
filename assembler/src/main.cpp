/**
 * @file main.cpp
 * @author Kosta Vukicevic (stcksmsh@gmail.com)
 * @brief The `main` function for the assembler, handles command line arguments and file input, invokes the `Driver` class
 * @version 0.1
 * @date 2024-05-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>

#include "driver.hpp"
#include "arguments.hpp"

#include "elf.hpp"

int main(const int argc, const char **argv)
{
   Arguments args({
      {"", "", "Translates the input assembly file to ELF format", 1, false, true, -1},
      {"output", "o", "Specifies the output file name", 1, false, false, -1},
      {"log-level", "l", "Specifies the log level: 0 - no logging, 1 - errors only, 2 - warnings, 3 - info, 4 - debug", 1, false, false, -1},
		{"log-file", "f", "Specifies the log file", 1, false, false, -1}
   });

   if (args.parse(argc, argv) == EXIT_FAILURE)
   {
      return (EXIT_FAILURE);
   }

   std::string in_file_name = args.getArguments()[0][0];
   std::string out_file_name = "a.o";

   if (args.isPresent("output"))
   {
      out_file_name = args.getArguments("output")[0][0];
   }

   int log_level = 1; /// Default log level is errors only
   if (args.isPresent("log-level"))
   {
      log_level = std::stoi(args.getArguments("log-level")[0][0]);
   }

   std::ostream &logger_stream = std::cout;
	std::ofstream log_file;
	if (args.isPresent("log-file"))
	{
		log_file.open(args.getArguments("log-file")[0][0]);
		logger_stream.rdbuf(log_file.rdbuf());
	}

   Logger logger(log_level, true, logger_stream);
   Assembler::Driver driver(&logger);
   std::ofstream out_file;
   out_file.open(out_file_name, std::ios::binary | std::ios::trunc);
   if (!out_file.good())
   {
      std::cerr << "Unable to open file: " << out_file_name << "\n";
      return (EXIT_FAILURE);
   }

   driver.parse(in_file_name);
   driver.create_shared_file(out_file_name);

   if (log_file.is_open())
		log_file.close();

   return (EXIT_SUCCESS);
}
