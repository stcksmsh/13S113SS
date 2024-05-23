#ifndef __ELF_STRUCTS_HPP__
#define __ELF_STRUCTS_HPP__

#include <cstdint>

/// @brief The program header of the ELF file
struct Program_Header
{
    /// @brief The type of the program header ( 1 for loadable segment )
    enum Program_Type : uint32_t
    {
        /// @brief Unused program header
        PT_NULL = 0x0,
        /// @brief Loadable segment
        PT_LOAD = 0x1,
        /// @brief Dynamic linking information
        PT_DYNAMIC = 0x2,
        /// @brief Program interpreter
        PT_INTERP = 0x3,
        /// @brief Auxiliary information
        PT_NOTE = 0x4,
        /// @brief Reserved
        PT_SHLIB = 0x5,
        /// @brief Program header table
        PT_PHDR = 0x6,
        /// @brief Reserved
        PT_LOPROC = 0x70000000,
        /// @brief Reserved
        PT_HIPROC = 0x7FFFFFFF
    } type = Program_Type::PT_LOAD;
    /// @brief The offset of the segment in the file
    uint32_t offset = 0x0;
    /// @brief The virtual address of the segment in memory
    uint32_t vaddr = 0x0;
    /// @brief The physical address of the segment in memory (can be left as 0?)
    uint32_t paddr = 0x0;
    /// @brief The size of the segment in the file
    uint32_t filesz = 0x0;
    /// @brief The size of the segment in memory
    uint32_t memsz = 0x0;
    /// @brief The flags of the segment (1,2,4 are executable, writable, readable, ORed together)
    uint32_t flags = 0x5;
    /// @brief The alignment of the segment (0 or 1 for no alignment)
    uint32_t align = 0x0;
};

/// @brief The flags of the section header
enum Section_Header_Flags : uint32_t
{
    /// @brief Section contains data that should be writable during execution
    SHF_WRITE = 0x1,
    /// @brief Section occupies memory during program execution
    SHF_ALLOC = 0x2,
    /// @brief Section contains executable machine instructions
    SHF_EXECINSTR = 0x4,
    /// @brief Section contains data that should be zeroed during execution
    SHF_MERGE = 0x10,
    /// @brief Section contains data that should be zeroed during execution
    SHF_STRINGS = 0x20,
    /// @brief Section contains data that should be zeroed during execution
    SHF_INFO_LINK = 0x40,
    /// @brief Section contains data that should be zeroed during execution
    SHF_LINK_ORDER = 0x80,
    /// @brief Section contains data that should be zeroed during execution
    SHF_OS_NONCONFORMING = 0x100,
    /// @brief Section contains data that should be zeroed during execution
    SHF_GROUP = 0x200,
    /// @brief Section contains data that should be zeroed during execution
    SHF_TLS = 0x400,
    /// @brief Section contains data that should be zeroed during execution
    SHF_COMPRESSED = 0x800,
    /// @brief Section contains data that should be zeroed during execution
    SHF_MASKOS = 0x0FF00000,
    /// @brief Section contains data that should be zeroed during execution
    SHF_MASKPROC = 0xF0000000,
    /// @brief Section contains data that should be zeroed during execution
    SHF_ORDERED = 0x4000000,
    /// @brief Section contains data that should be zeroed during execution
    SHF_EXCLUDE = 0x8000000
};

/// @brief The section header of the ELF file
struct Section_Header
{
    /// @brief The name of the section (index in the string table)
    uint32_t name = 0x0;
    /// @brief The type of the section
    /// @brief The type of the section header
    enum Section_Type : uint32_t
    {
        /// @brief Unused section
        SHT_NULL = 0x0,
        /// @brief Section with data
        SHT_PROGBITS = 0x1,
        /// @brief Section containing symbol table
        SHT_SYMTAB = 0x2,
        /// @brief Section containing string table
        SHT_STRTAB = 0x3,
        /// @brief Section containing relocation entries
        SHT_RELA = 0x4,
        /// @brief Section containing hash table
        SHT_HASH = 0x5,
        /// @brief Section containing dynamic linking information
        SHT_DYNAMIC = 0x6,
        /// @brief Section containing notes
        SHT_NOTE = 0x7,
        /// @brief Section containing uninitialized data
        SHT_NOBITS = 0x8,
        /// @brief Section containing relocation entries with explicit addends
        SHT_REL = 0x9,
        /// @brief Section containing reserved range
        SHT_SHLIB = 0x0A,
        /// @brief Section containing dynamic linker symbol table
        SHT_DYNSYM = 0x0B,
        /// @brief Section containing array of constructors
        SHT_INIT_ARRAY = 0x0E,
        /// @brief Section containing array of destructors
        SHT_FINI_ARRAY = 0x0F,
        /// @brief Section containing pre-init values
        SHT_PREINIT_ARRAY = 0x10,
        /// @brief Section containing section group
        SHT_GROUP = 0x11,
        /// @brief Section containing extended section indices
        SHT_SYMTAB_SHNDX = 0x12,
        /// @brief Section containing number of reserved types
        SHT_NUM = 0x13,
        /// @brief Section with unknown type
        SHT_UNKNOWN = 0xFFFFFFFF
    } type = Section_Type::SHT_NULL;
    /// @brief The flags of the section
    uint32_t flags = 0x0;
    /// @brief The virtual address of the section in memory
    uint32_t addr = 0x0;
    /// @brief The offset of the section in the file
    uint32_t offset = 0x0;
    /// @brief The size of the section in the file
    uint32_t size = 0x0;
    /// @brief The index of the section header table entry
    uint32_t link = 0x0;
    /// @brief The extra information about the section
    uint32_t info = 0x0;
    /// @brief The alignment of the section
    uint32_t addralign = 0x0;
    /// @brief The size of each entry in the section
    uint32_t entsize = 0x0;
};

