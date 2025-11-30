/* Int128_IO.cpp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 01/20/2024
 *  Last Modified   : 01/20/2024
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/BasicLibs/StringTools/ConvertInteger.h"
#include "Int128_Basic.h"
#include "Int128_IO.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
std::string to_string(UInt128 x){
    if (x == 0){
        return "0";
    }
    if ((u64_t)x == x){
        return std::to_string((u64_t)x);
    }
    char digits[40];
    int index = 40;
    digits[--index] = '\0';
    do{
        digits[--index] = x.divmod10() + '0';
    }while (x != 0);
    return digits + index;
}
std::string to_string(SInt128 x){
    if (x == 0){
        return "0";
    }
    if ((s64_t)x == x){
        return std::to_string((s64_t)x);
    }
    if (x < 0){
        return "-" + to_string((UInt128)x);
    }
    return to_string((UInt128)x);
}
std::ostream& operator<<(std::ostream& stream, UInt128 x){
    if (x == 0){
        stream << 0;
        return stream;
    }
    if ((u64_t)x == x){
        stream << (u64_t)x;
        return stream;
    }
    char digits[40];
    int index = 40;
    digits[--index] = '\0';
    do{
        digits[--index] = x.divmod10() + '0';
    }while (x != 0);
    stream << digits + index;
    return stream;
}
std::ostream& operator<<(std::ostream& stream, SInt128 x){
    if (x == 0){
        stream << 0;
        return stream;
    }
    if ((s64_t)x == x){
        stream << (s64_t)x;
        return stream;
    }
    if (x < 0){
        stream << "-" << (UInt128)-x;
        return stream;
    }
    stream << (UInt128)x;
    return stream;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
