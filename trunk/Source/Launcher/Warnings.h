/* Warnings.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/01/2015
 * Last Modified    : 02/01/2015
 * 
 */

#pragma once
#ifndef _ycr_Launcher_Warnings_H
#define _ycr_Launcher_Warnings_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "Dispatcher.h"
using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void warn_32bit_on_x64(const cpu_x86& features){
    if (hardware_2004_x64_SSE3(features) && !features.OS_x64){
        Console::println("", 'R');
        Console::println("Performance Warning:");
        Console::println();
        Console::println("A 32-bit OS has been detected on an x64 SSE3 capable processor.");
        Console::println();
        Console::println("To achieve maximum performance, you must run a 64-bit OS.");
        Console::println("The 64-bit version of this program can be more than 2x faster.");
        Console::println();
        Console::SetColor('w');
        Console::Pause();
        Console::println("\n");
    }
}
void warn_AVX_OS(const cpu_x86& features){
    if (hardware_2011_x64_SandyBridge(features) && !features.OS_AVX){
        Console::println("", 'R');
        Console::println("Performance Warning:");
        Console::println();
        Console::println("This processor supports AVX instructions. However, it cannot be used");
        Console::println("because the operating system has not enabled it.");
        Console::println();
        Console::println("This could be due to either of the following reasons:");
        Console::println("  - The operating system is too old and does not support AVX.");
        Console::println("  - AVX has been explicitly disabled.");
        Console::println();
        Console::println("To achieve maximum performance, AVX must be enabled in the OS.");
        Console::println();
        Console::SetColor('w');
        Console::Pause();
        Console::println("\n");
    }
}
void warn_AVX512_OS(const cpu_x86& features){
#ifdef YCR_ENABLE_2017_x64_Skylake
    if (hardware_2017_x64_Skylake(features) && !features.OS_AVX512){
        Console::println("", 'R');
        Console::println("Performance Warning:");
        Console::println();
        Console::println("This processor supports AVX512 instructions. However, it cannot be used");
        Console::println("because the operating system has not enabled it.");
        Console::println();
        Console::println("This could be due to either of the following reasons:");
        Console::println("  - The operating system is too old and does not support AVX512.");
        Console::println("  - AVX512 has been explicitly disabled.");
        Console::println();
        Console::println("To achieve maximum performance, AVX512 must be enabled in the OS.");
        Console::println();
        Console::SetColor('w');
        Console::Pause();
        Console::println("\n");
    }
#endif
}
void warn_noAVX512(const cpu_x86& features){
#ifdef YCR_ENABLE_2017_x64_Skylake
    if (can_run_2013_x64_Haswell(features) &&
        features.HW_ADX &&
        features.HW_MPX &&
        !features.HW_AVX512_F
    ){
        Console::println("", 'Y');
        Console::println("Well that's a shame... A Skylake processor without AVX512? :(");
        Console::println();
        Console::SetColor('w');
//        Console::Pause();
        Console::println("\n");
    }
#endif
}
void warn_KnightsLanding(const cpu_x86& features){
#ifndef YCR_ENABLE_2016_x64_KnightsLanding
    if (can_run_2016_x64_KnightsLanding(features) && !can_run_2017_x64_Skylake(features)){
        Console::println("", 'Y');
        Console::println("Knights Landing AVX512 is no longer supported. Falling back to");
        Console::println("best available AVX2 binary.");
        Console::println();
        Console::println("To use AVX512 on Knights Landing, please use y-cruncher v0.7.7.");
        Console::println("Alternatively, please switch to Skylake-based AVX512 hardware.");
        Console::println();
        Console::SetColor('w');
        Console::Pause();
        Console::println("\n");
    }
#endif
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
