/* FastWallClock.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/24/2018
 * Last Modified    : 08/24/2018
 * 
 * 
 *      This is a low-overhead clock that trades away accuracy for better
 *  performance. The main use-case is for high-frequency operations where you
 *  really don't want to call into the system clock which might go all the way
 *  into the kernel for something like the HPET.
 * 
 *  This clock will drift over time by as much as 0.1%.
 * 
 */

#pragma once
#ifndef ymp_FastWallClock_H
#define ymp_FastWallClock_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/Types.h"
#include "PublicLibs/SystemLibs/Environment/Environment.h"
namespace ymp{
namespace Time{
    class FastWallClock;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class FastDuration{
public:
    FastDuration() = default;
    FastDuration(double seconds)
        : m_ticks((s64_t)(seconds * (double)Environment::x86_rdtsc_ticks_per_sec()))
    {}

//    s64_t ticks() const{ return m_ticks; }
    double seconds() const{ return (double)m_ticks * scale(); }

    friend bool operator<(const FastDuration& a, const FastDuration& b){
        return a.m_ticks < b.m_ticks;
    }


public:
    static double scale(){
        static double secs_per_tick = 1. / (double)Environment::x86_rdtsc_ticks_per_sec();
        return secs_per_tick;
    }


private:
    FastDuration(void*, s64_t ticks)
        : m_ticks(ticks)
    {}


private:
    s64_t m_ticks;

    friend class FastWallClock;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class FastWallClock{
public:
    FastWallClock()
        : m_ticks(0)
    {}

    static FastWallClock now(){
        FastWallClock clock;
        clock.set_to_now();
        return clock;
    }
    void set_to_now(){
        m_ticks = Environment::x86_rdtsc();
    }

    FastDuration elapsed_time() const{
        return now() - *this;
    }

    FastDuration operator-(const FastWallClock& b) const{
        return FastDuration(nullptr, (s64_t)(m_ticks - b.m_ticks));
    }
    friend bool operator<(const FastWallClock& a, const FastWallClock& b){
        return a.m_ticks < b.m_ticks;
    }


private:
    u64_t m_ticks;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
