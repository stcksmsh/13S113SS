/**
 * @file timer.cpp
 * @author Kosta Vukicevic (107367925+stcksmsh@users.noreply.github.com)
 * @brief The implementation of the Timer class (used for handling the timer in the emulator)
 * @version 0.1
 * @date 2024-06-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "timer.hpp"

void Timer::start(){
    // Start the timer thread
    timerThread = std::thread(threadFunc, this);
}

void Timer::stop(){
    // Stop the timer thread
    running = false;
    timerThread.join();
}

void Timer::write(uint32_t data){
    // Set the timer value
    try{
        timer_cfg = timer_cfg_values[data];
    }catch(std::out_of_range &e){
        // If the value is out of range, set the timer to the default value (index 0)
        timer_cfg = timer_cfg_values[0];
    }
    parent->logger->logInfo("Timer value set to " + std::to_string(timer_cfg) + "ms");
}

void Timer::threadFunc(void *arg){
    // Cast the argument to a Timer object
    Timer *timer = (Timer *)arg;
    timer->running = true;
    while(timer->running){
        // Sleep for the specified time
        std::this_thread::sleep_for(std::chrono::milliseconds(timer->timer_cfg));
        // Send an interrupt to the CPU
        timer->parent->registers.SR().fields.TiR = 1;
    }
}