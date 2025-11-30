/* FastWallClock.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 08/24/2018
 *  Last Modified   : 08/24/2018
 * 
 * 
 *      This is a low-overhead clock that trades away accuracy for better
 *  performance. The main use-case is for high-frequency operations where you
 *  really don't want to call into the system clock which might go all the way
 *  into the kernel for something like the HPET.
 * 
 *  This clock will drift over time by as much as 0.1%.
 * 
 */

#pragma once
#ifndef ymp_Time_FastWallClock_H
#define ymp_Time_FastWallClock_H

#include "PublicLibs/SystemLibs/ProcessorCapability/ProcessorCapability.h"

#if 0
#elif defined YMP_Arch_2004_x64
#include "FastWallClock_x86.h"
#else
#include "FastWallClock_chrono.h"
#endif


#endif
