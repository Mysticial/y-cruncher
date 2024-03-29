/* PrintHelpers.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 01/15/2015
 *  Last Modified   : 02/09/2024
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_PrintHelpers_H
#define ydv_DigitViewer_PrintHelpers_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string>
#include "PublicLibs/Types.h"
namespace DigitViewer2{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void clear_line();
void display_char(char digit, uiL_t& c, upL_t& b, upL_t& r);
void display_fancy(uiL_t pos, const char* str, upL_t digits);
std::string to_string_formatted(uiL_t pos, const char* str, upL_t digits);
uiL_t prompt_digits(const std::string& label = "Digits: ", uiL_t min = 0, uiL_t max = (uiL_t)0 - 1);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
