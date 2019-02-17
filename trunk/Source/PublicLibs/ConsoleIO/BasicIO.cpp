/* BasicIO.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/31/2014
 * Last Modified    : 08/31/2014
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <cmath>
#include <mutex>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/BasicLibs/StringTools/ToString.h"
#include "BasicIO.h"
////////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include "ConsoleIO_Windows.ipp"
#else
#include "ConsoleIO_Posix.ipp"
#endif
////////////////////////////////////////////////////////////////////////////////
namespace ymp{
namespace Console{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool EnableColors = true;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Basic Derived
YM_NO_INLINE upL_t println(){
    return print("\n");
}
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE upL_t println(std::string str, char color){
    str += '\n';
    return print(str.c_str(), color);
}
YM_NO_INLINE upL_t println(std::wstring str, char color){
    str += '\n';
    return print(str.c_str(), color);
}
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE void ClearLine(int characters){
    std::string x;
    x += '\r';
    x.resize(characters + 1, ' ');
    x += '\r';
    print(x);
}
YM_NO_INLINE void Warning(std::string str, bool sticky){
    println();
    println(std::move(str), 'R');
    if (!sticky){
        SetColor('w');
    }
}
bool pause_on_error = true;
[[noreturn]] YM_NO_INLINE void Quit(int code){
    SetColorDefault();
    if (pause_on_error){
        Pause();
    }
    exit(code);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Console Lock
std::recursive_mutex console_lock;
ConsoleLock::ConsoleLock(){
    console_lock.lock();
}
ConsoleLock::~ConsoleLock(){
    console_lock.unlock();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE upL_t println(const ColoredLine& str){
    upL_t ret = 0;
    for (const ColorString& x : str){
        ret += print(x.text, x.color);
    }
    ret += print("\n", 'w');
    return ret;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Integer
YM_NO_INLINE upL_t print(uiL_t x, char color){
    return print(StringTools::tostr(x, StringTools::NORMAL), color);
}
YM_NO_INLINE upL_t print(siL_t x, char color){
    return print(StringTools::tostr(x, StringTools::NORMAL), color);
}
YM_NO_INLINE upL_t println(uiL_t x, char color){
    return println(StringTools::tostr(x, StringTools::NORMAL), color);
}
YM_NO_INLINE upL_t println(siL_t x, char color){
    return println(StringTools::tostr(x, StringTools::NORMAL), color);
}
YM_NO_INLINE upL_t print_commas(uiL_t x, char color){
    return print(StringTools::tostr(x, StringTools::COMMAS), color);
}
YM_NO_INLINE upL_t print_commas(siL_t x, char color){
    return print(StringTools::tostr(x, StringTools::COMMAS), color);
}
YM_NO_INLINE upL_t println_commas(uiL_t x, char color){
    return println(StringTools::tostr(x, StringTools::COMMAS), color);
}
YM_NO_INLINE upL_t println_commas(siL_t x, char color){
    return println(StringTools::tostr(x, StringTools::COMMAS), color);
}
YM_NO_INLINE upL_t print_bytes(uiL_t x, char color){
    return print(StringTools::tostr(x, StringTools::BYTES), color);
}
YM_NO_INLINE upL_t println_bytes(uiL_t x, char color){
    return println(StringTools::tostr(x, StringTools::BYTES), color);
}
YM_NO_INLINE upL_t print_ebytes(uiL_t x, char color){
    return print(StringTools::tostr(x, StringTools::BYTES_EXPANDED), color);
}
YM_NO_INLINE upL_t println_ebytes(uiL_t x, char color){
    return println(StringTools::tostr(x, StringTools::BYTES_EXPANDED), color);
}
YM_NO_INLINE siL_t scan_siL(char color){
    auto str = scan_utf8(color);
    const char* iter = str.c_str();

    siL_t out = 0;
    bool negative = false;

    if (iter[0] == '-'){
        negative = true;
        iter++;
    }

    do{
        char ch = *iter++;
        if ('0' > ch || ch > '9')
            break;
    
        out *= 10;
        out += ch - '0';
    }while (true);

    if (negative)
        out = -out;
    return out;
}
YM_NO_INLINE siL_t scan_siL_suffix(char color){
    auto str = scan_utf8(color);
    const char* iter = str.c_str();

    siL_t x = 0;
    bool negative = false;

    if (iter[0] == '-'){
        negative = true;
        iter++;
    }

    char ch;
    do{
        ch = *iter++;

        //  Skip commas
        if (ch == ','){
            continue;
        }

        //  Stop when we hit a non-digit.
        if ('0' > ch || ch > '9'){
            break;
        }

        x *= 10;
        x += ch - '0';
    }while (true);

    //  Skip spaces.
    while (ch == ' '){
        ch = *iter++;
    }

    if (negative)
        x = -x;

    switch (ch){
    case 'K': return x << 10;   //  Kilo
    case 'M': return x << 20;   //  Mega
    case 'G': return x << 30;   //  Giga
    case 'T': return x << 40;   //  Tera
    case 'P': return x << 50;   //  Peta
    case 'E': return x << 60;   //  Exa
    case 'm': return x * 1000000;           //  Million
    case 'b': return x * 1000000000;        //  Billion
    case 't': return x * 1000000000000;     //  Trillion
    };
    return x;
}
YM_NO_INLINE uiL_t scan_bytes(char color){
    auto str = scan_utf8(color);

    uiL_t x = 0;
    upL_t c = 0;
    char ch = str[c++];
    while (ch >= '0' && ch <= '9'){
        x *= 10;
        x += ch - '0';
        ch = str[c++];
    }

    double frac = 0;
    if (ch == '.'){
        double f = 0.1;

        ch = str[c++];
        while (ch >= '0' && ch <= '9'){
            frac += f * (ch - '0');
            f *= 0.1;
            ch = str[c++];
        }
    }

    //  Skip spaces.
    while (ch == ' '){
        ch = str[c++];
    }

    switch (ch){
    case 'k':
    case 'K':
        return (x << 10) + (uiL_t)std::round(frac * ((uiL_t)1 << 10));
    case 'm':
    case 'M':
        return (x << 20) + (uiL_t)std::round(frac * ((uiL_t)1 << 20));
    case 'g':
    case 'G':
        return (x << 30) + (uiL_t)std::round(frac * ((uiL_t)1 << 30));
    case 't':
    case 'T':
        return (x << 40) + (uiL_t)std::round(frac * ((uiL_t)1 << 40));
    case 'p':
    case 'P':
        return (x << 50) + (uiL_t)std::round(frac * ((uiL_t)1 << 50));
    case 'e':
    case 'E':
        return (x << 60) + (uiL_t)std::round(frac * ((uiL_t)1 << 60));
    default:
        return x;
    };
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Floating-Point
YM_NO_INLINE upL_t print_float(double x, int precision, char color){
    return print(StringTools::tostr_float(x, precision), color);
}
YM_NO_INLINE upL_t println_float(double x, int precision, char color){
    return println(StringTools::tostr_float(x, precision), color);
}
YM_NO_INLINE upL_t print_fixed(double x, int precision, char color){
    return print(StringTools::tostr_fixed(x, precision), color);
}
YM_NO_INLINE upL_t println_fixed(double x, int precision, char color){
    return println(StringTools::tostr_fixed(x, precision), color);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Unit Tests
YM_NO_INLINE void print_test(bool x){
    if (x){
        Console::print("Passed", 'G');
    }else{
        Console::print("Failed", 'R');
    }
    Console::SetColor('w');
}
YM_NO_INLINE void println_test(bool x){
    if (x){
        Console::println("Passed", 'G');
    }else{
        Console::println("Failed", 'R');
    }
    Console::SetColor('w');
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
