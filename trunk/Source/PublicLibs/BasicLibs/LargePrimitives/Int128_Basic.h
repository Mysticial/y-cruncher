/* Int128.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 01/20/2024
 *  Last Modified   : 05/11/2025
 * 
 */

#pragma once
#ifndef ymp_Int128_Basic_H
#define ymp_Int128_Basic_H
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
#include "PublicLibs/SystemLibs/ProcessorCapability/ProcessorCapability.h"
#include "Int128.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
inline UInt128 operator+(const UInt128& a, const UInt128& b){
#if 0
#elif defined YMP_Arch_2004_x64
    unsigned long long lo, hi;
    char carry = 0;
    carry = _addcarry_u64(carry, a.m_lo, b.m_lo, &lo);
    carry = _addcarry_u64(carry, a.m_hi, b.m_hi, &hi);
    return UInt128(lo, hi);
#else
    u64_t lo = a.m_lo + b.m_lo;
    u64_t hi = a.m_hi + b.m_hi;
    hi += lo < a.m_lo ? 1 : 0;
    return UInt128(lo, hi);
#endif
}
inline UInt128 operator-(const UInt128& a, const UInt128& b){
#if 0
#elif defined YMP_Arch_2004_x64
    unsigned long long lo, hi;
    char carry = 0;
    carry = _subborrow_u64(carry, a.m_lo, b.m_lo, &lo);
    carry = _subborrow_u64(carry, a.m_hi, b.m_hi, &hi);
    return UInt128(lo, hi);
#else
    u64_t lo = a.m_lo - b.m_lo;
    u64_t hi = a.m_hi - b.m_hi;
    hi -= lo > a.m_lo ? 1 : 0;
    return UInt128(lo, hi);
#endif
}
inline UInt128 UInt128::mul128(u64_t a, u64_t b){
#if 0
    UInt128 ret;
    Intrinsics::mul(ret.m_lo, ret.m_hi, a, b);
    return ret;
#elif defined YMP_Arch_2013_x64_Haswell
    UInt128 ret;
    ret.m_lo = _mulx_u64(a, b, (unsigned long long*)&ret.m_hi);
    return ret;
#elif _M_X64
    UInt128 ret;
    ret.m_lo = _umul128(a, b, &ret.m_hi);
    return ret;
#elif __GNUC__
    unsigned __int128 temp = (unsigned __int128)a * b;
    return UInt128((u64_t)temp, (u64_t)(temp >> 64));
#else
    u64_t L, H;

    u32_t AL = (u32_t)a;
    u32_t BL = (u32_t)b;
    u32_t AH = (u32_t)(a >> 32);
    u32_t BH = (u32_t)(b >> 32);

    u64_t r0, r1;

    L  = (u64_t)AL * BL;
    r0 = (u64_t)AH * BL;
    r1 = (u64_t)AL * BH;
    H  = (u64_t)AH * BH;

    r0 += L >> 32;
    L  &= 0xffffffff;

    r1 += r0 & 0xffffffff;
    L  |= r1 << 32;
    H  += r0 >> 32;
    H  += r1 >> 32;

    return UInt128(L, H);
#endif
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
