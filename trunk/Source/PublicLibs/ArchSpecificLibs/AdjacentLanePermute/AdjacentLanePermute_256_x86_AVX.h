/* AdjacentLanePermute_256_x86_AVX.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 08/30/2015
 *  Last Modified   : 03/08/2024
 * 
 *      New interface for Adjacent Lane Permutes.
 * 
 */

#pragma once
#ifndef ymp_AdjacentLanePermute_256_x86_AVX_H
#define ymp_AdjacentLanePermute_256_x86_AVX_H
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
//  Float64
template <int bit_granularity> YM_FORCE_INLINE __m256d permuteaj_00(__m256d a, __m256d b);
template <int bit_granularity> YM_FORCE_INLINE __m256d permuteaj_11(__m256d a, __m256d b);
template <int bit_granularity> YM_FORCE_INLINE __m256d permuteaj_01(__m256d a, __m256d b);
template <int bit_granularity> YM_FORCE_INLINE __m256d permuteaj_10(__m256d a, __m256d b);
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m256d permuteaj_00<64>(__m256d a, __m256d b){
    return _mm256_unpacklo_pd(a, b);
}
template <> YM_FORCE_INLINE
__m256d permuteaj_11<64>(__m256d a, __m256d b){
    return _mm256_unpackhi_pd(a, b);
}
template <> YM_FORCE_INLINE
__m256d permuteaj_01<64>(__m256d a, __m256d b){
    return _mm256_blend_pd(a, b, 10);
}
template <> YM_FORCE_INLINE
__m256d permuteaj_10<64>(__m256d a, __m256d b){
    return _mm256_shuffle_pd(a, b, 5);
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m256d permuteaj_00<128>(__m256d a, __m256d b){
    return _mm256_permute2f128_pd(a, b, 32);
}
template <> YM_FORCE_INLINE
__m256d permuteaj_11<128>(__m256d a, __m256d b){
    return _mm256_permute2f128_pd(a, b, 49);
}
template <> YM_FORCE_INLINE
__m256d permuteaj_01<128>(__m256d a, __m256d b){
    return _mm256_blend_pd(a, b, 0xc);
}
template <> YM_FORCE_INLINE
__m256d permuteaj_10<128>(__m256d a, __m256d b){
    return _mm256_permute2f128_pd(a, b, 33);
}
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void splitload(__m256d& x, const void* L, const void* H){
    x = _mm256_loadu2_m128d((const double*)(H), (const double*)(L));
}
YM_FORCE_INLINE void splitstore(__m256d x, void* L, void* H){
    _mm256_storeu2_m128d((double*)(H), (double*)(L), x);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Float32
template <int bit_granularity> YM_FORCE_INLINE __m256 permuteaj_00(__m256 a, __m256 b);
template <int bit_granularity> YM_FORCE_INLINE __m256 permuteaj_11(__m256 a, __m256 b);
template <int bit_granularity> YM_FORCE_INLINE __m256 permuteaj_01(__m256 a, __m256 b);
template <int bit_granularity> YM_FORCE_INLINE __m256 permuteaj_10(__m256 a, __m256 b);
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m256 permuteaj_00<32>(__m256 a, __m256 b){
#if (defined YMP_Arch_2017_x64_Skylake) || (defined YMP_Arch_20XX_x64_AVX10_2_256)
    return _mm256_castsi256_ps(_mm256_mask_shuffle_epi32(_mm256_castps_si256(a), 0xaa, _mm256_castps_si256(b), _MM_PERM_CDAB));
#else
    b = _mm256_shuffle_ps(b, b, 177);
    return _mm256_blend_ps(a, b, 0xaa);
#endif
}
template <> YM_FORCE_INLINE
__m256 permuteaj_11<32>(__m256 a, __m256 b){
#if (defined YMP_Arch_2017_x64_Skylake) || (defined YMP_Arch_20XX_x64_AVX10_2_256)
    return _mm256_castsi256_ps(_mm256_mask_shuffle_epi32(_mm256_castps_si256(b), 0x55, _mm256_castps_si256(a), _MM_PERM_CDAB));
#else
    a = _mm256_shuffle_ps(a, a, 177);
    return _mm256_blend_ps(a, b, 0xaa);
#endif
}
template <> YM_FORCE_INLINE
__m256 permuteaj_01<32>(__m256 a, __m256 b){
    return _mm256_blend_ps(a, b, 0xaa);
}
template <> YM_FORCE_INLINE
__m256 permuteaj_10<32>(__m256 a, __m256 b){
#if (defined YMP_Arch_2019_x64_IceLake) || (defined YMP_Arch_20XX_x64_AVX10_2_256)
    return _mm256_castsi256_ps(_mm256_shrdi_epi64(_mm256_castps_si256(a), _mm256_castps_si256(b), 32));
#elif defined YMP_Arch_2017_x64_Skylake
    return _mm256_permutex2var_ps(a, _mm256_setr_epi32(  1, 8, 3, 10, 5, 12, 7, 14), b);
#else
    b = _mm256_blend_ps(b, a, 0xaa);
    return _mm256_shuffle_ps(b, b, 177);
#endif
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m256 permuteaj_00<64>(__m256 a, __m256 b){
    return _mm256_shuffle_ps(a, b, 68);
}
template <> YM_FORCE_INLINE
__m256 permuteaj_11<64>(__m256 a, __m256 b){
    return _mm256_shuffle_ps(a, b, 238);
}
template <> YM_FORCE_INLINE
__m256 permuteaj_01<64>(__m256 a, __m256 b){
    return _mm256_blend_ps(a, b, 0xcc);
}
template <> YM_FORCE_INLINE
__m256 permuteaj_10<64>(__m256 a, __m256 b){
    return _mm256_shuffle_ps(a, b, 78);
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m256 permuteaj_00<128>(__m256 a, __m256 b){
    return _mm256_permute2f128_ps(a, b, 32);
}
template <> YM_FORCE_INLINE
__m256 permuteaj_11<128>(__m256 a, __m256 b){
    return _mm256_permute2f128_ps(a, b, 49);
}
template <> YM_FORCE_INLINE
__m256 permuteaj_01<128>(__m256 a, __m256 b){
    return _mm256_blend_ps(a, b, 0xf0);
}
template <> YM_FORCE_INLINE
__m256 permuteaj_10<128>(__m256 a, __m256 b){
    return _mm256_permute2f128_ps(a, b, 33);
}
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void splitload(__m256& x, const void* L, const void* H){
    x = _mm256_loadu2_m128((const float*)(H), (const float*)(L));
}
YM_FORCE_INLINE void splitstore(__m256 x, void* L, void* H){
    _mm256_storeu2_m128((float*)(H), (float*)(L), x);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
