/* BasicIO.cpp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 08/31/2014
 *  Last Modified   : 08/31/2014
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <limits.h>
#include <cmath>
#include <atomic>
#include <mutex>
#include <set>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Exceptions/InvalidParametersException.h"
#include "PublicLibs/BasicLibs/StringTools/ToString.h"
#include "PublicLibs/BasicLibs/LargePrimitives/Int128_Basic.h"
#include "BasicIO.h"
////////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include "ConsoleIO_Windows.ipp"
#include "OutputStreams/ConsoleSink_WindowsAPI.h"
#include "OutputStreams/ConsoleSink_WindowsVTerm.h"
#else
#include "ConsoleIO_Posix.ipp"
#endif
#include "OutputStreams/ConsoleSink_LinuxVTerm.h"
////////////////////////////////////////////////////////////////////////////////
namespace ymp{
namespace Console{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Global Logger
class BatchLogger : public ConsoleLogger{
public:
    BatchLogger(){
#ifdef _WIN32
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD ignored = 0x13371337;
        if (GetConsoleMode(handle, &ignored)){
            m_console.reset(new ConsoleSink_WindowsAPI(StatusPrintMode::OVERWRITE, true, 79));
        }else{
            m_console.reset(new ConsoleSink_WindowsAPI(StatusPrintMode::NONE, true, 79));
        }
#else
        m_console.reset(new ConsoleSink_LinuxVTerm(StatusPrintMode::OVERWRITE, true, 79));
#endif
    }

public:
    virtual void flush() override{
        ConsoleLockScope lock;
        m_console->flush();
        for (ConsoleLogger* logger : m_loggers){
            logger->flush();
        }
    }

    virtual void operator+=(const ColorString& str) override{
        ConsoleLockScope lock;
        *m_console += str;
        for (ConsoleLogger* logger : m_loggers){
            *logger += str;
        }
    }
    virtual void log_input(const std::string& str) override{
        std::string line = str + "\n";
        ConsoleLockScope lock;
        for (ConsoleLogger* logger : m_loggers){
            *logger += line;
        }
    }

    virtual void clear_temp_lines() override{
        ConsoleLockScope lock;
        m_console->clear_temp_lines();
        for (ConsoleLogger* logger : m_loggers){
            logger->clear_temp_lines();
        }
    }
    virtual void print_temp_line(const ColoredLine& line) override{
        ConsoleLockScope lock;
        m_console->print_temp_line(line);
        for (ConsoleLogger* logger : m_loggers){
            logger->print_temp_line(line);
        }
    }
    virtual void print_temp_lines(const std::vector<ColoredLine>& lines) override{
        ConsoleLockScope lock;
        m_console->print_temp_lines(lines);
        for (ConsoleLogger* logger : m_loggers){
            logger->print_temp_lines(lines);
        }
    }

public:
    std::unique_ptr<ConsoleSink> m_console;
    std::set<ConsoleLogger*> m_loggers;
};
BatchLogger batch_logger;
ConsoleLogger& global_logger = batch_logger;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void set_console(const std::string& name, StatusPrintMode mode, bool enable_colors, upL_t line_length){
    if (name == "linux-vterm"){
        batch_logger.m_console = std::make_unique<ConsoleSink_LinuxVTerm>(mode, enable_colors, line_length);
        return;
    }

#ifdef _WIN32
    if (name == "winapi"){
        batch_logger.m_console = std::make_unique<ConsoleSink_WindowsAPI>(mode, enable_colors, line_length);
        return;
    }
    if (name == "win-vterm"){
        batch_logger.m_console = std::make_unique<ConsoleSink_WindowsVTerm>(mode, enable_colors, line_length);
        return;
    }
#endif

    throw InvalidParametersException("set_console()", "Invalid Backend: " + name);
}
bool get_colors_enabled(){
    ConsoleLockScope lock;
    return batch_logger.m_console->colors_enabled();
}
void set_colors_enabled(bool enabled){
    ConsoleLockScope lock;
    batch_logger.m_console = batch_logger.m_console->clone(
        batch_logger.m_console->status_line_mode(),
        enabled,
        batch_logger.m_console->line_length()
    );
}
YM_NO_INLINE StatusPrintMode get_status_line_mode(){
    ConsoleLockScope lock;
    return batch_logger.m_console->status_line_mode();
}
YM_NO_INLINE void set_status_line_mode(StatusPrintMode mode){
    ConsoleLockScope lock;
    batch_logger.m_console = batch_logger.m_console->clone(
        mode,
        batch_logger.m_console->colors_enabled(),
        batch_logger.m_console->line_length()
    );
}
void set_console_backend(const std::string& name){
    ConsoleLockScope lock;
    set_console(
        name,
        batch_logger.m_console->status_line_mode(),
        batch_logger.m_console->colors_enabled(),
        batch_logger.m_console->line_length()
    );
}
void add_console_logger(ConsoleLogger& logger){
    ConsoleLockScope lock;
    batch_logger.m_loggers.insert(&logger);
}
void remove_console_logger(ConsoleLogger& logger){
    ConsoleLockScope lock;
    batch_logger.m_loggers.erase(&logger);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE void set_color(char color){
    batch_logger += ColorString(color, "");
}
YM_NO_INLINE void set_color_default(){
    batch_logger += ColorString('*', "");
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Basic Derived
YM_NO_INLINE void flush(){
    batch_logger.flush();
}
YM_NO_INLINE std::string print(std::string str, char color){
    batch_logger += ColorString(color, str);
    return str;
}
YM_NO_INLINE std::string println(){
    print("\n");
    return "\n";
}
YM_NO_INLINE std::string println(std::string str, char color){
    str += "\n";
    print(str.c_str(), color);
    return str;
}
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE void clear_line(int characters){
    batch_logger.clear_temp_lines();
}
YM_NO_INLINE void warning(std::string str, bool sticky){
    ConsoleLockScope lock;
    println("\n" + std::move(str), 'R');
    if (!sticky){
        set_color('w');
    }
}
bool pause_on_error = true;
[[noreturn]] YM_NO_INLINE void quit_program(int code){
    ConsoleLockScope lock;
    set_color_default();
    if (pause_on_error){
        pause();
    }
    exit(code);
}
[[noreturn]] YM_NO_INLINE void critical_error(const char* function, std::string message){
    ConsoleLockScope lock;
    println("\n\nCritical Error! Program cannot continue.\n", 'R');
    println();
    println("Location: " + std::string(function));
    println();
    println(std::move(message));
    println();
    println("If your hardware is stable, please report this to the developer.");
    println();
    pause();
    exit(1);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Console Lock
std::recursive_mutex& CONSOLE_LOCK(){
    static std::recursive_mutex console_lock;
    return console_lock;
}
ConsoleLockScope::ConsoleLockScope(){
    CONSOLE_LOCK().lock();
}
ConsoleLockScope::~ConsoleLockScope(){
    CONSOLE_LOCK().unlock();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE std::string println(const ColoredLine& str){
    ConsoleLockScope lock;
    std::string ret;
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
YM_NO_INLINE std::string println(u32_t x, char color){
    return println(StringTools::tostr(x, StringTools::NORMAL), color);
}
YM_NO_INLINE std::string println(s32_t x, char color){
    return println(StringTools::tostr(x, StringTools::NORMAL), color);
}
YM_NO_INLINE std::string println(u64_t x, char color){
    return println(StringTools::tostr(x, StringTools::NORMAL), color);
}
YM_NO_INLINE std::string println(s64_t x, char color){
    return println(StringTools::tostr(x, StringTools::NORMAL), color);
}
YM_NO_INLINE std::string println(u128_t x, char color){
    return println(StringTools::tostr(x, StringTools::NORMAL), color);
}
YM_NO_INLINE std::string println(s128_t x, char color){
    return println(StringTools::tostr(x, StringTools::NORMAL), color);
}
////////////////////////////////////////////////////////////////////////////////
template <typename IntegerType>
YM_NO_INLINE IntegerType scan_int(char color){
    std::string str = scan_utf8(color);
    const char* ptr = str.c_str();
    return StringTools::fromstr<IntegerType>(ptr);
}
template u32_t scan_int(char color);
template s32_t scan_int(char color);
template u64_t scan_int(char color);
template s64_t scan_int(char color);
template u128_t scan_int(char color);
template s128_t scan_int(char color);
////////////////////////////////////////////////////////////////////////////////
template <typename IntegerType>
YM_NO_INLINE IntegerType scan_int_suffix(char color){
    std::string str = scan_utf8(color);
    const char* iter = str.c_str();

    IntegerType x = 0;
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

    if (negative){
        x = 0 - x;
    }

    constexpr size_t BITS = CHAR_BIT * sizeof(IntegerType);

    switch (ch){
    case 'K': return x << 10;   //  Kilo
    case 'M': return x << 20;   //  Mega
    case 'G': return x << 30;   //  Giga
    case 'T':   //  Tera
        if constexpr (BITS <= 40){
            return 0;
        }else{
            return x << 40;
        }
    case 'P':   //  Peta
        if constexpr (BITS <= 50){
            return 0;
        }else{
            return x << 50;
        }
    case 'E':   //  Exa
        if constexpr (BITS <= 60){
            return 0;
        }else{
            return x << 60;
        }
    case 'm': return x * 1000000;           //  Million
    case 'b': return x * 1000000000;        //  Billion
    case 't': return (IntegerType)(x * 1000000000000);      //  Trillion
    case 'q': return (IntegerType)(x * 1000000000000000);   //  Quadrillion
    };
    return x;
}
template u32_t scan_int_suffix(char color);
template s32_t scan_int_suffix(char color);
template u64_t scan_int_suffix(char color);
template s64_t scan_int_suffix(char color);
template u128_t scan_int_suffix(char color);
template s128_t scan_int_suffix(char color);
////////////////////////////////////////////////////////////////////////////////
template <typename IntegerType>
YM_NO_INLINE IntegerType scan_bytes(char color){
    std::string str = scan_utf8(color);

    IntegerType x = 0;
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

    size_t shift = 0;
    switch (ch){
    case 'k':
    case 'K':
        shift = 10;
        break;
    case 'm':
    case 'M':
        shift = 20;
        break;
    case 'g':
    case 'G':
        shift = 30;
        break;
    case 't':
    case 'T':
        shift = 40;
        break;
    case 'p':
    case 'P':
        shift = 50;
        break;
    case 'e':
    case 'E':
        shift = 60;
        break;
    };

    constexpr size_t BITS = CHAR_BIT * sizeof(IntegerType);
    if (shift >= BITS){
        x = 0;
    }else{
        x <<= shift;
    }
    x += (IntegerType)std::round(frac * ((u64_t)1 << shift));
    return x;
}
template u32_t scan_bytes(char color);
template u64_t scan_bytes(char color);
template u128_t scan_bytes(char color);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Floating-Point
YM_NO_INLINE std::string print_float(double x, int precision, char color){
    return print(StringTools::tostr_float(x, precision), color);
}
YM_NO_INLINE std::string println_float(double x, int precision, char color){
    return println(StringTools::tostr_float(x, precision), color);
}
YM_NO_INLINE std::string print_fixed(double x, int precision, char color){
    return print(StringTools::tostr_fixed(x, precision), color);
}
YM_NO_INLINE std::string println_fixed(double x, int precision, char color){
    return println(StringTools::tostr_fixed(x, precision), color);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Unit Tests
YM_NO_INLINE void print_test(bool x){
    ConsoleLockScope lock;
    if (x){
        Console::print("Passed", 'G');
    }else{
        Console::print("Failed", 'R');
    }
    Console::set_color('w');
}
YM_NO_INLINE void println_test(bool x){
    ConsoleLockScope lock;
    if (x){
        Console::println("Passed", 'G');
    }else{
        Console::println("Failed", 'R');
    }
    Console::set_color('w');
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
