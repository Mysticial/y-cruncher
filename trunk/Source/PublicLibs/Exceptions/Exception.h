/* Exception.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/09/2017
 * Last Modified    : 04/09/2017
 * 
 */

#pragma once
#ifndef ymp_Exceptions_Exception_H
#define ymp_Exceptions_Exception_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <memory>
#include <string>
#include "PublicLibs/CompilerSettings.h"
namespace ymp{
    class DllSafeStream;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class Exception{
public:
    Exception() = default;
    virtual ~Exception() = default;

public:
    //  These are the same for all child classes.
    [[noreturn]] virtual void fire() const = 0;
    virtual const char* get_typename() const = 0;
    virtual std::unique_ptr<Exception> move_from() = 0;

public:
    virtual void print() const = 0;

protected:
    //  Serialization
    enum class SerializationPassKey{};
    template <typename ExceptionType> friend class ExceptionFactoryT;
public:
    Exception(SerializationPassKey key, const char*& stream){}
    virtual void serialize(std::string& stream) const{}
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Required Exception Fields
#define YMP_STRINGIFY(x) #x
#define YMP_TOSTRING(x) YMP_STRINGIFY(x)

#define YMP_EXCEPTION_DECLARATIONS()    \
public: \
    static const char TYPENAME[];   \
    [[noreturn]] virtual void fire() const override;    \
    virtual const char* get_typename() const override;  \
    virtual std::unique_ptr<Exception> move_from() override;    \

#define YMP_EXCEPTION_DEFINITIONS(ExceptionName)    \
    const char ExceptionName::TYPENAME[] = YMP_STRINGIFY(ExceptionName);    \
    ExceptionFactoryT<ExceptionName> ExceptionName##_Instance;  \
    [[noreturn]] void ExceptionName::fire() const{  \
        throw *this;    \
    }   \
    const char* ExceptionName::get_typename() const{    \
        return TYPENAME;    \
    }   \
    std::unique_ptr<ymp::Exception> ExceptionName::move_from(){ \
        using SelfType = typename std::remove_reference<decltype(*this)>::type; \
        return std::make_unique<SelfType>(std::move(*this));    \
    }   \

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  DLL Boundary Crossing
class ExceptionFactory{
public:
    //  Must be manually freed.
    virtual Exception* deserialize(const char*& stream) const = 0;
};
////////////////////////////////////////////////////////////////////////////////
void register_exception(const char* name, ExceptionFactory* factory);
DllSafeStream serialize_exception(const Exception& exception);
void rethrow_serialized_exception(const DllSafeStream& data);
////////////////////////////////////////////////////////////////////////////////
template <typename ExceptionType>
class ExceptionFactoryT : public ExceptionFactory{
public:
    ExceptionFactoryT(){
        register_exception(ExceptionType::TYPENAME, this);
    }
    virtual Exception* deserialize(const char*& stream) const override{
        return new ExceptionType(Exception::SerializationPassKey(), stream);
    }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
