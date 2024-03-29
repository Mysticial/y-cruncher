/* VersionNumber.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 12/05/2009
 *  Last Modified   : Always...
 * 
 */

#pragma once
#ifndef ymp_VersionNumber_H
#define ymp_VersionNumber_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
//#include <vector>
#include <string>
#include "Settings.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const std::string VERSION_NUMBER    =   "0.8.4";
const std::string VERSION_BUILD     =   "9538";
////////////////////////////////////////////////////////////////////////////////
const char PROGRAM_COPYRIGHT[]      =   "2008-2024 Alexander J. Yee";
const char PROGRAM_EMAIL[]          =   "( a-yee@u.northwestern.edu )";
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Checkpoint Compatible Builds
//const std::vector<std::string> COMPATIBLE_BUILDS{
//    "9506",
//};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const std::string VERSION_SHORT     =   VERSION_NUMBER + "."       + VERSION_BUILD + YCG_PROGRAM_SUFFIX;
const std::string VERSION_LONG      =   VERSION_NUMBER + " Build " + VERSION_BUILD + YCG_PROGRAM_SUFFIX;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
