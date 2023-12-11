/* ExceptionHolder.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 11/09/2021
 *  Last Modified   : 11/09/2021
 * 
 */

#pragma once
#ifndef ymp_Concurrency_ExceptionHolder_H
#define ymp_Concurrency_ExceptionHolder_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <atomic>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class ExceptionHolder{
    ExceptionHolder(const ExceptionHolder&) = delete;
    void operator=(const ExceptionHolder&) = delete;

public:
    YM_FORCE_INLINE ExceptionHolder()
        : m_pending_exception(nullptr)
    {}
    YM_FORCE_INLINE ~ExceptionHolder(){
        if (!m_exceptions_handled){
            throw_not_handled();
        }
        void* pending_exception = m_pending_exception.load(std::memory_order_relaxed);
        if (pending_exception){
            free(pending_exception);
        }
    }

public:
    YM_FORCE_INLINE bool has_exception() const{
        return m_pending_exception.load(std::memory_order_relaxed) != nullptr;
    }
    YM_FORCE_INLINE void rethrow_stored_exceptions(){
        m_exceptions_handled = true;
        void* pending_exception = m_pending_exception.load(std::memory_order_relaxed);
        if (pending_exception){
            rethrow(pending_exception);
        }
    }
    YM_NO_INLINE void store_current_exception();

private:
    YM_NO_INLINE void throw_not_handled();
    YM_NO_INLINE void set_exception(void* exception);
    static YM_NO_INLINE void free(void* pending_exception);
    static YM_NO_INLINE void rethrow(void* pending_exception);

private:
    bool m_exceptions_handled = false;
    std::atomic<void*> m_pending_exception;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
