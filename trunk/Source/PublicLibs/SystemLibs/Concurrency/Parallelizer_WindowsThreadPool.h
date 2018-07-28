/* Parallelizer_WindowsThreadPool.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/19/2017
 * Last Modified    : 02/19/2017
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
#include <vector>
#include <Windows.h>
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/Exceptions/Exception.h"
#include "PublicLibs/Exceptions/SystemException.h"
#include "PublicLibs/BasicLibs/Concurrency/BasicParallelizer.h"
namespace ymp{
namespace BasicFrameworks{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class WinPoolWork{
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
        WaitForThreadpoolWorkCallbacks(m_work, FALSE);
        CloseThreadpoolWork(m_work);
    }

    void run(){
        SubmitThreadpoolWork(m_work);
    }

private:
    BasicAction& m_action;
    upL_t m_index;
    TP_WORK* m_work;

    static void NTAPI callback(PTP_CALLBACK_INSTANCE instance, void* context, PTP_WORK){
        try{
            WinPoolWork* work = (WinPoolWork*)context;
            work->m_action.run(work->m_index);
        }catch (Exception& e){
            e.print();
            Console::Quit(1);
        }catch (std::exception& e){
            Console::Warning(e.what());
            Console::Quit(1);
        }catch (...){
            Console::Warning(EXCEPTION_THREAD_BARRIER);
            Console::Quit(1);
        }
    }
};
class WindowsThreadPool : public BasicParallelizer{
public:
    virtual void run_in_parallel(BasicAction& a0, BasicAction& a1) override{
        TP_WORK* work = CreateThreadpoolWork(basic_callback, &a0, nullptr);
        if (work == nullptr){
            throw SystemException(
                "CreateThreadpoolWork()",
                "Unable to create thread pool work.",
                GetLastError()
            );
        }

        SubmitThreadpoolWork(work);

        try{
            a1.run();
        }catch (...){
            WaitForThreadpoolWorkCallbacks(work, FALSE);
            CloseThreadpoolWork(work);
            throw;
        }

        WaitForThreadpoolWorkCallbacks(work, FALSE);
        CloseThreadpoolWork(work);
    }
    virtual void run_in_parallel(BasicAction& action, upL_t si, upL_t ei) override{
        if (si >= ei){
            return;
        }

        std::vector<WinPoolWork> work;
        work.reserve(ei - si - 1);

        //  Run everything.
        for (upL_t c = si + 1; c < ei; c++){
            work.emplace_back(action, c);
            work.back().run();
        }
        action.run(si);

        //  Waits are done implicitly on destruction.
    }

private:
    static void NTAPI basic_callback(PTP_CALLBACK_INSTANCE instance, void* context, PTP_WORK){
        try{
            BasicAction* action = (BasicAction*)context;
            action->run();
        }catch (Exception& e){
            e.print();
            Console::Quit(1);
        }catch (std::exception& e){
            Console::Warning(e.what());
            Console::Quit(1);
        }catch (...){
            Console::Warning(EXCEPTION_THREAD_BARRIER);
            Console::Quit(1);
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
