/* Dispatcher.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 05/18/2014
 * Last Modified    : 05/18/2014
 * 
 */

#pragma once
#ifndef ycr_Dispatcher_H
#define ycr_Dispatcher_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/SystemLibs/ProcessorCapability/cpu_x86.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool hardware_2000_x86(const cpu_x86& features){
    return true;
}
bool hardware_2004_x86_SSE3(const cpu_x86& features){
    return hardware_2000_x86(features)
        && features.HW_SSE
        && features.HW_SSE2
        && features.HW_SSE3;
}
bool hardware_2004_x64_SSE3(const cpu_x86& features){
    return hardware_2004_x86_SSE3(features)
        && features.HW_x64;
}
bool hardware_2007_x64_Penryn(const cpu_x86& features){
    return hardware_2004_x64_SSE3(features)
        && features.HW_SSSE3
        && features.HW_SSE41;
}
bool hardware_2008_x64_Nehalem(const cpu_x86& features){
    //  Currently, the Nehalem binary doesn't use SSE4.2.
    //  So it has the same requirements as Penryn.
    return hardware_2007_x64_Penryn(features);
}
bool hardware_2011_x64_SandyBridge(const cpu_x86& features){
    return hardware_2008_x64_Nehalem(features)
        && features.HW_SSE42
        && features.HW_AVX;
}
bool hardware_2011_x64_Bulldozer(const cpu_x86& features){
    return hardware_2011_x64_SandyBridge(features)
        && features.HW_ABM
//        && features.HW_XOP
        && features.HW_FMA4;
}
bool hardware_2013_x64_Haswell(const cpu_x86& features){
    return hardware_2011_x64_SandyBridge(features)
        && features.HW_ABM
        && features.HW_BMI1
        && features.HW_BMI2
        && features.HW_FMA3
        && features.HW_AVX2;
}
bool hardware_2014_x64_Broadwell(const cpu_x86& features){
    return hardware_2013_x64_Haswell(features)
        && features.HW_ADX;
}
bool hardware_2016_x64_KnightsLanding(const cpu_x86& features){
    return hardware_2014_x64_Broadwell(features)
        && features.HW_ADX
        && features.HW_AVX512_F
        && features.HW_AVX512_CD;
//        && features.HW_AVX512_ER
//        && features.HW_AVX512_PF;
}
bool hardware_2017_x64_Zen(const cpu_x86& features){
    return hardware_2014_x64_Broadwell(features);
}
bool hardware_2017_x64_Skylake(const cpu_x86& features){
    return hardware_2014_x64_Broadwell(features)
        && features.HW_ADX
        && features.HW_AVX512_F
        && features.HW_AVX512_CD
        && features.HW_AVX512_VL
        && features.HW_AVX512_BW
        && features.HW_AVX512_DQ;
}
bool hardware_2018_x64_CannonLake(const cpu_x86& features){
    return hardware_2017_x64_Skylake(features)
        && features.HW_AVX512_IFMA
        && features.HW_AVX512_VBMI;
}
bool hardware_2019_x64_Zen2(const cpu_x86& features){
    return hardware_2017_x64_Zen(features);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool can_run_2000_x86(const cpu_x86&){
    return true;
}
bool can_run_2004_x86_SSE3(const cpu_x86& features){
    return hardware_2004_x86_SSE3(features);
}
bool can_run_2004_x64_SSE3(const cpu_x86& features){
    return hardware_2004_x64_SSE3(features)
        && features.OS_x64;
}
bool can_run_2007_x64_Penryn(const cpu_x86& features){
    return hardware_2007_x64_Penryn(features)
        && features.OS_x64;
}
bool can_run_2008_x64_Nehalem(const cpu_x86& features){
    return hardware_2008_x64_Nehalem(features)
        && features.OS_x64;
}
bool can_run_2011_x64_SandyBridge(const cpu_x86& features){
    return hardware_2011_x64_SandyBridge(features)
        && features.OS_x64
        && features.OS_AVX;
}
bool can_run_2011_x64_Bulldozer(const cpu_x86& features){
    return hardware_2011_x64_Bulldozer(features)
        && features.OS_x64
        && features.OS_AVX;
}
bool can_run_2013_x64_Haswell(const cpu_x86& features){
    return hardware_2013_x64_Haswell(features)
        && features.OS_x64
        && features.OS_AVX;
}
bool can_run_2014_x64_Broadwell(const cpu_x86& features){
    return hardware_2014_x64_Broadwell(features)
        && features.OS_x64
        && features.OS_AVX;
}
bool can_run_2016_x64_KnightsLanding(const cpu_x86& features){
    return hardware_2016_x64_KnightsLanding(features)
        && features.OS_x64
        && features.OS_AVX
        && features.OS_AVX512;
}
bool can_run_2017_x64_Zen1(const cpu_x86& features){
    return hardware_2017_x64_Zen(features)
        && features.OS_x64
        && features.OS_AVX;
}
bool can_run_2017_x64_Skylake(const cpu_x86& features){
    return hardware_2017_x64_Skylake(features)
        && features.OS_x64
        && features.OS_AVX
        && features.OS_AVX512;
}
bool can_run_2018_x64_CannonLake(const cpu_x86& features){
    return hardware_2018_x64_CannonLake(features)
        && features.OS_x64
        && features.OS_AVX
        && features.OS_AVX512;
}
bool can_run_2019_x64_Zen2(const cpu_x86& features){
    return hardware_2019_x64_Zen2(features)
        && features.OS_x64
        && features.OS_AVX;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
