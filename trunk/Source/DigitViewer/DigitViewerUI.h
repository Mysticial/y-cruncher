/* DigitViewerUI.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/02/2012
 * Last Modified    : 01/25/2015
 * 
 *  This file implements the UI of the stand-alone Digit Viewer.
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "DigitConverter/DigitConverter.h"
#include "Globals.h"
#include "Menu_DigitViewer.h"
namespace DigitViewer{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Sub-Menus
void CompilationOptions(){
    Console::println("\n");
    Console::println("================================================================");
    Console::println("Compilation Options\n");

#if defined __INTEL_COMPILER
    Console::println_labelm("Compiler", "Intel Compiler", 'T');
    Console::println_labelm("Version", __INTEL_COMPILER, 'T');
#elif defined _MSC_VER
    Console::println_labelm("Compiler", "Visual C++", 'T');
    Console::println_labelm("Version", _MSC_FULL_VER, 'T');
#elif defined __GNUC__
    Console::println_labelm("Compiler", "GCC", 'T');
    std::string version = std::to_string(__GNUC__) + "." + std::to_string(__GNUC_MINOR__);
    Console::println_labelm("Version", version.c_str(), 'T');
#else
    Console::println_labelm("Compiler", "Unknown", 'R');
    Console::println_labelm("Version", "Unknown", 'R');
#endif
    Console::println_labelm("Compile Date", __DATE__, 'Y');
    Console::println();


    Console::println("================================================================");
    Console::println("Digit Conversion\n");

    CompileOptions_DigitConverter();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Main Menu
void Menu_Main(){
    Console::println(VERSION, 'G');
    Console::println();

    Console::print("Copyright ", 'T');
    Console::print("2008-2017 Alexander J. Yee");
    Console::print("      ");
    Console::print("( a-yee@u.northwestern.edu )");
    Console::println("\n");

    Console::print("Distribute Freely - Please Report any Bugs", 'w');
    Console::println("\n\n");

    Console::print("  0     ");
    Console::println("Digit Viewer", 'G');
    Console::SetColor('w');
    //Console::print("  1     ");
    //Console::println("Digit Compare", 'G');
    //Console::SetColor('w');

    Console::println();
    Console::print("  2     ");
    Console::println("Compilation Options", 'G');
    Console::SetColor('w');

    Console::println("\n\n");

    Console::println("\nEnter your choice:");
    upL_t c = Console::scan_label_upL_range("option: ", 0, 2);

    switch (c){
        case 0:
            Menu_DigitViewer();
            break;
        //case 1:
        //    Menu_DigitCompare();
        //    break;
        case 2:
            CompilationOptions();
            break;
        default:;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
