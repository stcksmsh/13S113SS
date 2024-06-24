/**
 * @file emulator.cpp
 * @author Kosta Vukicevic (stcksmsh@gmail.com)
 * @brief Implementation of the Emulator class
 * @version 0.1
 * @date 2024-06-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "emulator.hpp"
#include <fstream>
#include <format>

void Emulator::loadFromDump(std::string filename)
{
    std::ifstream file(filename, std::ios::binary);
    if(!file.is_open())
    {
        logger->logError("Could not open file");
        throw std::runtime_error("Could not open file");
    }

    logger->logInfo("Reading memory dump from " + filename);

    /// Read the file line by line and fill a vector with the data
    /// call loadMemory with the data so far when a 'gap' is found
    /// (when there is a '--' or the next line address is not the expected one)

    uint32_t currentAddress;
    uint32_t startAddress;
    std::vector<uint8_t> currentData = {};
    std::string line;
    while(std::getline(file, line))
    {
        uint32_t address;
        /// The address is before the ':'
        address = std::stoi(line.substr(0, line.find(':')), nullptr, 16);
        logger->logDebug("Reading line at address " + std::format("{:x}", address));
        if(currentData.size() == 0)
        {
            currentAddress = address;
            startAddress = address;
        }else if(address != currentAddress)
        {
            logger->logDebug("Uninitialized memory found at address " + std::format("{:x}", address) + " from " + std::format("{:x}", currentAddress));
            loadMemory(startAddress, startAddress + currentData.size(), currentData);
            currentData.clear();
            currentAddress = address;
            startAddress = address;
        }
        /// The data is the 24 characters after the ": ""
        std::string data = line.substr(line.find(':') + 2, 24);
        for(int i = 0; i < 8; i ++)
        {
            char val[2] = {data[i*3+1], data[i*3+2]};
            if(val[0] == '-'){
                /// If the first character is a '-' the data is not present at this address
                if(currentData.size() != 0)
                {
                    logger->logDebug("Uninitialized memory found at address " + std::format("{:x}", address + i) + " from " + std::format("{:x}", currentAddress));
                    loadMemory(startAddress, startAddress + currentData.size(), currentData);
                    currentData.clear();
                }
                startAddress = address + i;
            }else{
                try{
                    currentData.push_back(std::stoi(val, nullptr, 16));
                }catch(std::invalid_argument &e){
                    logger->logError("Invalid byte '" + std::string(val) + "' in file");
                    throw std::runtime_error("Invalid data in dump file");
                }
            }
        }
        currentAddress += 8;
    }
    if(!currentData.empty()) loadMemory(startAddress, startAddress + currentData.size(), currentData);

}

void Emulator::printMemory(){
    for(auto segment : memory)
    {
        /// Print the start and end of the segment (in hex)
        logger->logInfo("Memory segment from " + std::format("{:x}", segment.start) + " to " + std::format("{:x}", segment.end));
        for(auto byte : segment.data)
        {
            logger->logInfo(std::format("{:x}", byte));
        }
    }
}

void Emulator::printRegisters(){
    std::cout << "Emulated processor state:" << std::endl;
    for(int i = 0; i < 15;){
        if(i < 10) std::cout << ' ';
        if(i == 0){
            std::cout << "R" << i << ": " << std::format("{:08x}", 0) << "\t";
            i++;
        }else{
        std::cout << "R" << i << ": " << std::format("{:08x}", registers[i++]) << "\t";
        }
        if(i < 10) std::cout << ' ';
        std::cout << "R" << i << ": " << std::format("{:08x}", registers[i++]) << "\t";
        if(i < 10) std::cout << ' ';
        std::cout << "R" << i << ": " << std::format("{:08x}", registers[i++]) << "\t";
        if(i < 10) std::cout << ' ';
        std::cout << "R" << i << ": " << std::format("{:08x}", registers[i++]) << std::endl;
    
    }
}

void Emulator::run(uint32_t entry){
    logger->logInfo("Starting emulator at entry point " + std::format("{:x}", entry));
    registers.PC() = entry;
    while(step() == 0);
}

/// @brief The opcode enum (taken from parser.y)
enum Opcode
{
    HALT = 0b00000,
    INT = 0b00001,
    IRET = 0b00010,
    CALL = 0b00011,
    RET = 0b00100,
    JMP = 0b00101,
    BEQ = 0b00110,
    BNE = 0b00111,
    BGT = 0b01000,
    PUSH = 0b01001,
    POP = 0b01010,
    XCHG = 0b01011,
    ADD = 0b01100,
    SUB = 0b01101,
    MUL = 0b01110,
    DIV = 0b01111,
    NOT = 0b10000,
    AND = 0b10001,
    OR = 0b10010,
    XOR = 0b10011,
    SHL = 0b10100,
    SHR = 0b10101,
    LD = 0b10110,
    ST = 0b10111,
    CSRRD = 0b11000,
    CSRWR = 0b11001,
};

/// @brief The modifier (taken from parser.y)
enum Modifier
{
    LIT_DIR = 0b000,
    SYM_DIR = 0b001,
    LIT_IND = 0b010,
    SYM_IND = 0b011,
    REG_DIR = 0b100,
    REG_IND = 0b101,
    REG_LIT_IND = 0b110,
    REG_SYM_IND = 0b111,
};

/// @brief The control status register type (taken from parser.y)
enum csr_type: char{
    STATUS = 0,
    HANDLER = 1,
    CAUSE = 2
};

int Emulator::step(){
    logger->logInfo("Stepping");
    logger->logDebug("PC: " + std::format("{:x}", registers.PC()));
    instruction instr;
    /// Fetch and decode the instruction
    instr.raw = getMemory(registers.PC());
    instr.raw = __builtin_bswap32(instr.raw);
    logger->logDebug("Instruction: " + std::format("{:08x}", instr.raw));
    /// Automatically increment the PC
    registers.PC() += 4;
    
    /// Execute the instruction
    int retval = execute(instr);

    /// Handle interrupts (if any)
    /// TODO: Implement interrupt handling
    return retval;
}

int Emulator::execute(instruction instr){
    switch (instr.fields.OC)
    {
        case Opcode::HALT:{
            logger->logInfo("HALT");
            return 1;
        }case Opcode::INT:{
            logger->logInfo("INT");
            push(registers.SR().raw);
            push(registers.PC());
            registers.SR().raw = 0x00000001;
            registers.PC() = registers.HR();
            return 0;
        }case Opcode::IRET:{
            logger->logInfo("IRET");
            registers.PC() = pop();
            registers.SR().raw = pop();
            return 0;
        }case Opcode::CALL:{
            logger->logInfo("CALL");
            push(registers.PC());
            int64_t operand = calculateOperand(instr);
            operand += registers.PC();
            if(operand < 0 || operand > 0xffffffff){
                logger->logError("Invalid call address '" + std::format("{:x}", operand) + "' at PC " + std::format("{:x}", registers.PC()));
                throw std::runtime_error("Invalid call address");
            }
            return 0;
        }case Opcode::RET:{
            logger->logInfo("RET");
            registers.PC() = pop();
            return 0;
        }case Opcode::JMP:{
            logger->logInfo("JMP");
            registers.PC() = calculateOperand(instr);
            return 0;
        }case Opcode::BEQ:{
            logger->logInfo("BEQ");
            if(registers[instr.fields.RegA] == registers[instr.fields.RegB]){
                registers.PC() = calculateOperand(instr);
            }
            return 0;
        }case Opcode::BNE:{
            logger->logInfo("BNE");
            if(registers[instr.fields.RegA] != registers[instr.fields.RegB]){
                registers.PC() = calculateOperand(instr);
            }
            return 0;
        }case Opcode::BGT:{
            logger->logInfo("BGT");
            if(*(int32_t*)&registers[instr.fields.RegA] > *(int32_t*)&registers[instr.fields.RegB]){
                registers.PC() = calculateOperand(instr);
            }
            return 0;
        }case Opcode::PUSH:{
            logger->logInfo("PUSH");
            push(registers[instr.fields.RegA]);
            return 0;
        }case Opcode::POP:{
            logger->logInfo("POP");
            registers[instr.fields.RegA] = pop();
            return 0;
        }case Opcode::XCHG:{
            logger->logInfo("XCHG");
            uint32_t temp = registers[instr.fields.RegA];
            registers[instr.fields.RegA] = registers[instr.fields.RegB];
            registers[instr.fields.RegB] = temp;
            return 0;
        }case Opcode::ADD:{
            logger->logInfo("ADD");
            /// We need to cast to int64_t to prevent overflow
            int64_t result = (int64_t)registers[instr.fields.RegA] + (int64_t)registers[instr.fields.RegB];
            /// And finally we put the lower 32 bits into the destination register
            registers[instr.fields.RegA] = result & 0xffffffff;
            return 0;
        }case Opcode::SUB:{
            logger->logInfo("SUB");
            /// We need to cast to int64_t to prevent overflow
            int64_t result = (int64_t)registers[instr.fields.RegA] - (int64_t)registers[instr.fields.RegB];
            /// And finally we put the lower 32 bits into the destination register
            registers[instr.fields.RegA] = result & 0xffffffff;
            return 0;
        }case Opcode::MUL:{
            logger->logInfo("MUL");
            /// We need to cast to int64_t to prevent overflow
            int64_t result = (int64_t)registers[instr.fields.RegA] * (int64_t)registers[instr.fields.RegB];
            /// And finally we put the lower 32 bits into the destination register
            registers[instr.fields.RegA] = result & 0xffffffff;
            return 0;
        }case Opcode::DIV:{
            logger->logInfo("DIV");
            /// We need to cast to int64_t to prevent overflow
            int64_t result = (int64_t)registers[instr.fields.RegA] / (int64_t)registers[instr.fields.RegB];
            /// And finally we put the lower 32 bits into the destination register
            registers[instr.fields.RegA] = result & 0xffffffff;
            return 0;
        }case Opcode::NOT:{
            logger->logInfo("NOT");
            uint32_t result = ~registers[instr.fields.RegA];
            /// And finally we put the lower 32 bits into the destination register
            registers[instr.fields.RegA] = result & 0xffffffff;
            return 0;
        }case Opcode::AND:{
            logger->logInfo("AND");
            uint32_t result = registers[instr.fields.RegA] & registers[instr.fields.RegB];
            /// And finally we put the lower 32 bits into the destination register
            registers[instr.fields.RegA] = result;
            return 0;
        }case Opcode::OR:{
            logger->logInfo("OR");
            uint32_t result = registers[instr.fields.RegA] | registers[instr.fields.RegB];
            /// And finally we put the lower 32 bits into the destination register
            registers[instr.fields.RegA] = result;
            return 0;
        }case Opcode::XOR:{
            logger->logInfo("XOR");
            uint32_t result = registers[instr.fields.RegA] ^ registers[instr.fields.RegB];
            /// And finally we put the lower 32 bits into the destination register
            registers[instr.fields.RegA] = result;
            return 0;
        }case Opcode::SHL:{
            logger->logInfo("SHL");
            uint32_t result = registers[instr.fields.RegA] << registers[instr.fields.RegB];
            /// And finally we put the lower 32 bits into the destination register
            registers[instr.fields.RegA] = result;
            return 0;
        }case Opcode::SHR:{
            logger->logInfo("SHR");
            uint32_t result = registers[instr.fields.RegA] >> registers[instr.fields.RegB];
            /// And finally we put the lower 32 bits into the destination register
            registers[instr.fields.RegA] = result;
            return 0;
        }case Opcode::LD:{
            logger->logInfo("LD");
            registers[instr.fields.RegA] = calculateOperand(instr);
            return 0;
        }case Opcode::ST:{
            logger->logInfo("ST");
            setMemory(calculateOperand(instr), registers[instr.fields.RegA]);
            return 0;
        }case Opcode::CSRRD:{
            logger->logInfo("CSRRD");
            if(instr.fields.RegA == csr_type::STATUS){
                registers[instr.fields.RegB] = registers.SR().raw;
            }
            if(instr.fields.RegA == csr_type::HANDLER){
                registers[instr.fields.RegB] = registers.HR();
            }
            if(instr.fields.RegA == csr_type::CAUSE){
                registers[instr.fields.RegB] = registers.CS();
            }
            return 0;
        }case Opcode::CSRWR:{
            logger->logInfo("CSRWR");
            if(instr.fields.RegA == csr_type::STATUS){
                registers.SR().raw = registers[instr.fields.RegB];
            }
            if(instr.fields.RegA == csr_type::HANDLER){
                registers.HR() = registers[instr.fields.RegB];
            }
            if(instr.fields.RegA == csr_type::CAUSE){
                registers.CS() = registers[instr.fields.RegB];
            }
            return 0;
        }
            
    }
    return 0;
}

int32_t Emulator::calculateOperand(instruction instr){
    int32_t displacement = instr.fields.Disp;
    /// We need to sign extend the displacement
    if(displacement & 0x800){
        displacement |= 0xFFFFF000;
    }
    switch(instr.fields.MOD){
        case Modifier::LIT_DIR:
            logger->logDebug("LIT_DIR");
            return displacement;
        case Modifier::LIT_IND:
            logger->logDebug("LIT_IND");
            return getMemory(displacement);
        case Modifier::SYM_DIR:
            logger->logDebug("SYM_DIR");
            return registers.PC() - 4 + displacement;
        case Modifier::SYM_IND:
            logger->logDebug("SYM_IND");
            return getMemory(registers.PC() - 4 + displacement);
        case Modifier::REG_DIR:
            logger->logDebug("REG_DIR");
            return registers[instr.fields.RegA];
        case Modifier::REG_IND:
            logger->logDebug("REG_IND");
            return getMemory(registers[instr.fields.RegA]);
        case Modifier::REG_LIT_IND:
            logger->logDebug("REG_LIT_IND");
            return getMemory(displacement + registers[instr.fields.RegA]);
        case Modifier::REG_SYM_IND:
            logger->logDebug("REG_SYM_IND");
            return getMemory(registers.PC() - 4 + displacement + registers[instr.fields.RegA]);
    }
    logger->logError("Invalid operand modifier '" + std::format("{:03b}", (uint8_t)instr.fields.MOD) + "' for instruction '" + std::format("{:03b}", (uint8_t)instr.fields.OC) + "' at PC " + std::format("{:x}", registers.PC() - 4));
    throw std::runtime_error("Invalid operand modifier");
}

void Emulator::push(uint32_t value){
    /// Push the value onto the stack
    registers.SP() -= 4;
    setMemory(registers.SP(), value);
}

uint32_t Emulator::pop(){
    /// Pop a value from the stack
    uint32_t value = getMemory(registers.SP());
    registers.SP() += 4;
    return value;
}

void Emulator::loadMemory(uint32_t start, uint32_t end, std::vector<uint8_t> data)
{
    logger->logInfo("Loading memory from " + std::format("{:x}", start) + " to " + std::format("{:x}", end));
    memorySegment segment;
    segment.start = start;
    segment.end = end;
    segment.data = data;
    memory.push_back(segment);
}

uint32_t Emulator::getMemory(uint32_t address)
{
    logger->logDebug("Getting memory at address " + std::format("{:x}", address));
    if(address >= 0xffffff00)
    {
        /// TODO: Implement memory-mapped register read
        logger->logInfo("Reading from memory-mapped register");
        return 0;
    }
    for(auto segment : memory)
    {
        logger->logDebug("Checking segment from " + std::format("{:x}", segment.start) + " to " + std::format("{:x}", segment.end));
        if(address < segment.end)
        {
            if(address < segment.start)
            {
                logger->logError("Memory address " + std::format("{:x}", address) + " out of bounds");
                throw std::runtime_error("Memory access out of bounds");
            }
            if(address + 3 >= segment.end)
            {
                logger->logError("Memory address " + std::format("{:x}", address) + " out of bounds");
                throw std::runtime_error("Memory access out of bounds");
            }
            return segment.data[address - segment.start] << 24 | segment.data[address - segment.start + 1] << 16 | segment.data[address - segment.start + 2] << 8 | segment.data[address - segment.start + 3];
        }
    }
}

void Emulator::setMemory(uint32_t address, uint32_t value)
{
    if(address >= 0xffffff00)
    {
        /// TODO: Implement memory-mapped register write
        logger->logInfo("Writing to memory-mapped register");
        return;
    }
    for(int i = 0; i < memory.size(); i++)
    {
        if(address < memory[i].end)
        {
            if(address < memory[i].start)
            {
                /// If the address is not in any segment, either create a new segment or add to this one
                if(address < memory[i].start - 4)
                {
                    memorySegment segment;
                    segment.start = address;
                    segment.end = address + 4;
                    segment.data = {(uint8_t)(value), (uint8_t)(value>>8), (uint8_t)(value>>16), (uint8_t)(value>>24)};
                }else{
                    for(int byte = 0; byte < 4; byte++)
                    {
                        if(address + byte < memory[i].start){
                            memory[i].data.insert(memory[i].data.begin(), (uint8_t)(value>>(byte*8)));
                        }else{
                            memory[i].data[address - memory[i].start + byte] = (uint8_t)(value>>(byte*8));
                        }
                    }
                    memory[i].start = address;
                }
            }
            memory[i].data[address - memory[i].start] = value;
            return;
        }
    }
    logger->logError("Memory address " + std::format("{:x}", address) + " out of bounds");
    throw std::runtime_error("Memory access out of bounds");
}