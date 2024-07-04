/* AdjacentLanePermute_512_x86_AVX512.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 08/30/2015
 *  Last Modified   : 03/08/2024
 * 
 *      New interface for Adjacent Lane Permutes.
 * 
 */

#pragma once
#ifndef ymp_AdjacentLanePermute_512_x86_AVX512_H
#define ymp_AdjacentLanePermute_512_x86_AVX512_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <immintrin.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "PublicLibs/SystemLibs/ProcessorCapability/ProcessorCapability.h"
#include "PublicLibs/ArchSpecificLibs/x86_AVX512.h"
namespace ymp{
namespace SIMD{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Integer
template <int bit_granularity> YM_FORCE_INLINE __m512i permuteaj_00(__m512i a, __m512i b);
template <int bit_granularity> YM_FORCE_INLINE __m512i permuteaj_11(__m512i a, __m512i b);
template <int bit_granularity> YM_FORCE_INLINE __m512i permuteaj_01(__m512i a, __m512i b);
template <int bit_granularity> YM_FORCE_INLINE __m512i permuteaj_10(__m512i a, __m512i b);
////////////////////////////////////////////////////////////////////////////////
#ifdef YMP_Arch_2017_x64_Skylake
template <> YM_FORCE_INLINE
__m512i permuteaj_00<16>(__m512i a, __m512i b){
    return _mm512_mask_shuffle_epi8(a, 0xcccccccccccccccc, b, _mm512_setr_epi8(
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13,
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13,
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13,
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13
    ));
}
template <> YM_FORCE_INLINE
__m512i permuteaj_11<16>(__m512i a, __m512i b){
    return _mm512_mask_shuffle_epi8(b, 0x3333333333333333, a, _mm512_setr_epi8(
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13,
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13,
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13,
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13
    ));
}
template <> YM_FORCE_INLINE
__m512i permuteaj_01<16>(__m512i a, __m512i b){
    return _mm512_mask_blend_epi16(0xaaaaaaaa, a, b);
}
template <> YM_FORCE_INLINE
__m512i permuteaj_10<16>(__m512i a, __m512i b){
#ifdef YMP_Arch_2019_x64_IceLake
    return _mm512_shrdi_epi32(a, b, 16);
#else
    a = _mm512_mask_blend_epi16(0xaaaaaaaa, b, a);
    a = _mm512_shuffle_epi8(a, _mm512_setr_epi8(
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13,
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13,
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13,
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13
    ));
    return a;
#endif
}
#endif
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m512i permuteaj_00<32>(__m512i a, __m512i b){
    return _mm512_mask_shuffle_epi32(a, 0xaaaa, b, _MM_PERM_CDAB);
}
template <> YM_FORCE_INLINE
__m512i permuteaj_11<32>(__m512i a, __m512i b){
    return _mm512_mask_shuffle_epi32(b, 0x5555, a, _MM_PERM_CDAB);
}
template <> YM_FORCE_INLINE
__m512i permuteaj_01<32>(__m512i a, __m512i b){
    return _mm512_mask_blend_epi32(0xaaaa, a, b);
}
template <> YM_FORCE_INLINE
__m512i permuteaj_10<32>(__m512i a, __m512i b){
#ifdef YMP_Arch_2019_x64_IceLake
    return _mm512_shrdi_epi64(a, b, 32);
#else
    return _mm512_permutex2var_epi32(a, _mm512_setr_epi32(  1, 16, 3, 18, 5, 20, 7, 22, 9, 24, 11, 26, 13, 28, 15, 30), b);
#endif
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m512i permuteaj_00<64>(__m512i a, __m512i b){
    return _mm512_unpacklo_epi64(a, b);
}
template <> YM_FORCE_INLINE
__m512i permuteaj_11<64>(__m512i a, __m512i b){
    return _mm512_unpackhi_epi64(a, b);
}
template <> YM_FORCE_INLINE
__m512i permuteaj_01<64>(__m512i a, __m512i b){
    return _mm512_mask_blend_epi64(0xaa, a, b);
}
template <> YM_FORCE_INLINE
__m512i permuteaj_10<64>(__m512i a, __m512i b){
    return _mm512_castpd_si512(_mm512_shuffle_pd(_mm512_castsi512_pd(a), _mm512_castsi512_pd(b), 85));
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m512i permuteaj_00<128>(__m512i a, __m512i b){
    return _mm512_mask_permutex_epi64(a, 0xcc, b, 78);
}
template <> YM_FORCE_INLINE
__m512i permuteaj_11<128>(__m512i a, __m512i b){
    return _mm512_mask_permutex_epi64(b, 0x33, a, 78);
}
template <> YM_FORCE_INLINE
__m512i permuteaj_01<128>(__m512i a, __m512i b){
    return _mm512_mask_blend_epi64(0xcc, a, b);
}
template <> YM_FORCE_INLINE
__m512i permuteaj_10<128>(__m512i a, __m512i b){
    return _mm512_permutex2var_epi64(a, _mm512_setr_epi64(  2,  3,  8,  9,  6,  7, 12, 13), b);
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m512i permuteaj_00<256>(__m512i a, __m512i b){
    return _mm512_inserti64x4(a, _mm512_castsi512_si256(b), 1);
//    return _mm512_shuffle_i64x2(a, b,  68);
}
template <> YM_FORCE_INLINE
__m512i permuteaj_11<256>(__m512i a, __m512i b){
    return _mm512_shuffle_i64x2(a, b, 238);
}
template <> YM_FORCE_INLINE
__m512i permuteaj_01<256>(__m512i a, __m512i b){
    return _mm512_mask_blend_epi64(0xf0, a, b);
}
template <> YM_FORCE_INLINE
__m512i permuteaj_10<256>(__m512i a, __m512i b){
    return _mm512_shuffle_i64x2(a, b, 78);
}
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void splitload(__m512i& x, const void* L, const void* H){
    x = _mm512_castsi256_si512(_mm256_loadu_si256((const __m256i*)L));
    x = _mm512_inserti64x4(x, _mm256_loadu_si256((const __m256i*)H), 1);
}
YM_FORCE_INLINE void splitstore(__m512i x, void* L, void* H){
    _mm256_storeu_si256((__m256i*)L, _mm512_castsi512_si256(x));
    _mm256_storeu_si256((__m256i*)H, _mm512_extracti64x4_epi64(x, 1));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Float64
template <int bit_granularity> YM_FORCE_INLINE __m512d permuteaj_00(__m512d a, __m512d b);
template <int bit_granularity> YM_FORCE_INLINE __m512d permuteaj_11(__m512d a, __m512d b);
template <int bit_granularity> YM_FORCE_INLINE __m512d permuteaj_01(__m512d a, __m512d b);
template <int bit_granularity> YM_FORCE_INLINE __m512d permuteaj_10(__m512d a, __m512d b);
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m512d permuteaj_00<64>(__m512d a, __m512d b){
    return _mm512_unpacklo_pd(a, b);
}
template <> YM_FORCE_INLINE
__m512d permuteaj_11<64>(__m512d a, __m512d b){
    return _mm512_unpackhi_pd(a, b);
}
template <> YM_FORCE_INLINE
__m512d permuteaj_01<64>(__m512d a, __m512d b){
    return _mm512_mask_blend_pd(0xaa, a, b);
}
template <> YM_FORCE_INLINE
__m512d permuteaj_10<64>(__m512d a, __m512d b){
    return _mm512_shuffle_pd(a, b, 85);
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m512d permuteaj_00<128>(__m512d a, __m512d b){
    return _mm512_mask_permutex_pd(a, 0xcc, b, 78);
}
template <> YM_FORCE_INLINE
__m512d permuteaj_11<128>(__m512d a, __m512d b){
    return _mm512_mask_permutex_pd(b, 0x33, a, 78);
}
template <> YM_FORCE_INLINE
__m512d permuteaj_01<128>(__m512d a, __m512d b){
    return _mm512_mask_blend_pd(0xcc, a, b);
}
template <> YM_FORCE_INLINE
__m512d permuteaj_10<128>(__m512d a, __m512d b){
    return _mm512_permutex2var_pd(a, _mm512_setr_epi64(  2,  3,  8,  9,  6,  7, 12, 13), b);
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m512d permuteaj_00<256>(__m512d a, __m512d b){
    return _mm512_insertf64x4(a, _mm512_castpd512_pd256(b), 1);
//    return _mm512_shuffle_f64x2(a, b,  68);
}
template <> YM_FORCE_INLINE
__m512d permuteaj_11<256>(__m512d a, __m512d b){
    return _mm512_shuffle_f64x2(a, b, 238);
}
template <> YM_FORCE_INLINE
__m512d permuteaj_01<256>(__m512d a, __m512d b){
    return _mm512_mask_blend_pd(0xf0, a, b);
}
template <> YM_FORCE_INLINE
__m512d permuteaj_10<256>(__m512d a, __m512d b){
    return _mm512_shuffle_f64x2(a, b, 78);
}
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void splitload(__m512d& x, const void* L, const void* H){
    x = _mm512_castpd256_pd512(_mm256_loadu_pd((const double*)L));
    x = _mm512_insertf64x4(x, _mm256_loadu_pd((const double*)H), 1);
}
YM_FORCE_INLINE void splitstore(__m512d x, void* L, void* H){
    _mm256_storeu_pd((double*)L, _mm512_castpd512_pd256(x));
    _mm256_storeu_pd((double*)H, _mm512_extractf64x4_pd(x, 1));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Float32
template <int bit_granularity> YM_FORCE_INLINE __m512 permuteaj_00(__m512 a, __m512 b);
template <int bit_granularity> YM_FORCE_INLINE __m512 permuteaj_11(__m512 a, __m512 b);
template <int bit_granularity> YM_FORCE_INLINE __m512 permuteaj_01(__m512 a, __m512 b);
template <int bit_granularity> YM_FORCE_INLINE __m512 permuteaj_10(__m512 a, __m512 b);
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m512 permuteaj_00<32>(__m512 a, __m512 b){
    return _mm512_castsi512_ps(permuteaj_00<32>(_mm512_castps_si512(a), _mm512_castps_si512(b)));
}
template <> YM_FORCE_INLINE
__m512 permuteaj_11<32>(__m512 a, __m512 b){
    return _mm512_castsi512_ps(permuteaj_11<32>(_mm512_castps_si512(a), _mm512_castps_si512(b)));
}
template <> YM_FORCE_INLINE
__m512 permuteaj_01<32>(__m512 a, __m512 b){
    return _mm512_mask_blend_ps(0xaaaa, a, b);
}
template <> YM_FORCE_INLINE
__m512 permuteaj_10<32>(__m512 a, __m512 b){
    return _mm512_castsi512_ps(permuteaj_10<32>(_mm512_castps_si512(a), _mm512_castps_si512(b)));
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m512 permuteaj_00<64>(__m512 a, __m512 b){
    return _mm512_shuffle_ps(a, b, 68);
}
template <> YM_FORCE_INLINE
__m512 permuteaj_11<64>(__m512 a, __m512 b){
    return _mm512_shuffle_ps(a, b, 238);
}
template <> YM_FORCE_INLINE
__m512 permuteaj_01<64>(__m512 a, __m512 b){
    return _mm512_mask_blend_ps(0xcccc, a, b);
}
template <> YM_FORCE_INLINE
__m512 permuteaj_10<64>(__m512 a, __m512 b){
    return _mm512_shuffle_ps(a, b, 78);
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m512 permuteaj_00<128>(__m512 a, __m512 b){
    return _mm512_castsi512_ps(permuteaj_00<128>(_mm512_castps_si512(a), _mm512_castps_si512(b)));
}
template <> YM_FORCE_INLINE
__m512 permuteaj_11<128>(__m512 a, __m512 b){
    return _mm512_castsi512_ps(permuteaj_11<128>(_mm512_castps_si512(a), _mm512_castps_si512(b)));
}
template <> YM_FORCE_INLINE
__m512 permuteaj_01<128>(__m512 a, __m512 b){
    return _mm512_mask_blend_ps(0xf0f0, a, b);
}
template <> YM_FORCE_INLINE
__m512 permuteaj_10<128>(__m512 a, __m512 b){
    return _mm512_permutex2var_ps(a, _mm512_setr_epi32( 4, 5, 6, 7, 16, 17, 18, 19, 12, 13, 14, 15, 24, 25, 26, 27), b);
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m512 permuteaj_00<256>(__m512 a, __m512 b){
#ifdef YMP_Arch_2017_x64_Skylake
    return _mm512_insertf32x8(a, _mm512_castps512_ps256(b), 1);
#else
    return _mm512_castpd_ps(
        _mm512_insertf64x4(_mm512_castps_pd(a), _mm256_castps_pd(_mm512_castps512_ps256(b)), 1)
    );
#endif
//    return _mm512_shuffle_f32xx(a, b,  68);
}
template <> YM_FORCE_INLINE
__m512 permuteaj_11<256>(__m512 a, __m512 b){
    return _mm512_shuffle_f32x4(a, b, 238);
}
template <> YM_FORCE_INLINE
__m512 permuteaj_01<256>(__m512 a, __m512 b){
    return _mm512_mask_blend_ps(0xff00, a, b);
}
template <> YM_FORCE_INLINE
__m512 permuteaj_10<256>(__m512 a, __m512 b){
    return _mm512_shuffle_f32x4(a, b, 78);
}
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void splitload(__m512& x, const void* L, const void* H){
    x = _mm512_castps256_ps512(_mm256_loadu_ps((const float*)L));
#ifdef YMP_Arch_2017_x64_Skylake
    x = _mm512_insertf32x8(x, _mm256_loadu_ps((const float*)H), 1);
#else
    x = _mm512_castpd_ps(
        _mm512_insertf64x4(_mm512_castps_pd(x), _mm256_loadu_pd((const double*)H), 1)
    );
#endif
}
YM_FORCE_INLINE void splitstore(__m512 x, void* L, void* H){
    _mm256_storeu_ps((float*)L, _mm512_castps512_ps256(x));
#ifdef YMP_Arch_2017_x64_Skylake
    _mm256_storeu_ps((float*)H, _mm512_extractf32x8_ps(x, 1));
#else
    _mm256_storeu_pd((double*)H, _mm512_extractf64x4_pd(_mm512_castps_pd(x), 1));
#endif
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
