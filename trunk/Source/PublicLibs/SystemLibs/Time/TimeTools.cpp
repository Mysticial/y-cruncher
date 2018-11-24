/* TimeTools.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 09/16/2014
 * Last Modified    : 08/24/2018
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <cmath>
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/BasicLibs/StringTools/ToString.h"
#include "TimeTools.h"
namespace ymp{
namespace Time{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE std::string string_time_smart(double seconds){
    std::string str = StringTools::tostr_fixed(seconds, 3);
    str += " seconds  ( ";

    if (std::fabs(seconds) < 7200){
        str += StringTools::tostr_fixed(seconds / 60, 3);
        str += " minutes )";
    }else if (std::fabs(seconds) < 172800){
        str += StringTools::tostr_fixed(seconds / 3600, 3);
        str += " hours )";
    }else{
        str += StringTools::tostr_fixed(seconds / 86400, 3);
        str += " days )";
    }

    return str;
}
YM_NO_INLINE void print_time_smart(double seconds, char color){
    Console::SetColor(color);
    Console::print_fixed(seconds);
    Console::print(" seconds  ( ");

    if (std::fabs(seconds) < 7200){
        Console::print_fixed(seconds / 60);
        Console::print(" minutes )");
    }else if (std::fabs(seconds) < 172800){
        Console::print_fixed(seconds / 3600);
        Console::print(" hours )");
    }else{
        Console::print_fixed(seconds / 86400);
        Console::print(" days )");
    }

    if (color != ' '){
        Console::SetColor('w');
    }
}
YM_NO_INLINE void println_time_smart(double seconds, char color){
    print_time_smart(seconds, color);
    Console::println();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
