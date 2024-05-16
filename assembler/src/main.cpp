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

#include "driver.hpp"

int 
main( const int argc, const char **argv )
{
   /** check for the right # of arguments **/
   if( argc != 2 )
   {
      /** exit with failure condition **/
      return ( EXIT_FAILURE );
   }

   if( std::strncmp( argv[ 1 ], "-h", 2 ) == 0 )
   {
      /** simple help menu **/
      std::cout << "use -o for pipe to std::cin\n";
      std::cout << "just give a filename to count from a file\n";
      std::cout << "use -h to get this menu\n";
      return( EXIT_SUCCESS );
   }

   Assembler::Driver driver;
   driver.parse( argv[1] );

   driver.print();

   return( EXIT_SUCCESS );
}
