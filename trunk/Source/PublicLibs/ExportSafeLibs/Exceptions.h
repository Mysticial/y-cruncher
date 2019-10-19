/* Exceptions.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 05/31/2015
 * Last Modified    : 05/31/2015
 * 
 */

#pragma once
#ifndef ymp_ExportSafeLibs_Exceptions_H
#define ymp_ExportSafeLibs_Exceptions_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Exceptions/StdException.h"
#include "PublicLibs/Exceptions/StringException.h"
#include "Stream.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef YMP_STANDALONE
#define YMP_COLLAPSE_EXCEPTION(expression)  {expression;} return (ExceptionWrapper*)nullptr
#define YMP_EXPAND_EXCEPTION(expression)    expression
#else
#define YMP_COLLAPSE_EXCEPTION(expression)  \
    try{    \
        expression; \
    }catch (Exception& _err){   \
        return new ExceptionWrapper(_err);  \
    }catch (std::exception& _err){  \
        return new ExceptionWrapper(StdException(_err.what())); \
    }catch (...){   \
        return new ExceptionWrapper(StringException("Unknown Exception"));  \
    }   \
    return (ExceptionWrapper*)nullptr
#define YMP_EXPAND_EXCEPTION(expression){   \
        ExceptionWrapper* _exception = (expression);    \
        if (_exception != nullptr){ \
            ExceptionReleaser releaser(_exception); \
            _exception->rethrow();  \
        }   \
    }
#endif
////////////////////////////////////////////////////////////////////////////////
//  This should never be placement-newed.
struct ExceptionWrapper{
//public:
    ExceptionWrapper(const Exception& exception)
        : m_deleter(&deleter)
        , m_stream(serialize_exception(exception))
    {}
    void rethrow(){
        rethrow_serialized_exception(m_stream);
    }
    void suicide(){
        m_deleter(this);
    }

//private:
    ~ExceptionWrapper(){}
    static void deleter(ExceptionWrapper* ptr){
        delete ptr;
    }

//private:
    void (*m_deleter)(ExceptionWrapper*);
    DllSafeStream m_stream;
};
class ExceptionReleaser{
public:
    ExceptionReleaser(ExceptionWrapper* wrapper)
        : m_wrapper(wrapper)
    {}
    ~ExceptionReleaser(){
        m_wrapper->suicide();
    }
private:
    ExceptionWrapper* m_wrapper;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
