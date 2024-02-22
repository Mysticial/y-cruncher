/* ToString.h - Conversions to Strings
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 07/07/2013
 *  Last Modified   : 08/24/2014
 * 
 */

#pragma once
#ifndef ymp_ConvertInteger_H
#define ymp_ConvertInteger_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string>
#include <sstream>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "NumberFormat.h"
namespace ymp{
namespace StringTools{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <typename IntegerType> YM_NO_INLINE
std::string tostr_ui_commas(const IntegerType& x, char separator = ','){
    //  Prints out x with comma separators.

    //  Conditions:
    //      "x" must be non-negative

    std::stringstream ss;
    ss << x;
    std::string str = ss.str();
    std::string out;

    const char* ptr = str.c_str();
    upL_t len = str.size();

    upL_t commas = (len + 2) / 3 - 1;
    upL_t shift = len - commas * 3;

    while (1){
        char ch = *ptr++;
        if (ch == '\0'){
            break;
        }
        if (shift == 0){
            out += separator;
            shift = 3;
        }
        out += ch;
        shift--;
    }

    return out;
}
template <typename IntegerType>
std::string tostr_si_commas(const IntegerType& x, char separator = ','){
    if (x < 0){
        return std::string("-") + tostr_ui_commas<IntegerType>(-x, separator);
    }else{
        return tostr_ui_commas<IntegerType>(x, separator);
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
inline YM_NO_INLINE std::string byte_prefix(upL_t index){
    static const char* BYTE_NAMES[] = {
        " bytes",
        " KiB",
        " MiB",
        " GiB",
        " TiB",
        " PiB",
        " EiB",
        " ZiB",
        " YiB",
    };
    constexpr upL_t SUFFIX_SIZE = sizeof(BYTE_NAMES) / sizeof(const char*);

    if (index < SUFFIX_SIZE){
        return BYTE_NAMES[index];
    }

    return " * 2^" + std::to_string(index) + "0";
}
template <typename IntegerType> YM_NO_INLINE
std::string tostr_ui_bytes(IntegerType bytes){
    //  Prints out bytes in one of the following forms:
    //  0.xx suffix
    //  x.xx suffix
    //  xx.x suffix
    //   xxx suffix

    //  Conditions:
    //      "bytes" must be non-negative

    std::string out;
    if (bytes < 1000){
        //  Downcast to a more efficient type.
        u32_t sbytes = (u32_t)bytes;
        if (bytes < 10){
            out += " ";
        }
        out += std::to_string(sbytes);
        out += byte_prefix(0);
        return out;
    }

    upL_t suffix_index = 1;
    while (bytes >= 1024000){
        bytes >>= 10;
        suffix_index++;
    }

    //  Downcast to a more efficient type.
    u32_t sbytes = (u32_t)bytes;

    sbytes *= 1000;
    sbytes >>= 10;

    //  .xxx
    if (sbytes < 995){
        sbytes += 5;
        sbytes /= 10;

        out += "0.";
        out += std::to_string(sbytes);
        out += byte_prefix(suffix_index);
        return out;
    }

    //  x.xx
    if (sbytes < 9995){
        sbytes += 5;
        sbytes /= 10;

        out += std::to_string(sbytes / 100);
        sbytes %= 100;
        out += ".";
        if (sbytes >= 10){
            out += std::to_string(sbytes);
        }else{
            out += "0";
            out += std::to_string(sbytes);
        }
        out += byte_prefix(suffix_index);
        return out;
    }

    //  xx.x or (0.98)
    if (sbytes < 99950){
        sbytes += 50;
        sbytes /= 100;

        out += std::to_string(sbytes / 10);
        sbytes %= 10;
        out += ".";
        out += std::to_string(sbytes);
        out += byte_prefix(suffix_index);
        return out;
    }

    //  xxx or (1.00)
    {
        sbytes += 500;
        sbytes /= 1000;

        out += " ";
        out += std::to_string(sbytes);
        out += byte_prefix(suffix_index);
        return out;
    }
}
template <typename IntegerType>
std::string tostr_si_bytes(IntegerType bytes){
    if (bytes < 0){
        return std::string("-") + tostr_ui_bytes((IntegerType)-bytes);
    }else{
        return tostr_ui_bytes((IntegerType)bytes);
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <typename IntegerType> YM_NO_INLINE
std::string tostr_ui(IntegerType x, NumberFormat format){
    switch (format){
    case COMMAS:
        return tostr_ui_commas(x, ',');
    case QUOTES:
        return tostr_ui_commas(x, '\'');
    case BYTES:
        return tostr_ui_bytes(x);
    case BYTES_EXPANDED:{
        auto out = tostr_ui_commas(x, ',');
        out += " (";
        out += tostr_ui_bytes(x);
        out += ")";
        return out;
    }
    default:
        std::stringstream ss;
        ss << x;
        return ss.str();
    }
}
template <typename IntegerType> YM_NO_INLINE
std::string tostr_si(IntegerType x, NumberFormat format){
    switch (format){
    case COMMAS:
        return tostr_si_commas(x, ',');
    case QUOTES:
        return tostr_si_commas(x, '\'');
    case BYTES:
        return tostr_si_bytes(x);
    case BYTES_EXPANDED:{
        auto out = tostr_si_commas(x, ',');
        out += " (";
        out += tostr_si_bytes(x);
        out += ")";
        return out;
    }
    default:
        std::stringstream ss;
        ss << x;
        return ss.str();
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <typename IntegerType> YM_NO_INLINE
IntegerType parse_ui(const char* str){
    IntegerType x = 0;
    char ch = *str++;
    while (ch >= '0' && ch <= '9'){
        x *= 10;
        x += ch - '0';
        ch = *str++;
    }
    return x;
}
template <typename IntegerType> YM_NO_INLINE
IntegerType parse_si(const char* str){
    if (str[0] == '\0'){
        return 0;
    }
    if (str[0] == '-'){
        return -parse_ui<IntegerType>(str + 1);
    }
    return parse_ui<IntegerType>(str);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
