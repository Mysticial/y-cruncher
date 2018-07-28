/* Parallelizer_CppAsync.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/19/2017
 * Last Modified    : 02/19/2017
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
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/Exceptions/Exception.h"
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
        auto handle = std::async(std::launch::async, run, std::ref(a1), 0);
        run(a0, 0);
        //try{
        //    a1.run();
        //}catch (...){
        //    handle.get();
        //    throw;
        //}
        handle.get();
    }
    virtual void run_in_parallel(BasicAction& action, upL_t si, upL_t ei) override{
        if (si >= ei){
            return;
        }

        std::vector<std::future<void>> work;
        work.reserve(ei - si - 1);

        //  Run everything.
        for (upL_t c = si + 1; c < ei; c++){
            work.emplace_back(std::async(std::launch::async, run, std::ref(action), c));
        }
        run(action, si);

        //  Wait for everything.
        for (auto& unit : work){
            unit.get();
        }
    }

private:
    static void run(BasicAction& work, upL_t index){
        try{
            work.run(index);
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
