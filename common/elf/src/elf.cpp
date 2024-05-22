#include "elf.hpp"


std::ostream &
ELF::createShared(std::ostream &stream){

    /* First we generate the strtab and symtab structures */
    std::vector<uint8_t> strtab_data;
    { /// Generate the strtab data (so we can set the name_index field of the symbol table)
        strtab_data.push_back('\0');
        for(int i = 0; i < symbol_table.size(); i++){
            symbol_table[i].name_index = strtab_data.size();
            for(char c : symbol_table[i].name){
                strtab_data.push_back(c);
            }
            strtab_data.push_back('\0');
        }
    }

    std::vector<symtab_entry> symtab;
    int local_count = 0;
    { /// Generate the symbol table structure
        symtab_entry null_sym;
        /// Add the NULL symbol
        null_sym.name = null_sym.value = null_sym.size = null_sym.info = null_sym.other = null_sym.shndx = 0;
        symtab.push_back(null_sym);
        /// First the local symbols
        for(int i = 0; i < symbol_table.size(); i ++){
            if(!symbol_table[i].is_local) continue;
            symtab_entry sym;
            sym.name = symbol_table[i].name_index;
            sym.value = symbol_table[i].value;
            sym.size = 0;
            sym.info = symtab_entry_info( symbol_table[i].is_local ? symtab_entry_bindings::STB_LOCAL : symtab_entry_bindings::STB_GLOBAL, \
            symbol_table[i].is_section ? symtab_entry_types::STT_SECTION : symtab_entry_types::STT_NOTYPE );
            sym.other = 0;
            sym.shndx = 0;

            for(int j = 0; j < section_names.size(); j++){
                if(section_names[j] == symbol_table[i].section){
                    sym.shndx = j;
                    break;
                }
            }
            symtab.push_back(sym);

        }
        local_count = symtab.size();
        for(int i = 0; i < symbol_table.size(); i ++){
            if(symbol_table[i].is_local) continue;
            symtab_entry sym;
            sym.name = symbol_table[i].name_index;
            sym.value = symbol_table[i].value;
            sym.size = 0;
            sym.info = symtab_entry_info( symbol_table[i].is_local ? symtab_entry_bindings::STB_LOCAL : symtab_entry_bindings::STB_GLOBAL, symtab_entry_types::STT_NOTYPE );
            sym.other = 0;
            sym.shndx = 0;

            for(int j = 0; j < section_names.size(); j++){
                if(section_names[j] == symbol_table[i].section){
                    sym.shndx = j;
                    break;
                }
            }
            symtab.push_back(sym);

        }

    }

    /* Then we need to create the .rel sections */
    { /// We make a .rel section for each user defined section ( which have r
        /// We skip the first three sections, because they are the .text, .data and .bss sections
        int sections = section_names.size();
        std::vector < std::vector<rel_tab_entry> > rel_data;
        for(int i = 3; i < sections; i++){
            std::vector<rel_tab_entry> rel_section_data;
            Section_Header rel_section;

            rel_section.type = Section_Header::Section_Type::SHT_REL;
            rel_section.flags = Section_Header_Flags::SHF_ALLOC;
            rel_section.addralign = 1;
            rel_section.offset = 0; /// will be updated later
            rel_section.size = 0; /// will be updated later
            rel_section.link = section_headers.size() - 1; /// The index of the symtab section
            rel_section.info = i + 1; /// The index of the section to which the relocations apply

            /// Now fill the rel_section_data vector

            for(int j = 0; j < relocation_table.size(); j++){
                if(relocation_table[j].section == section_names[i]){
                    rel_tab_entry rel;
                    rel.offset = relocation_table[j].value;
                    for(int k = 0; k < symbol_table.size(); k++){
                        if(symbol_table[k].name == relocation_table[j].name){
                            rel.info = k;
                            break;
                        }
                    }
                    rel_section_data.push_back(rel);
                }
            }

            /// If there are no relocations for this section, we skip it
            if(rel_section_data.size() == 0) continue;

            /// Add the rel_section_data to the rel_data vector and the section to the names/headers vectors
            rel_data.push_back(rel_section_data);
            section_headers.push_back(rel_section);
            section_names.push_back(".rel" + section_names[i]);
        }

        /// Now we need to update te link field of the symtab section headers
        for(int i = sections; i < section_headers.size(); i++){
            section_headers[i].link = section_headers.size() + 1;
        }

        /// And now we need to write the rel sections to the binary and update the section offsets
        for(int i = 0; i < rel_data.size(); i++){
            section_headers[sections + i + 1].offset = sizeof(ELF_Header) + program_headers.size() * sizeof(Program_Header) + binary_data.size();
            section_headers[sections + i + 1].size = rel_data[i].size() * sizeof(rel_tab_entry);
            for(int j = 0; j < rel_data[i].size(); j++){
                for(int k = 0; k < sizeof(rel_tab_entry); k++){
                    binary_data.push_back(((uint8_t *)&rel_data[i][j])[k]);
                }
            }
        }
    }

    /* After that we need to create some common sections and fill their data structures */
    { /// Create the strtab section
        Section_Header strtab_header;
        strtab_header.type = Section_Header::Section_Type::SHT_STRTAB;
        strtab_header.flags = Section_Header_Flags::SHF_ALLOC;
        strtab_header.addralign = 1;
        strtab_header.size = strtab_data.size();

        section_headers.push_back(strtab_header);
        section_names.push_back(".strtab");
    }

    { /// Create the symtab section
        Section_Header symtab_header;
        
        symtab_header.type = Section_Header::Section_Type::SHT_SYMTAB;
        symtab_header.flags = Section_Header_Flags::SHF_ALLOC;
        symtab_header.entsize = sizeof(symtab_entry);
        symtab_header.info = local_count;
        symtab_header.size = symtab.size() * sizeof(symtab_entry);
        symtab_header.link = section_headers.size() - 1; /// The index of the strtab section

        section_headers.push_back(symtab_header);
        section_names.push_back(".symtab");
    
    }

    std::vector<uint8_t> shstrtab_data;
    Section_Header shstrtab_header;
    { /// Creating the shstrtab section
        shstrtab_data.push_back('\0'); /// first byte is always null
        for(int i = 0; i < section_names.size(); i++){
            section_headers[i+1].name = shstrtab_data.size();
            for(int j = 0; j < section_names[i].size(); j++){
                shstrtab_data.push_back(section_names[i][j]);
            }
            shstrtab_data.push_back('\0');
        }

        shstrtab_header.name = shstrtab_data.size();
        for(char c : ".shstrtab"){
            shstrtab_data.push_back(c);
        }
        shstrtab_data.push_back('\0');
    }

    /* Then we can write raw section data to the binary, and fix some values along the way */
    { /// Writing strtab to the binary
        section_headers[section_headers.size() - 2].offset = sizeof(ELF_Header) + program_headers.size() * sizeof(Program_Header) + binary_data.size();

        for(int i = 0; i < strtab_data.size(); i++){
            binary_data.push_back(strtab_data[i]);
        }
    }
        
    { /// Write symtab to the binary
        section_headers[section_headers.size() - 1].offset = sizeof(ELF_Header) + program_headers.size() * sizeof(Program_Header) + binary_data.size();
        for(symtab_entry sym : symtab){
            for(int i = 0; i < sizeof(symtab_entry); i++){
                binary_data.push_back(((uint8_t *)&sym)[i]);
            }
        }
    }

    { /// Writing shstrtab to the binary
        shstrtab_header.type = Section_Header::Section_Type::SHT_STRTAB;
        shstrtab_header.flags = 0;
        shstrtab_header.addralign = 1;
        shstrtab_header.offset = sizeof(ELF_Header) + program_headers.size() * sizeof(Program_Header) + binary_data.size();
        shstrtab_header.size = shstrtab_data.size();
        
        section_headers.push_back(shstrtab_header);

        for(int i = 0; i < shstrtab_data.size(); i++){
            binary_data.push_back(shstrtab_data[i]);
        }
    }

    /* Then fill the rest of the ELF file header */
    { /// Construct the ELF header
        file_header.type = ELF_Header::ELF_Type::ET_REL;
        file_header.sh_num = section_headers.size();
        file_header.ph_num = program_headers.size();
        file_header.sh_str_index = section_headers.size() - 1;
        if(file_header.ph_num == 0) file_header.ph_offset = 0;
        else file_header.ph_offset = sizeof(ELF_Header);
        file_header.sh_offset = sizeof(ELF_Header) + program_headers.size() * sizeof(Program_Header) + binary_data.size();
    }

    /* And finally write all of this to the ELF file */
    { /// Write the ELF file 
        /// First the ELF header
        write_to_stream( stream, file_header );
        /// Then the program headers to the stream
        for(Program_Header ph : program_headers) write_to_stream( stream, ph);

        /// Then the binary text
        stream.write( (char *)binary_data.data(), binary_data.size() );

        /// And finally the section headers
        for(Section_Header sh : section_headers) write_to_stream( stream, sh );
    }

    return stream;
};

void
ELF::add_section(const std::string name, const std::vector<uint8_t> data,
const Section_Header::Section_Type type, const uint32_t flags, const uint32_t addralign){
    Section_Header sh;
    sh.type = type;
    sh.flags = flags;
    sh.addralign = addralign;

    sh.offset = sizeof(ELF_Header) + program_headers.size() * sizeof(Program_Header) + binary_data.size();
    sh.size = data.size();

    section_names.push_back(name);
    section_headers.push_back(sh);
    for(uint8_t byte : data){
        binary_data.push_back(byte);
    }
}

void
ELF::add_symbol(const std::string name, const std::string section, const uint32_t value, bool is_local, bool is_section){
    ELF::Symbol_Table_Entry sym;
    sym.name = name;
    sym.section = section;
    sym.value = value;
    sym.is_section = is_section;
    sym.is_local = is_local;
    symbol_table.push_back(sym);
}

void
ELF::add_relocation(const std::string name, const std::string section, const uint32_t offset){
    ELF::Symbol_Table_Entry rel;
    rel.name = name;
    rel.section = section;
    rel.value = offset;
    relocation_table.push_back(rel);
}