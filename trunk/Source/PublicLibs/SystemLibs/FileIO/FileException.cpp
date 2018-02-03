/* FileException.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/11/2017
 * Last Modified    : 04/11/2017
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/ExportSafeLibs/Stream.h"
#include "FileException.h"
namespace ymp{
namespace FileIO{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const char FileException::TYPENAME[] = "FileException";
ExceptionFactoryT<FileException> FileException_Instance;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
FileException::FileException(const char* function, std::string path, std::string message)
    : m_function(function == nullptr ? "" : function)
    , m_message(std::move(message))
    , m_path(std::move(path))
    , m_code(0)
{}
FileException::FileException(int code, const char* function, std::string path, std::string message)
    : m_function(function == nullptr ? "" : function)
    , m_message(std::move(message))
    , m_path(std::move(path))
    , m_code(code)
{}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void FileException::print() const{
    Console::println("\n", 'R');
    Console::println_labelc("Exception Encountered", get_typename());
    Console::println();
    if (!m_function.empty()){
        Console::println_labelm("In Function:", m_function);
    }
    if (m_code != 0){
        Console::println_labelm("Error Code: ", m_code);
    }
    Console::println_labelm(8, "Path: ", m_path);
    Console::println();
    Console::println(m_message);
    Console::println("\n");
    Console::SetColor('w');
}
FileException::FileException(const DllSafeStream& data){
    const char* str = (const char*)data.get() + sizeof(TYPENAME);

    m_function = str;
    str += m_function.size() + 1;

    m_message = str;
    str += m_message.size() + 1;

    m_path = str;
    str += m_path.size() + 1;

    memcpy(&m_code, str, sizeof(m_code));
}
DllSafeStream FileException::serialize() const{
    upL_t function_size = m_function.size() + 1;
    upL_t message_size = m_message.size() + 1;
    upL_t path_size = m_path.size() + 1;

    DllSafeStream ptr(sizeof(TYPENAME) + function_size + message_size + path_size + sizeof(m_code));
    char* str = (char*)ptr.get();
    memcpy(str, TYPENAME, sizeof(TYPENAME));
    str += sizeof(TYPENAME);

    memcpy(str, &m_function.front(), function_size);
    str += function_size;

    memcpy(str, &m_message.front(), message_size);
    str += message_size;

    memcpy(str, &m_path.front(), path_size);
    str += path_size;

    memcpy(str, &m_code, sizeof(m_code));

    return ptr;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
