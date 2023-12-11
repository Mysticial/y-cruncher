/* Environment_Linux.ipp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 01/04/2015
 *  Last Modified   : 01/04/2015
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <algorithm>
#include <chrono>
#include <fstream>

//  There are no header guards in cpuid.h.
#ifndef cpuid_H
#define cpuid_H
#include <cpuid.h>
#endif

#include <unistd.h>
#include "PublicLibs/BasicLibs/StringTools/Unicode.h"
#include "PublicLibs/ConsoleIO/BasicIO.h"
//#include "PublicLibs/SystemLibs/Time/Time.h"
#include "Environment.h"
namespace ymp{
namespace Environment{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> parameters;
std::string process_path;
std::string process_directory;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void initialize_process_path(){
    //  This is really stupid. There's no way to get the size of the path.

    upL_t buffer_size = 1024;
    process_path.clear();
    ssize_t bytes;
    do{
        buffer_size *= 2;
        process_path.resize(buffer_size);
        bytes = readlink("/proc/self/exe", &process_path[0], buffer_size);
        if (bytes == -1){
            Console::warning("Failed to retrieve path.");
            Console::quit_program(1);
        }
    }while ((upL_t)bytes == buffer_size);
    process_path = process_path.c_str();
}
void initialize_process_directory(){
    process_directory = process_path;

    //  Strip off filename.
    while (!process_directory.empty()){
        char ch = process_directory.back();
        if (ch == '/' || ch == '\\'){
            break;
        }
        process_directory.pop_back();
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void initialize_environment(int argc, char* argva[], wchar_t* argvw[]){
    parameters.clear();
    if (argva != nullptr){
        for (int c = 0; c < argc; c++){
            parameters.emplace_back(argva[c]);
        }
    }else{
        for (int c = 0; c < argc; c++){
            parameters.emplace_back(StringTools::wstr_to_utf8(argvw[c]));
        }
    }

    initialize_process_path();
    initialize_process_directory();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void initialize_environment(int argc, char* argv[]){
    initialize_environment(argc, argv, nullptr);
}
void initialize_environment(int argc, wchar_t* argv[]){
    initialize_environment(argc, nullptr, argv);
}
const std::vector<std::string>& get_cmd_parameters(){
    return parameters;
}
const std::string& get_process_path(){
    return process_path;
}
const std::string& get_process_directory(){
    return process_directory;
}
bool RunFromConsole(){
    return true;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
upL_t get_logical_processors(){
    return sysconf(_SC_NPROCESSORS_ONLN);
}
upL_t get_free_physical_memory(){
    uiL_t bytes = 0;

#if 1
    const std::string TOKEN = "MemAvailable:";

    std::ifstream file("/proc/meminfo");
    std::string line;
    while (std::getline(file, line)){
        size_t pos = line.find(TOKEN);
        if (pos == std::string::npos){
            continue;
        }

        //  Skip the token.
        pos += TOKEN.size();

        //  Skip the white space.
        while (line[pos] != '\0' && line[pos] == ' '){
            pos++;
        }

        //  Parse the integer.
        while (true){
            char ch = line[pos++];
            if (ch < '0' || ch > '9'){
                break;
            }
            bytes *= 10;
            bytes += ch - '0';
        }
        bytes *= 1024;

        break;
    }
#endif

    if (bytes == 0){
        bytes = (uiL_t)sysconf(_SC_AVPHYS_PAGES) * sysconf(_SC_PAGE_SIZE);
    }
    bytes = std::min(bytes, (uiL_t)MAX_MEMORY);
    return static_cast<upL_t>(bytes);
}
uiL_t get_total_physical_memory(){
    return (uiL_t)sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGE_SIZE);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void x86_cpuid(u32_t eabcdx[4], u32_t eax, u32_t ecx){
    __cpuid_count(eax, ecx, eabcdx[0], eabcdx[1], eabcdx[2], eabcdx[3]);
}
u64_t cpu_timestamp(){
    unsigned int lo, hi;
    __asm__ volatile ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((u64_t)hi << 32) | lo;
}
u64_t measure_cpu_timestamp_frequency(){
//    Time::WallClock w_start = Time::WallClock::now();
    auto w_start = std::chrono::system_clock::now();
    u64_t r_start = cpu_timestamp();
    while (std::chrono::system_clock::now() - w_start < std::chrono::microseconds(62500));
    auto w_end = std::chrono::system_clock::now();
//    while (w_start.seconds_elapsed() < 0.0625);
//    Time::WallClock w_end = Time::WallClock::now();
    u64_t r_end = cpu_timestamp();

    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(w_end - w_start);
    double seconds = (double)elapsed.count() / 1000000.;

    return (u64_t)((double)(r_end - r_start) / seconds);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
