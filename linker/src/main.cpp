#include <iostream>
#include <fstream>

#include "linker.hpp"
#include "arguments.hpp"
#include <format>

uint32_t stringToAddress(std::string str){
    if(str.size() > 2 && str[0] == '0' && str[1] == 'x')
        return std::stoul(str, nullptr, 16);
    if(str.size() > 2 && str[0] == '0' && str[1] == 'b')
        return std::stoul(str, nullptr, 2);
    if(str.size() > 1 && str[0] == '0')
        return std::stoul(str, nullptr, 8);
    return std::stoul(str);

}

int main(int argc, const char **argv)
{
    Arguments args({
        {"", "", "Links ELF object files into either an executable or a relocatable object file", -1, true, true, -1},
        {"output", "o", "Specifies the output file name", 1, false, false, -1},
        {"log-level", "l", "Specifies the log level: 0 - no logging, 1 - errors only, 2 - warnings, 3 - info, 4 - debug", 1, false, false, -1},
        {"place", "p", "Specifies the address (arg2) at which the section (named arg1) is to start", 2, true, false, -1},
        {"hex", "x", "Makes the program output a memory hex dump", 0, false, false, 1},
        {"relocatable", "r", "Makes the program output a relocatable file (like the input files)", 0, false, false, 1},
		{"log-file", "f", "Specifies the log file", 1, false, false, -1}
    });
    
    if (args.parse(argc, argv) == EXIT_FAILURE)
        return EXIT_FAILURE;

    int log_level = 1;
    std::string output_file_name;
    bool hex = false;

    if (args.isPresent("log-level"))
        log_level = std::stoi(args.getArguments("log-level")[0][0]);

    std::ostream &logger_stream = std::cout;
	std::ofstream log_file;
	if (args.isPresent("log-file"))
	{
		log_file.open(args.getArguments("log-file")[0][0]);
		logger_stream.rdbuf(log_file.rdbuf());
	}
    Logger logger(log_level, true, logger_stream);

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
            section_addresses.push_back({arg[0], stringToAddress(arg[1])});

    for(auto section_address : section_addresses)
        logger.logInfo("Section " + section_address.first + " placed at address 0x" + std::format("{:08x}", section_address.second));



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
        if(logger.errorExists())
        {
            logger.logError("Reading failed on file: " + elf_file_names[i]);
            return EXIT_FAILURE;
        }

        file.close();

        linker.link(output_elf, input_elf);
        if(logger.errorExists())
        {
            logger.logError("Linking failed on file: " + elf_file_names[i]);
            return EXIT_FAILURE;
        }
    }

    for (auto section_address : section_addresses)
    {
        output_elf.setSectionAddress(section_address.first, section_address.second);
    }

    if (hex){
        output_elf.memDump(output_file);
        // std::ofstream output_elf_file;
        // output_elf_file.open(output_file_name + ".elf", std::ios::binary | std::ios::trunc);
        // if (!output_elf_file.good())
        // {
        //     std::cerr << "Unable to open file: " << output_file_name + ".elf" << "\n";
        //     return EXIT_FAILURE;
        // }
        // output_elf.createShared(output_elf_file);
    }else        output_elf.createShared(output_file);

    if(logger.errorExists())
    {
        logger.logError("Error while creating output file " + output_file_name);
        return EXIT_FAILURE;
    }

    if (log_file.is_open())
		log_file.close();

    return 0;
}