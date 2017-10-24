/* Vendor-AMD.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/01/2015
 * Last Modified    : 02/01/2015
 * 
 */

#pragma once
#ifndef _ycr_Vendor_AMD_H
#define _ycr_Vendor_AMD_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "TryDispatch.h"
#include "Arch-x86.h"
#include "Warnings.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void dispatch_AMD(const cpu_x86& features){
    //  AVX512
//#if _WIN32
//    if (can_run_2016_x64_KnightsLanding(features) || can_run_2017_x64_Skylake(features)){
//        Console::SetColor('R');
//        Console::println("Due to inadequate compiler support, AVX512 is currently only available for");
//        Console::println("Intel processors on Windows.");
//        Console::println();
//        Console::println("Please stay tuned for a future version of y-cruncher.");
//        Console::println();
//        Console::SetColor('w');
//        Console::Pause();
//        Console::println("\n");
//    }
//#else
#ifdef YCR_ENABLE_2018_x64_Cannonlake
#define YCR_ENABLE_AVX512
    dispatch_2018_x64_Cannonlake(features);
#endif
#ifdef YCR_ENABLE_2017_x64_Skylake
#define YCR_ENABLE_AVX512
    dispatch_2017_x64_Skylake(features);
#endif
#ifdef YCR_ENABLE_2016_x64_KnightsLanding
#define YCR_ENABLE_AVX512
    dispatch_2016_x64_KnightsLanding(features);
#endif
#ifdef YCR_ENABLE_AVX512
    warn_noAVX512(features);
    warn_AVX512_OS(features);
#else
    warn_AVX512_not_ready(features);
#endif
//#endif

    //  AVX
#ifdef YCR_ENABLE_2017_x64_Zen
    if (features.HW_ADX){
        dispatch_2017_x64_Zen(features);
    }
#endif
    dispatch_2011_x64_Bulldozer(features);
    warn_AVX_OS(features);

    //  x64
#ifdef YCR_ENABLE_2008_x64_Nehalem
    dispatch_2008_x64_Nehalem(features);
#endif
    dispatch_2004_x64_SSE3(features);
    warn_32bit_on_x64(features);

    //  x86
    dispatch_class_x86(features);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
