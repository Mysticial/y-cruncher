/* ToString.cpp - String Tools
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 07/07/2013
 *  Last Modified   : 08/24/2014
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <sstream>
#include <iomanip>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "ConvertInteger.h"
#include "ToString.h"
namespace ymp{
namespace StringTools{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
std::string tostr(uiL_t x, NumberFormat format){
    return tostr_ui(x, format);
}
std::string tostr(siL_t x, NumberFormat format){
    return tostr_si(x, format);
}
YM_NO_INLINE std::string tostrln(uiL_t x, NumberFormat format){
    return tostr(x, format) += "\r\n";
}
YM_NO_INLINE std::string tostrln(siL_t x, NumberFormat format){
    return tostr(x, format) += "\r\n";
}
YM_NO_INLINE uiL_t fromstr_uiL(const char*& str){
    uiL_t out = 0;
    do{
        char ch = *str++;
        if ('0' > ch || ch > '9'){
            break;
        }

        out *= 10;
        out += ch - '0';
    }while (true);
    return out;
}
YM_NO_INLINE siL_t fromstr_siL(const char*& str){
    if (str[0] == '-'){
        str++;
        return -(siL_t)fromstr_uiL(str);
    }else{
        return fromstr_uiL(str);
    }
}
YM_NO_INLINE uiL_t fromstr_uiL_commas(const char*& str){
    uiL_t out = 0;
    do{
        char ch = *str++;
        if (ch == ','){
            continue;
        }
        if ('0' > ch || ch > '9'){
            break;
        }

        out *= 10;
        out += ch - '0';
    }while (true);
    return out;
}
YM_NO_INLINE siL_t fromstr_siL_commas(const char*& str){
    if (str[0] == '-'){
        str++;
        return -(siL_t)fromstr_uiL_commas(str);
    }else{
        return fromstr_uiL_commas(str);
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Float
YM_NO_INLINE std::string tostr_float(double x, int precision){
    std::ostringstream out;
    out << std::setprecision(precision);
    out << x;
    return out.str();
}
YM_NO_INLINE std::string tostrln_float(double x, int precision){
    return tostr_float(x, precision) += "\r\n";
}
YM_NO_INLINE std::string tostr_fixed(double x, int precision){
    std::ostringstream out;
    out << std::setprecision(precision);
    out << std::fixed;
    out << x;
    return out.str();
}
YM_NO_INLINE std::string tostrln_fixed(double x, int precision){
    return tostr_fixed(x, precision) += "\r\n";
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Pointer
YM_NO_INLINE std::string tostr(const void* ptr){
    static const char HEX_DIGITS[] = "0123456789abcdef";
    const upL_t DIGITS = PTR_BITS / 4;

    upL_t x = (upL_t)ptr;

    std::string ret(DIGITS, ' ');
    for (upL_t c = DIGITS; c-- > 0;){
        ret[c] = HEX_DIGITS[x % 16];
        x /= 16;
    }

    return ret;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
