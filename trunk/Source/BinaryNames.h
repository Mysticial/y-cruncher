/* BinaryNames.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/13/2015
 * Last Modified    : 02/13/2015
 * 
 */

#pragma once
#ifndef ycr_BinaryNames_H
#define ycr_BinaryNames_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string>
#include <vector>
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const char BINARY_NAME_2000_x86[]                   = "00-x86";
const char BINARY_NAME_2004_x86_SSE3[]              = "04-P4P";
const char BINARY_NAME_2004_x64_SSE3[]              = "05-A64 ~ Kasumi";
const char BINARY_NAME_2007_x64_Penryn[]            = "07-PNR ~ Nagisa";
const char BINARY_NAME_2008_x64_Nehalem[]           = "08-NHM ~ Ushio";
const char BINARY_NAME_2011_x64_SandyBridge[]       = "11-SNB ~ Hina";
const char BINARY_NAME_2011_x64_Bulldozer[]         = "11-BD1 ~ Miyu";
const char BINARY_NAME_2013_x64_Haswell[]           = "13-HSW ~ Airi";
const char BINARY_NAME_2014_x64_Broadwell[]         = "14-BDW ~ Kurumi";
const char BINARY_NAME_2016_x64_KnightsLanding[]    = "16-KNL";
//const char BINARY_NAME_2016_x64_KnightsLanding[]    = "16-KNL ~ Midori";
const char BINARY_NAME_2017_x64_Zen1[]              = "17-ZN1 ~ Yukina";
const char BINARY_NAME_2017_x64_Skylake[]           = "17-SKX ~ Kotori";
const char BINARY_NAME_2018_x64_CannonLake[]        = "18-CNL ~ Shinoa";
const char BINARY_NAME_2019_x64_Zen2[]              = "19-ZN2 ~ Kagari";
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef YMP_BUILD_DEVELOPER
const std::vector<const char*> BINARY_NAME_LIST{
    "y-cruncher",
    BINARY_NAME_2000_x86,
    BINARY_NAME_2004_x86_SSE3,
    BINARY_NAME_2004_x64_SSE3,
    BINARY_NAME_2007_x64_Penryn,
    BINARY_NAME_2008_x64_Nehalem,
    BINARY_NAME_2011_x64_SandyBridge,
    BINARY_NAME_2011_x64_Bulldozer,
    BINARY_NAME_2013_x64_Haswell,
    BINARY_NAME_2014_x64_Broadwell,
    BINARY_NAME_2016_x64_KnightsLanding,
    BINARY_NAME_2017_x64_Zen1,
    BINARY_NAME_2017_x64_Skylake,
    BINARY_NAME_2018_x64_CannonLake,
    BINARY_NAME_2019_x64_Zen2,
};
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
