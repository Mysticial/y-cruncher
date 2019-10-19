/* Exception.cpp
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
#include <memory>
#include <string>
#include <map>
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/ExportSafeLibs/Stream.h"
#include "PublicLibs/Exceptions/ExceptionSerialization.h"
#include "Exception.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  UnknownException is special and doesn't follow the pattern of all other
//  exceptions. It can never be thrown and is only generated as a placeholder
//  when deserializing an exception that isn't known to this module.
//  There is no TYPENAME or exception factory for UnknownException.
class UnknownException : public Exception{
public:
    using SerializationPassKey = Exception::SerializationPassKey;

    [[noreturn]] virtual void fire() const override{
        throw *this;
    }
    virtual const char* get_typename() const{
        return (const char*)m_data.get();
    }
    virtual std::unique_ptr<Exception> move_from() override{
        return std::make_unique<UnknownException>(std::move(*this));
    }
    virtual void print() const{
        Console::println("\n", 'R');
        Console::println_labelc("Exception Encountered", get_typename());
        Console::println("\nException type unknown to this module. Unable to display.\n");
        Console::SetColor('w');
    }

public:
    //  Serialization

    //  Even if the exception is unknown to this module, the data is preserved
    //  so that if it propagates back to a module that is aware of it, it can
    //  still reconstruct the original exception.

    UnknownException(SerializationPassKey key, const char*& stream, upL_t bytes)
        : Exception(key, stream)
        , m_data(bytes)
    {
        memcpy(m_data.get(), stream, bytes);
    }
    virtual void serialize(std::string& stream) const override{
        stream.append((const char*)m_data.get(), m_data.size());
    }

private:
    DllSafeStream m_data;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
std::map<std::string, ExceptionFactory*>& exception_map(){
    static std::map<std::string, ExceptionFactory*> map;
    return map;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void register_exception(const char* name, ExceptionFactory* factory){
    std::string str = name;
    std::map<std::string, ExceptionFactory*>& map = exception_map();
    auto iter = map.find(str);
    if (iter != map.end()){
        Console::Warning("Duplicate Exception Type");
        Console::Quit(1);
    }
    map[str] = factory;
}
DllSafeStream serialize_exception(const Exception& exception){
    std::string stream;
    ExceptionTools::write(stream, exception.get_typename());
    exception.serialize(stream);
    DllSafeStream ret(stream.size());
    memcpy(ret.get(), stream.c_str(), stream.size());
    return ret;
}
void rethrow_serialized_exception(const DllSafeStream& data){
    const char* stream = (const char*)data.get();

    const char* name;
    ExceptionTools::parse(stream, name);

    upL_t bytes = stream - (const char*)data.get();
    std::unique_ptr<Exception> exception;

    std::map<std::string, ExceptionFactory*>& map = exception_map();
    auto iter = map.find(name);
    if (iter != map.end()){
        exception.reset(iter->second->deserialize(stream));
    }else{
        exception.reset(new UnknownException(UnknownException::SerializationPassKey(), stream, bytes));
    }

    exception->fire();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
