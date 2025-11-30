/* ConfigParser.cpp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 09/28/2016
 *  Last Modified   : 11/01/2023
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <limits>
#include "PublicLibs/Exceptions/InvalidParametersException.h"
#include "PublicLibs/Exceptions/ParseException.h"
#include "PublicLibs/BasicLibs/StringTools/Unicode.h"
#include "PrivateLibs/BasicLibs/TextReader/TextReader.h"
#include "ConfigArray.h"
#include "ConfigObject.h"
#include "ConfigParser.h"
namespace ymp{
namespace ConfigInternal{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Common Parsers
enum class ObjectType{
    EMPTY,
    BOOLEAN,
    IDENTIFIER,
    STRING,
    ARRAY,
    OBJECT,
};
[[noreturn]] void throw_error(upL_t line, const std::string& msg){
    throw ParseException("Line " + std::to_string(line + 1) + ": " + msg);
}
char parse_escape_character(const char*& str, upL_t& line){
    char ch = str[1];
    switch (ch){
    case '\0':
        throw EndOfStreamException();
//        throw_error(line, "Unexpected null character or end of file.");
    case '\\':
    case '\"':
        str += 2;
        return ch;
    }
    throw_error(line, std::string("Invalid escape character: ") + ch);
}
bool handle_comment(const char*& str, upL_t& line){
    char ch = str[1];
    if (ch != '/'){
        return false;
    }
    str += 2;
    while (true){
        switch (*str){
        case '\n':
            line++;
            str++;
        case '\0':
            return true;
        }
        str++;
    }
}
void handle_newline(const char*& str, upL_t& line){
    char ch = str[1];
    if (ch != '\n'){
        return;
    }
    str += 2;
    line++;
}
bool skip_whitespace(const char*& str, upL_t& line){
    bool found_whitespace = false;
    while (true){
        switch (*str){
        case '\0':
            throw EndOfStreamException();
//            throw_error(line, "Unexpected null character or end of file.");
        case '\n':
            line++;
        case ' ':
        case '\t':
        case '\r':
            found_whitespace = true;
            str++;
            continue;
        case '/':
            if (handle_comment(str, line)){
                found_whitespace = true;
                continue;
            }else{
                return found_whitespace;
            }
        default:
            return found_whitespace;
        }
    }
}
bool is_valid_identifier_character(char ch){
    if ('a' <= ch && ch <= 'z'){
        return true;
    }
    if ('A' <= ch && ch <= 'Z'){
        return true;
    }
    if ('0' <= ch && ch <= '9'){
        return true;
    }
    switch (ch){
    case '!':
    case '#':
    case '$':
    case '%':
    case '&':
    case '\'':
    case '(':
    case ')':
    case '*':
    case '+':
    case '-':
    case '.':
    case '/':
    case '=':
    case '?':
    case '@':
    case '^':
    case '_':
    case '|':
    case '~':
        return true;
    default:
        return false;
    }
}
std::string parse_indentifier(const char*& str, upL_t& line){
    //  Parse token.
    std::string token;
    while (true){
        char ch = *str;
        switch (ch){
        case '\0':
            throw EndOfStreamException();
//            throw_error(line, "Unexpected null character or end of file.");
        case '\n':
            line++;
        case ' ':
        case '\t':
            str++;
        case ':':
        case '\"':
        case ',':
            return token;
        case '\r':
            handle_newline(str, line);
            return token;
        case '/':
            if (!handle_comment(str, line)){
                token += ch;
                str++;
            }
            continue;
        case '\\':
            ch = parse_escape_character(str, line);
            continue;
        default:
            if (is_valid_identifier_character(ch)){
                token += ch;
                str++;
            }else{
//                throw_error(line, std::string("Invalid character in identifier: ") + ch);
                return token;
            }
        }
    }
}
bool find_colon(const char*& str, upL_t& line){
    skip_whitespace(str, line);
    if (*str != ':'){
        return false;
    }
    str++;
    return true;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  ACFG Parser
ConfigValue parse_acfg_node(const char*& str, upL_t& line);
ObjectType acfg_find_next_node(const char*& str, upL_t& line){
    skip_whitespace(str, line);
    char ch = *str;
    switch (ch){
    case 'n':   return ObjectType::EMPTY;
    case 't':   return ObjectType::BOOLEAN;
    case 'f':   return ObjectType::BOOLEAN;
    case '\"':  return ObjectType::STRING;
    case '[':   return ObjectType::ARRAY;
    case '{':   return ObjectType::OBJECT;
    case ']':   throw_error(line, "Unexpected ']'.");
    case '}':   throw_error(line, "Unexpected '}'.");
    case ',':   throw_error(line, "Don't use commas to separate elements.");
    }
    return ObjectType::IDENTIFIER;
}
ConfigValue parse_acfg_null(const char*& str, upL_t& line){
    std::string token = parse_indentifier(str, line);
    if (token == "null"){
        return ConfigValue();
    }
    throw_error(line, "Expected \"null\".");
}
bool parse_acfg_boolean(const char*& str, upL_t& line){
    std::string token = parse_indentifier(str, line);
    if (token == "true"){
        return true;
    }
    if (token == "false"){
        return false;
    }
    throw_error(line, "Expected \"true\" or \"false\".");
}
siL_t parse_acfg_integer(const char*& str, upL_t& line){
    uiL_t x = 0;
    bool negative = false;
    if (*str == '-'){
        negative = true;
        str++;
    }
    while (true){
        char ch = *str;
        if ('0' <= ch && ch <= '9'){
            u64_t add = ch - '0';
            if (x > std::numeric_limits<u64_t>::max() / 10){
                throw_error(line, "Integer literal is too large.");
            }else{
                x *= 10;
            }
            if (x > std::numeric_limits<u64_t>::max() - add){
                throw_error(line, "Integer literal is too large.");
            }else{
                x += add;
            }
            str++;
            continue;
        }
        switch (*str){
        case '}':
        case ']':
            break;
        case ',':
            throw_error(line, "Don't use commas to separate elements.");
        default:
            if (!skip_whitespace(str, line)){
                throw_error(line, "Invalid integer literal.");
            }
        }
        break;
    }

    if (negative){
        if (x > (u64_t)0 - std::numeric_limits<s64_t>::min()){
            throw_error(line, "Integer literal is too large.");
        }
        return (u64_t)0 - x;
    }else{
        if (x > (u64_t)std::numeric_limits<s64_t>::max()){
            throw_error(line, "Integer literal is too large.");
        }
        return x;
    }
}
std::string parse_acfg_string(const char*& str, upL_t& line){
    str++;  //  Skip leading quote.
    std::string ret;
    while (true){
        char ch = *str;
        switch (ch){
        case '\0':
            throw EndOfStreamException();
//            throw_error(line, "Unexpected null character or end of file.");
        case '\"':
            str++;
            return ret;
        case '\\':
            ch = parse_escape_character(str, line);
            ret += ch;
            continue;
        case '\n':
            line++;
        default:
            ret += ch;
            str++;
        }
    }
}
ConfigArray parse_acfg_array(const char*& str, upL_t& line){
    ConfigArray ret;
    str++;  //  Skip leading "[".
    while (true){
        skip_whitespace(str, line);
        if (*str == ']'){
            str++;
            break;
        }
        ret += parse_acfg_node(str, line);
    }
    return ret;
}
ConfigObject parse_acfg_object(const char*& str, upL_t& line){
    ConfigObject ret;
    str++;  //  Skip leading "{".
    while (true){
        skip_whitespace(str, line);
        if (*str == '}'){
            str++;
            break;
        }
        std::string key = parse_indentifier(str, line);
        if (key.empty()){
            throw_error(line, "Expected identifier.");
        }
        if (!find_colon(str, line)){
            throw_error(line, "Expected colon after identifier.");
        }
        try{
            ret.add_value(key, parse_acfg_node(str, line));
        }catch (InvalidParametersException& x){
            throw_error(line, x.message());
        }
    }
    return ret;
}
ConfigValue parse_acfg_node(const char*& str, upL_t& line){
    ObjectType type = acfg_find_next_node(str, line);
    switch (type){
    case ObjectType::EMPTY:
        return parse_acfg_null(str, line);
    case ObjectType::BOOLEAN:
        return parse_acfg_boolean(str, line);
    case ObjectType::IDENTIFIER:
        return parse_acfg_integer(str, line);
    case ObjectType::STRING:
        return parse_acfg_string(str, line);
    case ObjectType::ARRAY:
        return parse_acfg_array(str, line);
    case ObjectType::OBJECT:
        return parse_acfg_object(str, line);
    }
    throw_error(line, "Invalid object type: " + std::to_string((int)type));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  JSON Parser
ConfigValue parse_json_node(const char*& str, upL_t& line);
ObjectType json_find_next_node(const char*& str, upL_t& line){
    skip_whitespace(str, line);
    char ch = *str;
    switch (ch){
    case 'n':   return ObjectType::EMPTY;
    case 't':   return ObjectType::BOOLEAN;
    case 'f':   return ObjectType::BOOLEAN;
    case '\"':  return ObjectType::STRING;
    case '[':   return ObjectType::ARRAY;
    case '{':   return ObjectType::OBJECT;
    case ']':   throw_error(line, "Unexpected ']'.");
    case '}':   throw_error(line, "Unexpected '}'.");
    case ',':   throw_error(line, "Unexpected ','.");
    }
    return ObjectType::IDENTIFIER;
}
ConfigValue parse_json_null(const char*& str, upL_t& line){
    return parse_acfg_null(str, line);
}
bool parse_json_boolean(const char*& str, upL_t& line){
    return parse_acfg_boolean(str, line);
}
siL_t parse_json_integer(const char*& str, upL_t& line){
    constexpr s64_t MUL10_MIN = std::numeric_limits<s64_t>::min() / 10;
    constexpr s64_t MUL10_MAX = std::numeric_limits<s64_t>::max() / 10;

    siL_t x = 0;
    bool negative = false;
    if (*str == '-'){
        negative = true;
        str++;
    }
    while (true){
        char ch = *str;
        if ('0' <= ch && ch <= '9'){
            s64_t add = ch - '0';
            if (x < MUL10_MIN || x > MUL10_MAX){
                throw_error(line, "Integer literal is too large.");
            }else{
                x *= 10;
            }
            if (x > std::numeric_limits<s64_t>::max() - add){
                throw_error(line, "Integer literal is too large.");
            }else{
                x += add;
            }
            str++;
            continue;
        }
        switch (*str){
        case '}':
        case ']':
        case ',':
            break;
        default:
            if (!skip_whitespace(str, line)){
                throw_error(line, "Invalid integer literal.");
            }
        }
        break;
    }

    return negative ? -x : x;
}
std::string parse_json_string(const char*& str, upL_t& line){
    return parse_acfg_string(str, line);
}
ConfigArray parse_json_array(const char*& str, upL_t& line){
    ConfigArray ret;
    str++;  //  Skip leading "[".
    bool comma = true;
    bool empty = true;
    while (true){
        skip_whitespace(str, line);
        if (*str == ','){
            if (comma){
                throw_error(line, "Unexpected ','.");
            }
            comma = true;
            str++;
            continue;
        }
        if (*str == ']'){
            if (comma && !empty){
                throw_error(line, "Unexpected ']'.");
            }
            str++;
            break;
        }
        if (!comma){
            throw_error(line, "Expected ',' before identifier.");
        }
        comma = false;
        empty = false;
        ret += parse_json_node(str, line);
    }
    return ret;
}
ConfigObject parse_json_object(const char*& str, upL_t& line){
    ConfigObject ret;
    str++;  //  Skip leading "{".
    bool comma = true;
    bool empty = true;
    while (true){
        skip_whitespace(str, line);
        if (*str == ','){
            if (comma){
                throw_error(line, "Unexpected ','.");
            }
            comma = true;
            str++;
            continue;
        }
        if (*str == '}'){
            if (comma && !empty){
                throw_error(line, "Unexpected '}'.");
            }
            comma = true;
            str++;
            break;
        }
        if (*str != '\"'){
            throw_error(line, "Expected object key string.");
        }
        if (!comma){
            throw_error(line, "Expected ',' before object key string.");
        }
        comma = false;
        std::string key = parse_json_string(str, line);
        if (!find_colon(str, line)){
            throw_error(line, "Expected colon after object key string.");
        }
        try{
            ret.add_value(key, parse_json_node(str, line));
            empty = false;
        }catch (InvalidParametersException&){
            throw_error(line, "Duplicate key in object.");
        }
    }
    return ret;
}
ConfigValue parse_json_node(const char*& str, upL_t& line){
    ObjectType type = json_find_next_node(str, line);
    switch (type){
    case ObjectType::EMPTY:
        return parse_json_null(str, line);
    case ObjectType::BOOLEAN:
        return parse_json_boolean(str, line);
    case ObjectType::IDENTIFIER:
        return parse_json_integer(str, line);
    case ObjectType::STRING:
        return parse_json_string(str, line);
    case ObjectType::ARRAY:
        return parse_json_array(str, line);
    case ObjectType::OBJECT:
        return parse_json_object(str, line);
    }
    throw_error(line, "Invalid object type: " + std::to_string((int)type));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Wrappers
void append_extension(std::string& filename, const std::string& extension){
    upL_t pos = filename.rfind('.');
    if (pos == std::string::npos){
        filename += '.';
        filename += extension;
    }
}
std::string get_extension(const std::string& filename){
    upL_t pos = filename.rfind('.');
    if (pos == std::string::npos){
        return "";
    }
    return filename.substr(pos + 1);
}
bool case_insensitive_equals(const std::string& x, const std::string& y){
    std::u32string u32x = StringTools::utf8_to_utf32(x);
    std::u32string u32y = StringTools::utf8_to_utf32(y);
    if (u32x.size() != u32y.size()){
        return false;
    }
    for (upL_t c = 0; c < u32x.size(); c++){
        char32_t chx = std::tolower(u32x[c]);
        char32_t chy = std::tolower(u32y[c]);
        if (chx != chy){
            return false;
        }
    }
    return true;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
ConfigValue parse_acfg_string(const char*& str){
    upL_t line = 0;
    return ConfigInternal::parse_acfg_node(str, line);
}
ConfigValue parse_acfg_string(const std::string& str){
    const char* raw = str.c_str();
    upL_t line = 0;
    return ConfigInternal::parse_acfg_node(raw, line);
}
ConfigValue parse_acfg_file(const std::string& filename){
    FileIO::TextReader reader(filename, true);
    return parse_acfg_string(reader.entire_file_to_utf8());
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
ConfigValue parse_json_string(const char*& str){
    upL_t line = 0;
    return ConfigInternal::parse_json_node(str, line);
}
ConfigValue parse_json_string(const std::string& str){
    const char* raw = str.c_str();
    upL_t line = 0;
    return ConfigInternal::parse_json_node(raw, line);
}
ConfigValue parse_json_file(const std::string& filename){
    FileIO::TextReader reader(filename, true);
    return parse_json_string(reader.entire_file_to_utf8());
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
ConfigValue parse_using_extension(const std::string& filename){
    std::string extension = get_extension(filename);
    if (extension.empty()){
        return parse_acfg_file(filename + ".cfg");
    }
    if (case_insensitive_equals(extension, "cfg")){
        return parse_acfg_file(filename);
    }
    if (case_insensitive_equals(extension, "json")){
        return parse_json_file(filename);
    }
    throw InvalidParametersException(
        "parse_using_extension()",
        "Extension must be .cfg or .json."
    );
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
