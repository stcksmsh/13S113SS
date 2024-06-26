#include "logger.hpp"

void Logger::logError(const std::string &message, const std::string &file, const int line, const std::string &function)
{
    if( debugLevel >= 1 ){
        if( live )
            stream << formatError(message, file, line, function) << std::endl;
        else
            errors.push_back(Message{message, file, line, function});
    }
}

void Logger::logWarning(const std::string &message, const std::string &file, const int line, const std::string &function)
{
    if( debugLevel >= 2 ){
        if( live )
            stream << formatWarning(message, file, line, function) << std::endl;
        else
            warnings.push_back(Message{message, file, line, function});
    }
}

void Logger::logInfo(const std::string &message, const std::string &file, const int line, const std::string &function)
{
    if( debugLevel >= 3 ){
        if( live )
            stream << formatInfo(message, file, line, function) << std::endl;
        else
            infos.push_back(Message{message, file, line, function});
    }
}

void Logger::logDebug(const std::string &message, const std::string &file, const int line, const std::string &function)
{
    if( debugLevel >= 4 ){
        if( live )
            stream << formatDebug(message, file, line, function) << std::endl;
        else
            debugs.push_back(Message{message, file, line, function});
    }
}


void Logger::printErrors(std::ostream &stream)
{
    for (Message &error : errors)
    {
        stream << formatError(error.message, error.file, error.line, error.function) << std::endl;
    }
}

void Logger::printWarnings(std::ostream &stream)
{
    for (Message &warning : warnings)
    {
        stream << formatWarning(warning.message, warning.file, warning.line, warning.function) << std::endl;
    }
}

void Logger::printInfos(std::ostream &stream)
{
    for (Message &info : infos)
    {
        stream << formatInfo(info.message, info.file, info.line, info.function) << std::endl;
    }
}

void Logger::printDebugs(std::ostream &stream)
{
    for (Message &debug : debugs)
    {
        stream << formatDebug(debug.message, debug.file, debug.line, debug.function) << std::endl;
    }
}

void Logger::print(std::ostream &stream)
{
    if (debugLevel >= 1)
        printErrors(stream);
    if (debugLevel >= 2)
        printWarnings(stream);
    if (debugLevel >= 3)
        printInfos(stream);
    if (debugLevel >= 4)
        printDebugs(stream);
}
