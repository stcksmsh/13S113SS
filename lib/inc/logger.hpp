/**
 * @file logger.hpp
 * @author Kosta Vukicevic (stcksmsh@gmail.com)
 * @brief A class that logs messages to a stream
 * @version 0.1
 * @date 2024-05-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include "colors.h"
#include <vector>
#include <iostream>

class Logger
{
public:
    Logger(bool live = false, int debugLevel = 0, std::ostream &stream = std::cout) : live(live), debugLevel(debugLevel) {}
    ~Logger(){};

    /// @brief Logs an error message
    /// @param message The message to log
    /// @param file Whic
    /// @param line
    /// @param function
    void logError(const std::string &message, const std::string &file = "", const int line = -1, const std::string &function = "");

    void logWarning(const std::string &message, const std::string &file = "", const int line = -1, const std::string &function = "");

    void logInfo(const std::string &message, const std::string &file = "", const int line = -1, const std::string &function = "");

    void logDebug(const std::string &message, const std::string &file = "", const int line = -1, const std::string &function = "");

    void printErrors(std::ostream &stream = std::cout);
    void printWarnings(std::ostream &stream = std::cout);
    void printInfos(std::ostream &stream = std::cout);
    void printDebugs(std::ostream &stream = std::cout);

    void print(std::ostream &stream = std::cout);

    void printAll(std::ostream &stream = std::cout)
    {
        printErrors(stream);
        printWarnings(stream);
        printInfos(stream);
        printDebugs(stream);
    }

    bool errorExists() const { return errors.size() > 0; }

private:
    inline std::string formatError(const std::string &message, const std::string &file = "", const int line = -1, const std::string &function = "")
    {
        std::string formatted = colors::boldred + "ERROR" + colors::reset + ": " + message;
        if (file != "")
        {
            formatted += " in " + file;
        }
        if (line != -1)
        {
            formatted += " at line " + std::to_string(line);
        }
        if (function != "")
        {
            formatted += " in function " + function;
        }
        return formatted;
    }

    inline std::string formatWarning(const std::string &message, const std::string &file = "", const int line = -1, const std::string &function = "")
    {
        std::string formatted = colors::boldyellow + "WARNING" + colors::reset + ": " + message;
        if (file != "")
        {
            formatted += " in " + file;
        }
        if (line != -1)
        {
            formatted += " at line " + std::to_string(line);
        }
        if (function != "")
        {
            formatted += " in function " + function;
        }
        return formatted;
    }

    inline std::string formatInfo(const std::string &message, const std::string &file = "", const int line = -1, const std::string &function = "")
    {
        std::string formatted = colors::boldyellow + "WARNING" + colors::reset + ": " + message;
        if (file != "")
        {
            formatted += " in " + file;
        }
        if (line != -1)
        {
            formatted += " at line " + std::to_string(line);
        }
        if (function != "")
        {
            formatted += " in function " + function;
        }
        return formatted;
    }

    inline std::string formatDebug(const std::string &message, const std::string &file = "", const int line = -1, const std::string &function = "")
    {
        std::string formatted = colors::boldyellow + "WARNING" + colors::reset + ": " + message;
        if (file != "")
        {
            formatted += " in " + file;
        }
        if (line != -1)
        {
            formatted += " at line " + std::to_string(line);
        }
        if (function != "")
        {
            formatted += " in function " + function;
        }
        return formatted;
    }

    int debugLevel = 0;
    bool live = false;

    struct Message
    {
        std::string message;
        std::string file;
        int line;
        std::string function;
    };

    std::vector<Message> errors;
    std::vector<Message> warnings;
    std::vector<Message> infos;
    std::vector<Message> debugs;
};

#endif // __LOGGER_HPP__