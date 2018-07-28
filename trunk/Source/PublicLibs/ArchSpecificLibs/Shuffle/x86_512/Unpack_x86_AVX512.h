/* Unpack_x86_AVX512.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/30/2015
 * Last Modified    : 08/30/2015
 * 
 */

#pragma once
#ifndef ymp_Unpack_x86_AVX512_H
#define ymp_Unpack_x86_AVX512_H
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
YM_FORCE_INLINE __m512d load2_m128d_AVX512(const void* L, const void* H){
    __m512d x = _mm512_castpd256_pd512(_mm256_loadu_pd((const double*)L));
    return _mm512_insertf64x4(x, *(__m256d*)H, 1);
} 
YM_FORCE_INLINE void store2_m128d_AVX512(void* L, void* H, __m512d x){
    _mm256_storeu_pd((double*)L, _mm512_castpd512_pd256(x));
    *(__m256d*)H = _mm512_extractf64x4_pd(x, 1);
}
YM_FORCE_INLINE __m512i load2_m256i_AVX512(const void* L, const void* H){
    __m512i x = _mm512_castsi256_si512(_mm256_loadu_si256((const __m256i*)L));
    return _mm512_inserti64x4(x, *(const __m256i*)H, 1);
}
YM_FORCE_INLINE void store2_m256i_AVX512(void* L, void* H, __m512i x){
    _mm256_storeu_si256((__m256i*)L, _mm512_castsi512_si256(x));
    *(__m256i*)H = _mm512_extracti64x4_epi64(x, 1);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Double Precision
template <int width> YM_FORCE_INLINE __m512d unpacklo_f64_AVX512(__m512d a, __m512d b);
template <int width> YM_FORCE_INLINE __m512d unpackhi_f64_AVX512(__m512d a, __m512d b);
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE __m512d unpacklo_f64_AVX512<1>(__m512d a, __m512d b){
#ifdef ymp_SIMD_fast_2_input_permute
    return _mm512_unpacklo_pd(a, b);
#else
    return _mm512_mask_permutex_pd(a, 0xaa, b, 177);
#endif
}
template <> YM_FORCE_INLINE __m512d unpackhi_f64_AVX512<1>(__m512d a, __m512d b){
#ifdef ymp_SIMD_fast_2_input_permute
    return _mm512_unpackhi_pd(a, b);
#else
    return _mm512_mask_permutex_pd(b, 0x55, a, 177);
#endif
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE __m512d unpacklo_f64_AVX512<2>(__m512d a, __m512d b){
    return _mm512_mask_permutex_pd(a, 0xcc, b, 78);
}
template <> YM_FORCE_INLINE __m512d unpackhi_f64_AVX512<2>(__m512d a, __m512d b){
    return _mm512_mask_permutex_pd(b, 0x33, a, 78);
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE __m512d unpacklo_f64_AVX512<4>(__m512d a, __m512d b){
    return _mm512_insertf64x4(a, _mm512_castpd512_pd256(b), 1);
//    return _mm512_shuffle_f64x2(a, b,  68);
}
template <> YM_FORCE_INLINE __m512d unpackhi_f64_AVX512<4>(__m512d a, __m512d b){
    return _mm512_shuffle_f64x2(a, b, 238);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Integer
template <int bits> YM_FORCE_INLINE __m512i unpacklo_AVX512(__m512i a, __m512i b);
template <int bits> YM_FORCE_INLINE __m512i unpackhi_AVX512(__m512i a, __m512i b);
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE __m512i unpacklo_AVX512<32>(__m512i a, __m512i b){
    return _mm512_mask_shuffle_epi32(a, 0xaaaa, b, _MM_PERM_CDAB);
}
template <> YM_FORCE_INLINE __m512i unpackhi_AVX512<32>(__m512i a, __m512i b){
    return _mm512_mask_shuffle_epi32(b, 0x5555, a, _MM_PERM_CDAB);
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE __m512i unpacklo_AVX512<64>(__m512i a, __m512i b){
#ifdef ymp_SIMD_fast_2_input_permute
    return _mm512_unpacklo_epi64(a, b);
#else
    return _mm512_mask_permutex_epi64(a, 0xaa, b, 177);
#endif
}
template <> YM_FORCE_INLINE __m512i unpackhi_AVX512<64>(__m512i a, __m512i b){
#ifdef ymp_SIMD_fast_2_input_permute
    return _mm512_unpackhi_epi64(a, b);
#else
    return _mm512_mask_permutex_epi64(b, 0x55, a, 177);
#endif
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE __m512i unpacklo_AVX512<128>(__m512i a, __m512i b){
    return _mm512_mask_permutex_epi64(a, 0xcc, b, 78);
}
template <> YM_FORCE_INLINE __m512i unpackhi_AVX512<128>(__m512i a, __m512i b){
    return _mm512_mask_permutex_epi64(b, 0x33, a, 78);
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE __m512i unpacklo_AVX512<256>(__m512i a, __m512i b){
    return _mm512_inserti64x4(a, _mm512_castsi512_si256(b), 1);
//    return _mm512_shuffle_i64x2(a, b,  68);
}
template <> YM_FORCE_INLINE __m512i unpackhi_AVX512<256>(__m512i a, __m512i b){
    return _mm512_shuffle_i64x2(a, b, 238);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Non-Uniform
YM_FORCE_INLINE __m512d shuffle_f64_01_AVX512(__m512d a, __m512d b){
    return _mm512_mask_blend_pd(0xaa, a, b);
}
YM_FORCE_INLINE __m512d shuffle_f64_10_AVX512(__m512d a, __m512d b){
    return _mm512_shuffle_pd(a, b, 85);
}
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE __m512d shuffle_f128_01_AVX512(__m512d a, __m512d b){
    return _mm512_mask_blend_pd(0xcc, a, b);
}
YM_FORCE_INLINE __m512d shuffle_f128_10_AVX512(__m512d a, __m512d b){
    return _mm512_permutex2var_pd(a, _mm512_setr_epi64(  2,  3,  8,  9,  6,  7, 12, 13), b);
}
YM_FORCE_INLINE __m512d shuffle_f128_00_AVX512(__m512d a, __m512d b){
    return _mm512_mask_permutex_pd(a, 0xcc, b, 78);
}
YM_FORCE_INLINE __m512d shuffle_f128_11_AVX512(__m512d a, __m512d b){
    return _mm512_mask_permutex_pd(b, 0x33, a, 78);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
