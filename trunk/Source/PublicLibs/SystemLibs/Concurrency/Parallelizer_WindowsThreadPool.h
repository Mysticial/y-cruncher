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
#include "PublicLibs/BasicLibs/Concurrency/Parallelizer.h"
#include "PublicLibs/BasicLibs/Concurrency/ParallelRangeEndPoints.h"
namespace ymp{
namespace BasicFrameworks{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class WinPoolWork : public ExceptionHolder{
public:
    WinPoolWork(ParallelContext& parallel_context, ParallelAction& action, upL_t index)
        : m_parallel_context(parallel_context)
        , m_action(action)
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
    ParallelContext m_parallel_context;
    ParallelAction& m_action;
    upL_t m_index;
    TP_WORK* m_work;

    static void NTAPI callback(PTP_CALLBACK_INSTANCE instance, void* context, PTP_WORK){
        WinPoolWork* work = (WinPoolWork*)context;
        try{
            work->m_action.run(work->m_parallel_context, work->m_index);
        }catch (...){
            work->store_current_exception();
        }
    }
};
class WindowsThreadPool : public Parallelizer{
public:
    virtual void run_in_parallel_2(
        ParallelContext& parallel_context, upL_t tds,
        ParallelAction& a0, ParallelAction& a1
    ) override{
        WinPoolWork work(parallel_context, a0, 0);
        work.run();
        a1.run(parallel_context);
        work.wait_and_rethrow_exceptions();
    }
    virtual void run_in_parallel_range(
        ParallelContext& parallel_context, upL_t tds,
        ParallelAction& action, upL_t si, upL_t ei
    ) override{
        if (ParallelizerTools::run_in_parallel_range_degenerate(parallel_context, tds, action, si, ei)){
            return;
        }

        //  TODO: Tolerate (ei - si) much greater than (tds).

        std::deque<WinPoolWork> tasks;
//        tasks.reserve(ei - si - 1);

        //  Run everything.
        for (upL_t c = si + 1; c < ei; c++){
            tasks.emplace_back(parallel_context, action, c);
            tasks.back().run();
        }
        action.run(parallel_context, si);

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
