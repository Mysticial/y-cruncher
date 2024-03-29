/* BufferTooSmallException.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 04/09/2017
 *  Last Modified   : 04/09/2017
 * 
 */

#pragma once
#ifndef ymp_Exceptions_BufferTooSmallException_H
#define ymp_Exceptions_BufferTooSmallException_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "Exception.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class BufferTooSmallException : public Exception{
    YMP_EXCEPTION_DECLARATIONS()

public:
    YM_NO_INLINE BufferTooSmallException(const char* function, siL_t buffer_size, uiL_t required_size);

    virtual void print() const override;

public:
    //  Serialization
    BufferTooSmallException(SerializationPassKey key, const char*& stream);
    virtual void serialize(std::string& stream) const override;

private:
    std::string m_function;
    siL_t m_buffer_size;
    uiL_t m_required_size;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
