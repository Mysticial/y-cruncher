/* OutputStream.cpp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 07/29/2024
 *  Last Modified   : 08/08/2024
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/Exceptions/InvalidParametersException.h"
#include "PublicLibs/BasicLibs/StringTools/Unicode.h"
#include "OutputStream.h"
namespace ymp{
namespace Console{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
std::string to_string(StatusPrintMode mode){
    switch (mode){
    case StatusPrintMode::NONE:
        return "none";
    case StatusPrintMode::OVERWRITE:
        return "overwrite";
    case StatusPrintMode::APPEND:
        return "append";
    default:
        throw InvalidParametersException("to_string()", "Invalid Status-Line Mode: " + std::to_string((int)mode));
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void ConsoleLogger::operator+=(const ColoredLine& line){
    for (const ColorString& str : line){
        *this += str;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void ConsoleSink::print_temp_line(const ColoredLine& line){
    if (m_mode == StatusPrintMode::NONE){
        return;
    }
    prepare_overwrite(1);
    upL_t tokens, line_length;
    trim_line_length_limit(
        tokens, line_length,
        line, m_line_length
    );
    for (upL_t c = 0; c < tokens; c++){
        internal_print(line[c]);
    }
    m_temp_lines++;
    if (m_mode == StatusPrintMode::APPEND){
        internal_print("\n");
    }else{
        internal_print(std::string(m_line_length - line_length, ' '));
    }
}
void ConsoleSink::print_temp_lines(const std::vector<ColoredLine>& lines){
    if (m_mode == StatusPrintMode::NONE){
        return;
    }
    prepare_overwrite(lines.size());
    bool first = true;
    for (const ColoredLine& line : lines){
        if (!first){
            internal_print("\n");
        }
        first = false;
        upL_t tokens, line_length;
        trim_line_length_limit(
            tokens, line_length,
            line, m_line_length
        );
        for (upL_t c = 0; c < tokens; c++){
            internal_print(line[c]);
        }
        m_temp_lines++;
        if (m_mode != StatusPrintMode::APPEND){
            internal_print(std::string(m_line_length - line_length, ' '));
        }
    }
    if (m_mode == StatusPrintMode::APPEND){
        internal_print("\n");
    }
}
void ConsoleSink::prepare_overwrite(upL_t lines_to_replace_with){
    clear_temp_lines();
}
bool ConsoleSink::clear_temp_on_print(const std::string& text){
    //  No temp lines. Nothing to clear.
    if (m_temp_lines == 0){
        return false;
    }

    //  No text means it's just a color change.
    if (text.empty()){
        return false;
    }

    //  Newline. Don't clear.
    if (text == "\n" || text == "\r\n"){
        return false;
    }

    return true;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void trim_line_length_limit(
    upL_t& tokens, upL_t& line_length,
    const ColoredLine& line, upL_t length_limit
){
    upL_t count = 0;
    upL_t length = 0;
    for (const ColorString& item : line){
        upL_t current = StringTools::utf8_to_utf32(item.text).size();
        length += current;
        if (length > length_limit){
            break;
        }
        count++;
        tokens = count;
        line_length = length;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
std::string vterm_color(char color){
    switch (color){
    case ' ':   return "";              //  No change
    case '*':   return "\033[39;49m";   //  Default
    case 'R':   return "\033[01;31m";
    case 'r':   return "\033[22;31m";
    case 'Y':   return "\033[01;33m";
    case 'y':   return "\033[22;33m";
    case 'G':   return "\033[01;32m";
    case 'g':   return "\033[22;32m";
    case 'B':   return "\033[01;34m";
    case 'b':   return "\033[22;34m";
    case 'T':   return "\033[01;36m";
    case 't':   return "\033[22;36m";
    case 'P':   return "\033[01;35m";
    case 'p':   return "\033[22;35m";
    default:    return "\033[01;37m";
    }
}
std::string vterm_window_size(int width, int height){
    return "\033[8;" + std::to_string(height) + ";" + std::to_string(width) + "t";
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
