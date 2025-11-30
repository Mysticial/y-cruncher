/* ConsoleIO_WindowsVTerm.ipp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 08/05/2024
 *  Last Modified   : 08/05/2024
 * 
 */

#if _WIN32
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "Settings.h"
#include <Windows.h>
#include "PublicLibs/BasicLibs/StringTools/Unicode.h"
#include "ConsoleSink_WindowsVTerm.h"
namespace ymp{
namespace Console{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
ConsoleSink_WindowsVTerm::ConsoleSink_WindowsVTerm(StatusPrintMode mode, bool enable_colors, upL_t line_length)
    : ConsoleSink(mode, enable_colors, line_length)
    , m_handle(GetStdHandle(STD_OUTPUT_HANDLE))
    , m_blank_line('\r' + std::string(line_length, ' ') + '\r')
{
    DWORD flags;
    m_console = GetConsoleMode(m_handle, &flags);
    if (m_console){
        flags |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(m_handle, flags);
    }
}
std::unique_ptr<ConsoleSink> ConsoleSink_WindowsVTerm::clone(
    StatusPrintMode mode,
    bool enable_colors,
    upL_t line_length
) const{
    return std::make_unique<ConsoleSink_WindowsVTerm>(mode, enable_colors, line_length);
}
void ConsoleSink_WindowsVTerm::operator+=(const ColorString& str){
    if (clear_temp_on_print(str.text)){
        clear_temp_lines();
    }else{
        m_temp_lines = 0;
    }
    internal_print(str);
}
void ConsoleSink_WindowsVTerm::clear_temp_lines(){
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
void ConsoleSink_WindowsVTerm::prepare_overwrite(upL_t lines_to_replace_with){
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
void ConsoleSink_WindowsVTerm::internal_WriteFile(const std::string& str){
    const DWORD MAX_BLOCK = 4096;

    upL_t length = str.size();
    const char* data = str.c_str();

    //  Split it up into blocks of no more than MAX_BLOCK characters.
    while (length > 0){
        DWORD block = length < MAX_BLOCK ? (DWORD)length : MAX_BLOCK;
        DWORD written;
        WriteFile(m_handle, data, block, &written, nullptr);
        data += block;
        length -= block;
    }
}
void ConsoleSink_WindowsVTerm::internal_WriteConsole(const std::wstring& str){
    const DWORD MAX_BLOCK = 4096;

    upL_t length = str.size();
    const wchar_t* data = str.c_str();

    //  Split it up into blocks of no more than MAX_BLOCK characters.
    while (length > 0){
        DWORD block = length < MAX_BLOCK ? (DWORD)length : MAX_BLOCK;
        DWORD written;
        WriteConsoleW(m_handle, data, block, &written, nullptr);
        data += block;
        length -= block;
    }
}
void ConsoleSink_WindowsVTerm::internal_print(const ColorString& str){
    if (m_console){
        internal_WriteConsole(StringTools::utf8_to_wstr(
            m_enable_colors
                ? vterm_color(str.color) + str.text
                : str.text
        ));
    }else{
        internal_WriteFile(
            m_enable_colors
                ? vterm_color(str.color) + str.text
                : str.text
        );
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
