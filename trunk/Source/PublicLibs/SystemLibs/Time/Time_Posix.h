/* Thread_Posix.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 09/16/2014
 *  Last Modified   : 03/07/2016
 * 
 */

#pragma once
#ifndef ymp_Time_Time_Posix_H
#define ymp_Time_Time_Posix_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <sys/time.h>
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
        return (x - y) == 0;
    }
    friend bool operator!=(const WallClock& x, const WallClock& y){
        return (x - y) != 0;
    }
    friend bool operator<(const WallClock& x, const WallClock& y){
        return (x - y) < 0;
    }
    friend bool operator>(const WallClock& x, const WallClock& y){
        return (x - y) > 0;
    }
    friend bool operator<=(const WallClock& x, const WallClock& y){
        return (x - y) <= 0;
    }
    friend bool operator>=(const WallClock& x, const WallClock& y){
        return (x - y) >= 0;
    }

    double seconds_elapsed() const{
        return now() - *this;
    }

private:
    struct timeval m_time;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
struct PerformanceTimeDuration;
struct PerformanceTimeStamp{
    WallClock wall_clock;
    clock_t user_clock_ticks;
    clock_t kernel_clock_ticks;

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
