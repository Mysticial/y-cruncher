/* BasicIO.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 08/31/2014
 *  Last Modified   : 08/31/2014
 * 
 */

#pragma once
#ifndef ymp_Console_BasicIO_H
#define ymp_Console_BasicIO_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "PublicLibs/BasicLibs/StringTools/ToString.h"
#include "PublicLibs/BasicLibs/LargePrimitives/Int128.h"
#include "ColorStrings.h"
#include "OutputStream.h"
namespace ymp{
namespace Console{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const int DEFAULT_PRECISION = 6;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Global Logger
extern ConsoleLogger& global_logger;
bool            get_colors_enabled      ();
void            set_colors_enabled      (bool enabled);
StatusPrintMode get_status_line_mode    ();
void            set_status_line_mode    (StatusPrintMode mode);
void            set_console_backend     (const std::string& name);
void            add_console_logger      (ConsoleLogger& logger);    //  Must live until it is removed.
void            remove_console_logger   (ConsoleLogger& logger);
////////////////////////////////////////////////////////////////////////////////
//  Core
void            pause       (char color = ' ');
std::string     scan_utf8   (char color = 'T');
////////////////////////////////////////////////////////////////////////////////
//  Colors
void    set_color               (char color);
void    set_color_default       ();
bool    set_console_window_size (int width = 80, int height = 25);
////////////////////////////////////////////////////////////////////////////////
//  Basic Derived
void        flush       ();
std::string print       (std::string str, char color = ' ');
std::string println     ();
std::string println     (std::string str, char color = ' ');
void        clear_line  (int characters = 79);
void        warning     (std::string str, bool sticky = false);
////////////////////////////////////////////////////////////////////////////////
extern bool pause_on_error;
[[noreturn]] void quit_program(int code);
[[noreturn]] void critical_error(const char* function, std::string message);
////////////////////////////////////////////////////////////////////////////////
class ConsoleLockScope{
public:
    ConsoleLockScope();
    ~ConsoleLockScope();

    ConsoleLockScope(const ConsoleLockScope&) = delete;
    void operator=(const ConsoleLockScope&) = delete;
};
////////////////////////////////////////////////////////////////////////////////
//  Chains
std::string println         (const ColoredLine& str);
////////////////////////////////////////////////////////////////////////////////
//  Integer
template <typename IntegerType>
std::string print_int(const IntegerType& x, char color = ' '){
    return print(StringTools::tostr(x, StringTools::NORMAL), color);
}
template <typename IntegerType>
std::string println_int(const IntegerType& x, char color = ' '){
    return println(StringTools::tostr(x, StringTools::NORMAL), color);
}
template <typename IntegerType>
std::string print_commas(const IntegerType& x, char color = ' '){
    return print(StringTools::tostr(x, StringTools::COMMAS), color);
}
template <typename IntegerType>
std::string println_commas(const IntegerType& x, char color = ' '){
    return println(StringTools::tostr(x, StringTools::COMMAS), color);
}
template <typename IntegerType>
std::string print_bytes(const IntegerType& x, char color = ' '){
    return print(StringTools::tostr(x, StringTools::BYTES), color);
}
template <typename IntegerType>
std::string println_bytes(const IntegerType& x, char color = ' '){
    return println(StringTools::tostr(x, StringTools::BYTES), color);
}
template <typename IntegerType>
std::string print_ebytes(const IntegerType& x, char color = ' '){
    return print(StringTools::tostr(x, StringTools::BYTES_EXPANDED), color);
}
template <typename IntegerType>
std::string println_ebytes(const IntegerType& x, char color = ' '){
    return println(StringTools::tostr(x, StringTools::BYTES_EXPANDED), color);
}
////////////////////////////////////////////////////////////////////////////////
template <typename IntegerType>
IntegerType scan_int        (char color = 'T');
template <typename IntegerType>
IntegerType scan_int_suffix (char color = 'T');
template <typename IntegerType>
IntegerType scan_bytes      (char color = 'T');
////////////////////////////////////////////////////////////////////////////////
//  Floating Point
std::string print_float     (double x, int precision = DEFAULT_PRECISION, char color = ' ');
std::string println_float   (double x, int precision = DEFAULT_PRECISION, char color = ' ');
std::string print_fixed     (double x, int precision = 3, char color = ' ');
std::string println_fixed   (double x, int precision = 3, char color = ' ');
////////////////////////////////////////////////////////////////////////////////
//  Unit Tests
void    print_test      (bool x);
void    println_test    (bool x);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
