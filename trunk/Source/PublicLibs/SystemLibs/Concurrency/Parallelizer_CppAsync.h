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
#include "PublicLibs/BasicLibs/Concurrency/Parallelizer.h"
#include "PublicLibs/BasicLibs/Concurrency/ParallelRangeEndPoints.h"
namespace ymp{
namespace BasicFrameworks{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class CppAsync : public Parallelizer{
public:
    virtual void run_in_parallel_2(
        ParallelContext& parallel_context,
        upL_t tds, ParallelAction& a0, ParallelAction& a1
    ) override{
        ExceptionHolder exception;
        auto handle = std::async(std::launch::async, [&]{
            run(exception, parallel_context, a1, 0);
        });
        run(exception, parallel_context, a0, 0);
        handle.get();
        exception.rethrow_stored_exceptions();
    }
    virtual void run_in_parallel_range(
        ParallelContext& parallel_context, upL_t tds,
        ParallelAction& action, upL_t si, upL_t ei
    ) override{
        if (ParallelizerTools::run_in_parallel_range_degenerate(parallel_context, tds, action, si, ei)){
            return;
        }

        //  TODO: Tolerate (ei - si) much greater than (tds).

        ExceptionHolder exception;
        std::vector<std::future<void>> work;
        work.reserve(ei - si - 1);

        //  Run everything.
        for (upL_t c = si + 1; c < ei; c++){
            work.emplace_back(
                std::async(std::launch::async, [&, c]{
                    run(exception, parallel_context, action, c);
                })
            );
        }
        run(exception, parallel_context, action, si);

        //  Wait for everything.
        for (auto& unit : work){
            unit.get();
        }
        exception.rethrow_stored_exceptions();
    }

private:
    static void run(
        ExceptionHolder& exception,
        ParallelContext& parallel_context, ParallelAction& work, upL_t index
    ) noexcept{
        try{
            work.run(parallel_context, index);
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