/// @brief The ELF header of the file
struct ELF_Header
{
    /// @brief The magic number of the ELF file
    uint8_t magic[4] = {0x7F, 'E', 'L', 'F'};
    /// @brief The class of the ELF file ( 1 for 32-bit )
    uint8_t class_ = 1;
    /// @brief The data encoding of the ELF file ( 1 for little-endian )
    uint8_t data = 1;
    /// @brief The version of the ELF file ( 1 for original )
    uint8_t version = 1;
    /// @brief The OS ABI of the ELF file ( 0 for System V )
    uint8_t os_abi = 0;
    /// @brief The ABI version of the ELF file ( undefined here, so basically extra padding )
    uint8_t abi_version = 0;
    /// @brief 7 padding bytes
    uint8_t padding[7] = {0};
    /// @brief The type of the ELF file ( 1 - relocatable, 2 - exec, 3 - shared )
    enum ELF_Type : uint16_t
    {
        /// @brief Relocatable file
        ET_REL = 0x1,
        /// @brief Executable file
        ET_EXEC = 0x2,
        /// @brief Shared object file
        ET_DYN = 0x3,
        /// @brief Core file
        ET_CORE = 0x4,
        /// @brief Processor-specific
        ET_LOPROC = 0xFF00,
        /// @brief Processor-specific
        ET_HIPROC = 0xFFFF
    } type = ELF_Type::ET_REL; /// default to relocatable ( .o file )
    /// @brief The machine architecture of the ELF file ( 3e for AMD x86-64 )
    uint16_t machine = 0x3e;
    /// @brief The version of the ELF file ( 1 for original )
    uint32_t version2 = 0x1;
    /// @brief The entry point of the ELF file
    uint32_t entry = 0x0; /// default to 0x0 since default is a relocatable file
    /// @brief The offset of the program header table (right after this one)
    uint32_t ph_offset = sizeof(ELF_Header);
    /// @brief The offset of the section header table
    uint32_t sh_offset = 0x0;
    /// @brief The flags of the ELF file (unused here, so 0x0)
    uint32_t flags = 0x0;
    /// @brief The size of this header
    uint16_t eh_size = sizeof(ELF_Header);
    /// @brief The size of a program header
    uint16_t ph_size = sizeof(Program_Header);
    /// @brief The number of program headers
    uint16_t ph_num = 0x1;
    /// @brief The size of a section header
    uint16_t sh_size = sizeof(Section_Header);
    /// @brief The number of section headers
    uint16_t sh_num = 0x0;
    /// @brief The index of the section header string table
    uint16_t sh_str_index = 0x0;
};

/// @brief The symbol table entry
struct symtab_entry
{
    /// @brief The name of the symbol (index in the string table)
    uint32_t name;
    /// @brief The value of the symbol
    uint32_t value;
    /// @brief The size of the symbol
    uint32_t size;
    /// @brief The info byte of the symbol
    uint8_t info;
    /// @brief The other byte of the symbol (unused, set to 0x0)
    uint8_t other = 0x0;
    /// @brief The section index of the symbol (the index of its section in the section header table)
    uint16_t shndx;
};

/// @brief The symbol table entry info bindings
enum symtab_entry_bindings
{
    /// @brief The symbol is not visible outside the object file
    STB_LOCAL = 0,
    /// @brief The symbol is visible outside the object file
    STB_GLOBAL = 1,
    /// @brief The symbol is weak, like a global symbol but lower precedence (will be overridden by a global symbol)
    STB_WEAK = 2,
    /// @brief Lower bound of the reserved bindings
    STB_LOPROC = 13,
    /// @brief Upper bound of the reserved bindings
    STB_HIPROC = 15,
};

/// @brief The symbol table entry types
enum symtab_entry_types
{
    /// @brief Not specified
    STT_NOTYPE = 0,
    /// @brief The symbol is an object
    STT_OBJECT = 1,
    /// @brief The symbol is a function
    STT_FUNC = 2,
    /// @brief The symbol is a section
    STT_SECTION = 3,
    /// @brief The symbol is a file
    STT_FILE = 4,
    /// @brief Lower bound of the reserved types
    STT_LOPROC = 13,
    /// @brief Upper bound of the reserved types
    STT_HIPROC = 15
};

/// @brief Create the symbol table entry info byte
/// @param binding The binding of the symbol
/// @param type The type of the symbol
/// @return The info byte
inline uint8_t symtab_entry_info(uint8_t binding, uint8_t type)
{
    return (binding << 4) | (type & 0x0F);
}

/// @brief The relocation table entry
struct rel_tab_entry
{
    /// @brief The offset of the relocation
    uint32_t offset;
    /* @brief Information on the relocation
     *   In our case this is the index of the symbol in the symbol table, as we only have one type of relocation
     *   If we had more types of relocations, we would need to store the type of the relocation as well
     */
    uint32_t info;
};

#endif // __ELF_STRUCTS_HPP__