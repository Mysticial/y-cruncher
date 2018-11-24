/* ParseException.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/11/2017
 * Last Modified    : 04/11/2017
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
public:
    static const char TYPENAME[];

    using StringException::StringException;

public:
    [[noreturn]] virtual void fire() const override{
        throw *this;
    }
    virtual const char* get_typename() const override{
        return TYPENAME;
    }
    virtual Exception* clone() const override{
        return new ParseException(*this);
    }
};
////////////////////////////////////////////////////////////////////////////////
class EndOfStreamException : public ParseException{
public:
    static const char TYPENAME[];

    EndOfStreamException();

public:
    [[noreturn]] virtual void fire() const override{
        throw *this;
    }
    virtual const char* get_typename() const override{
        return TYPENAME;
    }
    virtual Exception* clone() const override{
        return new EndOfStreamException(*this);
    }

public:
    EndOfStreamException(const DllSafeStream& data)
        : ParseException(data)
    {}
};
////////////////////////////////////////////////////////////////////////////////
class KeyNotFoundException : public ParseException{
public:
    static const char TYPENAME[];

    KeyNotFoundException(std::string key)
        : ParseException(std::move(key))
    {}

public:
    [[noreturn]] virtual void fire() const override{
        throw *this;
    }
    virtual const char* get_typename() const override{
        return TYPENAME;
    }
    virtual Exception* clone() const override{
        return new KeyNotFoundException(*this);
    }

public:
    KeyNotFoundException(const DllSafeStream& data)
        : ParseException(data)
    {}
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
