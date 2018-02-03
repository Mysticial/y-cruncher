/* ConsoleIO_Posix.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/26/2014
 * Last Modified    : 08/26/2014
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
#include "PublicLibs/BasicLibs/StringTools/ToString.h"
#include "PublicLibs/BasicLibs/StringTools/Unicode.h"
#include "Label.h"
namespace ymp{
namespace Console{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE void CompileOptions(){
    Console::println_labelm("Console IO", "Color Codes", 'G');
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
YM_NO_INLINE upL_t print(const std::string& str, char color){
    SetColor(color);
    if (fwide(stdout, 0) <= 0){
        std::cout << str;
    }else{
        std::wcout << StringTools::utf8_to_wstr(str);
    }
    fflush(stdout);
    sequence++;
    return str.size();
}
YM_NO_INLINE upL_t print(const std::wstring& str, char color){
    SetColor(color);
    if (fwide(stdout, 0) <= 0){
        std::cout << StringTools::wstr_to_utf8(str);
    }else{
        std::wcout << str;
    }
    fflush(stdout);
    sequence++;
    return str.size();
}
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE std::string scan_utf8(char color){
    if (fwide(stdin, 0) > 0){
        return StringTools::wstr_to_utf8(scan_wstr(color));
    }

    SetColor(color);
    std::string out;
    std::getline(std::cin, out);
    if (color != ' '){
        SetColor('w');
    }
    return out;
}
YM_NO_INLINE std::wstring scan_wstr(char color){
    if (fwide(stdin, 0) <= 0){
        return StringTools::utf8_to_wstr(scan_utf8(color));
    }

    SetColor(color);
    std::wstring out;
    wchar_t ch;
    while (true){
        ch = fgetwc(stdin);
        if (ch == '\n')
            break;
        out.push_back(ch);
    }
    if (color != ' '){
        SetColor('w');
    }
    return out;
}
YM_NO_INLINE void Pause(char color){
    print("Press ENTER to continue . . .", color);
    scan_utf8();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Console Colors
YM_NO_INLINE void SetColor(char color){
    if (!EnableColors || color == ' ')
        return;
    const char* color_string;
    switch (color){
        case 'R':
            color_string = "\033[01;31m";
            break;
        case 'r':
            color_string = "\033[22;31m";
            break;
        case 'Y':
            color_string = "\033[01;33m";
            break;
        case 'y':
            color_string = "\033[22;33m";
            break;
        case 'G':
            color_string = "\033[01;32m";
            break;
        case 'g':
            color_string = "\033[22;32m";
            break;
        case 'B':
            color_string = "\033[01;34m";
            break;
        case 'b':
            color_string = "\033[22;34m";
            break;
        case 'T':
            color_string = "\033[01;36m";
            break;
        case 't':
            color_string = "\033[22;36m";
            break;
        case 'P':
            color_string = "\033[01;35m";
            break;
        case 'p':
            color_string = "\033[22;35m";
            break;
        default:
            color_string = "\033[01;37m";
    }
    print(color_string);
}
YM_NO_INLINE void SetColorDefault(){
    print("\033[39;49m");
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Console Window
YM_NO_INLINE bool SetConsoleWindowSize(int width, int height){
    //  TODO
    return false;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
