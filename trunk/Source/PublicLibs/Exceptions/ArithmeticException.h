/* ArithmeticException.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 04/09/2017
 *  Last Modified   : 04/09/2017
 * 
 */

#pragma once
#ifndef ymp_Exceptions_ArithmeticException_H
#define ymp_Exceptions_ArithmeticException_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/Types.h"
#include "StringException.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class ArithmeticException : public StringException{
    YMP_EXCEPTION_DECLARATIONS()

public:
    using StringException::StringException;

};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class ExponentOverflowException : public ArithmeticException{
    YMP_EXCEPTION_DECLARATIONS()

public:
    ExponentOverflowException(const char* function, siL_t value, siL_t limit);

    virtual void print() const override;

public:
    //  Serialization
    ExponentOverflowException(SerializationPassKey key, const char*& stream);
    virtual void serialize(std::string& stream) const override;

private:
    siL_t m_value;
    siL_t m_limit;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
