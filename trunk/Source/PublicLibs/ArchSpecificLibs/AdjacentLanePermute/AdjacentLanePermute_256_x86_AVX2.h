/* AdjacentLanePermute_256_x86_AVX2.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 08/30/2015
 *  Last Modified   : 03/08/2024
 * 
 *      New interface for Adjacent Lane Permutes.
 * 
 */

#pragma once
#ifndef ymp_AdjacentLanePermute_256_x86_AVX2_H
#define ymp_AdjacentLanePermute_256_x86_AVX2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <immintrin.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "PublicLibs/SystemLibs/ProcessorCapability/ProcessorCapability.h"
namespace ymp{
namespace SIMD{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Integer
template <int bit_granularity> YM_FORCE_INLINE __m256i permuteaj_00(__m256i a, __m256i b);
template <int bit_granularity> YM_FORCE_INLINE __m256i permuteaj_11(__m256i a, __m256i b);
template <int bit_granularity> YM_FORCE_INLINE __m256i permuteaj_01(__m256i a, __m256i b);
template <int bit_granularity> YM_FORCE_INLINE __m256i permuteaj_10(__m256i a, __m256i b);
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m256i permuteaj_00<16>(__m256i a, __m256i b){
#ifdef YMP_Arch_2017_x64_Skylake
    return _mm256_mask_shuffle_epi8(a, 0xcccccccc, b, _mm256_setr_epi8(
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13,
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13
    ));
#else
    b = _mm256_shuffle_epi8(b, _mm256_setr_epi8(
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13,
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13
    ));
    return _mm256_blend_epi16(a, b, 0xaa);
#endif
}
template <> YM_FORCE_INLINE
__m256i permuteaj_11<16>(__m256i a, __m256i b){
#ifdef YMP_Arch_2017_x64_Skylake
    return _mm256_mask_shuffle_epi8(b, 0x33333333, a, _mm256_setr_epi8(
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13,
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13
    ));
#else
    a = _mm256_shuffle_epi8(a, _mm256_setr_epi8(
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13,
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13
    ));
    return _mm256_blend_epi16(a, b, 0xaa);
#endif
}
template <> YM_FORCE_INLINE
__m256i permuteaj_01<16>(__m256i a, __m256i b){
    return _mm256_blend_epi16(a, b, 0xaa);
}
template <> YM_FORCE_INLINE
__m256i permuteaj_10<16>(__m256i a, __m256i b){
#ifdef YMP_Arch_2019_x64_IceLake
    return _mm256_shrdi_epi32(a, b, 16);
#else
    a = _mm256_blend_epi16(a, b, 0x55);
    a = _mm256_shuffle_epi8(a, _mm256_setr_epi8(
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13,
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13
    ));
    return a;
#endif
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m256i permuteaj_00<32>(__m256i a, __m256i b){
#if (defined YMP_Arch_2017_x64_Skylake) || (defined YMP_Arch_20XX_x64_AVX10_2_256)
    return _mm256_mask_shuffle_epi32(a, 0xaa, b, _MM_PERM_CDAB);
#else
    b = _mm256_shuffle_epi32(b, 177);
    return _mm256_blend_epi16(a, b, 0xcc);
#endif
}
template <> YM_FORCE_INLINE
__m256i permuteaj_11<32>(__m256i a, __m256i b){
#if (defined YMP_Arch_2017_x64_Skylake) || (defined YMP_Arch_20XX_x64_AVX10_2_256)
    return _mm256_mask_shuffle_epi32(b, 0x55, a, _MM_PERM_CDAB);
#else
    a = _mm256_shuffle_epi32(a, 177);
    return _mm256_blend_epi16(a, b, 0xcc);
#endif
}
template <> YM_FORCE_INLINE
__m256i permuteaj_01<32>(__m256i a, __m256i b){
    return _mm256_blend_epi32(a, b, 0xaa);
}
template <> YM_FORCE_INLINE
__m256i permuteaj_10<32>(__m256i a, __m256i b){
#if (defined YMP_Arch_2019_x64_IceLake) || (defined YMP_Arch_20XX_x64_AVX10_2_256)
    return _mm256_shrdi_epi64(a, b, 32);
#elif defined YMP_Arch_2017_x64_Skylake
    return _mm256_permutex2var_epi32(a, _mm256_setr_epi32(  1, 8, 3, 10, 5, 12, 7, 14), b);
#else
    a = _mm256_blend_epi32(a, b, 0x55);
    return _mm256_shuffle_epi32(a, 177);
#endif
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m256i permuteaj_00<64>(__m256i a, __m256i b){
    return _mm256_unpacklo_epi64(a, b);
}
template <> YM_FORCE_INLINE
__m256i permuteaj_11<64>(__m256i a, __m256i b){
    return _mm256_unpackhi_epi64(a, b);
}
template <> YM_FORCE_INLINE
__m256i permuteaj_01<64>(__m256i a, __m256i b){
    return _mm256_blend_epi32(a, b, 0xcc);
}
template <> YM_FORCE_INLINE
__m256i permuteaj_10<64>(__m256i a, __m256i b){
    return _mm256_castpd_si256(_mm256_shuffle_pd(_mm256_castsi256_pd(a), _mm256_castsi256_pd(b), 5));
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m256i permuteaj_00<128>(__m256i a, __m256i b){
    return _mm256_permute2x128_si256(a, b, 32);
}
template <> YM_FORCE_INLINE
__m256i permuteaj_11<128>(__m256i a, __m256i b){
    return _mm256_permute2x128_si256(a, b, 49);
}
template <> YM_FORCE_INLINE
__m256i permuteaj_01<128>(__m256i a, __m256i b){
    return _mm256_blend_epi32(a, b, 0xf0);
}
template <> YM_FORCE_INLINE
__m256i permuteaj_10<128>(__m256i a, __m256i b){
    return _mm256_permute2x128_si256(a, b, 33);
}
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void splitload(__m256i& x, const void* L, const void* H){
    x = _mm256_loadu2_m128i((const __m128i*)(H), (const __m128i*)(L));
}
YM_FORCE_INLINE void splitstore(__m256i x, void* L, void* H){
    _mm256_storeu2_m128i((__m128i*)(H), (__m128i*)(L), x);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
