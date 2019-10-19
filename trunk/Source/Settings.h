/* Settings.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 12/05/2009
 * Last Modified    : Always...
 * 
 */

#pragma once
#ifndef ycs_Settings_H
#define ycs_Settings_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//#define     YMP_BUILD_LIBRARY           //  YMP Library
//#define     YMP_BUILD_RELEASE           //  For Public Release
//#define     YMP_BUILD_PRIVATE           //  Private Betas/Dogfooding
//#define     YMP_BUILD_DEVELOPER         //  For Developer Builds
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Library
#ifdef YMP_BUILD_LIBRARY
#ifdef YCC_BUILD
#error "More than one mode selected."
#endif
#define YCC_BUILD

#define     YCG_PROGRAM_SUFFIX      " (lib)"

//  Binaries
#define     YCR_ENABLE_2000_x86
#define     YCR_ENABLE_2007_x64_Penryn
#define     YCR_ENABLE_2008_x64_Nehalem
#define     YCR_ENABLE_2014_x64_Broadwell
//#define     YCR_ENABLE_2016_x64_KnightsLanding
#define     YCR_ENABLE_2017_x64_Zen1
#define     YCR_ENABLE_2017_x64_Skylake
//#define     YCR_ENABLE_2018_x64_CannonLake
//#define     YCR_ENABLE_2019_x64_Zen2

#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Public Release
#ifdef YMP_BUILD_RELEASE
#ifdef YCC_BUILD
#error "More than one mode selected."
#endif
#define YCC_BUILD

#define     YCG_PROGRAM_SUFFIX      ""

//  Binaries
#define     YCR_ENABLE_2000_x86
//#define     YCR_ENABLE_2007_x64_Penryn
#define     YCR_ENABLE_2008_x64_Nehalem
#define     YCR_ENABLE_2014_x64_Broadwell
//#define     YCR_ENABLE_2016_x64_KnightsLanding
#define     YCR_ENABLE_2017_x64_Zen1
#define     YCR_ENABLE_2017_x64_Skylake
#define     YCR_ENABLE_2018_x64_CannonLake
//#define     YCR_ENABLE_2019_x64_Zen2

#endif
////////////////////////////////////////////////////////////////////////////////
//  Private Beta
#ifdef YMP_BUILD_PRIVATE
#ifdef YCC_BUILD
#error "More than one mode selected."
#endif
#define YCC_BUILD

#define     YCG_PROGRAM_SUFFIX      " (PB)"

//  Binaries
#define     YCR_ENABLE_2000_x86
#define     YCR_ENABLE_2007_x64_Penryn
#define     YCR_ENABLE_2008_x64_Nehalem
#define     YCR_ENABLE_2014_x64_Broadwell
//#define     YCR_ENABLE_2016_x64_KnightsLanding
#define     YCR_ENABLE_2017_x64_Zen1
#define     YCR_ENABLE_2017_x64_Skylake
#define     YCR_ENABLE_2018_x64_CannonLake
//#define     YCR_ENABLE_2019_x64_Zen2

#endif
////////////////////////////////////////////////////////////////////////////////
//  Author/Developer Build
#ifdef YMP_BUILD_DEVELOPER
#ifdef YCC_BUILD
#error "More than one mode selected."
#endif
#define YCC_BUILD

#define     YCG_PROGRAM_SUFFIX      " (AV)"

//  Binaries
#define     YCR_ENABLE_2000_x86
#define     YCR_ENABLE_2007_x64_Penryn
#define     YCR_ENABLE_2008_x64_Nehalem
#define     YCR_ENABLE_2014_x64_Broadwell
#define     YCR_ENABLE_2016_x64_KnightsLanding
#define     YCR_ENABLE_2017_x64_Zen1
#define     YCR_ENABLE_2017_x64_Skylake
#define     YCR_ENABLE_2018_x64_CannonLake
#define     YCR_ENABLE_2019_x64_Zen2

#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
