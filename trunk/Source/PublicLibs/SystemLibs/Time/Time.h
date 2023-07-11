/* Time.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 09/16/2014
 *  Last Modified   : 09/16/2014
 * 
 */

#pragma once
#ifndef ymp_Time_H
#define ymp_Time_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string>
#include "PublicLibs/CompilerSettings.h"
#ifdef _WIN32
#include "Time_Windows.h"
#else
#include "Time_Posix.h"
#endif
namespace ymp{
namespace Time{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CompileOptions();
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
std::string tostr_now();
std::string now_to_filestring();
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
