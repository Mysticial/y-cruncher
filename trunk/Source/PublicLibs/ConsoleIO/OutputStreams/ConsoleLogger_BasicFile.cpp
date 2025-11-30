/* ConsoleSink_BasicFile.cpp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 10/12/2024
 *  Last Modified   : 10/12/2024
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "ConsoleLogger_BasicFile.h"
namespace ymp{
namespace Console{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
ConsoleLogger_BasicFile::ConsoleLogger_BasicFile(const std::string& path)
    : m_file(0, path)
{}
void ConsoleLogger_BasicFile::flush(){
    m_file.flush();
}
void ConsoleLogger_BasicFile::operator+=(const ColorString& str){
    std::string formatted;
    bool has_newline = false;
    bool carriage_return = false;
    for (char ch : str.text){
        switch (ch){
        case '\r':
            carriage_return = true;
            continue;
        case '\n':
            if (!carriage_return){
                formatted += '\r';
            }
            break;
        }
        formatted += ch;
        carriage_return = false;
    }

    if (clear_temp_on_print(str.text)){
        clear_temp_lines();
    }else{
        m_temp_lines.clear();
    }
    m_file.write(formatted);
    if (has_newline){
        m_file.flush();
    }
}
void ConsoleLogger_BasicFile::operator+=(const ColoredLine& line){
    std::string formatted;
    bool has_newline = false;
    bool carriage_return = false;
    for (const ColorString& item : line){
        for (char ch : item.text){
            switch (ch){
            case '\r':
                carriage_return = true;
                continue;
            case '\n':
                if (!carriage_return){
                    formatted += '\r';
                }
                break;
            }
            formatted += ch;
            carriage_return = false;
        }
    }

    if (clear_temp_on_print(formatted)){
        clear_temp_lines();
    }else{
        m_temp_lines.clear();
    }
    m_file.write(formatted);
    if (has_newline){
        m_file.flush();
    }
}
void ConsoleLogger_BasicFile::clear_temp_lines(){
    m_temp_lines.clear();
}
void ConsoleLogger_BasicFile::print_temp_line(const ColoredLine& line){
    m_temp_lines.clear();
    std::string str;
    for (const ColorString& item : line){
        str += item.text;
    }
    m_temp_lines.push_back(std::move(str));
}
void ConsoleLogger_BasicFile::print_temp_lines(const std::vector<ColoredLine>& lines){
    m_temp_lines.clear();
    for (const ColoredLine& line : lines){
        std::string str;
        for (const ColorString& item : line){
            str += item.text;
        }
        m_temp_lines.push_back(std::move(str));
    }
}
bool ConsoleLogger_BasicFile::clear_temp_on_print(const std::string& text){
    //  No temp lines. Nothing to clear.
    if (m_temp_lines.empty()){
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
}
}
