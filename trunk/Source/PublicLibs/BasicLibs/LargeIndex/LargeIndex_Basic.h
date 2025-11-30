/* LargeIndex.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 01/21/2024
 *  Last Modified   : 05/11/2025
 * 
 */

#pragma once
#ifndef ymp_LargeIndex_Basic_H
#define ymp_LargeIndex_Basic_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#ifdef _WIN32
#include <intrin.h>
#endif
#ifdef __GNUC__
#include <x86intrin.h>
#endif
#include "LargeIndex.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef YMP_LargeIndex_Int128
#include "PublicLibs/BasicLibs/LargePrimitives/Int128.h"
#include "PublicLibs/BasicLibs/LargePrimitives/Int128_Basic.h"
namespace ymp{
    using LargeIndexU = UInt128;
    using LargeIndexS = SInt128;

    YM_FORCE_INLINE LargeIndexU string_to_ui(const char* str){
        return LargeIndexU(str);
    }
    YM_FORCE_INLINE LargeIndexS string_to_si(const char* str){
        return LargeIndexS(str);
    }
    YM_FORCE_INLINE LargeIndexS round_si(double x){
        return LargeIndexS(x);
    }
}
#else
#include "PublicLibs/BasicLibs/LargePrimitives/Double_Rounding.h"
namespace ymp{
    using LargeIndexU = u64_t;
    using LargeIndexS = s64_t;

    inline LargeIndexU string_to_ui(const char* str){
        LargeIndexU x(0);
        while (true){
            char ch = *str++;
            if (ch == ',')              continue;
            if (ch < '0' || ch > '9')   return x; 
            x *= 10;
            x += ch - '0';
        }
        return x;
    }
    inline LargeIndexS string_to_si(const char* str){
        return str[0] == '-'
            ? -(LargeIndexS)string_to_ui(str + 1)
            : (LargeIndexS)string_to_ui(str);
    }
    YM_FORCE_INLINE LargeIndexS round_si(double x){
        return round_s64(x);
    }
}
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
