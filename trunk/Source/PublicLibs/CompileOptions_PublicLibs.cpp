/* CompileOptions_PublicLibs.cpp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 07/19/2015
 *  Last Modified   : 07/19/2015
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include "PublicLibs/ConsoleIO/BasicIO.h"
#ifdef YMP_STANDALONE
#include "SystemLibs/Time/Time.h"
#endif
#include "SystemLibs/FileIO/FileIO.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CompileOptions_PublicLibs(){
    Console::set_color('T');
    Console::println("================================================================");
    Console::set_color('Y');
    Console::println("External Library\n");
    Console::set_color('w');

    Console::compile_options();
#ifdef YMP_STANDALONE
    Time::CompileOptions();
#endif
    FileIO::CompileOptions();
    Console::println();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
