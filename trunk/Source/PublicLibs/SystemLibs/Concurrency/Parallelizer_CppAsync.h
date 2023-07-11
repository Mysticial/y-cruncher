/* Parallelizer_CppAsync.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 02/19/2017
 *  Last Modified   : 02/19/2017
 * 
 */

#pragma once
#ifndef ymp_Concurrency_Parallelizer_CppAsync_H
#define ymp_Concurrency_Parallelizer_CppAsync_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <vector>
#include <future>
#include "PublicLibs/BasicLibs/Concurrency/ExceptionHolder.h"
#include "PublicLibs/BasicLibs/Concurrency/BasicParallelizer.h"
namespace ymp{
namespace BasicFrameworks{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class CppAsync : public BasicParallelizer{
public:
    virtual void run_in_parallel(BasicAction& a0, BasicAction& a1) override{
        ExceptionHolder exception;
        auto handle = std::async(std::launch::async, run, std::ref(exception), std::ref(a1), 0);
        run(exception, a0, 0);
        handle.get();
        exception.rethrow_stored_exceptions();
    }
    virtual void run_in_parallel(BasicAction& action, upL_t si, upL_t ei) override{
        if (si >= ei){
            return;
        }
        
        ExceptionHolder exception;
        std::vector<std::future<void>> work;
        work.reserve(ei - si - 1);

        //  Run everything.
        for (upL_t c = si + 1; c < ei; c++){
            work.emplace_back(std::async(std::launch::async, run, std::ref(exception), std::ref(action), c));
        }
        run(exception, action, si);

        //  Wait for everything.
        for (auto& unit : work){
            unit.get();
        }
        exception.rethrow_stored_exceptions();
    }

private:
    static void run(ExceptionHolder& exception, BasicAction& work, upL_t index){
        try{
            work.run(index);
        }catch (...){
            exception.store_current_exception();
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
