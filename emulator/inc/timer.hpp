/**
 * @file timer.hpp
 * @author Kosta Vukicevic (107367925+stcksmsh@users.noreply.github.com)
 * @brief The header file for the Timer class (used for handling the timer in the emulator)
 * @version 0.1
 * @date 2024-06-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __TIMER_HPP__
#define __TIMER_HPP__


#include "emulator.hpp"
#include <thread>
#include <cstdint>

uint32_t const timer_cfg_values[] = {500, 1000, 1500, 2000, 5000, 10000, 30000, 60000};

class Timer{
public:
    Timer(Emulator *emulator): parent(emulator){
        timer_cfg = timer_cfg_values[0];
    }

    /// @brief Function to call when the program writes to the timer
    /// @param data The data to write to the timer
    void write(uint32_t data);

    /// @brief Start the timer thread
    void start();

    /// @brief Stop the timer thread
    void stop();


private:

    /// @brief A thread function to continuously send interrupts to the CPU with the specified frequency
    /// @param arg A pointer to the Timer object
    static void threadFunc(void *arg);

    /// @brief The parent emulator
    Emulator *parent;

    /// @brief The timer value
    uint32_t timer_cfg;

    /// @brief The thread which sends interrupts
    std::thread timerThread;

    /// @brief Is the timer running
    bool running;
};


#endif // __TIMER_HPP__