
//#include <iostream>
//using std::cout;
//using std::endl;
//using std::cin;

#include "Launch.h"

#include "VersionNumber.h"
#include "PublicLibs/BasicLibs/StringTools/ymb_STR.h"
#include "PublicLibs/BasicLibs/CommandLine.h"
#include "PublicLibs/SystemLibs/Environment/Environment.h"

//  Pull in just the necessary stuff from PublicLibs.
#include "PublicLibs/ConsoleIO/BasicIO.cpp"
#include "PublicLibs/ConsoleIO/Margin.cpp"
#include "PublicLibs/ConsoleIO/Label.cpp"
#include "PublicLibs/BasicLibs/StringTools/ToString.cpp"
#include "PublicLibs/BasicLibs/StringTools/Unicode.cpp"
#include "PublicLibs/BasicLibs/StringTools/ymb_STR.cpp"
#include "PublicLibs/SystemLibs/Time/Time.cpp"
#include "PublicLibs/SystemLibs/Environment/Environment.cpp"
#include "PublicLibs/SystemLibs/ProcessorCapability/cpu_x86.cpp"

#include "Warnings.h"
#include "Vendor-AMD.h"
#include "Vendor-Intel.h"

#ifdef _WIN32
#include "Launch_Windows.ipp"
#else
#include "Launch_Linux.ipp"
#endif

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool pause_on_warning = true;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void handle_pause(CommandLine::Parameters& cmds){
    auto& value = cmds.current_value();
    cmds.advance();
    int pause_on_exit = static_cast<int>(StringTools::parse_sL_text(value));
    pause_on_warning = pause_on_exit > -1;
    Console::pause_on_error = pause_on_exit > -2;
}
void handle_colors(CommandLine::Parameters& cmds){
    auto& value = cmds.current_value();
    cmds.advance();
    Console::enable_colors = StringTools::parse_sL_text(value) > 0;
}
void handle_height(CommandLine::Parameters& cmds){
    auto& value = cmds.current_value();
    cmds.advance();
    int height = static_cast<int>(StringTools::parse_sL_text(value));
    Console::set_console_window_size(80, height);
}
void handles_skip_warnings(CommandLine::Parameters& cmds){
    cmds.advance();
    pause_on_warning = false;
}
CommandLine::ActionMap GLOBAL_COMMANDS{
    {"pause", handle_pause},
    {"colors", handle_colors},
    {"height", handle_height},
    {"skip-warnings", handles_skip_warnings},
};
void process_startup_commands(CommandLine::Parameters& cmds){
    while (cmds.has_more() && cmds.match_execute(GLOBAL_COMMANDS, false));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#if _WIN32
int wmain(int argc, wchar_t* argv[]){
#else
int main(int argc, char *argv[]){
#endif

    //  Initialize Globals
    CommandLine::Parameters cmds(argc, argv);
    Environment::initialize_environment(argc, argv);

    if (!Environment::RunFromConsole()){
        Console::set_console_window_size();
    }
    process_startup_commands(cmds);

    Console::print("y-cruncher v", 'g');
    Console::print(VERSION_LONG);

    Console::println("\n", 'w');

    Console::println("Detecting Environment...\n");

    cpu_x86 features;
    features.detect_host();
    features.print();

    if (features.Vendor_AMD){
        dispatch_AMD(features);
    }else{
        dispatch_Intel(features);
    }

    if (Environment::get_cmd_parameters().size() <= 1 && !Environment::RunFromConsole()){
        Console::pause();
    }
    Console::set_color_default();
}
