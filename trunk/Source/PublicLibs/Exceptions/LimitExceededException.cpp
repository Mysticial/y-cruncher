/* LimitExceededException.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/09/2017
 * Last Modified    : 04/09/2017
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
#include "LimitExceededException.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const char LimitExceededException::TYPENAME[] = "LimitExceededException";
ExceptionFactoryT<LimitExceededException> LimitExceededException_Instance;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE LimitExceededException::LimitExceededException(uiL_t attempted)
    : m_limit(0)
    , m_attempted(attempted)
{}
YM_NO_INLINE LimitExceededException::LimitExceededException(const char* function, uiL_t limit, uiL_t attempted)
    : m_function(function)
    , m_limit(limit)
    , m_attempted(attempted)
{}
YM_NO_INLINE LimitExceededException::LimitExceededException(
    const char* function,
    std::string message,
    uiL_t limit, uiL_t attempted
)
    : m_function(function)
    , m_message(message)
    , m_limit(limit)
    , m_attempted(attempted)
{}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void LimitExceededException::print() const{
    Console::println("\n", 'R');
    Console::println_labelc("Exception Encountered", get_typename());
    Console::println();
    Console::println("Limit Exceeded:");
    if (!m_function.empty()){
        Console::println_labelm("    Function: ", m_function);
    }
    if (!m_message.empty()){
        Console::println(m_message);
    }
    Console::println();
    if (m_limit == 0){
        Console::println("    Limit: Unknown");
    }else{
        Console::println_labelm_commas("    Limit: ", m_limit);
    }
    Console::println_labelm_commas("    Attempted: ", m_attempted);
    Console::println("\n");
    Console::SetColor('w');
}
LimitExceededException::LimitExceededException(const DllSafeStream& data){
    const char* str = (const char*)data.get() + sizeof(TYPENAME);

    m_function = str;
    str += m_function.size() + 1;

    m_message = str;
    str += m_message.size() + 1;

    memcpy(&m_limit, str, sizeof(m_limit));
    str += sizeof(m_limit);

    memcpy(&m_attempted, str, sizeof(m_attempted));
}
DllSafeStream LimitExceededException::serialize() const{
    upL_t func_length = m_function.size() + 1;
    upL_t msg_length = m_message.size() + 1;
    upL_t bytes = func_length + msg_length + sizeof(m_limit) + sizeof(m_attempted);

    DllSafeStream ptr(sizeof(TYPENAME) + bytes);
    char* str = (char*)ptr.get();
    memcpy(str, TYPENAME, sizeof(TYPENAME));
    str += sizeof(TYPENAME);

    memcpy(str, m_function.c_str(), func_length);
    str += func_length;

    memcpy(str, m_message.c_str(), msg_length);
    str += func_length;

    memcpy(str, &m_limit, sizeof(m_limit));
    str += sizeof(m_limit);

    memcpy(str, &m_attempted, sizeof(m_attempted));

    return ptr;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
