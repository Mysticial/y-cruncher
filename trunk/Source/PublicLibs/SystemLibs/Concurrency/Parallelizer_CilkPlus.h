/* Parallelizer_CilkPlus.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 02/19/2017
 *  Last Modified   : 02/19/2017
 * 
 */

#pragma once
#ifndef ymp_Concurrency_Parallelizer_CilkPlus_H
#define ymp_Concurrency_Parallelizer_CilkPlus_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <cilk/cilk_api.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/BasicLibs/Concurrency/ExceptionHolder.h"
#include "PublicLibs/BasicLibs/Concurrency/BasicParallelizer.h"
namespace ymp{
namespace BasicFrameworks{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class CilkPlus : public BasicParallelizer{
public:
    virtual void run_in_parallel(BasicAction& a0, BasicAction& a1) override{
        ExceptionHolder exception;
        {
            _Cilk_spawn run(exception, a0, 0);
            run(exception, a1, 0);
        }
        exception.rethrow_stored_exceptions();
    }
    virtual void run_in_parallel(BasicAction& action, upL_t si, upL_t ei) override{
        ExceptionHolder exception;
        {
            #pragma cilk grainsize = 1
            _Cilk_for (upL_t c = si; c < ei; c++){
                run(exception, action, c);
            }
        }
        exception.rethrow_stored_exceptions();
    }

private:
    //  COMPILER-BUG-ICC2019: ICE if this function gets inlined.
    static YM_NO_INLINE void run(ExceptionHolder& exception, BasicAction& action, upL_t index){
        try{
            action.run(index);
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
