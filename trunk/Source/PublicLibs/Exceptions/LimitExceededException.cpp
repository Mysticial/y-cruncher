/* LimitExceededException.cpp
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
#include "LimitExceededException.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//#define YMP_FATAL_ON_EXCEPTION
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YMP_EXCEPTION_DEFINITIONS(LimitExceededException)
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE LimitExceededException::LimitExceededException(uiL_t attempted)
    : m_limit(0)
    , m_attempted(attempted)
{
#ifdef YMP_FATAL_ON_EXCEPTION
    std::terminate();
#endif
}
YM_NO_INLINE LimitExceededException::LimitExceededException(const char* function, uiL_t limit, uiL_t attempted)
    : StringException(function, "")
    , m_limit(limit)
    , m_attempted(attempted)
{
#ifdef YMP_FATAL_ON_EXCEPTION
    std::terminate();
#endif
}
YM_NO_INLINE LimitExceededException::LimitExceededException(const char* function, std::string limit, std::string attempted)
    : StringException(function, "")
    , m_str_limit(std::move(limit))
    , m_str_attempted(std::move(attempted))
{}
YM_NO_INLINE LimitExceededException::LimitExceededException(
    const char* function,
    std::string message,
    uiL_t limit, uiL_t attempted
)
    : StringException(function, message)
    , m_limit(limit)
    , m_attempted(attempted)
{
#ifdef YMP_FATAL_ON_EXCEPTION
    std::terminate();
#endif
}
YM_NO_INLINE LimitExceededException::LimitExceededException(
    const char* function,
    std::string message,
    std::string limit, std::string attempted
)
    : StringException(function, message)
    , m_str_limit(std::move(limit))
    , m_str_attempted(std::move(attempted))
{
#ifdef YMP_FATAL_ON_EXCEPTION
    std::terminate();
#endif
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void LimitExceededException::print() const{
    Console::ConsoleLockScope lock;
    Console::println("\n", 'R');
    Console::println_labelc("Exception Encountered", get_typename());
    Console::println();
    Console::println("Limit Exceeded:");
    if (!m_function.empty()){
        Console::println_labelm("    Function: ", m_function);
        Console::println();
    }
    if (!m_message.empty()){
        Console::println(m_message);
    }
    Console::println();
    if (m_str_limit.empty() && m_str_attempted.empty()){
        if (m_limit == 0){
            Console::println("    Limit: Unknown");
        }else{
            Console::println_labelm_commas("    Limit: ", m_limit);
        }
        Console::println_labelm_commas("    Attempted: ", m_attempted);
    }else{
        Console::println_labelm("    Limit: ", m_str_limit);
        Console::println_labelm("    Attempted: ", m_str_attempted);
    }
    Console::println("\n");
    Console::set_color('w');
}
LimitExceededException::LimitExceededException(SerializationPassKey key, const char*& stream)
    : StringException(key, stream)
{
    ExceptionTools::parse(stream, m_limit);
    ExceptionTools::parse(stream, m_attempted);
}
void LimitExceededException::serialize(std::string& stream) const{
    StringException::serialize(stream);
    ExceptionTools::write(stream, m_limit);
    ExceptionTools::write(stream, m_attempted);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
[[noreturn]] YM_NO_INLINE void throw_LimitExceededException(const char* function, uiL_t limit, uiL_t attempted){
    throw LimitExceededException(function, limit, attempted);
}
[[noreturn]] YM_NO_INLINE void throw_LimitExceededException(const char* function, const char* message, uiL_t limit, uiL_t attempted){
    throw LimitExceededException(function, message, limit, attempted);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
