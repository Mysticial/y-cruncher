/* ConsoleIO_Windows.ipp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 08/26/2014
 *  Last Modified   : 12/29/2015
 * 
 *      Use only ReadConsoleW() and WriteConsoleW(). These are the only
 *  functions that seem to work with unicode in Windows.
 * 
 *  All the normal (1 byte) char strings are assumed to be UTF-8 and will be
 *  converted to/from UTF-16 before interfacing with the Windows API.
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <atomic>
#include <Windows.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/BasicLibs/StringTools/Unicode.h"
#include "Label.h"
namespace ymp{
namespace Console{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE void compile_options(){
    Console::println_labelm("Console IO", "WinAPI", 'G');
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Core I/O
std::atomic<uiL_t> sequence(0);
uiL_t sequence_number(){
    return sequence.load(std::memory_order_acquire);
}
////////////////////////////////////////////////////////////////////////////////
void internal_print_console(const std::wstring& str){
    const DWORD MAX_BLOCK = 4096;

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

    upL_t length = str.size();
    const wchar_t* data = str.c_str();

    //  Split it up into blocks of no more than MAX_BLOCK characters.
    while (length > 0){
        DWORD block = length < MAX_BLOCK ? (DWORD)length : MAX_BLOCK;
        DWORD written;
        WriteConsoleW(handle, data, block, &written, nullptr);
        data += block;
        length -= block;
    }
}
void internal_write_file(const std::string& str){
    const DWORD MAX_BLOCK = 4096;

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

    std::string actual_print;

    //  Process the string to remove any overwritten status lines.
    static std::string buffer;
    for (size_t c = 0; c < str.size(); c++){
        char ch = str[c];
        switch (ch){
        case '\r':
            if (str[c + 1] != '\n'){
                buffer.clear();
            }
            break;

        case '\n':
            buffer += ch;
            actual_print += buffer;
            buffer.clear();
            break;

        default:
            buffer += ch;
        }
    }

    upL_t length = actual_print.size();
    const char* data = actual_print.c_str();

    //  Split it up into blocks of no more than MAX_BLOCK characters.
    while (length > 0){
        DWORD block = length < MAX_BLOCK ? (DWORD)length : MAX_BLOCK;
        DWORD written;
        WriteFile(handle, data, block, &written, nullptr);
        data += block;
        length -= block;
    }
}
YM_NO_INLINE upL_t print(const std::string& str, char color){
    std::u32string u32 = StringTools::utf8_to_utf32(str);

    ConsoleLockScope lock;

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD ignored;
    bool is_console = GetConsoleMode(handle, &ignored);

    set_color(color);
    if (is_console){
        internal_print_console(StringTools::utf32_to_wstr(u32));
    }else{
        internal_write_file(str);
    }
    sequence++;
    return u32.size();
}
YM_NO_INLINE upL_t print(const std::wstring& str, char color){
    std::u32string u32 = StringTools::wstr_to_utf32(str);

    ConsoleLockScope lock;

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD ignored;
    bool is_console = GetConsoleMode(handle, &ignored);

    set_color(color);
    if (is_console){
        internal_print_console(str);
    }else{
        internal_write_file(StringTools::utf32_to_utf8(u32));
    }
    sequence++;
    return u32.size();
}
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE std::string scan_utf8(char color){
    return StringTools::wstr_to_utf8(scan_wstr(color));
}
YM_NO_INLINE std::wstring scan_wstr(char color){
    const DWORD MAX_BLOCK = 256;

    ConsoleLockScope lock;
    set_color(color);
    HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);

    std::wstring out;

    wchar_t buffer[MAX_BLOCK];

    //  Split it up into blocks of no more than MAX_BLOCK characters.
    //  This seems to behave fine when splitting up a surrogate pair.
    bool end = false;
    while (!end){
        DWORD read;
        ReadConsoleW(handle, buffer, MAX_BLOCK, &read, nullptr);

        //  Strip off control characters.
        while (read > 0){
            wchar_t ch = buffer[read - 1];
            if ((u32_t)ch >= 32){
                break;
            }
            if (ch == 10){
                end = true;
            }
            read--;
        }

        out.append(buffer, read);
    }

    if (color != ' '){
        set_color_default();
    }

    return out;
}
YM_NO_INLINE void pause(char color){
    ConsoleLockScope lock;
    set_color(color);
    sequence++;
    system("pause");
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Console Colors
YM_NO_INLINE void set_color(char color){
    ConsoleLockScope lock;
    if (!enable_colors || color == ' '){
        return;
    }
    WORD attributes;
    switch (color){
    case 'R':
        attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
        break;
    case 'r':
        attributes = FOREGROUND_RED;
        break;
    case 'Y':
        attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        break;
    case 'y':
        attributes = FOREGROUND_RED | FOREGROUND_GREEN;
        break;
    case 'G':
        attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        break;
    case 'g':
        attributes = FOREGROUND_GREEN;
        break;
    case 'B':
        attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        break;
    case 'b':
        attributes = FOREGROUND_BLUE;
        break;
    case 'T':
        attributes = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        break;
    case 't':
        attributes = FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
    case 'P':
        attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        break;
    case 'p':
        attributes = FOREGROUND_RED | FOREGROUND_BLUE;
        break;
    default:
        attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attributes);
}
YM_NO_INLINE void set_color_default(){
    set_color('w');
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Console Window
YM_NO_INLINE bool set_console_window_size(int width, int height){
    width--;
    height--;

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (handle == INVALID_HANDLE_VALUE)
        return false;

    COORD coord = GetLargestConsoleWindowSize(handle);
    if (coord.X == 0 || coord.Y == 0){
        return false;
    }

    //  Clip
    if (width > coord.X) width = coord.X;
    if (height > coord.Y) height = coord.Y;

    SMALL_RECT rectangle;
    rectangle.Left      = 0;
    rectangle.Right     = (SHORT)width;
    rectangle.Top       = 0;
    rectangle.Bottom    = (SHORT)height;

    if (!SetConsoleWindowInfo(handle, TRUE, &rectangle)){
        return false;
    }

    return true;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
