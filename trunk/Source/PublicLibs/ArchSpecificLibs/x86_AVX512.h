/* x86_AVX512.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 09/03/2014
 * Last Modified    : 09/03/2014
 * 
 */

#pragma once
#ifndef ymp_ArchSpecific_x86_AVX512_H
#define ymp_ArchSpecific_x86_AVX512_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <immintrin.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#if !defined __INTEL_COMPILER && (defined _MSC_VER) && (_MSC_VER < 1914)
#error "Visual Studio 15.7 or later is required."
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  COMPILER-BUG-GCC: AVX512 Missing intrinsics
#if (!defined __INTEL_COMPILER) && (defined __GNUC__)
YM_FORCE_INLINE __m512i _mm512_zextsi256_si512(__m256i x){
    return _mm512_maskz_broadcast_i64x4(0x0f, x);
}
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  This is REALLY stupid...
#define _mm512_setr_epi16(  \
    e00, e01, e02, e03, e04, e05, e06, e07, \
    e08, e09, e10, e11, e12, e13, e14, e15, \
    e16, e17, e18, e19, e20, e21, e22, e23, \
    e24, e25, e26, e27, e28, e29, e30, e31  \
)   \
    _mm512_setr_epi32(  \
        (u16_t)(e00) | ((u32_t)(e01) << 16), (u16_t)(e02) | ((u32_t)(e03) << 16), (u16_t)(e04) | ((u32_t)(e05) << 16), (u16_t)(e06) | ((u32_t)(e07) << 16),  \
        (u16_t)(e08) | ((u32_t)(e09) << 16), (u16_t)(e10) | ((u32_t)(e11) << 16), (u16_t)(e12) | ((u32_t)(e13) << 16), (u16_t)(e14) | ((u32_t)(e15) << 16),  \
        (u16_t)(e16) | ((u32_t)(e17) << 16), (u16_t)(e18) | ((u32_t)(e19) << 16), (u16_t)(e20) | ((u32_t)(e21) << 16), (u16_t)(e22) | ((u32_t)(e23) << 16),  \
        (u16_t)(e24) | ((u32_t)(e25) << 16), (u16_t)(e26) | ((u32_t)(e27) << 16), (u16_t)(e28) | ((u32_t)(e29) << 16), (u16_t)(e30) | ((u32_t)(e31) << 16)   \
    )
#define _mm512_setr_epi8(   \
    e00, e01, e02, e03, e04, e05, e06, e07, \
    e08, e09, e10, e11, e12, e13, e14, e15, \
    e16, e17, e18, e19, e20, e21, e22, e23, \
    e24, e25, e26, e27, e28, e29, e30, e31, \
    e32, e33, e34, e35, e36, e37, e38, e39, \
    e40, e41, e42, e43, e44, e45, e46, e47, \
    e48, e49, e50, e51, e52, e53, e54, e55, \
    e56, e57, e58, e59, e60, e61, e62, e63  \
)   \
    _mm512_setr_epi16(  \
        (e00) | ((e01) << 8), (e02) | ((e03) << 8), (e04) | ((e05) << 8), (e06) | ((e07) << 8), \
        (e08) | ((e09) << 8), (e10) | ((e11) << 8), (e12) | ((e13) << 8), (e14) | ((e15) << 8), \
        (e16) | ((e17) << 8), (e18) | ((e19) << 8), (e20) | ((e21) << 8), (e22) | ((e23) << 8), \
        (e24) | ((e25) << 8), (e26) | ((e27) << 8), (e28) | ((e29) << 8), (e30) | ((e31) << 8), \
        (e32) | ((e33) << 8), (e34) | ((e35) << 8), (e36) | ((e37) << 8), (e38) | ((e39) << 8), \
        (e40) | ((e41) << 8), (e42) | ((e43) << 8), (e44) | ((e45) << 8), (e46) | ((e47) << 8), \
        (e48) | ((e49) << 8), (e50) | ((e51) << 8), (e52) | ((e53) << 8), (e54) | ((e55) << 8), \
        (e56) | ((e57) << 8), (e58) | ((e59) << 8), (e60) | ((e61) << 8), (e62) | ((e63) << 8)  \
    )
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Bitwise Blend
//  For each bit, if the mask is 0, take the bit from a. Otherwise, take from b.
//
//  Premature Optimization:
//      The ternary logic instruction must overwrite the 1st operand. Furthermore,
//  the memory operand is restricted to the 3rd operand. Re-ordering of the
//  operands is possible, but requires that imm be updated. If we assume that
//  the compiler will be incapable of updating imm, we must do it manually if
//  we want the operands in a different order.
//
//  All of these macros do the same thing. But they use a different order of
//  operands for the vpternlogd instruction.
//
//  The naming convention follows that of the FMAs:
//      -   blend231    overwrite the mask, 2nd operand can be memory
//      -   blend213    overwrite the 1st operand, mask can be memory
//      -   blend132    overwrite the 1st operand, 2nd operand can be memory
//
//  The most common one will be 213 where we overwrite the 1st operand and
//  allow the mask to be taken from memory.
#define ih_AVX512_blend231_i64(a, b, mask)    _mm512_ternarylogic_epi64(mask, a, b, 0xac)
#define ih_AVX512_blend213_i64(a, b, mask)    _mm512_ternarylogic_epi64(a, b, mask, 0xd8)
#define ih_AVX512_blend132_i64(a, b, mask)    _mm512_ternarylogic_epi64(a, mask, b, 0xb8)
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#if 0
YM_FORCE_INLINE __m512i _mm512_permutex2var_epi8_AVX512BW(__m512i a, __m512i index, __m512i b){
    //  Emulation of AVX512-VBMI "_mm512_permutex2var_epi8()" using only AVX512-BW.
    __m512i i0 = _mm512_srli_epi16(index, 1);
    __m512i i1 = _mm512_srli_epi16(index, 9);

    __m512i p0 = _mm512_permutex2var_epi16(a, i0, b);
    __m512i p1 = _mm512_permutex2var_epi16(a, i1, b);

    i0 = _mm512_slli_epi16(index, 3);
    i1 = _mm512_srli_epi16(index, 5);

    i0 = _mm512_and_si512(i0, _mm512_set1_epi16(8));
    i1 = _mm512_andnot_si512(i1, _mm512_set1_epi16(8));

    p0 = _mm512_srlv_epi16(p0, i0);
    p1 = _mm512_sllv_epi16(p1, i1);

    return _mm512_mask_mov_epi8(p0, 0xaaaaaaaaaaaaaaaa, p1);
}
#endif
YM_FORCE_INLINE __m512i _mm512_cvtsi64_si512(u64_t x){
#if (!defined __INTEL_COMPILER) && (defined __GNUC__)
    //  COMPILER-BUG-GCC: AVX512 Missing intrinsics
    return _mm512_maskz_set1_epi64(1, x);
#else
    return _mm512_zextsi128_si512(_mm_cvtsi64_si128(x));
#endif
}
YM_FORCE_INLINE __m512i _mm512_loadl_epi64(const void* ptr){
#if (!defined __INTEL_COMPILER) && (defined __GNUC__)
    //  COMPILER-BUG-GCC: AVX512 Missing intrinsics
    return _mm512_maskz_set1_epi64(1, *(const u64_t*)ptr);
#else
    return _mm512_zextsi128_si512(_mm_loadl_epi64((const __m128i*)ptr));
#endif
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
