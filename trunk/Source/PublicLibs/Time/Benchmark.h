/* Benchmark.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 09/16/2014
 * Last Modified    : 09/16/2014
 * 
 *      A helper class for performing iteration-based benchmarks.
 * 
 */

#pragma once
#ifndef ymp_Time_Benchmark_H
#define ymp_Time_Benchmark_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "Time.h"
namespace ymp{
namespace Time{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class IterationBenchmark{
    const char* m_units;
    uiL_t m_iterations;
    double m_seconds;
    WallClock m_start;

public:
    IterationBenchmark(const char* units, double seconds = 4.0)
        : m_units(units)
        , m_iterations(0)
        , m_seconds(seconds)
        , m_start(WallClock::Now())
    {}
    IterationBenchmark(double seconds = 4.0)
        : m_units(nullptr)
        , m_iterations(0)
        , m_seconds(seconds)
        , m_start(WallClock::Now())
    {}
    ~IterationBenchmark(){
        if (m_units != nullptr){
            Console::println_unitl_float(m_units, throughput());
        }
    }

public:
    void operator++(int){
        m_iterations++;
    }
    void operator+=(uiL_t new_iterations){
        m_iterations += new_iterations;
    }
    bool keep_going() const{
        auto elapsed = m_start.SecondsElapsed();
        return elapsed < m_seconds;
    }

public:
    double seconds_elapsed() const{
        return m_start.SecondsElapsed();
    }
    uiL_t iterations() const{
        return m_iterations;
    }
    double latency() const{
        return m_start.SecondsElapsed() / m_iterations;
    }
    double throughput() const{
        return m_iterations / m_start.SecondsElapsed();
    }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
