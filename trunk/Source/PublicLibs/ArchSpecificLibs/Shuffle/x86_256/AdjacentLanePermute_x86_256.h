/* AdjacentLanePermute_x86_AVX.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/30/2015
 * Last Modified    : 06/22/2019
 * 
 */

#pragma once
#ifndef ymp_AdjacentLanePermute_x86_256_H
#define ymp_AdjacentLanePermute_x86_256_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <immintrin.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/ArchSpecificLibs/x86_AVX.h"
#include "PublicLibs/SystemLibs/ProcessorCapability/ProcessorCapability_x86.h"
#ifdef YMP_Arch_2013_x64_Haswell
#include "PublicLibs/ArchSpecificLibs/x86_AVX2.h"
#endif
namespace ymp{
namespace SIMD{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Double
YM_FORCE_INLINE __m256d mm256_permuteaj64_00(__m256d a, __m256d b){
    return _mm256_unpacklo_pd(a, b);
}
YM_FORCE_INLINE __m256d mm256_permuteaj64_11(__m256d a, __m256d b){
    return _mm256_unpackhi_pd(a, b);
}
YM_FORCE_INLINE __m256d mm256_permuteaj64_01(__m256d a, __m256d b){
    return _mm256_blend_pd(a, b, 10);
}
YM_FORCE_INLINE __m256d mm256_permuteaj64_10(__m256d a, __m256d b){
    return _mm256_shuffle_pd(a, b, 5);
}
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE __m256d mm256_permuteaj128_00(__m256d a, __m256d b){
    return _mm256_permute2f128_pd(a, b, 32);
}
YM_FORCE_INLINE __m256d mm256_permuteaj128_11(__m256d a, __m256d b){
    return _mm256_permute2f128_pd(a, b, 49);
}
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE __m256d mm256_splitload_pd(const void* L, const void* H){
    return _mm256_loadu2_m128d((const double*)(H), (const double*)(L));
}
YM_FORCE_INLINE void mm256_splitstore_pd(void* L, void* H, __m256d x){
    _mm256_storeu2_m128d((double*)(H), (double*)(L), x);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Float
YM_FORCE_INLINE __m256 mm256_splitload_ps(const void* L, const void* H){
    return _mm256_loadu2_m128((const float*)(H), (const float*)(L));
}
YM_FORCE_INLINE void mm256_splitstore_ps(void* L, void* H, __m256 x){
    _mm256_storeu2_m128((float*)(H), (float*)(L), x);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Integer
#ifdef YMP_Arch_2013_x64_Haswell
YM_FORCE_INLINE __m256i mm256_permuteaj32_00(__m256i a, __m256i b){
    b = _mm256_shuffle_epi32(b, 177);
    return _mm256_blend_epi16(a, b, 0xcc);
}
YM_FORCE_INLINE __m256i mm256_permuteaj32_11(__m256i a, __m256i b){
    a = _mm256_shuffle_epi32(a, 177);
    return _mm256_blend_epi16(a, b, 0xcc);
}
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE __m256i mm256_permuteaj64_00(__m256i a, __m256i b){
    return _mm256_unpacklo_epi64(a, b);
}
YM_FORCE_INLINE __m256i mm256_permuteaj64_11(__m256i a, __m256i b){
    return _mm256_unpackhi_epi64(a, b);
}
YM_FORCE_INLINE __m256i mm256_permuteaj64_01(__m256i a, __m256i b){
    return _mm256_blend_epi32(a, b, 0xcc);
}
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE __m256i mm256_permuteaj128_00(__m256i a, __m256i b){
    return _mm256_permute2x128_si256(a, b, 32);
}
YM_FORCE_INLINE __m256i mm256_permuteaj128_11(__m256i a, __m256i b){
    return _mm256_permute2x128_si256(a, b, 49);
}
YM_FORCE_INLINE __m256i mm256_permuteaj128_01(__m256i a, __m256i b){
    return _mm256_blend_epi32(a, b, 0xf0);
}
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE __m256i mm256_splitload_si256(const void* L, const void* H){
    return _mm256_loadu2_m128i((const __m128i*)(H), (const __m128i*)(L));
}
YM_FORCE_INLINE void mm256_splitstore_si256(void* L, void* H, __m256i x){
    _mm256_storeu2_m128i((__m128i*)(H), (__m128i*)(L), x);
}
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
