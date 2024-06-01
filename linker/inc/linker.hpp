#ifndef __LINKER_HPP__
#define __LINKER_HPP__

#include "logger.hpp"
#include "elf.hpp"

class Linker
{
public:
    Linker(Logger *logger) : logger(logger) {
        logger->logDebug("Linker object created");
    }

    ~Linker() {
        logger->logDebug("Linker object destroyed");
    }

    void link(ELF &output, ELF input);

private:

    Logger *logger;

};

#endif // __LINKER_HPP__