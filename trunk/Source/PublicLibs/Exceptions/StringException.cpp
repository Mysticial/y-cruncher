/* StringException.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 04/09/2017
 *  Last Modified   : 04/09/2017
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
#include "ExceptionSerialization.h"
#include "StringException.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef YMP_BUILD_DEVELOPER
//#define YMP_PRINT_STRING_EXCEPTIONS
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YMP_EXCEPTION_DEFINITIONS(StringException)
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE StringException::StringException(){}
YM_NO_INLINE StringException::StringException(const char* message)
    : m_function("")
    , m_message(message)
{
#ifdef YMP_PRINT_STRING_EXCEPTIONS
    StringException::print();
#endif
}
YM_NO_INLINE StringException::StringException(std::string message)
    : m_function("")
    , m_message(std::move(message))
{
#ifdef YMP_PRINT_STRING_EXCEPTIONS
    StringException::print();
#endif
}
YM_NO_INLINE StringException::StringException(const char* function, const char* message)
    : m_function(function)
    , m_message(message)
{
#ifdef YMP_PRINT_STRING_EXCEPTIONS
    StringException::print();
#endif
}
YM_NO_INLINE StringException::StringException(const char* function, std::string message)
    : m_function(function)
    , m_message(std::move(message))
{
#ifdef YMP_PRINT_STRING_EXCEPTIONS
    StringException::print();
#endif
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void StringException::print() const{
    Console::ConsoleLockScope lock;
    Console::println("\n", 'R');
    Console::println_labelc("Exception Encountered", get_typename());
    Console::println();
    if (!m_function.empty()){
        Console::println_labelc("In Function", m_function);
        Console::println();
    }
    Console::println(m_message);
    Console::println("\n");
    Console::set_color('w');
}
StringException::StringException(SerializationPassKey key, const char*& stream)
    : Exception(key, stream)
{
    ExceptionTools::parse(stream, m_function);
    ExceptionTools::parse(stream, m_message);
}
void StringException::serialize(std::string& stream) const{
    Exception::serialize(stream);
    ExceptionTools::write(stream, m_function);
    ExceptionTools::write(stream, m_message);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
