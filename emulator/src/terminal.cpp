/**
 * @file terminal.cpp
 * @author Kosta Vukicevic (stcksmsh@gmail.com)
 * @brief The implementation of the Terminal class (used for MMIO operations in the emulator)
 * @version 0.1
 * @date 2024-06-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "terminal.hpp"
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

void Terminal::write(char data){
    // Write the data to the terminal (only the higher 8 bits are used)
    printf("%c", data);
    fflush(stdout);
}

char Terminal::read(){
    /// Read a character from the terminal
    return (uint32_t)character;
}

void Terminal::start(){
    // Start the terminal thread
    running = true;
    inputThread = std::thread(threadFunc, this);
}

void Terminal::stop(){
    running = false;
    ::write(pipefd[1], ".", 1); // Write a byte to the pipe to wake up the thread
    inputThread.join();
    /// Reset terminal settings
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
            perror("tcsetattr()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSANOW, &old) < 0)
            perror("tcsetattr ICANON");
}

void Terminal::threadFunc(void *arg){
    // Cast the argument to a Terminal object
    Terminal *terminal = (Terminal *)arg;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
            perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
            perror("tcsetattr ICANON");
    while(terminal->running){
        fd_set readfds;
        FD_ZERO(&readfds); /// Initialize the file descriptor set
        FD_SET(0, &readfds);  // Add stdin (file descriptor 0) to the set
        FD_SET(terminal->pipefd[0], &readfds); // Add pipe read end to the set
        
        int maxfd = std::max(0, terminal->pipefd[0]); // Determine the maximum file descriptor
        
        int ret = select(maxfd + 1, &readfds, NULL, NULL, NULL); // Wait for an input event
        
        if (ret > 0) {
            if (FD_ISSET(0, &readfds)) { /// If the input is from stdin
                terminal->character = getchar();
                terminal->parent->registers.SR().fields.TeR = 1;
            } else if (FD_ISSET(terminal->pipefd[0], &readfds)) { /// If the input is from the pipe
                char buf;
                ::read(terminal->pipefd[0], &buf, 1); // Read the wake-up byte
                // If running is false, exit the loop
                if (!terminal->running) {
                    break;
                }
            }
        }    }
}