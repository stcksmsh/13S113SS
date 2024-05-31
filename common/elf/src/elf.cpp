#include "elf.hpp"
#include <string.h>
#include <format>
#include <algorithm>

std::istream &
ELF::readFromStream(std::istream &stream)
{
    logger->logInfo("Reading ELF file from stream\n----------------------------------");

    program_headers.clear();
    binary_data.clear();
    section_headers.clear();
    section_names.clear();
    symbol_table.clear();
    relocation_table.clear();

    logger->logDebug("Cleared all data structures");
    /* First we have to read the file */

    /// First we read the ELF headers
    { /// Read the ELF file
        logger->logInfo("Reading the ELF headers");
        logger->logDebug("Reading the ELF file header");
        stream.read((char *)&file_header, sizeof(ELF_Header));


        logger->logDebug("Reading the program headers");
        /// Then we read the program headers
        for (int i = 0; i < file_header.ph_num; i++)
        {
            Program_Header ph;
            stream.read((char *)&ph, sizeof(Program_Header));
            program_headers.push_back(ph);
        }

        /// Then we read the section headers
        logger->logDebug("Reading the section headers");
        stream.seekg(file_header.sh_offset);
        for (int i = 0; i < file_header.sh_num; i++)
        {
            Section_Header sh;
            logger->logDebug("Reading section header " + std::to_string(i));
            stream.read((char *)&sh, sizeof(Section_Header));
            section_headers.push_back(sh);
        }

        /// Then we read the section names
        /// First we need to find the shstrtab section
        logger->logDebug("Reading the .shstrtab section");
        Section_Header shstrtab_header = section_headers[file_header.sh_str_index];
        std::vector<char> shstrtab_data(shstrtab_header.size);
        stream.seekg(shstrtab_header.offset);
        stream.read(shstrtab_data.data(), shstrtab_data.size());
        /// Then we can read the section names
        for (int i = 0; i < section_headers.size(); i++)
        {
            std::string name(shstrtab_data.data() + section_headers[i].name);
            logger->logDebug("Reading section name " + std::to_string(i) + ": " + name);
            section_names.push_back(name);
        }

        logger->logDebug("Creating the symbol table");
        /// Then we fill the symbol table
        /// We need the .strtab section to get the names of the symbols
        logger->logDebug("Reading the .strtab section");
        Section_Header strtab_header = section_headers[section_headers.size() - 3];
        std::vector<char> strtab_data(strtab_header.size);
        stream.seekg(strtab_header.offset);
        stream.read(strtab_data.data(), strtab_data.size());
        /// Then we need the symtab section to get the symbols
        logger->logDebug("Reading the .symtab section");
        Section_Header symtab_header = section_headers[section_headers.size() - 2];
        std::vector<char> symtab_data(symtab_header.size);
        stream.seekg(symtab_header.offset);
        stream.read(symtab_data.data(), symtab_data.size());
        /// Now we can fill the symbol table 
        for (int i = 0; i < symtab_header.size / symtab_header.entsize; i++)
        {
            symtab_entry sym;
            memcpy(&sym, symtab_data.data() + i * sizeof(symtab_entry), sizeof(symtab_entry));
            std::string name(strtab_data.data() + sym.name);

            Symbol_Table_Entry sym_entry;
            sym_entry.name = name;
            sym_entry.value = sym.value;
            sym_entry.is_local = (symtab_entry_binding(sym.info) == symtab_entry_bindings::STB_GLOBAL ? false : true);
            sym_entry.is_section = (symtab_entry_type(sym.info) == symtab_entry_types::STT_SECTION);
            sym_entry.is_const = (symtab_entry_type(sym.info) == symtab_entry_types::STT_CONST);
            sym_entry.is_extern = !sym_entry.is_local && sym.shndx == 0;
            sym_entry.section = section_names[sym.shndx];
            symbol_table.push_back(sym_entry);
            logger->logDebug("Added symbol " + name + " to the symbol table with value " + std::format("{:x}", sym.value) + " and " + (sym_entry.is_local ? "local" : "global") + " binding");
        }
        
        logger->logDebug("Creating the relocation table");
        /// And finally we fill the relocation table
        for (int i = 1; i < section_headers.size() - 3; i++)
        {
            if (section_headers[i].type != Section_Header::Section_Type::SHT_RELA)
                continue;
            logger->logDebug("Reading relocations for section " + section_names[i]);
            std::vector<char> rel_data(section_headers[i].size);
            stream.seekg(section_headers[i].offset);
            stream.read(rel_data.data(), rel_data.size());
            for (int j = 0; j < section_headers[i].size / section_headers[i].entsize; j++)
            {
                rel_tab_entry rel;
                memcpy(&rel, rel_data.data() + j * section_headers[i].entsize, section_headers[i].entsize);
                std::string name(strtab_data.data() + symbol_table[rel_tab_entry_symbol(rel.info)].name);
                logger->logDebug("Reading relocation " + std::to_string(j) + " for section " + section_names[i].substr(4) + " for symbol " + name);
                add_relocation(name, section_names[i].substr(4), rel.offset, rel.addend);
            }
        }
    }

    logger->logInfo("Finished reading ELF file");
    logger->logDebug("Read " + std::to_string(program_headers.size()) + " program headers, " + std::to_string(section_headers.size()) + " section headers, " + std::to_string(binary_data.size()) + " bytes of binary data, " + std::to_string(symbol_table.size()) + " symbols and " + std::to_string(relocation_table.size()) + " relocations");

    /* Then we have to remove some sections */

    std::size_t new_end = 0;
    { /// Now remove the .strtab, .symtab and .shstrtab sections
        logger->logInfo("Removing the .strtab, .symtab and .shstrtab sections");
        
        section_headers.pop_back();
        section_names.pop_back();
        section_headers.pop_back();
        section_names.pop_back();
        new_end = section_headers.back().offset;
        section_headers.pop_back();
        section_names.pop_back();
    }

    { /// Also remove the .rel sections
        logger->logInfo("Removing the .rel sections");
        for (int i = 0; i < section_headers.size(); i++)
        {
            if (section_headers[i].type == Section_Header::Section_Type::SHT_RELA)
            {
                new_end -= section_headers[i].size;
                logger->logDebug("Removing " + section_names[i] + " section");
                section_headers.erase(section_headers.begin() + i);
                section_names.erase(section_names.begin() + i);
                i--;
            }
        }

    }

    { /// Now we can read the binary data of the sections
        logger->logDebug("Reading the binary data");
        binary_data.resize(new_end - sizeof(ELF_Header) - program_headers.size() * sizeof(Program_Header));
        stream.seekg(sizeof(ELF_Header) + program_headers.size() * sizeof(Program_Header));
        stream.read((char *)binary_data.data(), binary_data.size());
    }

    logger->logInfo("Finished deserializing ELF file");

    return stream;
}

