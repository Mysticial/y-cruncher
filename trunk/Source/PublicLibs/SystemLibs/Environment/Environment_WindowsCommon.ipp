/* Environment_Windows7.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/04/2015
 * Last Modified    : 08/27/2016
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <algorithm>
#include <Windows.h>
#include "PublicLibs/BasicLibs/StringTools/Unicode.h"
#include "PublicLibs/ConsoleIO/BasicIO.h"
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
bool run_from_console;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool init_run_from_console(){
    //  Copied from: http://stackoverflow.com/a/513574/922184

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)){
        return true;    //  Assume true on failure.
    }

    // If cursor position is (0,0) then we were launched in a separate console.
    return csbi.dwCursorPosition.X || csbi.dwCursorPosition.Y;
}
void initialize_process_path(){
    //  This is really stupid. There's no way to get the size of the path.

    DWORD buffer_size = MAX_PATH;
    std::wstring path;
    DWORD bytes;
    do{
        buffer_size *= 2;
        path.resize(buffer_size);
        bytes = GetModuleFileNameW(nullptr, &path[0], buffer_size);
        if (bytes == 0){
            Console::Warning("Failed to retrieve path.");
            Console::Quit(1);
        }
    }while (bytes == buffer_size);
    process_path = StringTools::wstr_to_utf8(path.c_str());
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
    run_from_console = init_run_from_console();

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
    return run_from_console;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
upL_t GetFreePhysicalMemory(){
    uiL_t bytes;

    MEMORYSTATUSEX data;
    data.dwLength = sizeof(data);
    if (GlobalMemoryStatusEx(&data)){
        bytes = (uiL_t)data.ullAvailPhys;
    }else{
        bytes = 0;
    }

    bytes = std::min(bytes, (uiL_t)MAX_MEMORY);
    return static_cast<upL_t>(bytes);
}
uiL_t GetTotalPhysicalMemory(){
    MEMORYSTATUSEX data;
    data.dwLength = sizeof(data);
    if (GlobalMemoryStatusEx(&data)){
        return (uiL_t)data.ullTotalPhys;
    }else{
        return 0;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void x86_cpuid(u32_t eabcdx[4], u32_t eax, u32_t ecx){
    int out[4];
    __cpuidex(out, eax, ecx);
    eabcdx[0] = out[0];
    eabcdx[1] = out[1];
    eabcdx[2] = out[2];
    eabcdx[3] = out[3];
}
u64_t x86_rdtsc(){
    return __rdtsc();
}
u64_t x86_measure_rdtsc_ticks_per_sec(){
    HANDLE thread = GetCurrentThread();

    GROUP_AFFINITY before_affinity;
    if (GetThreadGroupAffinity(thread, &before_affinity) == 0){
        Console::Warning("Unable to read thread affinity.");
    }

    KAFFINITY t = 1;
    while ((t & before_affinity.Mask) == 0){
        t <<= 1;
    }

    GROUP_AFFINITY placeholder;
    GROUP_AFFINITY new_affinity = before_affinity;
    new_affinity.Mask = t;
    if (SetThreadGroupAffinity(thread, &new_affinity, &placeholder) == 0){
        Console::Warning("Unable to set Affinity Mask.");
        Console::Quit(1);
    }

    LARGE_INTEGER frequency;
    if (!QueryPerformanceFrequency(&frequency)){
        Console::Warning("Unable to measure clock speed.");
        Console::Quit(1);
    }
    u64_t freq = frequency.QuadPart;
    freq >>= 4;


    u64_t start_cycles = __rdtsc();

    LARGE_INTEGER start_timer;
    if (!QueryPerformanceCounter(&start_timer)){
        Console::Warning("Unable to measure clock speed.");
        Console::Quit(1);
    }
    LARGE_INTEGER current_timer;
    do {
        if (!QueryPerformanceCounter(&current_timer)){
            Console::Warning("Unable to measure clock speed.");
            Console::Quit(1);
        }
    }while ((u64_t)current_timer.QuadPart - (u64_t)start_timer.QuadPart < freq);

    u64_t end_cycles = __rdtsc();

    if (SetThreadGroupAffinity(thread, &before_affinity, &placeholder) == 0){
        Console::Warning("Unable to set Affinity Mask.");
        Console::Quit(1);
    }

    double cycle_dif = (double)(end_cycles - start_cycles);
    double timer_dif = (double)((u64_t)current_timer.QuadPart - (u64_t)start_timer.QuadPart);

    return (u64_t)(cycle_dif / timer_dif * frequency.QuadPart);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
