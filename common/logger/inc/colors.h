/**
 * @file colors.h
 * @author Kosta Vukicevic (stcksmsh@gmail.com)
 * @brief Color definitions for output
 * @version 0.1
 * @date 2024-05-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __COLORS_H__
#define __COLORS_H__

#include <string>

namespace colors{
    const std::string reset = "\033[0m";
    const std::string black = "\033[30m";      /* Black */
    const std::string red = "\033[31m";      /* Red */
    const std::string green = "\033[32m";      /* Green */
    const std::string yellow = "\033[33m";      /* Yellow */
    const std::string blue = "\033[34m";      /* Blue */
    const std::string magenta = "\033[35m";      /* Magenta */
    const std::string cyan = "\033[36m";      /* Cyan */
    const std::string white = "\033[37m";      /* White */
    const std::string boldblack = "\033[1m\033[30m";      /* Bold Black */
    const std::string boldred = "\033[1m\033[31m";      /* Bold Red */
    const std::string boldgreen = "\033[1m\033[32m";      /* Bold Green */
    const std::string boldyellow = "\033[1m\033[33m";      /* Bold Yellow */
    const std::string boldblue = "\033[1m\033[34m";      /* Bold Blue */
    const std::string boldmagenta = "\033[1m\033[35m";      /* Bold Magenta */
    const std::string boldcyan = "\033[1m\033[36m";      /* Bold Cyan */
    const std::string boldwhite = "\033[1m\033[37m";      /* Bold White */
}


#endif // __COLORS_H__