/* cpu_x86_Macros.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/14/2015
 * Last Modified    : 02/14/2015
 * 
 */

#pragma once
#ifndef cpu_x86_Macros_H
#define cpu_x86_Macros_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#if 0
#elif defined X64_19_Zen2
#define YMP_Arch_2014_x64_Broadwell
#elif defined X64_18_CannonLake
#define YMP_Arch_2018_x64_CannonLake
#elif defined X64_17_Skylake
#define YMP_Arch_2017_x64_Skylake
#elif defined X64_17_Zen1
#define YMP_Arch_2014_x64_Broadwell
#elif defined X64_16_KnightsLanding
#define YMP_Arch_2016_x64_KnightsLanding
#elif defined X64_14_Broadwell
#define YMP_Arch_2014_x64_Broadwell
#elif defined X64_13_Haswell
#define YMP_Arch_2013_x64_Haswell
#elif defined X64_11_Bulldozer
#define YMP_Arch_2011_x64_Bulldozer
#elif defined X64_11_SandyBridge
#define YMP_Arch_2011_x64_SandyBridge
#elif defined X64_08_Nehalem
#define YMP_Arch_2008_x64_Nehalem
#elif defined X64_07_Penryn
#define YMP_Arch_2007_x64_Penryn
#elif defined X64_04_SSE3
#define YMP_Arch_2004_x64_SSE3
#elif defined X86_04_SSE3
#define YMP_Arch_2004_x86_SSE3
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef YMP_Arch_2018_x64_CannonLake
#define YMP_Arch_2017_x64_Skylake
#endif

#ifdef YMP_Arch_2017_x64_Skylake
#define YMP_Arch_2014_x64_Broadwell
#endif

#ifdef YMP_Arch_2016_x64_KnightsLanding
#define YMP_Arch_2014_x64_Broadwell
#endif

#ifdef YMP_Arch_2014_x64_Broadwell
#define YMP_Arch_2013_x64_Haswell
#endif

#ifdef YMP_Arch_2013_x64_Haswell
#define YMP_Arch_2011_x64_SandyBridge
#endif

#ifdef YMP_Arch_2011_x64_Bulldozer
#define YMP_Arch_2011_x64_SandyBridge
#endif

#ifdef YMP_Arch_2011_x64_SandyBridge
#define YMP_Arch_2008_x64_Nehalem
#endif

#ifdef YMP_Arch_2008_x64_Nehalem
#define YMP_Arch_2007_x64_Penryn
#endif

#ifdef YMP_Arch_2007_x64_Penryn
#define YMP_Arch_2004_x64_SSE3
#endif

#ifdef YMP_Arch_2004_x64_SSE3
#define YMP_Arch_2004_x64
#define YMP_Arch_2004_x86_SSE3
#endif

#ifdef YMP_Arch_2004_x86_SSE3
#define YMP_Arch_2000_x86
#define YMP_Arch_2001_x86_SSE2
#endif

#ifdef YMP_Arch_2004_x64
#define YMP_Arch_2000_x86
#endif

#define YMP_Arch_2000_x86
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
