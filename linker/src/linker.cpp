#include "linker.hpp"
#include <string.h>

void
Linker::link(ELF &output, ELF input)
{
    logger->logInfo("Merging ELFs\n------------------------------------");
    logger->logDebug("Input ELF file has " + std::to_string(input.section_names.size()) + " sections");
    for(std::size_t i = 1; i < input.section_names.size(); i++)
    {
        std::string section_name = input.section_names[i];
        logger->logInfo("Processing section: " + section_name);
        /// Check whether the section is already present in the output file
        std::size_t index = 0;
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
            output.section_headers.back().offset = output.binary_data.size() + sizeof(ELF_Header);
            
            /// Also add the section symbol to the output file
            ELF::Symbol_Table_Entry section_symbol;
            section_symbol.is_const = false;
            section_symbol.is_extern = false;
            section_symbol.is_section = true;
            section_symbol.name = section_name;
            section_symbol.section = section_name;
            section_symbol.value = 0;

            output.symbol_table.push_back(section_symbol);

            output.binary_data.resize(output.binary_data.size() + input.section_headers[i].size);
            /// Add the sections binary data to the output file
            char *dst = output.binary_data.data() + (output.section_headers.back().offset - sizeof(ELF_Header));
            char *src = input.binary_data.data() + (input.section_headers[i].offset - sizeof(ELF_Header));
            std::size_t size = input.section_headers[i].size;
            memcpy(dst, src, size);
            logger->logDebug("Section '" + section_name + "' added to the output file");
        }else{
            if(section_name != ".bss"){ /// If the section is not .bss, then we need to copy its data to the output file and shift the data after the section to make space for the new section data
                logger->logDebug("Merging section '" + section_name + "'");
                /// First resize the output file to accomodate the new section data
                // output.binary_data.resize(output.binary_data.size() + input.section_headers[i].size);

                // char *src = output.binary_data.data() + (output.section_headers[index].offset - sizeof(ELF_Header)) + output.section_headers[index].size;
                // char *dst = output.binary_data.data() + (output.section_headers[index].offset - sizeof(ELF_Header)) + output.section_headers[index].size + input.section_headers[i].size;

                // std::size_t size = output.binary_data.size() -((output.section_headers[index].offset - sizeof(ELF_Header)) + output.section_headers[index].size);
                // memmove(dst, src, size); /// TODO: This part is making all of the valgrind errors
                // /// And finally copy the new section data to the output file
                // dst = output.binary_data.data() + (output.section_headers[index].offset - sizeof(ELF_Header))+ output.section_headers[index].size;
                // src = input.binary_data.data() + (input.section_headers[i].offset - sizeof(ELF_Header));
                // size = input.section_headers[i].size;
                // memcpy(dst, src, size);

                /// just insert byte by byte
                int src_index = input.section_headers[i].offset - sizeof(ELF_Header);
                int dst_index = output.section_headers[index].offset - sizeof(ELF_Header) + output.section_headers[index].size;
                for(std::size_t j = 0; j < input.section_headers[i].size; j++)
                {
                    output.binary_data.insert(output.binary_data.begin() + dst_index, input.binary_data[src_index]);
                    src_index++;
                    dst_index++;
                }

                /// Also shift the headers of the sections after the current section to the right by the size of the output section
                logger->logInfo("Shifting section headers");
                for(std::size_t j = 0; j < output.section_headers.size(); j++)
                {
                    if(output.section_headers[j].offset <= output.section_headers[index].offset)
                        continue;
                    logger->logDebug("Section header '" + output.section_names[j] + "' shifted by " + std::to_string(input.section_headers[i].size) + " bytes");
                    output.section_headers[j].offset += input.section_headers[i].size;
                }
            }
            /// Then shift all of the input section symbols to the right by the size of the output section
            logger->logInfo("Shifting symbols");
            for(auto &symbol : input.symbol_table)
            {
                if(symbol.section != section_name || symbol.is_extern || symbol.is_section || symbol.is_extern)
                    continue;
                symbol.value += output.section_headers[index].size;
                logger->logDebug("Symbol '" + symbol.name + "' shifted by " + std::to_string(output.section_headers[index].size) + " bytes, new value: " + std::to_string(symbol.value));
            }
            /// Now update the relevant input files relocation entries, they will be added to the output file later
            logger->logInfo("Updating relocation entries in the input file");
            for(auto &relocation : input.relocation_table)
            {
                logger->logDebug("Updating relocation entry in section '" + relocation.section + "' at offset " + std::to_string(relocation.offset));
                /// If the relocation is referencing the section, increase the addend by the size of the output section
                if(relocation.name == section_name)
                    relocation.addend += output.section_headers[index].size;
                
                /// If the relocation is in the section, shift it by the size of the output section
                if(relocation.section == section_name)
                    relocation.offset += output.section_headers[index].size;
            }

            /// Finally, update the size of the output section
            output.section_headers[index].size += input.section_headers[i].size;
        }
        /// Now add the symbols of this section to the output file
        for(auto &symbol : input.symbol_table)
        {
            /// If the symbol is from a different section, skip it
            if(symbol.section != section_name || symbol.is_section)
                continue;
            /// Check if the symbol is already present in the output file
            std::size_t symbol_index = 0;
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
                    /// The symbol is extern in the input file, and defined in the output file, update the input file
                    symbol.section = output.symbol_table[symbol_index].section;
                    symbol.value = output.symbol_table[symbol_index].value;
                    symbol.is_extern = false;
                    logger->logInfo("Updating relocation entries for symbol '" + symbol.name + "'");
                    for(auto &relocation : input.relocation_table)
                    {
                        if(relocation.name == symbol.name)
                        {
                            logger->logDebug("Updating relocation entry at offset " + std::to_string(relocation.offset));
                            relocation.name = symbol.section;
                            relocation.addend = symbol.value;
                        }
                    }
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
                            logger->logDebug("Updating relocation entry at offset " + std::to_string(relocation.offset) + " in section '" + relocation.section + "' to addend " + std::to_string(symbol.value));
                            relocation.name = section_name;
                            relocation.addend = symbol.value;
                        }
                    }
                }
                if(symbol.is_extern && output.symbol_table[symbol_index].is_extern){
                    logger->logDebug("Symbol '" + symbol.name + "' which is extern in section '" + section_name + "' is also extern in section '" + output.symbol_table[symbol_index].section + "'");
                    /// The symbol is extern in both the input and output files, skip it? TODO: Check this

                }
            }else{
                /// The symbol is not present in the output file, add it
                output.symbol_table.push_back(symbol);
                logger->logDebug("Symbol '" + symbol.name + "' with value " + std::to_string(symbol.value) + " added to the output file");
            }
        }
    }
    logger->logInfo("Adding the input files relocation entries to the output file");
    for(auto &relocation : input.relocation_table)
    {
        logger->logDebug("Relocation entry for symbol '" + relocation.name + "' at offset " + std::to_string(relocation.offset) + " added to the output file");
        output.relocation_table.push_back(relocation);
    }
}