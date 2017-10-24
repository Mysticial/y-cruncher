/* CompilerSettings.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/15/2015
 * Last Modified    : 01/15/2015
 * 
 */

#pragma once
#ifndef ymp_CompilerSettings_H
#define ymp_CompilerSettings_H

#if defined _MSC_VER
#include "CompilerSettings/Compiler_VSC.h"
#elif defined __GNUC__
#include "CompilerSettings/Compiler_GCC.h"
#else
#error "Unrecognized Compiler"
#endif

#endif
