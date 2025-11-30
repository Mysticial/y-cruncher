/* ConsoleIO_Posix.ipp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 08/26/2014
 *  Last Modified   : 08/26/2014
 * 
 *      These print functions try to be agnostic to the stream orientation.
 *  That is, they will detect the orientation of stdin and stdout and use the
 *  appropriate function calls to use them.
 * 
 *  If a stream orientation hasn't been set yet, they will be set to byte-oriented.
 *  Interestingly enough, wide-oriented streams seem to have issues with unicode
 *  glyphs in Linux. (tested on Gnome Terminal)
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string.h>
#include <atomic>
#include <iostream>
#include "PublicLibs/Exceptions/InvalidParametersException.h"
#include "PublicLibs/BasicLibs/StringTools/ToString.h"
#include "PublicLibs/BasicLibs/StringTools/Unicode.h"
#include "PublicLibs/ConsoleIO/BasicIO.h"
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
    ConsoleLockScope lock;

    flush();

    if (fwide(stdin, 0) <= 0){
        return StringTools::utf8_to_wstr(scan_utf8(color));
    }

    set_color(color);
    std::wstring out;
    wchar_t ch;
    while (true){
        ch = fgetwc(stdin);
        if (ch == '\n'){
            break;
        }
        out.push_back(ch);
    }
    if (color != ' '){
        set_color_default();
    }
    return out;
}
YM_NO_INLINE std::string scan_utf8(char color){
    ConsoleLockScope lock;

    flush();

    if (fwide(stdin, 0) > 0){
        std::string ret = StringTools::wstr_to_utf8(internal_scan_wstr(color));
        global_logger.log_input(ret);
        return ret;
    }

    set_color(color);
    std::string ret;
    std::getline(std::cin, ret);
    if (color != ' '){
        set_color_default();
    }
    global_logger.log_input(ret);
    return ret;
}
YM_NO_INLINE void pause(char color){
    ConsoleLockScope lock;
    print("Press ENTER to continue . . .", color);
    scan_utf8();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Console Window
YM_NO_INLINE bool set_console_window_size(int width, int height){
    //  TODO-Linux: Not sure if this is even possible on Linux.
    return false;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
