#include "formatter.hpp"
#include <regex>
#include <iomanip>
#include <bitset>


// Base case for recursion
std::string Formatter::format_impl(const std::string& fmt) {
    return fmt;
}

// Helper to format a single value
std::string Formatter::format_value(const std::string& placeholder, int value) {
    std::regex placeholder_regex(R"(\{\:([><^])?([0]?)([0-9]*)([dxXb])\})");
    std::smatch match;

    if (!std::regex_match(placeholder, match, placeholder_regex)) {
        throw std::invalid_argument("Invalid placeholder: " + placeholder);
    }

    // Extract the components of the placeholder
    std::string alignment = match[1];     // Alignment: '>', '<', or '^'
    std::string padding = match[2];  // Padding character ('0' for zero-padding)
    std::string width_str = match[3];     // Width
    char specifier = match[4].str()[0];   // Specifier: 'x', 'X', 'd', or 'b'

    int width = 0;
    if (!width_str.empty()) {
        width = std::stoi(width_str);
    }

    std::ostringstream oss;

    // Determine the alignment
    if (alignment == "<") {
        oss << std::left;
    } else if (alignment == "^") {
        oss << std::internal;
    } else {
        oss << std::right; // Default to right alignment
    }

    // Determine the padding character
    char padding_char = ' ';
    if (!padding.empty() && padding == "0") {
        padding_char = '0';
        oss << std::setfill('0');
    } else {
        oss << std::setfill(' ');
    }

    // Apply width
    if (width > 0) {
        oss << std::setw(width);
    }

    // Format the number based on the specifier
    if (specifier == 'x') {
        oss << std::hex << std::nouppercase << value;
    } else if (specifier == 'X') {
        oss << std::hex << std::uppercase << value;
    } else if (specifier == 'd') {
        oss << std::dec << value;
    } else if (specifier == 'b') {
        std::string binary = std::bitset<64>(value).to_string();
        binary = binary.substr(binary.find('1')); // Remove leading zeros
        if (binary.empty()) binary = "0";         // Handle zero case
        if (width > static_cast<int>(binary.length())) {
            int padding = width - binary.length();
            if (alignment == "<") {
                return binary + std::string(padding, padding_char);
            } else if (alignment == "^") {
                int left_padding = padding / 2;
                int right_padding = padding - left_padding;
                return std::string(left_padding,  padding_char) + binary + std::string(right_padding,  padding_char);
            } else {
                return std::string(padding,  padding_char) + binary;
            }
        }
        return binary;
    } else {
        throw std::invalid_argument("Unsupported format specifier: " + std::string(1, specifier));
    }

    return oss.str();
}

// Helper to format a single value (unsigned int version)
std::string Formatter::format_value(const std::string& placeholder, unsigned int value) {
    std::regex placeholder_regex(R"(\{\:([><^])?([0]?)([0-9]*)([dxXb])\})");
    std::smatch match;

    if (!std::regex_match(placeholder, match, placeholder_regex)) {
        throw std::invalid_argument("Invalid placeholder: " + placeholder);
    }

    // Extract the components of the placeholder
    std::string alignment = match[1];     // Alignment: '>', '<', or '^'
    std::string padding = match[2];       // Padding character ('0' for zero-padding)
    std::string width_str = match[3];     // Width
    char specifier = match[4].str()[0];   // Specifier: 'x', 'X', 'd', or 'b'

    int width = 0;
    if (!width_str.empty()) {
        width = std::stoi(width_str);
    }

    std::ostringstream oss;

    // Determine the alignment
    if (alignment == "<") {
        oss << std::left;
    } else if (alignment == "^") {
        oss << std::internal;
    } else {
        oss << std::right; // Default to right alignment
    }

    // Determine the padding character
    char padding_char = ' ';
    if (!padding.empty() && padding == "0") {
        padding_char = '0';
        oss << std::setfill('0');
    } else {
        oss << std::setfill(' ');
    }

    // Apply width
    if (width > 0) {
        oss << std::setw(width);
    }

    // Format the number based on the specifier
    if (specifier == 'x') {
        oss << std::hex << std::nouppercase << value;
    } else if (specifier == 'X') {
        oss << std::hex << std::uppercase << value;
    } else if (specifier == 'd') {
        oss << std::dec << value;
    } else if (specifier == 'b') {
        std::string binary = std::bitset<64>(value).to_string();
        binary = binary.substr(binary.find('1')); // Remove leading zeros
        if (binary.empty()) binary = "0";         // Handle zero case
        if (width > static_cast<int>(binary.length())) {
            int padding = width - binary.length();
            if (alignment == "<") {
                return binary + std::string(padding, padding_char);
            } else if (alignment == "^") {
                int left_padding = padding / 2;
                int right_padding = padding - left_padding;
                return std::string(left_padding, padding_char) + binary + std::string(right_padding, padding_char);
            } else {
                return std::string(padding, padding_char) + binary;
            }
        }
        return binary;
    } else {
        throw std::invalid_argument("Unsupported format specifier: " + std::string(1, specifier));
    }

    return oss.str();
}
