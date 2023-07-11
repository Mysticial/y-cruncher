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
namespace ymp{
namespace Time{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class WallClock{
    long long m_ticks;

public:
    static WallClock now();
    double operator-(const WallClock& x) const;
    double seconds_elapsed() const{
        return now() - *this;
    }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
struct PerformanceTimeDuration;
struct PerformanceTimeStamp{
    WallClock wall_clock;
    double user_clock;
    double kernel_clock;

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
