/* AdjacentLanePermute_x86_512.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/30/2015
 * Last Modified    : 06/22/2019
 * 
 */

#pragma once
#ifndef ymp_AdjacentLanePermute_x86_512_H
#define ymp_AdjacentLanePermute_x86_512_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <immintrin.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/SystemLibs/ProcessorCapability/ProcessorCapability.h"
namespace ymp{
namespace SIMD{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifndef X64_16_KnightsLanding
#define ymp_SIMD_fast_2_input_permute
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Double
YM_FORCE_INLINE __m512d mm512_permuteaj64_00(__m512d a, __m512d b){
    return _mm512_unpacklo_pd(a, b);
}
YM_FORCE_INLINE __m512d mm512_permuteaj64_11(__m512d a, __m512d b){
    return _mm512_unpackhi_pd(a, b);
}
YM_FORCE_INLINE __m512d mm512_permuteaj64_01(__m512d a, __m512d b){
    return _mm512_mask_blend_pd(0xaa, a, b);
}
YM_FORCE_INLINE __m512d mm512_permuteaj64_10(__m512d a, __m512d b){
    return _mm512_shuffle_pd(a, b, 85);
}
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE __m512d mm512_permuteaj128_00(__m512d a, __m512d b){
    return _mm512_mask_permutex_pd(a, 0xcc, b, 78);
}
YM_FORCE_INLINE __m512d mm512_permuteaj128_11(__m512d a, __m512d b){
    return _mm512_mask_permutex_pd(b, 0x33, a, 78);
}
YM_FORCE_INLINE __m512d mm512_permuteaj128_01(__m512d a, __m512d b){
    return _mm512_mask_blend_pd(0xcc, a, b);
}
YM_FORCE_INLINE __m512d mm512_permuteaj128_10(__m512d a, __m512d b){
    return _mm512_permutex2var_pd(a, _mm512_setr_epi64(  2,  3,  8,  9,  6,  7, 12, 13), b);
}
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE __m512d mm512_permuteaj256_00(__m512d a, __m512d b){
    return _mm512_insertf64x4(a, _mm512_castpd512_pd256(b), 1); //  Faster on Knights Landing
//    return _mm512_shuffle_f64x2(a, b,  68);
}
YM_FORCE_INLINE __m512d mm512_permuteaj256_11(__m512d a, __m512d b){
    return _mm512_shuffle_f64x2(a, b, 238);
}
YM_FORCE_INLINE __m512d mm512_permuteaj256_01(__m512d a, __m512d b){
    return _mm512_mask_blend_pd(0xf0, a, b);
}
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE __m512d mm512_splitload_pd(const void* L, const void* H){
    __m512d x = _mm512_castpd256_pd512(_mm256_loadu_pd((const double*)L));
    return _mm512_insertf64x4(x, _mm256_loadu_pd((const double*)H), 1);
} 
YM_FORCE_INLINE void mm512_splitstore_pd(void* L, void* H, __m512d x){
    _mm256_storeu_pd((double*)L, _mm512_castpd512_pd256(x));
    _mm256_storeu_pd((double*)H, _mm512_extractf64x4_pd(x, 1));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Integer
YM_FORCE_INLINE __m512i mm512_permuteaj32_00(__m512i a, __m512i b){
    return _mm512_mask_shuffle_epi32(a, 0xaaaa, b, _MM_PERM_CDAB);
}
YM_FORCE_INLINE __m512i mm512_permuteaj32_11(__m512i a, __m512i b){
    return _mm512_mask_shuffle_epi32(b, 0x5555, a, _MM_PERM_CDAB);
}
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE __m512i mm512_permuteaj64_00(__m512i a, __m512i b){
#ifdef ymp_SIMD_fast_2_input_permute
    return _mm512_unpacklo_epi64(a, b);
#else
    return _mm512_mask_permutex_epi64(a, 0xaa, b, 177);
#endif
}
YM_FORCE_INLINE __m512i mm512_permuteaj64_11(__m512i a, __m512i b){
#ifdef ymp_SIMD_fast_2_input_permute
    return _mm512_unpackhi_epi64(a, b);
#else
    return _mm512_mask_permutex_epi64(b, 0x55, a, 177);
#endif
}
YM_FORCE_INLINE __m512i mm512_permuteaj64_01(__m512i a, __m512i b){
    return _mm512_mask_blend_epi64(0xaa, a, b);
}
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE __m512i mm512_permuteaj128_00(__m512i a, __m512i b){
    return _mm512_mask_permutex_epi64(a, 0xcc, b, 78);
}
YM_FORCE_INLINE __m512i mm512_permuteaj128_11(__m512i a, __m512i b){
    return _mm512_mask_permutex_epi64(b, 0x33, a, 78);
}
YM_FORCE_INLINE __m512i mm512_permuteaj128_01(__m512i a, __m512i b){
    return _mm512_mask_blend_epi64(0xcc, a, b);
}
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE __m512i mm512_permuteaj256_00(__m512i a, __m512i b){
    return _mm512_inserti64x4(a, _mm512_castsi512_si256(b), 1);
//    return _mm512_shuffle_i64x2(a, b,  68);
}
YM_FORCE_INLINE __m512i mm512_permuteaj256_11(__m512i a, __m512i b){
    return _mm512_shuffle_i64x2(a, b, 238);
}
YM_FORCE_INLINE __m512i mm512_permuteaj256_01(__m512i a, __m512i b){
    return _mm512_mask_blend_epi64(0xf0, a, b);
}
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE __m512i mm512_splitload_si512(const void* L, const void* H){
    __m512i x = _mm512_castsi256_si512(_mm256_loadu_si256((const __m256i*)L));
    return _mm512_inserti64x4(x, _mm256_loadu_si256((const __m256i*)H), 1);
}
YM_FORCE_INLINE void mm512_splitstore_si512(void* L, void* H, __m512i x){
    _mm256_storeu_si256((__m256i*)L, _mm512_castsi512_si256(x));
    _mm256_storeu_si256((__m256i*)H, _mm512_extracti64x4_epi64(x, 1));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
