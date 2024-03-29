/* StdException.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 04/14/2017
 *  Last Modified   : 04/14/2017
 * 
 */

#pragma once
#ifndef ymp_Exceptions_StdException_H
#define ymp_Exceptions_StdException_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string>
#include "Exception.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class StdException : public Exception{
    YMP_EXCEPTION_DECLARATIONS()

public:
    YM_NO_INLINE StdException();
    YM_NO_INLINE StdException(const char* message);
    YM_NO_INLINE StdException(std::string message);

    virtual void print() const override;

    const std::string& message() const{
        return m_message;
    }

public:
    //  Serialization
    StdException(SerializationPassKey key, const char*& stream);
    virtual void serialize(std::string& stream) const override;

protected:
    std::string m_message;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
