#include <iostream>
#include <fstream>

#include "linker.hpp"
#include "arguments.hpp"

int main(int argc, const char **argv)
{
    Arguments args({
        {"", "", "Links ELF object files into either an executable or a relocatable object file", -1, true, true, -1},
        {"output", "o", "Specifies the output file name", 1, false, false, -1},
        {"log-level", "l", "Specifies the log level: 0 - no logging, 1 - errors only, 2 - warnings, 3 - info, 4 - debug", 1, false, false, -1},
        {"place", "p", "Specifies the address (arg2) at which the section (named arg1) is to start", 2, false, false, -1},
        {"hex", "x", "Makes the program output a memory hex dump", 0, false, false, 1},
        {"relocatable", "r", "Makes the program output a relocatable file (like the input files)", 0, false, false, 1},
    });
    
    if (args.parse(argc, argv) == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    int log_level = 0;
    std::string output_file_name;
    bool hex = false;

    if (args.isPresent("log-level"))
        log_level = std::stoi(args.getArguments("log-level")[0][0]);
    Logger logger(log_level, true);

    if (args.isPresent("hex"))
    {
        hex = true;
        output_file_name = "a.hex";
    }else if (args.isPresent("relocatable"))
    {
        hex = false;
        output_file_name = "a.o";
    }else{
        logger.logError("No output file type specified, exiting...");
    }

    if (args.isPresent("output"))
        output_file_name = args.getArguments("output")[0][0];   
    
    std::vector<std::pair<std::string, uint32_t>> section_addresses;
    if (args.isPresent("place"))
        for (auto arg : args.getArguments("place"))
            section_addresses.push_back({arg[0], std::stoi(arg[1])});

    std::vector<std::string> elf_file_names;
    for(std::vector<std::string> names : args.getArguments())
        for(std::string name : names)
            elf_file_names.push_back(name);

    if(elf_file_names.size() == 0)
    {
        logger.logError("No input files specified, exiting...");
    }

    if(logger.errorExists())
    {
        return EXIT_FAILURE;
    }

    std::ofstream output_file;
    output_file.open(output_file_name, std::ios::binary | std::ios::trunc);
    if (!output_file.good())
    {
        std::cerr << "Unable to open file: " << output_file_name << "\n";
        return EXIT_FAILURE;
    }

    Linker linker(&logger);

    logger.logInfo("Linking " + std::to_string(elf_file_names.size()) + " files");
    ELF output_elf(&logger);
    std::ifstream file;
    file.open(elf_file_names[0], std::ios::binary);

    if (!file.good())
    {
        logger.logError("Unable to open file: " + elf_file_names[0]);
        return EXIT_FAILURE;
    }

    output_elf.readFromStream(file);
    file.close();

    for (int i = 1; i < elf_file_names.size(); i++)
    {
        ELF input_elf(&logger);
        file.open(elf_file_names[i], std::ios::binary);

        if (!file.good())
        {
            logger.logError("Unable to open file: " + elf_file_names[i]);
            return EXIT_FAILURE;
        }

        input_elf.readFromStream(file);
        file.close();

        linker.link(output_elf, input_elf);
    }

    if (logger.errorExists())
    {
        return EXIT_FAILURE;
    }

    for (auto section_address : section_addresses)
    {
        output_elf.setSectionAddress(section_address.first, section_address.second);
    }

    if (hex)
    {
        output_elf.memDump(output_file);
        std::ofstream elfFile;
        elfFile.open("test.o", std::ios::binary);
        output_elf.createShared(elfFile);
    }
    else
    {
        std::cout << "Creating relocatable file" << std::endl;
        output_elf.createShared(output_file);
    }

    return 0;
}