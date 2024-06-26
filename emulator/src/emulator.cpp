/**
 * @file emulator.cpp
 * @author Kosta Vukicevic (107367925+stcksmsh@users.noreply.github.com)
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
#include "terminal.hpp"
#include "timer.hpp"
#include <cassert>


Terminal *terminal;
Timer *timer;

void blankWrite(uint32_t value){
    std::cout << "Writing " << std::format("{:08x}", value) << " to memory-mapped register" << std::endl;
}

uint32_t blankRead(){return 0;}

void terminalWrite(uint32_t value){
    terminal->write(value >> 24);
}

uint32_t terminalRead(){
    uint32_t value = terminal->read();
    return value << 24;
}

void timerWrite(uint32_t value){
    timer->write(value);
}

Emulator::Emulator(Logger *logger): logger(logger), registers(logger){
    
    terminal = new Terminal(this);
    timer = new Timer(this);

    terminal->start();
    timer->start();

    memoryMappedRegisters.push_back({
        0xffffff00,
        terminalWrite,
        blankRead
    });

    memoryMappedRegisters.push_back({
        0xffffff04,
        blankWrite,
        terminalRead
    });

    memoryMappedRegisters.push_back({
        0xffffff10,
        timerWrite,
        blankRead
    });

    registers.SR().raw = 0x00000000;
    registers.SR().fields.TiE = 1;
    registers.SR().fields.TeE = 1;
    registers.SR().fields.GlE = 1;

}

Emulator::~Emulator(){
    terminal->stop();
    delete terminal;
    timer->stop();
    delete timer;
}

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
        address = std::stol(line.substr(0, line.find(':')), nullptr, 16);
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
                    uint8_t byte = val[0] <= '9' ? val[0] - '0' : val[0] - 'a' + 10;
                    byte <<= 4;
                    byte |= val[1] <= '9' ? val[1] - '0' : val[1] - 'a' + 10;
                    currentData.push_back(byte);
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
            std::cout << "R" << i << ": " << std::format("{:08x}", registers[i]) << "\t";
            i++;
        }
        if(i < 10) std::cout << ' ';
        std::cout << "R" << i << ": " << std::format("{:08x}", registers[i]) << "\t";
        i++;
        if(i < 10) std::cout << ' ';
        std::cout << "R" << i << ": " << std::format("{:08x}", registers[i]) << "\t";
        i++;
        if(i < 10) std::cout << ' ';
        std::cout << "R" << i << ": " << std::format("{:08x}", registers[i]) << std::endl;
        i++;    
    }
}

void Emulator::run(uint32_t entry){
    logger->logInfo("Starting emulator at entry point " + std::format("{:x}", entry));
    registers.PC() = entry;
    while(step() == 0);
    std::cout << "-----------------------------------------------------------------\nEmulated processor executed halt instruction" << std::endl;
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
    logger->logInfo("PC: " + std::format("{:x}", registers.PC()));
    instruction instr;
    /// Fetch and decode the instruction
    instr.raw = getMemory(registers.PC());
    instr.raw = __builtin_bswap32(instr.raw);
    logger->logInfo("Instruction: " + std::format("{:08x}", instr.raw));
    /// Automatically increment the PC
    registers.PC() += 4;
    
    /// Execute the instruction
    int retval = execute(instr);

    /// Handle interrupts (if any)
    handleInterrupts();
    return retval;
}

void Emulator::handleInterrupts(){
    // logger->logDebug("Handling interrupts");
    if(registers.SR().fields.TiE && registers.SR().fields.TiR){
        logger->logDebug("Handling Timer interrupt");
        registers.SR().fields.TiR = 0;
        push(registers.SR().raw);
        push(registers.PC());
        registers.CS() = 2;
        registers.SR().raw &= ~0x1;
        registers.PC() = registers.HR();
    }else if(registers.SR().fields.TeE && registers.SR().fields.TeR){
        logger->logDebug("Handling Terminal interrupt");
        registers.SR().fields.TeR = 0;
        push(registers.SR().raw);
        push(registers.PC());
        registers.CS() = 3;
        registers.SR().raw &= ~0x1;
        registers.PC() = registers.HR();
    }
}

int Emulator::execute(instruction instr){
    switch (instr.fields.OC)
    {
        case Opcode::HALT:{
            logger->logInfo("HALT");
            return 1;
        }case Opcode::INT:{
            logger->logInfo("INT");
            if(!registers.SR().fields.GlE){
                logger->logWarning("Interrupts disabled");
                return 0;
            }
            push(registers.SR().raw);
            registers.CS() = 0x4;
            push(registers.PC());
            registers.SR().raw &= ~0x1;
            registers.PC() = registers.HR();
            return 0;
        }case Opcode::IRET:{
            logger->logInfo("IRET");
            registers.PC() = pop();
            registers.SR().raw = pop();
            return 0;
        }case Opcode::CALL:{
            logger->logInfo("CALL");
            int64_t operand = calculateAddress(instr);
            push(registers.PC());
            registers.PC() = operand;
            return 0;
        }case Opcode::RET:{
            logger->logInfo("RET");
            registers.PC() = pop();
            return 0;
        }case Opcode::JMP:{
            logger->logInfo("JMP");
            registers.PC() = calculateAddress(instr);
            return 0;
        }case Opcode::BEQ:{
            logger->logInfo("BEQ");
            uint32_t operand = calculateAddress(instr);
            if(registers[instr.fields.RegA] == registers[instr.fields.RegB]){
                registers.PC() = operand;
            }
            return 0;
        }case Opcode::BNE:{
            logger->logInfo("BNE");
            uint32_t operand = calculateAddress(instr);
            if(registers[instr.fields.RegA] != registers[instr.fields.RegB]){
                registers.PC() = operand;
            }
            return 0;
        }case Opcode::BGT:{
            logger->logInfo("BGT");
            uint32_t operand = calculateAddress(instr);
            if(*(int32_t*)&registers[instr.fields.RegA] > *(int32_t*)&registers[instr.fields.RegB]){
                registers.PC() = operand;
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
            registers[instr.fields.RegB] = registers[instr.fields.RegB] + registers[instr.fields.RegA];
            return 0;
        }case Opcode::SUB:{
            logger->logInfo("SUB");
            registers[instr.fields.RegB] = registers[instr.fields.RegB] - registers[instr.fields.RegA];
            return 0;
        }case Opcode::MUL:{
            logger->logInfo("MUL");
            registers[instr.fields.RegB] = registers[instr.fields.RegB] * registers[instr.fields.RegA];
            return 0;
        }case Opcode::DIV:{
            logger->logInfo("DIV");
            registers[instr.fields.RegB] = registers[instr.fields.RegB] / registers[instr.fields.RegA];
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
            registers[instr.fields.RegB] = result;
            return 0;
        }case Opcode::OR:{
            logger->logInfo("OR");
            uint32_t result = registers[instr.fields.RegA] | registers[instr.fields.RegB];
            /// And finally we put the lower 32 bits into the destination register
            registers[instr.fields.RegB] = result;
            return 0;
        }case Opcode::XOR:{
            logger->logInfo("XOR");
            uint32_t result = registers[instr.fields.RegA] ^ registers[instr.fields.RegB];
            /// And finally we put the lower 32 bits into the destination register
            registers[instr.fields.RegB] = result;
            return 0;
        }case Opcode::SHL:{
            logger->logInfo("SHL");
            uint32_t result = registers[instr.fields.RegB] << registers[instr.fields.RegA];
            /// And finally we put the lower 32 bits into the destination register
            registers[instr.fields.RegB] = result;
            return 0;
        }case Opcode::SHR:{
            logger->logInfo("SHR");
            uint32_t result = registers[instr.fields.RegB] >> registers[instr.fields.RegA];
            /// And finally we put the lower 32 bits into the destination register
            registers[instr.fields.RegB] = result;
            return 0;
        }case Opcode::LD:{
            logger->logInfo("LD");
            registers[instr.fields.RegA] = calculateOperand(instr);
            return 0;
        }case Opcode::ST:{
            logger->logInfo("ST");
            /// ST does not use `calculateOperand` because the operand is the destination
            setMemory(calculateAddress(instr), registers[instr.fields.RegA]);
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
            if(instr.fields.RegB == csr_type::STATUS){
                logger->logDebug("Setting status to " + std::format("{:x}", registers[instr.fields.RegA]));
                registers.SR().raw = registers[instr.fields.RegA];
            }else if(instr.fields.RegB == csr_type::HANDLER){
                logger->logDebug("Setting handler to " + std::format("{:x}", registers[instr.fields.RegA]));
                registers.HR() = registers[instr.fields.RegA];
            }else if(instr.fields.RegB == csr_type::CAUSE){
                logger->logDebug("Setting cause to " + std::format("{:x}", registers[instr.fields.RegA]));
                registers.CS() = registers[instr.fields.RegA];
            }
            return 0;
        }
    }
    logger->logError("Invalid opcode '" + std::format("{:03b}", (uint8_t)instr.fields.OC) + "' at PC " + std::format("{:x}", registers.PC() - 4));
    return 1;
}

int32_t Emulator::calculateOperand(instruction instr){
    int32_t displacement = instr.fields.Disp;
    /// We need to sign extend the displacement
    if(displacement & 0x800){
        displacement |= 0xFFFFF000;
    }
    uint32_t unsignedDisplacement = displacement;
    switch(instr.fields.MOD){
        case Modifier::LIT_DIR:
            logger->logDebug("LIT_DIR");
            unsignedDisplacement = getMemory(registers.PC());
            unsignedDisplacement = __builtin_bswap32(unsignedDisplacement);
            logger->logDebug("Fetched extended literal " + std::format("{:08x}", unsignedDisplacement));
            registers.PC() += 4;
            return unsignedDisplacement;
        case Modifier::LIT_IND:
            logger->logDebug("LIT_IND");
            unsignedDisplacement = getMemory(registers.PC());
            unsignedDisplacement = __builtin_bswap32(unsignedDisplacement);
            logger->logDebug("Fetched extended literal " + std::format("{:08x}", unsignedDisplacement));
            registers.PC() += 4;
            return getMemory(unsignedDisplacement);
        case Modifier::SYM_DIR:
            logger->logDebug("SYM_DIR");
            unsignedDisplacement = getMemory(registers.PC());
            unsignedDisplacement = __builtin_bswap32(unsignedDisplacement);
            logger->logDebug("Fetched extended symbol " + std::format("{:08x}", unsignedDisplacement));
            registers.PC() += 4;
            return unsignedDisplacement;
        case Modifier::SYM_IND:
            logger->logDebug("SYM_IND");
            unsignedDisplacement = getMemory(registers.PC());
            unsignedDisplacement = __builtin_bswap32(unsignedDisplacement);
            logger->logDebug("Fetched extended symbol " + std::format("{:08x}", unsignedDisplacement));
            registers.PC() += 4;
            return getMemory(unsignedDisplacement);
        case Modifier::REG_DIR:
            logger->logDebug("REG_DIR");
            return registers[instr.fields.RegC];
        case Modifier::REG_IND:
            logger->logDebug("REG_IND");
            return getMemory(registers[instr.fields.RegC]);
        case Modifier::REG_LIT_IND:
            logger->logDebug("REG_LIT_IND");
            return getMemory(unsignedDisplacement + registers[instr.fields.RegC]);
        case Modifier::REG_SYM_IND:
            logger->logDebug("REG_SYM_IND");
            return getMemory(unsignedDisplacement + registers[instr.fields.RegC]);
    }
    logger->logError("Invalid operand modifier '" + std::format("{:03b}", (uint8_t)instr.fields.MOD) + "' for instruction '" + std::format("{:03b}", (uint8_t)instr.fields.OC) + "' at PC " + std::format("{:x}", registers.PC() - 4));
    throw std::runtime_error("Invalid operand modifier");
}

uint32_t Emulator::calculateAddress(instruction instr){
    int32_t displacement = instr.fields.Disp;
    /// We need to sign extend the displacement
    if(displacement & 0x800){
        displacement |= 0xFFFFF000;
    }
    uint32_t unsignedDisplacement = displacement;
    switch(instr.fields.MOD){
        case Modifier::LIT_DIR:
            logger->logError("LIT_DIR not allowed for store instructions");
            return unsignedDisplacement;
        case Modifier::LIT_IND:
            logger->logDebug("LIT_IND");
            unsignedDisplacement = getMemory(registers.PC());
            unsignedDisplacement = __builtin_bswap32(unsignedDisplacement);
            logger->logDebug("Fetched extended literal " + std::format("{:08x}", unsignedDisplacement));
            registers.PC() += 4;
            return unsignedDisplacement;
        case Modifier::SYM_DIR:
            logger->logError("SYM_DIR not allowed for store instructions");
            registers.PC() += 4;
            return unsignedDisplacement;
        case Modifier::SYM_IND:
            logger->logDebug("SYM_IND");
            unsignedDisplacement = getMemory(registers.PC());
            unsignedDisplacement = __builtin_bswap32(unsignedDisplacement);
            logger->logDebug("Fetched extended symbol " + std::format("{:08x}", unsignedDisplacement));
            registers.PC() += 4;
            return unsignedDisplacement;
        case Modifier::REG_DIR:
            logger->logError("REG_DIR not allowed for store instructions");
            return unsignedDisplacement;
        case Modifier::REG_IND:
            logger->logDebug("REG_IND");
            return registers[instr.fields.RegC];
        case Modifier::REG_LIT_IND:
            logger->logDebug("REG_LIT_IND");
            return unsignedDisplacement + registers[instr.fields.RegC];
        case Modifier::REG_SYM_IND:
            logger->logDebug("REG_SYM_IND");
            return unsignedDisplacement + registers[instr.fields.RegC];
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
    assert(segment.end - segment.start == segment.data.size());
    memory.push_back(segment);
}

uint32_t Emulator::getMemory(uint32_t address)
{
    logger->logDebug("Getting memory at address " + std::format("{:08x}", address));
    if(address >= 0xffffff00)
    {
        for(auto mmreg : memoryMappedRegisters)
        {
            if(address == mmreg.address)
            {
                logger->logDebug("Reading from memory-mapped register at address " + std::format("{:x}", address));
                return mmreg.readFunction();
            }
        }
        logger->logError("Memory-mapped register at address " + std::format("{:x}", address) + " not found");
        return 0;
    }
    std::size_t i = 0;
    for(;i < memory.size(); i++){
        logger->logDebug("Checking memory segment from " + std::format("{:x}", memory[i].start) + " to " + std::format("{:x}", memory[i].end));
        if(address < memory[i].end) break;
    }
    if(i == memory.size()){
        logger->logError("Memory address " + std::format("{:x}", address) + " out of bounds");
        throw std::runtime_error("Memory access out of bounds");
    }
    if(address + 4 > memory[i].end){
        // If the whole value is not in the segment, pad with 0s
        uint32_t value = 0;
        for(int byte = 0; byte < 4; byte++){
            if(address  + byte < memory[i].end){
                value |= (uint32_t)memory[i].data[address - memory[i].start + byte] << (24 - byte * 8);
            }
        }
        return value;
    }
    uint32_t value = (uint32_t)memory[i].data[address - memory[i].start] << 24 | (uint32_t)memory[i].data[address - memory[i].start + 1] << 16 | (uint32_t)memory[i].data[address - memory[i].start + 2] << 8 | (uint32_t)memory[i].data[address - memory[i].start + 3];
    logger->logDebug("Found value " + std::format("{:08x}", value));
    return value;
}

void Emulator::setMemory(uint32_t address, uint32_t value)
{
    logger->logDebug("Setting memory at address " + std::format("{:x}", address) + " to " + std::format("{:x}", value));
    if(address >= 0xffffff00)
    {
        for(auto &mmreg : memoryMappedRegisters)
        {
            if(address == mmreg.address)
            {
                logger->logDebug("Writing to memory-mapped register at address " + std::format("{:x}", address));
                mmreg.writeFunction(value);
                return;
            }
        }
        logger->logError("Memory-mapped register at address " + std::format("{:x}", address) + " not found");
        return;
    }
    std::size_t i = 0;
    for(;i < memory.size(); i++){
        if(address < memory[i].end) break;
    }
    if(i == memory.size()){
        /// If the address is not in any segment, create a new one
        memorySegment segment;
        segment.start = address;
        segment.end = address + 4;
        segment.data = {(uint8_t)(value >> 24), (uint8_t)(value >> 16), (uint8_t)(value >> 8), (uint8_t)(value)};
        memory.push_back(segment);
        logger->logDebug("Created new memory segment from " + std::format("{:x}", segment.start) + " to " + std::format("{:x}", segment.end));
        return;
    }
    if(address < memory[i].start){
        if(address + 4 < memory[i].start){
            /// If the value requres a new segment, create one before it
            memorySegment segment;
            segment.start = address;
            segment.end = address + 4;
            segment.data = {(uint8_t)(value >> 24), (uint8_t)(value >> 16), (uint8_t)(value >> 8), (uint8_t)(value)};
            memory.insert(memory.begin() + i, segment);
            logger->logDebug("Created new memory segment from " + std::format("{:x}", segment.start) + " to " + std::format("{:x}", segment.end));
        }else{
            /// If the value is partly in the segment, push_front the bytes
            int byte = 0;
            for(; address + byte < memory[i].start && byte < 4; byte++){
                memory[i].data.insert(memory[i].data.begin()+byte, (uint8_t)(value >> (24 - byte * 8)));
            }
            logger->logDebug("Left extended memory segment from " + std::format("{:x}", memory[i].start) + " to " + std::format("{:x}", memory[i].end) + " by " + std::format("{:d}", byte) + " bytes");
            memory[i].start -= byte;
            /// Then set the remaining bytes
            while(byte < 4){
                memory[i].data[byte] = (uint8_t)(value >> (24 - byte * 8));
                byte++;
            }
        }
        return;
    }
    if(address + 4 > memory[i].end){
        /// If the value is partly in the segment, create a new segment after it
        int byte = 0;
        /// First set the bytes already in the segment
        for(; address + byte < memory[i].end && byte < 4; byte++){
            memory[i].data[address - memory[i].start + byte] = (uint8_t)(value >> (24 - byte * 8));
        }
        logger->logDebug("Right extended memory segment from " + std::format("{:x}", memory[i].start) + " to " + std::format("{:x}", memory[i].end) + " by " + std::format("{:d}", 4-byte) + " bytes");
        /// Then append the remaining bytes
        while(byte < 4){
            memory[i].data.push_back((uint8_t)(value >> (24 - byte * 8)));
            memory[i].end++;
            byte++;
        }
        return;
    }
    /// Normal case, set the bytes in the segment
    for(int byte = 0; byte < 4; byte++){
        memory[i].data[address - memory[i].start + byte] = (uint8_t)(value >> (24 - byte * 8));
    }
}