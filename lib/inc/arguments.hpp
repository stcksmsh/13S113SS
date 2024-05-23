/**
 * @file arguments.hpp
 * @author Kosta Vukicevic (stcksmsh@gmail.com)
 * @brief A class for easier argument handling
 * @version 0.1
 * @date 2024-05-22
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __ARGUMENTS_HPP__
#define __ARGUMENTS_HPP__

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

class Arguments
{
public:
    struct Option
    {
        /// @brief The name of the option (can be invoked with --<name>)
        std::string name;
        /// @brief The short name of the option (can be invoked with -<shortName>)
        std::string shortName;
        /// @brief The description of the argument
        std::string description;
        /// @brief The number of arguments (0 for no arguments)
        int numArgs;
        /// @brief Can the option be invoked multiple times
        bool multiple;
        /// @brief Whether the argument is required
        bool required;
        /// @brief All arguments with the same exclusiveGroup value are exclusive
        int exclusiveGroup;
    };

public:
    /// @brief Constructor with argument specifications
    /// @param argSpecs The specifications for the arguments
    Arguments(std::vector<Option> argSpecs);

    ~Arguments() {}

    /// @brief Parses the supplied arguments (calls parse(std::vector<std::string> args)
    /// @param argc The number of arguments
    /// @param argv The arguments
    /// @return EXIT_SUCCESS if the arguments were parsed successfully, EXIT_FAILURE otherwise
    int parse(const int argc, const char **argv);

    /// @brief Parses the supplied arguments
    /// @param args The arguments
    /// @return EXIT_SUCCESS if the arguments were parsed successfully, EXIT_FAILURE otherwise
    int parse(std::vector<std::string> args);

    /// @brief pints the automatically generated help message
    void printHelp(std::ostream &out = std::cout);

    /// @brief Check if an option is present
    /// @param name The name of the option
    /// @return true if the option is present, false otherwise
    bool isPresent(std::string name);

    /// @brief Get the arguments for the supplied option
    /// @param name The name of the option
    /// @return The arguments for the option
    std::vector<std::vector<std::string>> getArguments(std::string name = "");

private:
    /// @brief The argument specifications
    std::vector<Option> argSpecs;
    /// @brief The parsed arguments for each option (the first vector is for the default arguments)
    std::vector<std::vector<std::vector<std::string>>> arguments;
    /// @brief Keeps track of the exclusive groups
    /// @details The key is the exclusive group number, the value is the name of the options used
    std::unordered_map<int, std::vector<std::string>> exclusiveGroups;
};

#endif // __ARGUMENTS_HPP__