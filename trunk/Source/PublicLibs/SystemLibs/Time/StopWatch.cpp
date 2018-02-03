/* StopWatch.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/12/2015
 * Last Modified    : 03/07/2016
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/BasicLibs/StringTools/ToString.h"
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Margin.h"
#include "PublicLibs/BasicLibs/Serialization/Serialization.h"
#include "PublicLibs/SystemLibs/Environment/Environment.h"
#include "StopWatch.h"
namespace ymp{
namespace Time{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void StopWatch::Reset(){
    m_total.reset();
    m_is_running = false;
}
void StopWatch::Start(){
    if (m_is_running){
        return;
    }
    m_last = PerformanceTimeStamp::now();
    m_is_running = true;
}
void StopWatch::Stop(){
    if (!m_is_running){
        return;
    }
    m_total += PerformanceTimeDuration::time_since(m_last);
    m_is_running = false;
}
////////////////////////////////////////////////////////////////////////////////
double StopWatch::get_wall_time() const{
    return get_current().wall_time;
}
PerformanceTimeDuration StopWatch::get_current() const{
    PerformanceTimeDuration out = m_total;
    if (m_is_running){
        out += PerformanceTimeDuration::time_since(m_last);
    }
    return out;
}
////////////////////////////////////////////////////////////////////////////////
void StopWatch::print() const{
    const upL_t DIGITS = 2;
    const upL_t MARGIN_USER = 8;
    const upL_t MARGIN_KERNEL = 6;

    PerformanceTimeDuration stats = get_current();
    double user_utilization   = 100 * stats.user_time / stats.wall_time;
    double kernel_utilization = 100 * stats.kernel_time / stats.wall_time;
    double cpu_ratio = 1. / Environment::GetLogicalProcessors();

    if (stats.user_time <= 0){
        Console::print("CPU Utilization:        ", 'w');
        Console::println("Too Fast to Measure", 'Y');
        Console::SetColor('w');

        Console::print("Multi-core Efficiency:  ", 'w');
        Console::println("Too Fast to Measure", 'Y');
        Console::println("", 'w');
        return;
    }

    Console::print("CPU Utilization:        ", 'w');
    Console::print_marginr_fixed(MARGIN_USER, user_utilization, DIGITS, 'Y');
    Console::print(" %");
    if (kernel_utilization >= 0){
        Console::print("  +  ", 'w');
        Console::print_marginr_fixed(MARGIN_KERNEL, kernel_utilization, DIGITS, 'P');
        Console::print(" % kernel overhead");
    }
    Console::println();

    Console::print("Multi-core Efficiency:  ", 'w');
    Console::print_marginr_fixed(MARGIN_USER, user_utilization * cpu_ratio, DIGITS, 'Y');
    Console::print(" %");
    if (kernel_utilization >= 0){
        Console::print("  +  ", 'w');
        Console::print_marginr_fixed(MARGIN_KERNEL, kernel_utilization * cpu_ratio, DIGITS, 'P');
        Console::print(" % kernel overhead");
    }
    Console::println();

    Console::println("", 'w');
}
std::string StopWatch::to_string() const{
    const upL_t DIGITS = 2;

    PerformanceTimeDuration stats = get_current();
    double user_utilization   = 100 * stats.user_time / stats.wall_time;
    double kernel_utilization = 100 * stats.kernel_time / stats.wall_time;
    double cpu_ratio = 1. / Environment::GetLogicalProcessors();

    std::string stream;

    if (stats.user_time <= 0){
        stream += "CPU Utilization:           Too Fast to Measure\r\n";
        stream += "Multi-core Efficiency:     Too Fast to Measure\r\n";
        return stream;
    }

    stream += "CPU Utilization:           ";
    stream += StringTools::tostr_fixed(user_utilization, DIGITS);
    stream += " %";
    if (kernel_utilization >= 0){
        stream += "  +  ";
        stream += StringTools::tostr_fixed(kernel_utilization, DIGITS);
        stream += " % kernel overhead";
    }
    stream += "\r\n";

    stream += "Multi-core Efficiency:     ";
    stream += StringTools::tostr_fixed(user_utilization * cpu_ratio, DIGITS);
    stream += " %";
    if (kernel_utilization >= 0){
        stream += "  +  ";
        stream += StringTools::tostr_fixed(kernel_utilization * cpu_ratio, DIGITS);
        stream += " % kernel overhead";
    }
    stream += "\r\n";

    return stream;
}
////////////////////////////////////////////////////////////////////////////////
void StopWatch::Serialize(std::string& stream) const{
    PerformanceTimeDuration stats = get_current();
    Serialization::write_float(stream, "total.wall_time   :", stats.wall_time);
    Serialization::write_float(stream, "total.user_time   :", stats.user_time);
    Serialization::write_float(stream, "total.kernel_time :", stats.kernel_time);
}
void StopWatch::Deserialize(const char*& stream){
    m_total.wall_time = Serialization::parse_float(stream);
    m_total.user_time = Serialization::parse_float(stream);
    m_total.kernel_time = Serialization::parse_float(stream);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
