/* FastWallClock_chrono.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 10/25/2023
 *  Last Modified   : 10/25/2023
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
#ifndef ymp_Time_FastWallClock_chrono_H
#define ymp_Time_FastWallClock_chrono_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <chrono>
#include "PublicLibs/Types.h"
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
        : m_ticks(std::chrono::nanoseconds((int64_t)(seconds * 1000000000)))
    {}

    double seconds() const{
        return (double)std::chrono::duration_cast<std::chrono::nanoseconds>(m_ticks).count() * (1. / 1000000000);
    }

    friend bool operator<(const FastDuration& a, const FastDuration& b){
        return a.m_ticks < b.m_ticks;
    }

    static void init_frequency(){}

private:
    FastDuration(void*, std::chrono::high_resolution_clock::duration ticks)
        : m_ticks(ticks)
    {}


private:
    std::chrono::high_resolution_clock::duration m_ticks;

    friend class FastWallClock;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class FastWallClock{
public:
    FastWallClock()
        : m_ticks(std::chrono::high_resolution_clock::time_point::min())
    {}

    static FastWallClock now(){
        FastWallClock clock;
        clock.set_to_now();
        return clock;
    }
    void set_to_now(){
        m_ticks = std::chrono::high_resolution_clock::now();
    }

    FastDuration elapsed_time() const{
        return now() - *this;
    }

    FastDuration operator-(const FastWallClock& b) const{
        return FastDuration(nullptr, m_ticks - b.m_ticks);
    }
    friend bool operator<(const FastWallClock& a, const FastWallClock& b){
        return a.m_ticks < b.m_ticks;
    }


private:
    std::chrono::high_resolution_clock::time_point m_ticks;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
