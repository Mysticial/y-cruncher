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
#ifdef YCR_ENABLE_2024_x64_Zen5
    if (features.HW_AVX512_VP2INTERSECT){
        dispatch_2024_x64_Zen5(features);
    }
#else
    if (features.HW_AVX512_VP2INTERSECT && can_run_2024_x64_Zen5(features)){
        Console::println("", 'Y');
        Console::println("Detected a Zen5 processor!");
        Console::println();
        Console::println("Optimizations are not yet available for this processor as they are pending");
        Console::println("final hardware for proper tuning.");
        Console::println();
        Console::println("    https://twitter.com/Mysticial/status/1797478508157124717");
        Console::println();
        Console::println("As the release date for Zen5 approaches, please stay tuned for an update");
        Console::println("to this release that will have Zen5 optimizations.");
        Console::println();
        Console::println("Falling back to the Zen4 binary...");
        Console::println();
        Console::set_color('w');
        if (pause_on_warning){
            Console::pause();
        }
    }
#endif
#ifdef YCR_ENABLE_2022_x64_Zen4
    dispatch_2022_x64_Zen4(features);
#endif
//    warn_noAVX512(features);
    warn_AVX512_OS(features);

    //  AVX
//#ifdef YCR_ENABLE_2020_x64_Zen3
//    if (features.HW_VAES){
//        dispatch_2020_x64_Zen3(features);
//    }
//#endif
#ifdef YCR_ENABLE_2019_x64_Zen2
    if (features.HW_RDPID){
        dispatch_2019_x64_Zen2(features);
    }
#endif
#ifdef YCR_ENABLE_2017_x64_Zen1
    if (features.HW_ADX){
        dispatch_2017_x64_Zen1(features);
    }
#endif
#ifdef YCR_ENABLE_2012_x64_Piledriver
    dispatch_2012_x64_Piledriver(features);
#else
    dispatch_2011_x64_Bulldozer(features);
#endif
    dispatch_2011_x64_SandyBridge(features);
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
