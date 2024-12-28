#ifndef FORMATTER_HPP
#define FORMATTER_HPP

#include <string>
#include <sstream>
#include <stdexcept>
#include <regex>

class Formatter {
public:
    // Format a number based on the provided format string and arguments
    template <typename... Args>
    static std::string format(const std::string& fmt, Args... args) {
        return format_impl(fmt, args...);
    }

private:
    // Recursive variadic template implementation
    template <typename T, typename... Args>
    static std::string format_impl(const std::string& fmt, T value, Args... args) {
        std::regex placeholder_regex(R"(\{\:([><^])?([0]?)([0-9]*)([dxXb])\})");
        std::smatch match;

        if (std::regex_search(fmt, match, placeholder_regex)) {
            // Extract the matched placeholder and its position
            std::string placeholder = match.str();
            size_t placeholder_pos = match.position();
            size_t placeholder_length = match.length();

            // Format the current value
            std::string formatted_value = format_value(placeholder, value);

            // Construct the new format string with the current placeholder replaced
            std::string updated_fmt = fmt.substr(0, placeholder_pos) + formatted_value +
                                    fmt.substr(placeholder_pos + placeholder_length);

            // Recurse with the updated format string and remaining arguments
            return format_impl(updated_fmt, args...);
        }

        // If no more placeholders, check for extra arguments
        if constexpr (sizeof...(args) > 0) {
            throw std::invalid_argument("Too many arguments provided for format string.");
        }

        return fmt;
    }

    // Base case for recursion
    static std::string format_impl(const std::string& fmt);

    // Helpers to format a single value
    static std::string format_value(const std::string& placeholder, int value);

    static std::string format_value(const std::string& placeholder, unsigned int value);

};

#endif // FORMATTER_HPP
