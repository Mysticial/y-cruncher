/* LeadingZeros.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 07/26/2014
 *  Last Modified   : 07/26/2014
 * 
 * 
 *      Return false if no set bits are found.
 * 
 */

#pragma once
#ifndef ymp_Intrinsics_LeadingZeros_H
#define ymp_Intrinsics_LeadingZeros_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "PublicLibs/SystemLibs/ProcessorCapability/ProcessorCapability.h"
#ifdef _MSC_VER
#include <intrin.h>
#endif
#ifdef YMP_Arch_2013_x64_Haswell
#include <immintrin.h>
#endif
namespace ymp{
namespace Intrinsics{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Headers
template <typename wtype>
YM_FORCE_INLINE bool leading_zeros(ukL_t& zeros, wtype x);
template <typename wtype>
YM_FORCE_INLINE ukL_t leading_zeros(wtype x);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  32-bit (Branch on Zero)
#if 0
#elif _MSC_VER
template <>
YM_FORCE_INLINE bool leading_zeros<u32_t>(ukL_t& zeros, u32_t x){
    unsigned long tmp;
    char ret = _BitScanReverse(&tmp, x);
    zeros = 31 - (ukL_t)tmp;
    return ret != 0;
}
#elif __GNUC__
template <>
YM_FORCE_INLINE bool leading_zeros<u32_t>(ukL_t& zeros, u32_t x){
    zeros = (ukL_t)__builtin_clz(x);
    return x != 0;
}
#else
static_assert(false, "No leading_zeros<u32_t> implementation.");
template <>
YM_FORCE_INLINE bool leading_zeros<u32_t>(ukL_t& zeros, u32_t x){
    const u32_t MASK = 0x80000000ull;

    if (x == 0){
        return false;
    }

    zeros = 0;
    while ((x & MASK) == 0){
        x <<= 1;
        zeros++;
    }

    return true;
}
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  64-bit (Branch on Zero)
#if 0
#elif _WIN64 && _MSC_VER
template <>
YM_FORCE_INLINE bool leading_zeros<u64_t>(ukL_t& zeros, u64_t x){
    unsigned long tmp;
    char ret = _BitScanReverse64(&tmp, x);
    zeros = 63 - (ukL_t)tmp;
    return ret != 0;
}
#elif __GNUC__
template <>
YM_FORCE_INLINE bool leading_zeros<u64_t>(ukL_t& zeros, u64_t x){
    zeros = (ukL_t)__builtin_clzll(x);
    return x != 0;
}
#elif _MSC_VER
template <>
YM_FORCE_INLINE bool leading_zeros<u64_t>(ukL_t& zeros, u64_t x){
    unsigned long tmp;
    char ret = _BitScanReverse(&tmp, (u32_t)(x >> 32));
    if (ret){
        zeros = 31 - (ukL_t)tmp;
        return ret != 0;
    }
    ret = _BitScanReverse(&tmp, (u32_t)x);
    zeros = 63 - (ukL_t)tmp;
    return ret != 0;
}
#else
static_assert(false, "No leading_zeros<u64_t> implementation.");
template <>
YM_FORCE_INLINE bool leading_zeros<u64_t>(ukL_t& zeros, u64_t x){
    const u64_t MASK = 0x8000000000000000ull;

    if (x == 0){
        return false;
    }

    zeros = 0;
    while ((x & MASK) == 0){
        x <<= 1;
        zeros++;
    }

    return true;
}
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  64-bit (Integer width on Zero)
#if 0
#elif defined YMP_Arch_2013_x64_Haswell
template <>
YM_FORCE_INLINE ukL_t leading_zeros<u64_t>(u64_t x){
    return (ukL_t)_lzcnt_u64(x);
}
#elif _WIN64 && _MSC_VER
template <>
YM_FORCE_INLINE ukL_t leading_zeros<u64_t>(u64_t x){
    unsigned long zeros;
    char ret = _BitScanReverse64(&zeros, x);
    return ret != 0 ? 63 - (ukL_t)zeros : 64;
}
#elif __GNUC__
template <>
YM_FORCE_INLINE ukL_t leading_zeros<u64_t>(u64_t x){
    ukL_t zeros = (ukL_t)__builtin_clzll(x);
    return x != 0 ? zeros : 64;
}
#elif _MSC_VER
template <>
YM_FORCE_INLINE ukL_t leading_zeros<u64_t>(u64_t x){
    u32_t hi = (u32_t)(x >> 32);
    ukL_t zeros = 0;
    if (hi == 0){
        zeros = 32;
        hi = (u32_t)x;
    }
    unsigned long tmp;
    char ret = _BitScanReverse(&tmp, hi);
    return zeros + (ret != 0 ? 31 - (ukL_t)tmp : 32);
}
#else
static_assert(false, "No leading_zeros<u64_t> implementation.");
template <>
YM_FORCE_INLINE ukL_t leading_zeros<u64_t>(u64_t x){
    const u64_t MASK = 0x8000000000000000ull;

    if (x == 0){
        return 64;
    }

    ukL_t zeros = 0;
    while ((x & MASK) == 0){
        x <<= 1;
        zeros++;
    }

    return zeros;
}
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
