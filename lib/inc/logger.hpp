/**
 * @file logger.hpp
 * @author Kosta Vukicevic (107367925+stcksmsh@users.noreply.github.com)
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
    /// @brief Logger constructor
    /// @param debugLevel The level of logging to be done
    /// @param live Whether to log live or not
    /// @param stream The stream to log (live)
    Logger(int debugLevel = 0, bool live = false, std::ostream &stream = std::cout) : live(live), debugLevel(debugLevel), stream(stream) {}
    ~Logger(){};

    /// @brief Sets the log level
    /// @param level What level to set the log to
    void setLogLevel(int level) { debugLevel = level; }

    /// @brief Sets whether to log live or not
    /// @param live Whether to log live or not
    void setLive(bool live) { this->live = live; }

    /// @brief Logs an error message
    /// @param message The message to log
    /// @param file Which file the error occured in
    /// @param line Which line the error occured on
    /// @param function The function the error occured in
    void logError(const std::string &message, const std::string &file = "", const int line = -1, const std::string &function = "");

    /// @brief Logs a warning message
    /// @param message The message to log
    /// @param file Which file the warning occured in
    /// @param line Which line the warning occured on
    /// @param function The function the warning occured in
    void logWarning(const std::string &message, const std::string &file = "", const int line = -1, const std::string &function = "");

    /// @brief Logs an info message
    /// @param message The message to log
    /// @param file Which file the info occured in
    /// @param line Which line the info occured on
    /// @param function The function the info occured in
    void logInfo(const std::string &message, const std::string &file = "", const int line = -1, const std::string &function = "");


    /// @brief Logs a debug message
    /// @param message The message to log
    /// @param file Which file the debug occured in
    /// @param line Which line the debug occured on
    /// @param function The function the debug occured in
    void logDebug(const std::string &message, const std::string &file = "", const int line = -1, const std::string &function = "");

    void printErrors(std::ostream &stream = std::cout);
    void printWarnings(std::ostream &stream = std::cout);
    void printInfos(std::ostream &stream = std::cout);
    void printDebugs(std::ostream &stream = std::cout);

    /// @brief Prints all ( up to the log level ) messages
    /// @param stream The stream to print to
    void print(std::ostream &stream = std::cout);

    /// @brief Prints all messages, regardless of log level
    /// @param stream The stream to print to
    void printAll(std::ostream &stream = std::cout)
    {
        printErrors(stream);
        printWarnings(stream);
        printInfos(stream);
        printDebugs(stream);
    }

    /// @brief Whether an error exists
    /// @return bool Whether an error exists
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
        std::string formatted = colors::boldgreen + "INFO" + colors::reset + ": " + message;
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
        std::string formatted = colors::boldcyan + "DEBUG" + colors::reset + ": " + message;
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

    std::ostream &stream;

    std::vector<Message> errors;
    std::vector<Message> warnings;
    std::vector<Message> infos;
    std::vector<Message> debugs;
};

#endif // __LOGGER_HPP__