/* Margin.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 09/01/2014
 *  Last Modified   : 09/01/2014
 * 
 */

#pragma once
#ifndef ymp_Console_Margin_H
#define ymp_Console_Margin_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/BasicLibs/StringTools/ToString.h"
#include "BasicIO.h"
namespace ymp{
namespace Console{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  String Margins
YM_NO_INLINE    std::string print_marginL         (upL_t margin, std::string x, char color = ' ');
YM_NO_INLINE    std::string println_marginL       (upL_t margin, std::string x, char color = ' ');
YM_NO_INLINE    std::string print_marginR         (upL_t margin, const std::string& x, char color = ' ');
YM_NO_INLINE    std::string println_marginR       (upL_t margin, const std::string& x, char color = ' ');
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Integer Margins
template <typename IntegerType>
std::string print_marginL_int(upL_t margin, const IntegerType& x, char color = ' '){
    return print_marginL(margin, StringTools::tostr(x, StringTools::NORMAL), color);
}
template <typename IntegerType>
std::string println_marginL_int(upL_t margin, const IntegerType& x, char color = ' '){
    return println_marginL(margin, StringTools::tostr(x, StringTools::NORMAL), color);
}
template <typename IntegerType>
std::string print_marginR_int(upL_t margin, const IntegerType& x, char color = ' '){
    return print_marginR(margin, StringTools::tostr(x, StringTools::NORMAL), color);
}
template <typename IntegerType>
std::string println_marginR_int(upL_t margin, const IntegerType& x, char color = ' '){
    return println_marginR(margin, StringTools::tostr(x, StringTools::NORMAL), color);
}
template <typename IntegerType>
std::string print_marginL_commas(upL_t margin, const IntegerType& x, char color = ' '){
    return print_marginL(margin, StringTools::tostr(x, StringTools::COMMAS), color);
}
template <typename IntegerType>
std::string println_marginL_commas(upL_t margin, const IntegerType& x, char color = ' '){
    return println_marginL(margin, StringTools::tostr(x, StringTools::COMMAS), color);
}
template <typename IntegerType>
std::string print_marginR_commas(upL_t margin, const IntegerType& x, char color = ' '){
    return print_marginR(margin, StringTools::tostr(x, StringTools::COMMAS), color);
}
template <typename IntegerType>
std::string println_marginR_commas(upL_t margin, const IntegerType& x, char color = ' '){
    return println_marginR(margin, StringTools::tostr(x, StringTools::COMMAS), color);
}
template <typename IntegerType>
std::string print_marginL_quotes(upL_t margin, const IntegerType& x, char color = ' '){
    return print_marginL(margin, StringTools::tostr(x, StringTools::QUOTES), color);
}
template <typename IntegerType>
std::string print_marginR_quotes(upL_t margin, const IntegerType& x, char color = ' '){
    return print_marginR(margin, StringTools::tostr(x, StringTools::QUOTES), color);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Float Margins
YM_NO_INLINE    std::string print_marginL_float   (upL_t margin, double x, int precision = DEFAULT_PRECISION, char color = ' ');
YM_NO_INLINE    std::string println_marginL_float (upL_t margin, double x, int precision = DEFAULT_PRECISION, char color = ' ');
YM_NO_INLINE    std::string print_marginR_fixed   (upL_t margin, double x, int precision = DEFAULT_PRECISION, char color = ' ');
YM_NO_INLINE    std::string println_marginR_fixed (upL_t margin, double x, int precision = DEFAULT_PRECISION, char color = ' ');
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
