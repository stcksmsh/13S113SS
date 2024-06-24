/**
 * @file emulator.hpp
 * @author Kosta Vukicevic (stcksmsh@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-06-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __EMULATOR_HPP__
#define __EMULATOR_HPP__

#include "logger.hpp"
#include <vector>
#include <cstdint>

class Emulator
{
public:
    /// @brief Emulator constructor
    /// @param logger The logger for the emulator
    Emulator(Logger *logger): logger(logger), registers(logger) {}

    /// @brief Load memory from a memory dump file
    /// @param filename The filename of the memory dump file
    void loadFromDump(std::string filename);

    /// @brief Print the memory
    void print();

    /// @brief Run the emulator
    void run();


private:

    /// @brief Struct representing an instruction (taken from parser.y)
    union instruction{
        struct{
            uint32_t Disp: 12,
                    RegC: 4,
                    RegB: 4,
                    RegA: 4,
                    MOD: 3,
                    OC: 5;
        } fields;
        uint32_t raw = 0;
    };


    /// @brief Step the emulator (execute one instruction)
    /// @return Whether the emulator should continue running (0) or stop (1)
    int step();

    /// @brief Execute an instruction
    /// @param instr The instruction to execute
    /// @return Whether the emulator should continue running (0) or stop (1)
    int execute(instruction instr);

    /// @brief Calculate an operand
    /// @param instr The instruction to calculate the operand from
    /// @return The operand
    int32_t calculateOperand(instruction instr);    

    /// @brief Pushes a value onto the stack
    /// @param value The value to push onto the stack
    void push(uint32_t value);

    /// @brief Pops a value from the stack
    /// @return The value popped from the stack
    uint32_t pop();

    /// @brief Loads a contiguous memory segment into the memory
    /// @param start The start address of the memory segment
    /// @param end The end address of the memory segment
    /// @param data The data to load into the memory segment
    void loadMemory(uint32_t start, uint32_t end, std::vector<uint8_t> data);

    /// @brief Gets the memory at a certain address
    /// @param address The address to get the memory from
    uint32_t getMemory(uint32_t address);

    /// @brief Sets the memory at a certain address
    /// @param address The address to set the memory at
    /// @param value The value to set the memory to
    void setMemory(uint32_t address, uint32_t value);

    /// @brief The logger for the emulator
    Logger *logger;

    /// @brief A struct representing a memory segment
    struct memorySegment
    {
        /// @brief The start of the memory segment
        uint32_t start;
        /// @brief The end of the memory segment
        uint32_t end;
        /// @brief The data in the memory segment
        std::vector<uint8_t> data;
    };
    /// @brief The memory of the emulated system
    std::vector<memorySegment> memory;

    /// @brief A class representing the registers
    class Registers{
    public:
        Registers(Logger *logger): registers{0}, logger(logger) {}
        ~Registers(){};

        /// @brief Gets a reference to a register
        /// @param index The index of the register to get
        /// @return A reference to the register
        uint32_t &operator[](int index) { 
            if(index == 0){
                logger->logError("Register 0 is wired to 0", __FILE__, __LINE__, __FUNCTION__);
                throw std::runtime_error("Register 0 should not be accessed in this way, it is wired to 0");
            }
            if(index < 1 || index > 15){
                logger->logError("Index out of bounds", __FILE__, __LINE__, __FUNCTION__);
                return registers[0];
            }
            return registers[index];
        }       
        
        /// @brief Gets a reference to the program counter
        /// @return A reference to the program counter
        uint32_t &PC() { return registers[15]; }

        /// @brief Gets a reference to the stack pointer
        /// @return A reference to the stack pointer
        uint32_t &SP() { return registers[14]; } 

        union Status{
            struct{
            uint32_t    Tr: 1, /// @brief The terminal interrupt mask bit
                        Ti: 1, /// @brief The timer interrupt mask bit
                        I: 1,  /// @brief The global interrupt mask bit
                        N: 1,  /// @brief The negative flag
                        Z: 1,  /// @brief The zero flag
                        C: 1,  /// @brief The carry flag
                        unused : 26;  /// @brief Unused bits

            } fields;
            uint32_t raw;
        };
        

        /// @brief Gets a reference to the status register
        /// @return A reference to the status register
        Status &SR() { return status; }

        /// @brief Gets a reference to the handler register
        /// @return A reference to the handler register
        uint32_t &HR() { return handler; }

        /// @brief Gets a reference to the cause register
        /// @return A reference to the cause register
        uint32_t &CS() { return cause; }
    private:
        /// @brief The registers of the emulated system
        uint32_t registers[15];
        /// @brief The logger for the registers
        Logger *logger;
        /// @brief The status register
        Status status;
        /// @brief The handler register
        uint32_t handler;
        /// @brief The cause register
        uint32_t cause;

    } registers;

    

};

#endif // __EMULATOR_HPP__