std::ostream &
ELF::createShared(std::ostream &stream)
{
    logger->logInfo("Creating shared ELF file\n----------------------------------");
    /* First we generate the strtab and symtab structures */
    /// First sort the symbol table so that the local symbols come first
    std::sort(symbol_table.begin(), symbol_table.end(), [](Symbol_Table_Entry a, Symbol_Table_Entry b) {
        return !b.is_local && a.is_local;
    });

    std::vector<char> strtab_data;
    { /// Generate the strtab data (so we can set the name_index field of the symbol table)
        logger->logInfo("Generating strtab data");
        /// Adding empty string
        for (int i = 0; i < symbol_table.size(); i++)
        {
            logger->logDebug("Adding symbol '" + symbol_table[i].name + "' to strtab");
            symbol_table[i].name_index = strtab_data.size();
            for (char c : symbol_table[i].name)
            {
                strtab_data.push_back(c);
            }
            strtab_data.push_back('\0');
        }
    }

    std::vector<symtab_entry> symtab;
    { /// Generate the symbol table structure
        logger->logInfo("Generating the symbol table");
        symtab_entry null_sym;
        /// Then add the symbols to the symtab        
        for (int i = 0; i < symbol_table.size(); i++)
        {
            symtab_entry sym;
            sym.name = symbol_table[i].name_index;
            sym.value = symbol_table[i].value;
            sym.size = 0;
            sym.info = symtab_entry_info(symbol_table[i].is_local ? symtab_entry_bindings::STB_LOCAL : symtab_entry_bindings::STB_GLOBAL,
                                         symbol_table[i].is_section ? symtab_entry_types::STT_SECTION : (symbol_table[i].is_const ? symtab_entry_types::STT_CONST : symtab_entry_types::STT_NOTYPE));
            sym.other = 0;
            sym.shndx = 0;

            for (int j = 0; j < section_names.size(); j++)
            {
                if (section_names[j] == symbol_table[i].section)
                {
                    sym.shndx = j;
                    break;
                }
            }
            logger->logDebug("Adding symbol '" + symbol_table[i].name + "' to symtab with value " + std::format("{:x}", symbol_table[i].value) + " and " + (symbol_table[i].is_local ? "local" : "global") + " binding");
            symtab.push_back(sym);
        }
    }

    /* Then we need to create the .rel sections */
    { /// We make a .rel section for each user defined section ( which have relocations )
        /// We skip the first three sections, because they are the .text, .data and .bss sections
        logger->logInfo("Generating relocation sections");
        int sections = section_names.size();
        std::vector<std::vector<rel_tab_entry>> rel_data;

        for (int i = 1; i < sections; i++)
        {
            if(section_headers[i].type != Section_Header::Section_Type::SHT_PROGBITS)
                continue;
            logger->logDebug("Generating relocation section for section '" + section_names[i] + "'");
            std::vector<rel_tab_entry> rel_section_data;
            Section_Header rel_section;

            rel_section.type = Section_Header::Section_Type::SHT_RELA;
            rel_section.flags = Section_Header_Flags::SHF_ALLOC;
            rel_section.addralign = 1;
            rel_section.offset = 0;                        /// will be updated later
            rel_section.size = 0;                          /// will be updated later
            rel_section.info = i;                      /// The index of the section to which the relocations apply
            rel_section.entsize = sizeof(rel_tab_entry);

            /// Now fill the rel_section_data vector

            for (int j = 0; j < relocation_table.size(); j++)
            {
                if (relocation_table[j].section == section_names[i])
                {
                    logger->logDebug("Relocation for symbol '" + relocation_table[j].name + "' in section '" + relocation_table[j].section + "' at offset " + std::format("{:x}", relocation_table[j].offset));
                    rel_tab_entry rel;
                    rel.offset = relocation_table[j].offset;
                    for (int k = 0; k < symbol_table.size(); k++)
                    {
                        if (symbol_table[k].name == relocation_table[j].name)
                        {
                            rel.info = k; /// The index of the symbol in the symtab
                            break;
                        }
                    }
                    logger->logDebug("Adding relocation entry for symbol '" + relocation_table[j].name + "' in section: '" + relocation_table[j].section + "' at offset " + std::format("{:x}", relocation_table[j].offset), " referencing symbol at index " + std::to_string(rel.info));
                    rel.info = rel_tab_entry_info(rel.info, rel_tab_types::R_386_32);
                    rel.addend = relocation_table[j].addend;
                    rel_section_data.push_back(rel);
                }
            }

            /// If there are no relocations for this section, we skip it
            if (rel_section_data.size() == 0){
                logger->logDebug("No relocations for section '" + section_names[i] + "'");
                continue;
            }

            /// Sort the rel_section_data by offset
            std::sort(rel_section_data.begin(), rel_section_data.end(), [](rel_tab_entry a, rel_tab_entry b) {
                return a.offset < b.offset;
        });

            logger->logDebug("Generated " + std::to_string(rel_section_data.size()) + " relocations for section '" + section_names[i] + "'");
            /// Add the rel_section_data to the rel_data vector and the section to the names/headers vectors
            rel_data.push_back(rel_section_data);
            rel_section.size = rel_section_data.size() * sizeof(rel_tab_entry);
            rel_section.offset = sizeof(ELF_Header) + program_headers.size() * sizeof(Program_Header) + binary_data.size();
            /// Add the new rel section after the section to which the relocations apply, and increase i to skip the new section
            /// std::vector::insert inserts before the element at the specified position, so we need to increment i (if we are not at the end of the sections)
            if(i < sections - 1){
                i++;
                sections++;
                section_headers.insert(section_headers.begin() + i, rel_section);
                section_names.insert(section_names.begin() + i, ".rel" + section_names[i-1]); // section_names[i-1] because we incremented i
            }else{
                /// If we are at the end of the sections, we just push the new section to the end
                section_headers.push_back(rel_section);
                section_names.push_back(".rel" + section_names[i]);
            }

            /// And now write the rel_section_data to the binary
            logger->logDebug("writing '.rel" + section_names[i] + "' section with " + std::to_string(rel_section_data.size()) + " entries to the binary");
            for(int j = 0; j < rel_section_data.size(); j++){
                for(int k = 0; k < sizeof(rel_tab_entry); k++){
                    binary_data.push_back(((char *)&rel_section_data[j])[k]);
                }
            }

        }
    }

    { /// Then we need to update the symbols ndx due to the added rel sections
        int disp = 0;
        for(int i = 3; i < section_names.size(); i++){
            if(section_headers[i].type == Section_Header::Section_Type::SHT_RELA){
                disp++;
                continue;
            }
            for(int j = 0; j < symtab.size(); j++){
                if(symtab[j].shndx == i - disp){
                    symtab[j].shndx = i;
                }
            }
        }
    }
    /* After that we need to create some common sections and fill their data structures */
    { /// Create the strtab section
        logger->logInfo("Creating the .strtab section");
        Section_Header strtab_header;
        strtab_header.type = Section_Header::Section_Type::SHT_STRTAB;
        strtab_header.flags = Section_Header_Flags::SHF_ALLOC;
        strtab_header.addralign = 1;
        strtab_header.size = strtab_data.size();

        section_headers.push_back(strtab_header);
        section_names.push_back(".strtab");
    }

    { /// Create the symtab section
        logger->logInfo("Creating the .symtab section");
        Section_Header symtab_header;

        symtab_header.type = Section_Header::Section_Type::SHT_SYMTAB;
        symtab_header.flags = Section_Header_Flags::SHF_ALLOC;
        symtab_header.entsize = sizeof(symtab_entry);
        symtab_header.size = symtab.size() * sizeof(symtab_entry);
        symtab_header.link = section_headers.size() - 1; /// The index of the strtab section

        /// Get number of local symbols
        int num_local = 0;
        for (; num_local < symbol_table.size(); num_local++)
        {
            if (!symbol_table[num_local].is_local)
                break;
        }
        symtab_header.info = num_local;

        section_headers.push_back(symtab_header);
        section_names.push_back(".symtab");

        /// Update the .rel sections link field
        for(int i = 0; i < section_headers.size(); i++)
        {
            if(section_headers[i].type == Section_Header::Section_Type::SHT_RELA)
            {
                logger->logDebug("Updating info field for section " + section_names[i] + " to " + std::to_string(section_headers.size() - 1));
                section_headers[i].link = section_headers.size() - 1;
            }
        }
    }

    std::vector<char> shstrtab_data;
    Section_Header shstrtab_header;
    { /// Creating the shstrtab section
        logger->logInfo("Creating the .shstrtab section");
        for (int i = 0; i < section_names.size(); i++)
        {
            section_headers[i].name = shstrtab_data.size();
            logger->logDebug("Adding section " + section_names[i] + " to shstrtab");
            for (int j = 0; j < section_names[i].size(); j++)
            {
                shstrtab_data.push_back(section_names[i][j]);
            }
            shstrtab_data.push_back('\0');
        }

        shstrtab_header.name = shstrtab_data.size();
        for (char c : ".shstrtab")
        {
            shstrtab_data.push_back(c);
        }

        shstrtab_data.push_back('\0');
    }

    /* Then we can write raw section data to the binary, and fix some values along the way */
    { /// Writing strtab to the binary
        logger->logInfo("Writing the .strtab section to the binary");
        section_headers[section_headers.size() - 2].offset = sizeof(ELF_Header) + program_headers.size() * sizeof(Program_Header) + binary_data.size();

        for (int i = 0; i < strtab_data.size(); i++)
        {
            binary_data.push_back(strtab_data[i]);
        }
    }

    { /// Write symtab to the binary
        logger->logInfo("Writing the .symtab section to the binary");
        section_headers[section_headers.size() - 1].offset = sizeof(ELF_Header) + program_headers.size() * sizeof(Program_Header) + binary_data.size();
        for (symtab_entry sym : symtab)
        {
            for (int i = 0; i < sizeof(symtab_entry); i++)
            {
                binary_data.push_back(((char *)&sym)[i]);
            }
        }
    }

    { /// Writing shstrtab to the binary
        logger->logInfo("Writing the .shstrtab section to the binary");
        shstrtab_header.type = Section_Header::Section_Type::SHT_STRTAB;
        shstrtab_header.flags = 0;
        shstrtab_header.addralign = 1;
        shstrtab_header.offset = sizeof(ELF_Header) + program_headers.size() * sizeof(Program_Header) + binary_data.size();
        shstrtab_header.size = shstrtab_data.size();

        section_headers.push_back(shstrtab_header);

        for (int i = 0; i < shstrtab_data.size(); i++)
        {
            binary_data.push_back(shstrtab_data[i]);
        }
    }

    /* Then fill the rest of the ELF file header */
    { /// Construct the ELF header
        logger->logInfo("Constructing the ELF header");
        file_header.type = ELF_Header::ELF_Type::ET_REL;
        file_header.sh_offset = sizeof(ELF_Header) + program_headers.size() * sizeof(Program_Header) + binary_data.size();
        file_header.sh_num = section_headers.size();
        file_header.ph_num = program_headers.size();
        file_header.sh_str_index = section_headers.size() - 1;
        file_header.ph_offset = 0;
        file_header.sh_offset = sizeof(ELF_Header) + program_headers.size() * sizeof(Program_Header) + binary_data.size();
    }


    /* And finally write all of this to the ELF file */
    { /// Write the ELF file
        logger->logInfo("Writing the ELF file");
        /// First the ELF header
        write_to_stream(stream, file_header);
        /// Then the program headers to the stream
        for (Program_Header ph : program_headers)
            write_to_stream(stream, ph);

        /// Then the binary text
        stream.write((char *)binary_data.data(), binary_data.size());

        /// And finally the section headers
        for (Section_Header sh : section_headers)
            write_to_stream(stream, sh);
    }

    return stream;
};

