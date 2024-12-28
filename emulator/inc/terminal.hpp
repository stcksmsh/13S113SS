/**
 * @file terminal.hpp
 * @author Kosta Vukicevic (107367925+stcksmsh@users.noreply.github.com)
 * @brief Terminal class declaration (used for MMIO operations in the emulator)
 * @version 0.1
 * @date 2024-06-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __TERMINAL_HPP__
#define __TERMINAL_HPP__


#include "emulator.hpp"
#include <thread>
#include <cstdint>
#include <unistd.h>

class Terminal{
public:
    Terminal(Emulator *emulator): parent(emulator){
        // Create a pipe
        if(pipe(pipefd) == -1){
            perror("pipe");
            exit(1);
        }
    }

    /// @brief Function to call when the program writes to the terminal
    /// @param data The data to write to the terminal
    void write(char data);

    /// @brief Function to call when the program reads from the terminal
    /// @return The data read from the terminal
    char read();


    /// @brief Start the terminal thread
    void start();

    /// @brief Stop the terminal thread
    void stop();

private:

    /// @brief A thread function to continuously send interrupts to the CPU when the data is available
    /// @param arg A pointer to the Terminal object
    static void threadFunc(void *arg);

    /// @brief The parent emulator
    Emulator *parent;

    /// @brief The inputted character
    char character = '.';

    /// @brief The thread which reads stdin
    std::thread inputThread;

    /// @brief Is the terminal running
    bool running;

    /// @brief The pipe file descriptors
    int pipefd[2];
};

#endif // __TERMINAL_HPP__