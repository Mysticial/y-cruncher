/* CommandLine.h - Command Line Helpers
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/12/2014
 * Last Modified    : 07/12/2014
 * 
 */

#pragma once
#ifndef CommandLine_H
#define CommandLine_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <vector>
#include <map>
#include <exception>
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/Exceptions/InvalidParametersException.h"
#include "PublicLibs/BasicLibs/StringTools/Unicode.h"
namespace ymp{
namespace CommandLine{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Helpers
class Parameters;
inline void UnrecognizedCommand(const char* command){
    UnrecognizedCommand(command);
}
inline void UnrecognizedCommand(const std::string& command){
    throw InvalidParametersException("", "Invalid Command: " + command);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  No Parameters
using Action    = void (*)(Parameters&);
using ActionMap = std::map<std::string, Action>;
////////////////////////////////////////////////////////////////////////////////
//  Object Parameter
template <typename T> using Method    = void (*)(T&, Parameters&);
template <typename T> using MethodMap = std::map<std::string, Method<T>>;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class CommandLineException : public std::exception{
    const char* m_msg;
public:
    CommandLineException(const char* msg)
        : m_msg(msg)
    {}
    virtual const char* what() const noexcept{
        return m_msg;
    }
};
enum class OnFailAction{
    DO_NOTHING,
    ADVANCE,
    FATAL,
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class Parameters{
private:
    using string = std::string;
    using Token = std::pair<string, string>;

    std::vector<string> m_parameters;
    std::vector<Token> m_tokens;
    size_t m_currentIndex = 1;

public:
    Parameters() = default;
    Parameters(int argc, char* argv[]){
        for (int c = 0; c < argc; c++){
            m_parameters.push_back(argv[c]);
            m_tokens.push_back(split(argv[c]));
        }
    }
    Parameters(int argc, wchar_t* argv[]){
        for (int c = 0; c < argc; c++){
            m_parameters.push_back(StringTools::wstr_to_utf8(argv[c]));
            m_tokens.push_back(split(StringTools::wstr_to_utf8(argv[c])));
        }
    }

    bool has_more() const{
        return m_currentIndex < m_parameters.size();
    }
    const string& CurrentParam() const{
        if (m_currentIndex >= m_parameters.size()){
            throw CommandLineException("Expected more parameters.");
        }
        return m_parameters[m_currentIndex];
    }
    const string& CurrentKey() const{
        if (m_currentIndex >= m_parameters.size()){
            throw CommandLineException("Expected more parameters.");
        }
        return m_tokens[m_currentIndex].first;
    }
    const string& CurrentValue() const{
        if (m_currentIndex >= m_parameters.size()){
            throw CommandLineException("Expected more parameters.");
        }
        return m_tokens[m_currentIndex].second;
    }

    void advance(){
        if (m_currentIndex >= m_parameters.size()){
            throw CommandLineException("Expected more parameters.");
        }
        m_currentIndex++;
    }

public:
    bool match_execute(const ActionMap& map, bool fatal_on_fail = true){
        auto iter = map.find(CurrentKey());
        if (iter != map.end()){
            iter->second(*this);
            return true;
        }
        if (fatal_on_fail){
            Console::Warning("Invalid Parameter: " + CurrentKey());
            Console::Quit(1);
        }
        return false;
    }
    template <typename T>
    bool match_execute(const MethodMap<T>& map, T &obj, bool fatal_on_fail = true){
        auto iter = map.find(CurrentKey());
        if (iter != map.end()){
            iter->second(obj, *this);
            return true;
        }
        if (fatal_on_fail){
            Console::Warning("Invalid Parameter: " + CurrentKey());
            Console::Quit(1);
        }
        return false;
    }

    void execute_all(const ActionMap& map){
        while (has_more()){
            match_execute(map);
        }
    }

    template <typename T>
    void execute_all(const MethodMap<T>& map, T &obj){
        while (has_more()){
            match_execute(map, obj);
        }
    }

private:
    Token split(const string& token){
        size_t pos = token.find(':');
        if (pos == string::npos)
            return Token(token, string());

        return Token(
            token.substr(0, pos),
            token.substr(pos + 1, token.size())
        );
    }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
