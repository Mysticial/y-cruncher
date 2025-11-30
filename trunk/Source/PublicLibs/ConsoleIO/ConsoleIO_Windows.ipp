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
#include "BasicIO.h"
#include "Label.h"
#include "OutputStream.h"
namespace ymp{
namespace Console{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Core I/O
YM_NO_INLINE std::wstring internal_scan_wstr(char color){
    const DWORD MAX_BLOCK = 256;

    ConsoleLockScope lock;

    flush();

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
YM_NO_INLINE std::string scan_utf8(char color){
    std::string str = StringTools::wstr_to_utf8(internal_scan_wstr(color));
    global_logger.log_input(str);
    return str;
}
YM_NO_INLINE void pause(char color){
    ConsoleLockScope lock;
    set_color(color);
    system("pause");
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Console Window
YM_NO_INLINE bool set_console_window_size(int width, int height){
#if 0
    print("\033[8;" + std::to_string(height) + ";" + std::to_string(width) + "t");
#else
    width--;
    height--;

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (handle == INVALID_HANDLE_VALUE){
        return false;
    }

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

#endif
    return true;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
