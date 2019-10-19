/* TryDispatch.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/01/2015
 * Last Modified    : 02/01/2015
 * 
 */

#pragma once
#ifndef _ycr_TryDispatch_H
#define _ycr_TryDispatch_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#ifdef YMC_FULL_COMPILE
#include "Settings.h"
#else
#define     YCR_ENABLE_2000_x86
//#define     YCR_ENABLE_2007_x64_Penryn
#define     YCR_ENABLE_2008_x64_Nehalem
#define     YCR_ENABLE_2014_x64_Broadwell
#define     YCR_ENABLE_2016_x64_KnightsLanding
#define     YCR_ENABLE_2017_x64_Zen1
#define     YCR_ENABLE_2017_x64_Skylake
#define     YCR_ENABLE_2018_x64_CannonLake
#define     YCR_ENABLE_2019_x64_Zen2
#endif
#include "BinaryNames.h"
#include "Dispatcher.h"
#include "Launch.h"
using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void dispatch_2000_x86(const cpu_x86& features){
#ifdef YCR_ENABLE_2000_x86
    launch(BINARY_NAME_2000_x86);
#endif
}
void dispatch_2004_x86_SSE3(const cpu_x86& features){
    if (can_run_2004_x86_SSE3(features)){
        launch(BINARY_NAME_2004_x86_SSE3);
    }
}
void dispatch_2004_x64_SSE3(const cpu_x86& features){
    if (can_run_2004_x64_SSE3(features)){
        launch(BINARY_NAME_2004_x64_SSE3);
    }
}
void dispatch_2007_x64_Penryn(const cpu_x86& features){
#ifdef YCR_ENABLE_2007_x64_Penryn
    if (can_run_2007_x64_Penryn(features)){
        launch(BINARY_NAME_2007_x64_Penryn);
    }
#endif
}
void dispatch_2008_x64_Nehalem(const cpu_x86& features){
    if (can_run_2008_x64_Nehalem(features)){
        launch(BINARY_NAME_2008_x64_Nehalem);
    }
}
void dispatch_2011_x64_SandyBridge(const cpu_x86& features){
    if (can_run_2011_x64_SandyBridge(features)){
        launch(BINARY_NAME_2011_x64_SandyBridge);
    }
}
void dispatch_2011_x64_Bulldozer(const cpu_x86& features){
    if (can_run_2011_x64_Bulldozer(features)){
        launch(BINARY_NAME_2011_x64_Bulldozer);
    }
}
void dispatch_2013_x64_Haswell(const cpu_x86& features){
    if (can_run_2013_x64_Haswell(features)){
        launch(BINARY_NAME_2013_x64_Haswell);
    }
}
void dispatch_2014_x64_Broadwell(const cpu_x86& features){
#ifdef YCR_ENABLE_2014_x64_Broadwell
    if (can_run_2014_x64_Broadwell(features)){
        launch(BINARY_NAME_2014_x64_Broadwell);
    }
#endif
}
void dispatch_2016_x64_KnightsLanding(const cpu_x86& features){
#ifdef YCR_ENABLE_2016_x64_KnightsLanding
    if (can_run_2016_x64_KnightsLanding(features)){
        launch(BINARY_NAME_2016_x64_KnightsLanding);
    }
#endif
}
void dispatch_2017_x64_Zen1(const cpu_x86& features){
#ifdef YCR_ENABLE_2017_x64_Zen1
    if (can_run_2017_x64_Zen1(features)){
        launch(BINARY_NAME_2017_x64_Zen1);
    }
#endif
}
void dispatch_2017_x64_Skylake(const cpu_x86& features){
#ifdef YCR_ENABLE_2017_x64_Skylake
    if (can_run_2017_x64_Skylake(features)){
        launch(BINARY_NAME_2017_x64_Skylake);
    }
#endif
}
void dispatch_2018_x64_CannonLake(const cpu_x86& features){
#ifdef YCR_ENABLE_2018_x64_CannonLake
    if (can_run_2018_x64_CannonLake(features)){
        launch(BINARY_NAME_2018_x64_CannonLake);
    }
#endif
}
void dispatch_2019_x64_Zen2(const cpu_x86& features){
#ifdef YCR_ENABLE_2019_x64_Zen2
    if (can_run_2019_x64_Zen2(features)){
        launch(BINARY_NAME_2017_x64_Zen1);
    }
#endif
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
