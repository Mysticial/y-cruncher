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
#include <Windows.h>
#include "PublicLibs/BasicLibs/StringTools/Unicode.h"
#include "ConsoleSink_WindowsAPI.h"
namespace ymp{
namespace Console{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
ConsoleSink_WindowsAPI::ConsoleSink_WindowsAPI(StatusPrintMode mode, bool enable_colors, upL_t line_length)
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
std::unique_ptr<ConsoleSink> ConsoleSink_WindowsAPI::clone(
    StatusPrintMode mode,
    bool enable_colors,
    upL_t line_length
) const{
    return std::make_unique<ConsoleSink_WindowsAPI>(mode, enable_colors, line_length);
}
void ConsoleSink_WindowsAPI::operator+=(const ColorString& str){
    if (clear_temp_on_print(str.text)){
        clear_temp_lines();
    }else{
        m_temp_lines = 0;
    }
    internal_print(str);
}
void ConsoleSink_WindowsAPI::clear_temp_lines(){
    if (m_mode != StatusPrintMode::OVERWRITE){
        return;
    }
    while (m_temp_lines > 1){
        internal_print(m_blank_line);

        CONSOLE_SCREEN_BUFFER_INFO info;
        if (!GetConsoleScreenBufferInfo(m_handle, &info)){
            return;
        }
        COORD coord{0, std::max<SHORT>(info.dwCursorPosition.Y - 1, 0)};
        SetConsoleCursorPosition(m_handle, coord);

        m_temp_lines--;
    }
    internal_print(m_blank_line);
    m_temp_lines = 0;
}
void ConsoleSink_WindowsAPI::prepare_overwrite(upL_t lines_to_replace_with){
    if (m_mode != StatusPrintMode::OVERWRITE){
        return;
    }
    while (m_temp_lines > 1){
        if (m_temp_lines > lines_to_replace_with){
            internal_print(m_blank_line);
        }

        CONSOLE_SCREEN_BUFFER_INFO info;
        if (!GetConsoleScreenBufferInfo(m_handle, &info)){
            return;
        }
        COORD coord{0, std::max<SHORT>(info.dwCursorPosition.Y - 1, 0)};
        SetConsoleCursorPosition(m_handle, coord);

        m_temp_lines--;
    }
    internal_print("\r");
    m_temp_lines = 0;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void ConsoleSink_WindowsAPI::internal_WriteFile(const std::string& str){
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
void ConsoleSink_WindowsAPI::internal_WriteConsole(const std::wstring& str){
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
void ConsoleSink_WindowsAPI::internal_print(const ColorString& str){
    if (m_enable_colors && str.color != ' '){
        WORD attributes;
        switch (str.color){
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

    if (m_console){
        internal_WriteConsole(StringTools::utf8_to_wstr(str.text));
    }else{
        internal_WriteFile(str.text);
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
