
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
void handle_colors(CommandLine::Parameters& cmds){
    auto& value = cmds.CurrentValue();
    cmds.advance();
    Console::EnableColors = StringTools::parse_sL_text(value) > 0;
}
void handle_height(CommandLine::Parameters& cmds){
    auto& value = cmds.CurrentValue();
    cmds.advance();
    int height = static_cast<int>(StringTools::parse_sL_text(value));
    Console::SetConsoleWindowSize(80, height);
}
CommandLine::ActionMap GLOBAL_COMMANDS{
    {"colors", handle_colors},
    {"height", handle_height},
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
        Console::SetConsoleWindowSize();
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
        Console::Pause();
    }
    Console::SetColorDefault();
}
