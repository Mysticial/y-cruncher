/* Launch_Windows.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/01/2016
 * Last Modified    : 01/01/2016
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <Windows.h>
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/BasicLibs/StringTools/Unicode.h"
#include "PublicLibs/SystemLibs/Environment/Environment.h"
#include "Launch.h"
using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
std::string build_command(const std::string& module){
    const auto& params = Environment::get_cmd_parameters();

    std::string str("\"");
    str += module;
    str += "\"";
    for (size_t c = 1; c < params.size(); c++){
        str += " \"";
        str += params[c];
        str += "\"";
    }
    return str;
}
void launch(const char* name){
    Console::println();
    Console::println_labelc("Auto-Selecting", name);
    Console::println();

    std::string path = Environment::get_process_directory() + "Binaries/" + name + ".exe";
    Console::println(path);
    Console::println("\n");

    Console::println("Launching y-cruncher...", 'Y');
    Console::println("================================================================\n", 'g');
    Console::SetColor('w');


    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcessW(
        StringTools::utf8_to_wstr(path).c_str(),
        &StringTools::utf8_to_wstr(build_command(path))[0],
        nullptr,
        nullptr,
        false,
        0,
        nullptr,
        nullptr,
        &si,
        &pi
    )){
        Console::println_labelc("Unable to start y-cruncher", GetLastError());
        Console::Pause();
        exit(1);
    }

    int ret = WaitForSingleObject(pi.hProcess, INFINITE);
    if (ret != 0){
        Console::println("\n");
    }
    if (Environment::get_cmd_parameters().size() <= 1 && !Environment::RunFromConsole()){
        Console::Pause();
    }
    exit(ret);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
