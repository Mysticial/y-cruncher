/* ConsoleSink_LinuxVTerm.ipp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 08/05/2024
 *  Last Modified   : 08/05/2024
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <iostream>
#if _WIN32
#include <Windows.h>
#endif
#include "PublicLibs/BasicLibs/StringTools/Unicode.h"
#include "ConsoleSink_LinuxVTerm.h"
namespace ymp{
namespace Console{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
ConsoleSink_LinuxVTerm::ConsoleSink_LinuxVTerm(StatusPrintMode mode, bool enable_colors, upL_t line_length)
    : ConsoleSink(mode, enable_colors, line_length)
    , m_blank_line('\r' + std::string(line_length, ' ') + '\r')
{
#if _WIN32
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD flags;
    bool is_console = GetConsoleMode(handle, &flags);
    if (is_console){
        flags |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(handle, flags);
    }
#endif
}
std::unique_ptr<ConsoleSink> ConsoleSink_LinuxVTerm::clone(
    StatusPrintMode mode,
    bool enable_colors,
    upL_t line_length
) const{
    return std::make_unique<ConsoleSink_LinuxVTerm>(mode, enable_colors, line_length);
}
void ConsoleSink_LinuxVTerm::operator+=(const ColorString& str){
    if (clear_temp_on_print(str.text)){
        clear_temp_lines();
    }else{
        m_temp_lines = 0;
    }
    internal_print(str);
}
void ConsoleSink_LinuxVTerm::clear_temp_lines(){
    if (m_mode != StatusPrintMode::OVERWRITE){
        return;
    }
    while (m_temp_lines > 1){
        internal_print(m_blank_line + "\033[F");
        m_temp_lines--;
    }
    internal_print(m_blank_line);
    m_temp_lines = 0;
}
void ConsoleSink_LinuxVTerm::prepare_overwrite(upL_t lines_to_replace_with){
    if (m_mode != StatusPrintMode::OVERWRITE){
        return;
    }
    while (m_temp_lines > 1){
        if (m_temp_lines > lines_to_replace_with){
            internal_print(m_blank_line + "\033[F");
        }else{
            internal_print("\033[F");
        }
        m_temp_lines--;
    }
    internal_print("\r");
    m_temp_lines = 0;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void ConsoleSink_LinuxVTerm::internal_print(const char* str){
    internal_print(std::string(str));
}
void ConsoleSink_LinuxVTerm::internal_print(const std::string& str){
    if (fwide(stdout, 0) <= 0){
        std::cout << str;
    }else{
        std::wcout << StringTools::utf8_to_wstr(str);
    }
    fflush(stdout);
}
void ConsoleSink_LinuxVTerm::internal_print(const ColorString& str){
    internal_print(
        m_enable_colors
            ? vterm_color(str.color) + str.text
            : str.text
    );
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
