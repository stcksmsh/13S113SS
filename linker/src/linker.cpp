#include "linker.hpp"
#include <string.h>

void
Linker::link(ELF &output, ELF &input)
{
    logger->logInfo("Merging ELFs\n------------------------------------");
    logger->logDebug("Input ELF file has " + std::to_string(input.section_names.size()) + " sections");
    for(int i = 1; i < input.section_names.size(); i++)
    {
        std::string section_name = input.section_names[i];
        logger->logInfo("Processing section: " + section_name);
        /// Check whether the section is already present in the output file
        int index = 0;
        for(;index < output.section_names.size(); index++)
        {
            if(output.section_names[index] == section_name)
            {
                logger->logDebug("Section '" + section_name + "' already present in the output file at index " + std::to_string(index));
                break;
            }
        }
        if(index == output.section_names.size())
        {
            logger->logDebug("Section '" + section_name + "' not found in the output file");
            /// Add the section to the output file
            output.section_names.push_back(section_name);
            output.section_headers.push_back(input.section_headers[i]);
            output.binary_data.resize(output.binary_data.size() + input.section_headers[i].size);
            /// Add the sections binary data to the output file
            memcpy(&output.binary_data[output.section_headers[i].offset], &input.binary_data[input.section_headers[i].offset], input.section_headers[i].size);
            logger->logDebug("Section '" + section_name + "' added to the output file");
        }
        else
        {
            logger->logDebug("Merging section '" + section_name + "'");
            if(section_name != ".bss" && input.section_headers[i].size > 0) /// If the section has data to copy
            {
                std::size_t header_size = sizeof(ELF_Header) + output.program_headers.size() * sizeof(Program_Header);
                /// First resize the output file to accomodate the new section data
                output.binary_data.resize(output.binary_data.size() + input.section_headers[i].size);
                /// Then shift the data after the section to make space for the new section data
                char *dest = output.binary_data.data() + output.section_headers[index].offset + output.section_headers[index].size - header_size;
                char *src = output.binary_data.data() + output.section_headers[index].offset + output.section_headers[index].size + input.section_headers[i].size - header_size;
                std::size_t size = output.binary_data.size() - (output.section_headers[index].offset - header_size) - output.section_headers[index].size + 1; /// TODO: Why +1???
                memmove(dest, src, size); /// THIS LINE IS THE PROBLEM
                /// And finally copy the new section data to the output file
                dest = output.binary_data.data() + output.section_headers[index].offset + output.section_headers[index].size - header_size;
                src = input.binary_data.data() + input.section_headers[i].offset - header_size;
                size = input.section_headers[i].size;
                memcpy(dest, src, size);
            }
            /// Then shift all of the input section symbols to the right by the size of the output section
            for(auto &symbol : input.symbol_table)
            {
                if(symbol.section != section_name || symbol.is_extern || symbol.is_section || symbol.is_extern)
                    continue;
                logger->logWarning("Symbol '" + symbol.name + "' shifted by " + std::to_string(output.section_headers[index].size) + " bytes");
                symbol.value += output.section_headers[index].size;
            }
            /// Also shift the headers of the sections after the current section to the right by the size of the output section
            for(int j = 0; j < output.section_headers.size(); j++)
            {
                if(output.section_headers[j].offset <= output.section_headers[index].offset)
                    continue;
                output.section_headers[j].offset += input.section_headers[i].size;
            }
            /// Finally, update the size of the output section
            output.section_headers[index].size += input.section_headers[i].size;
        }
        /// Now add the symbols of this section to the output file
        for(auto &symbol : input.symbol_table)
        {
            /// If the symbol is from a different section, skip it
            if(symbol.section != section_name)
                continue;
            /// If the symbol is a section symbol and the section is present in the output file, skip it
            if(symbol.is_section && index < output.section_names.size())
                continue;
            else if(symbol.is_section)
            {
                /// If the symbol is a section symbol and the section is not present in the output file, add it
                output.section_names.push_back(symbol.name);
                output.section_headers.push_back(input.section_headers[i]);
                output.binary_data.resize(output.binary_data.size() + input.section_headers[i].size);
                memcpy(&output.binary_data[output.section_headers[i].offset], &input.binary_data[input.section_headers[i].offset], input.section_headers[i].size);
                logger->logDebug("Section '" + section_name + "' added to the output file");
                continue;
            }
            /// Check if the symbol is already present in the output file
            int symbol_index = 0;
            for(;symbol_index < output.symbol_table.size(); symbol_index++)
            {
                if(output.symbol_table[symbol_index].name == symbol.name)
                {
                    logger->logDebug("Symbol '" + symbol.name + "' already present in the output file at index " + std::to_string(symbol_index));
                    break;
                }
            }
            /// If the symbol is already present in the output file, then it must be extern in one of the files
            if(symbol_index < output.symbol_table.size()){
                if(!symbol.is_extern && !output.symbol_table[symbol_index].is_extern){
                    logger->logError("Symbol '" + symbol.name + "' defined multiple locations " + std::to_string(symbol_index));
                    continue;
                }
                if(symbol.is_extern && !output.symbol_table[symbol_index].is_extern){
                    logger->logDebug("Symbol '" + symbol.name + "' which is extern is defined in section '" + output.symbol_table[symbol_index].section + "'");

                }
                if(!symbol.is_extern && output.symbol_table[symbol_index].is_extern){
                    logger->logDebug("Symbol '" + symbol.name + "' which is defined in section '" + section_name + "' is extern");
                    /// The symbol is defined in the input file, update the output file
                    output.symbol_table[symbol_index].section = section_name;
                    output.symbol_table[symbol_index].value = symbol.value;
                    output.symbol_table[symbol_index].is_extern = false;
                    logger->logInfo("Updating relocation entries for symbol '" + symbol.name + "'");
                    for(auto &relocation : output.relocation_table)
                    {
                        if(relocation.name == symbol.name)
                        {
                            logger->logDebug("Updating relocation entry at offset " + std::to_string(relocation.offset));
                            relocation.name = section_name;
                            relocation.addend = symbol.value;
                        }
                    }
                }
                if(symbol.is_extern && output.symbol_table[symbol_index].is_extern){
                    logger->logDebug("Symbol '" + symbol.name + "' which is extern in section '" + section_name + "' is also extern in section '" + output.symbol_table[symbol_index].section + "'");
                    /// The symbol is extern in both the input and output files, do nothing
                }
            }else{
                /// The symbol is not present in the output file, add it
                output.symbol_table.push_back(symbol);
                logger->logDebug("Symbol '" + symbol.name + "' with value " + std::to_string(symbol.value) + " added to the output file");
            }
        }


    }
}