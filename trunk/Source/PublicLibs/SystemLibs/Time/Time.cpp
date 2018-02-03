/* Time.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 09/17/2014
 * Last Modified    : 09/17/2014
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <cmath>
#ifdef _WIN32
#include "Time_Windows.ipp"
#else
#include "Time_Posix.ipp"
#endif
#include "Time.h"
namespace ymp{
namespace Time{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE std::string now_to_filestring(){
#if _WIN32
#pragma warning(disable:4996)
#endif

    time_t t = time(0);
    struct tm* now = localtime(&t);

    std::string str;
    str += std::to_string(now->tm_year + 1900);
    str += std::string(now->tm_mon + 1 < 10 ? "0" : "") + std::to_string(now->tm_mon + 1);
    str += std::string(now->tm_mday    < 10 ? "0" : "") + std::to_string(now->tm_mday);
    str += "-";
    str += std::string(now->tm_hour    < 10 ? "0" : "") + std::to_string(now->tm_hour);
    str += std::string(now->tm_min     < 10 ? "0" : "") + std::to_string(now->tm_min);
    str += std::string(now->tm_sec     < 10 ? "0" : "") + std::to_string(now->tm_sec);
    return str;
}
YM_NO_INLINE void print_secs_hrs(double seconds, char color){
    Console::SetColor(color);
    Console::print_fixed(seconds);
    Console::print(" seconds  ( ");
    Console::print_fixed(seconds / 3600);
    Console::print(" hours )");
    if (color != ' ')
        Console::SetColor('w');
}
YM_NO_INLINE void println_secs_hrs(double seconds, char color){
    print_secs_hrs(seconds, color);
    Console::println();
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
