/* ParseException.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 04/11/2017
 *  Last Modified   : 04/11/2017
 * 
 */

#pragma once
#ifndef ymp_Exceptions_ParseException_H
#define ymp_Exceptions_ParseException_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/Exceptions/StringException.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class ParseException : public StringException{
    YMP_EXCEPTION_DECLARATIONS()

public:
    using StringException::StringException;

};
////////////////////////////////////////////////////////////////////////////////
class EndOfStreamException : public ParseException{
    YMP_EXCEPTION_DECLARATIONS()

public:
    EndOfStreamException();

public:
    //  Serialization
    EndOfStreamException(SerializationPassKey key, const char*& stream)
        : ParseException(key, stream)
    {}
};
////////////////////////////////////////////////////////////////////////////////
class KeyNotFoundException : public ParseException{
    YMP_EXCEPTION_DECLARATIONS()

public:
    KeyNotFoundException(std::string key)
        : ParseException(std::move(key))
    {}

public:
    //  Serialization
    KeyNotFoundException(SerializationPassKey key, const char*& stream)
        : ParseException(key, stream)
    {}
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
