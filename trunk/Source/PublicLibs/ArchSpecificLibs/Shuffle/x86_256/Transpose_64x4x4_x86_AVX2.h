/* Transpose_64x4x4_x86_AVX2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/30/2015
 * Last Modified    : 01/20/2017
 * 
 */

#pragma once
#ifndef ymp_Transpose_64x4x4_x86_AVX2_H
#define ymp_Transpose_64x4x4_x86_AVX2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include "PublicLibs/Types.h"
#include "PublicLibs/ArchSpecificLibs/Shuffle/x86_256/AdjacentLanePermute_x86_256.h"
namespace ymp{
namespace SIMD{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void transpose_i64_4x4_AVX2(__m256i& r0, __m256i& r1, __m256i& r2, __m256i& r3){
    __m256i a0, a1, a2, a3;
    a0 = mm256_permuteaj128_00(r0, r2);
    a1 = mm256_permuteaj128_11(r0, r2);
    a2 = mm256_permuteaj128_00(r1, r3);
    a3 = mm256_permuteaj128_11(r1, r3);
    r0 = mm256_permuteaj64_00(a0, a2);
    r1 = mm256_permuteaj64_11(a0, a2);
    r2 = mm256_permuteaj64_00(a1, a3);
    r3 = mm256_permuteaj64_11(a1, a3);
}
YM_FORCE_INLINE void transpose_i64_4x4_forward_AVX2(
    const __m256i T[4],
    __m256i& r0, __m256i& r1, __m256i& r2, __m256i& r3
){
    __m256i a0, a1, a2, a3;
    a0 = mm256_splitload_si256((const u64_t*)T +  0, (const u64_t*)T +  8);
    a1 = mm256_splitload_si256((const u64_t*)T +  2, (const u64_t*)T + 10);
    a2 = mm256_splitload_si256((const u64_t*)T +  4, (const u64_t*)T + 12);
    a3 = mm256_splitload_si256((const u64_t*)T +  6, (const u64_t*)T + 14);
    r0 = mm256_permuteaj64_00(a0, a2);
    r1 = mm256_permuteaj64_11(a0, a2);
    r2 = mm256_permuteaj64_00(a1, a3);
    r3 = mm256_permuteaj64_11(a1, a3);
}
YM_FORCE_INLINE void transpose_i64_4x4_inverse_AVX2(
    __m256i T[4],
    __m256i& r0, __m256i& r1, __m256i& r2, __m256i& r3
){
    __m256i a0, a1, a2, a3;
    a0 = mm256_permuteaj128_00(r0, r2);
    a1 = mm256_permuteaj128_11(r0, r2);
    a2 = mm256_permuteaj128_00(r1, r3);
    a3 = mm256_permuteaj128_11(r1, r3);
    r0 = mm256_permuteaj64_00(a0, a2);
    r1 = mm256_permuteaj64_11(a0, a2);
    r2 = mm256_permuteaj64_00(a1, a3);
    r3 = mm256_permuteaj64_11(a1, a3);
    _mm256_store_si256(T + 0, r0);
    _mm256_store_si256(T + 1, r1);
    _mm256_store_si256(T + 2, r2);
    _mm256_store_si256(T + 3, r3);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
