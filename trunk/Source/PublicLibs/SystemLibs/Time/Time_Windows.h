/* Thread_Windows.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 09/15/2014
 *  Last Modified   : 03/07/2016
 * 
 */

#pragma once
#ifndef ymp_Time_Time_Windows_H
#define ymp_Time_Time_Windows_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/Types.h"
namespace ymp{
namespace Time{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class WallClock{
public:
    WallClock() = default;

    static WallClock now();
    static WallClock min();
    static WallClock max();

    double operator-(const WallClock& x) const;

    friend bool operator==(const WallClock& x, const WallClock& y){
        return x.m_ticks == y.m_ticks;
    }
    friend bool operator!=(const WallClock& x, const WallClock& y){
        return x.m_ticks == y.m_ticks;
    }
    friend bool operator<(const WallClock& x, const WallClock& y){
        return x.m_ticks < y.m_ticks;
    }
    friend bool operator>(const WallClock& x, const WallClock& y){
        return x.m_ticks > y.m_ticks;
    }
    friend bool operator<=(const WallClock& x, const WallClock& y){
        return x.m_ticks <= y.m_ticks;
    }
    friend bool operator>=(const WallClock& x, const WallClock& y){
        return x.m_ticks >= y.m_ticks;
    }

    double seconds_elapsed() const{
        return now() - *this;
    }

private:
    WallClock(long long x) : m_ticks(x) {}

    long long m_ticks;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
struct PerformanceTimeDuration;
struct PerformanceTimeStamp{
    WallClock wall_clock;
    u64_t user_clock_ticks;
    u64_t kernel_clock_ticks;

    static PerformanceTimeStamp now();
};
struct PerformanceTimeDuration{
    double wall_time = 0;
    double user_time = 0;
    double kernel_time = 0;

    void reset();
    static PerformanceTimeDuration time_since(const PerformanceTimeStamp& timestamp);
    void operator+=(const PerformanceTimeDuration& duration);
};
PerformanceTimeDuration operator-(const PerformanceTimeStamp& end, const PerformanceTimeStamp& start);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
