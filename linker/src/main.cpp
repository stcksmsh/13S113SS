#include <iostream>

#include "arguments.hpp"

int main(int argc, const char **argv)
{
    Arguments args({
        {"", "", "Links object files into either an executable or a relocatable object file", -1, false, false, -1},
        {"output", "o", "Specifies the output file name", 1, false, false, -1},
        {"log-level", "l", "Specifies the log level: 0 - no logging, 1 - errors only, 2 - warnings, 3 - info, 4 - debug", 1, false, false, -1},
        {"place=", "p", "Specifies the address (arg2) at which the section (named arg1) is to start", 2, false, false, -1},
        {"hex", "h", "Makes the program output a memory hex dump", 0, false, false, 1},
        {"relocatable", "r", "Makes the program output a relocatable file (like the input files)", 0, false, false, 1},
    });
    if (args.parse(argc, argv) == EXIT_FAILURE)
        return EXIT_FAILURE;

    if (args.isPresent("output"))
    {
        std::cout << "Output file: " << args.getArguments("output")[0][0] << std::endl;
    }
    if (args.isPresent("log-level"))
    {
        std::cout << "Log level: " << args.getArguments("log-level")[0][0] << std::endl;
    }
    if (args.isPresent("place"))
    {
        std::cout << "Place: " << args.getArguments("place")[0][0] << " " << args.getArguments("place")[0][1] << std::endl;
    }
    if (args.isPresent("hex"))
    {
        std::cout << "Hex: true" << std::endl;
    }
    if (args.isPresent("relocatable"))
    {
        std::cout << "Relocatable: true" << std::endl;
    }
    std::vector<std::vector<std::string>> otherArgs = args.getArguments();
    return 0;
}