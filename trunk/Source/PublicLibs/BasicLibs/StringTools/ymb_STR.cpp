/* ymb_STR.cpp - String Tools
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/07/2013
 * Last Modified    : 07/13/2014
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <cmath>
#include <vector>
#ifdef _WIN32
#include <Windows.h>
#endif
#include "PublicLibs/ConsoleIO/Label.h"
#include "ymb_STR.h"
namespace ymp{
namespace StringTools{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  String -> Integer
uiL_t parse_uL(const char* str){
    uiL_t x = 0;
    char ch = *str++;
    while (ch >= '0' && ch <= '9'){
        x *= 10;
        x += ch - '0';
        ch = *str++;
    }
    return x;
}
uiL_t parse_uL(const std::string& str){
    return parse_uL(str.c_str());
}
uiL_t parse_sL(const std::string& str){
    if (str.empty()){
        return 0;
    }
    if (str[0] == '-'){
        return -(siL_t)parse_uL(str.c_str() + 1);
    }
    return parse_uL(str);
}
uiL_t parse_uL_text(const char* str){
    uiL_t x = 0;
    char ch = *str++;
    while (ch >= '0' && ch <= '9'){
        x *= 10;
        x += ch - '0';
        ch = *str++;
    }

    //double frac = 0;
    //if (ch == '.'){
    //    double f = 0.1;

    //    ch = *str++;
    //    while (ch >= '0' && ch <= '9'){
    //        frac += f * (ch - '0');
    //        f *= 0.1;
    //        ch = *str++;
    //    }
    //}

    //while (ch == ' ')
    //    ch = *str++;

    switch (ch){
        case 'k':
        case 'K':
            return x * 1000;
        case 'm':
        case 'M':
            return x * 1000000;
        case 'b':
        case 'B':
            return x * 1000000000;
        case 't':
        case 'T':
            return x * 1000000000000;
        default:
            return x;
    };
}
uiL_t parse_uL_text(const std::string& str){
    return parse_uL_text(str.c_str());
}
siL_t parse_sL_text(const std::string& str){
    if (str.empty()){
        return 0;
    }
    if (str[0] == '-'){
        return -(siL_t)parse_uL_text(str.c_str() + 1);
    }
    return parse_uL_text(str);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Bytes
uiL_t parse_bytes(const char* str){
    uiL_t x = 0;
    char ch = *str++;
    while (ch >= '0' && ch <= '9'){
        x *= 10;
        x += ch - '0';
        ch = *str++;
    }

    double frac = 0;
    if (ch == '.'){
        double f = 0.1;

        ch = *str++;
        while (ch >= '0' && ch <= '9'){
            frac += f * (ch - '0');
            f *= 0.1;
            ch = *str++;
        }
    }

    while (ch == ' '){
        ch = *str++;
    }

    switch (ch){
        case 'k':
        case 'K':
            return (x << 10) + (uiL_t)std::ceil(frac * ((uiL_t)1 << 10));
        case 'm':
        case 'M':
            return (x << 20) + (uiL_t)std::ceil(frac * ((uiL_t)1 << 20));
        case 'g':
        case 'G':
            return (x << 30) + (uiL_t)std::ceil(frac * ((uiL_t)1 << 30));
        case 't':
        case 'T':
            return (x << 40) + (uiL_t)std::ceil(frac * ((uiL_t)1 << 40));
        case 'p':
        case 'P':
            return (x << 50) + (uiL_t)std::ceil(frac * ((uiL_t)1 << 50));
        case 'e':
        case 'E':
            return (x << 60) + (uiL_t)std::ceil(frac * ((uiL_t)1 << 60));
        default:
            return x;
    };
}
uiL_t parse_bytes(const std::string& str){
    return parse_bytes(str.c_str());
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Paths
std::string build_path(std::string base, const std::string name){
    if (!base.empty()){
        char last = base.back();
        if (last != '/' && last != '\\'){
            base += '/';
        }
    }
    base += name;
    return base;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
