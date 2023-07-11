/* Arch-x86.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/01/2015
 * Last Modified    : 02/01/2015
 * 
 */

#pragma once
#ifndef _ycr_Arch_x86_H
#define _ycr_Arch_x86_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "TryDispatch.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void warn_no_SSE3(const cpu_x86& features){
    if (!can_run_2004_x86_SSE3(features)){
        Console::set_color('R');
#ifdef YCR_ENABLE_2000_x86
        Console::println("Your system seems kind of old... Consider getting a new computer. :)");
        Console::println();
#else
        Console::println("This computer is too old to run this version of y-cruncher. :(");
        Console::println();
        Console::println("SSE3 instructions are required to run y-cruncher v0.6.1 and later.");
        Console::println();
#endif
        Console::set_color('w');
        Console::pause();
        Console::println("\n");
    }
}
void dispatch_class_x86(const cpu_x86& features){
#ifdef _WIN32
    dispatch_2004_x86_SSE3(features);
    warn_no_SSE3(features);
    dispatch_2000_x86(features);
#else
    warn_no_SSE3(features);
#endif
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
