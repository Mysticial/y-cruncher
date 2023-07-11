/* Parallelizer_WindowsThreadPool.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 02/19/2017
 *  Last Modified   : 02/19/2017
 * 
 */

#pragma once
#ifndef ymp_Concurrency_Parallelizer_WindowsThreadPool_H
#define ymp_Concurrency_Parallelizer_WindowsThreadPool_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <deque>
#include <Windows.h>
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/Exceptions/Exception.h"
#include "PublicLibs/Exceptions/SystemException.h"
#include "PublicLibs/BasicLibs/Concurrency/ExceptionHolder.h"
#include "PublicLibs/BasicLibs/Concurrency/BasicParallelizer.h"
namespace ymp{
namespace BasicFrameworks{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class WinPoolWork : public ExceptionHolder{
public:
    WinPoolWork(BasicAction& action, upL_t index)
        : m_action(action)
        , m_index(index)
        , m_work(CreateThreadpoolWork(callback, this, nullptr))
    {
        if (m_work == nullptr){
            throw SystemException(
                "CreateThreadpoolWork()",
                "Unable to create thread pool work.",
                GetLastError()
            );
        }
    }
    ~WinPoolWork(){
        wait_and_ignore_exceptions();
    }

    void run(){
        SubmitThreadpoolWork(m_work);
    }
    void wait_and_ignore_exceptions(){
        if (m_work != nullptr){
            WaitForThreadpoolWorkCallbacks(m_work, FALSE);
            CloseThreadpoolWork(m_work);
        }
    }
    void wait_and_rethrow_exceptions(){
        wait_and_ignore_exceptions();
        rethrow_stored_exceptions();
    }

private:
    BasicAction& m_action;
    upL_t m_index;
    TP_WORK* m_work;

    static void NTAPI callback(PTP_CALLBACK_INSTANCE instance, void* context, PTP_WORK){
        WinPoolWork* work = (WinPoolWork*)context;
        try{
            work->m_action.run(work->m_index);
        }catch (...){
            work->store_current_exception();
        }
    }
};
class WindowsThreadPool : public BasicParallelizer{
public:
    virtual void run_in_parallel(BasicAction& a0, BasicAction& a1) override{
        WinPoolWork work(a0, 0);
        work.run();
        a1.run();
        work.wait_and_rethrow_exceptions();
    }
    virtual void run_in_parallel(BasicAction& action, upL_t si, upL_t ei) override{
        if (si >= ei){
            return;
        }

        std::deque<WinPoolWork> tasks;
//        tasks.reserve(ei - si - 1);

        //  Run everything.
        for (upL_t c = si + 1; c < ei; c++){
            tasks.emplace_back(action, c);
            tasks.back().run();
        }
        action.run(si);

        for (WinPoolWork& work : tasks){
            work.wait_and_rethrow_exceptions();
        }
    }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