std::ostream &
ELF::createExecutable(std::ostream &stream){
    return stream << "Not implemented";
}

void
ELF::setSectionAddress(const std::string section_name, const uint32_t address){
    logger->logInfo("Setting the address of section " + section_name + " to " + std::format("{:x}", address));
}

void ELF::add_section(const std::string name, const std::vector<char> data,
                    const Section_Header::Section_Type type, const uint32_t flags, const uint32_t addralign)
{
    logger->logInfo("Adding section '" + name + "' to the ELF file");
    Section_Header sh;
    sh.type = type;
    sh.flags = flags;
    sh.addralign = addralign;

    sh.offset = sizeof(ELF_Header) + program_headers.size() * sizeof(Program_Header) + binary_data.size();
    sh.size = data.size();

    logger->logDebug("Section '" + name + "' has offset " + std::format("{:x}", sh.offset) + " and size " + std::to_string(sh.size));

    section_names.push_back(name);
    section_headers.push_back(sh);
    for (char byte : data)
    {
        binary_data.push_back(byte);
    }
}

void ELF::add_symbol(const std::string name, const std::string section, const uint32_t value, bool is_local, bool is_section, bool is_const, bool is_extern)
{
    logger->logDebug("Adding symbol '" + name + "' to section '" + section + "' with value " + std::format("{:x}", value));
    ELF::Symbol_Table_Entry sym;
    sym.name = name;
    sym.section = section;
    sym.value = value;
    sym.is_section = is_section;
    sym.is_local = is_local;
    sym.is_const = is_const;
    sym.is_extern = is_extern;
    symbol_table.push_back(sym);
}

void ELF::add_relocation(const std::string name, const std::string section, const uint32_t offset, const uint32_t addend)
{
    logger->logDebug("Adding relocation for symbol '" + name + "' in section '" + section + "' at offset " + std::format("{:x}", offset));
    Relocation_Entry rel;
    rel.section = section;
    rel.name = name;
    rel.offset = offset;
    rel.addend = addend;
    relocation_table.push_back(rel);
}
