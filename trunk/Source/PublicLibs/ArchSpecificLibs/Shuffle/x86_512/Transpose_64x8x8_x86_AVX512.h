/* Transpose_64x8x8_x86_AVX512.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/30/2015
 * Last Modified    : 01/20/2017
 * 
 */

#pragma once
#ifndef ymp_Transpose_64x8x8_x86_AVX512_H
#define ymp_Transpose_64x8x8_x86_AVX512_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include "PublicLibs/Types.h"
#include "PublicLibs/ArchSpecificLibs/Shuffle/x86_512/AdjacentLanePermute_x86_512.h"
namespace ymp{
namespace SIMD{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void transpose_f64_8x8_AVX512(
    __m512d& r0, __m512d& r1, __m512d& r2, __m512d& r3,
    __m512d& r4, __m512d& r5, __m512d& r6, __m512d& r7
){
#ifdef ymp_SIMD_fast_2_input_permute
    __m512d a0, a1;

    a0 = mm512_permuteaj64_11(r0, r1);
    r1 = mm512_permuteaj64_00(r0, r1);
    a1 = mm512_permuteaj64_11(r2, r3);
    r2 = mm512_permuteaj64_00(r2, r3);
    r0 = mm512_permuteaj64_11(r4, r5);
    r4 = mm512_permuteaj64_00(r4, r5);
    r5 = mm512_permuteaj64_00(r6, r7);
    r7 = mm512_permuteaj64_11(r6, r7);

    r3 = _mm512_shuffle_f64x2(r1, r2, 221);
    r1 = _mm512_shuffle_f64x2(r1, r2, 136);
    r2 = _mm512_shuffle_f64x2(a0, a1, 136);
    a1 = _mm512_shuffle_f64x2(a0, a1, 221);
    r6 = _mm512_shuffle_f64x2(r4, r5, 221);
    r4 = _mm512_shuffle_f64x2(r4, r5, 136);
    r5 = _mm512_shuffle_f64x2(r0, r7, 136);
    r7 = _mm512_shuffle_f64x2(r0, r7, 221);

    r0 = _mm512_shuffle_f64x2(r1, r4, 136);
    r4 = _mm512_shuffle_f64x2(r1, r4, 221);
    r1 = _mm512_shuffle_f64x2(r2, r5, 136);
    r5 = _mm512_shuffle_f64x2(r2, r5, 221);
    r2 = _mm512_shuffle_f64x2(r3, r6, 136);
    r6 = _mm512_shuffle_f64x2(r3, r6, 221);
    r3 = _mm512_shuffle_f64x2(a1, r7, 136);
    r7 = _mm512_shuffle_f64x2(a1, r7, 221);
#else
    __m512d a0, a1, a2, a3, a4, a5, a6, a7;

    a0 = mm512_permuteaj256_00(r0, r4);
    a1 = mm512_permuteaj256_00(r1, r5);
    a2 = mm512_permuteaj256_00(r2, r6);
    a3 = mm512_permuteaj256_00(r3, r7);
    a4 = mm512_permuteaj256_11(r0, r4);
    a5 = mm512_permuteaj256_11(r1, r5);
    a6 = mm512_permuteaj256_11(r2, r6);
    a7 = mm512_permuteaj256_11(r3, r7);

    r0 = mm512_permuteaj128_00(a0, a2);
    r1 = mm512_permuteaj128_11(a0, a2);
    r2 = mm512_permuteaj128_00(a1, a3);
    r3 = mm512_permuteaj128_11(a1, a3);
    r4 = mm512_permuteaj128_00(a4, a6);
    r5 = mm512_permuteaj128_11(a4, a6);
    r6 = mm512_permuteaj128_00(a5, a7);
    r7 = mm512_permuteaj128_11(a5, a7);

    a0 = mm512_permuteaj64_00(r0, r2);
    a1 = mm512_permuteaj64_11(r0, r2);
    a2 = mm512_permuteaj64_00(r1, r3);
    a3 = mm512_permuteaj64_11(r1, r3);
    a4 = mm512_permuteaj64_00(r4, r6);
    a5 = mm512_permuteaj64_11(r4, r6);
    a6 = mm512_permuteaj64_00(r5, r7);
    a7 = mm512_permuteaj64_11(r5, r7);

    r0 = a0;
    r1 = a1;
    r2 = a2;
    r3 = a3;
    r4 = a4;
    r5 = a5;
    r6 = a6;
    r7 = a7;
#endif
}
YM_FORCE_INLINE void transpose_i64_8x8_AVX512(
    __m512i& r0, __m512i& r1, __m512i& r2, __m512i& r3,
    __m512i& r4, __m512i& r5, __m512i& r6, __m512i& r7
){
#ifdef ymp_SIMD_fast_2_input_permute
    __m512i a0, a1;

    a0 = _mm512_unpackhi_epi64(r0, r1);
    r1 = _mm512_unpacklo_epi64(r0, r1);
    a1 = _mm512_unpackhi_epi64(r2, r3);
    r2 = _mm512_unpacklo_epi64(r2, r3);
    r0 = _mm512_unpackhi_epi64(r4, r5);
    r4 = _mm512_unpacklo_epi64(r4, r5);
    r5 = _mm512_unpacklo_epi64(r6, r7);
    r7 = _mm512_unpackhi_epi64(r6, r7);

    r3 = _mm512_shuffle_i64x2(r1, r2, 221);
    r1 = _mm512_shuffle_i64x2(r1, r2, 136);
    r2 = _mm512_shuffle_i64x2(a0, a1, 136);
    a1 = _mm512_shuffle_i64x2(a0, a1, 221);
    r6 = _mm512_shuffle_i64x2(r4, r5, 221);
    r4 = _mm512_shuffle_i64x2(r4, r5, 136);
    r5 = _mm512_shuffle_i64x2(r0, r7, 136);
    r7 = _mm512_shuffle_i64x2(r0, r7, 221);

    r0 = _mm512_shuffle_i64x2(r1, r4, 136);
    r4 = _mm512_shuffle_i64x2(r1, r4, 221);
    r1 = _mm512_shuffle_i64x2(r2, r5, 136);
    r5 = _mm512_shuffle_i64x2(r2, r5, 221);
    r2 = _mm512_shuffle_i64x2(r3, r6, 136);
    r6 = _mm512_shuffle_i64x2(r3, r6, 221);
    r3 = _mm512_shuffle_i64x2(a1, r7, 136);
    r7 = _mm512_shuffle_i64x2(a1, r7, 221);
#else
    __m512i a0, a1, a2, a3, a4, a5, a6, a7;

    a0 = mm512_permuteaj256_00(r0, r4);
    a1 = mm512_permuteaj256_00(r1, r5);
    a2 = mm512_permuteaj256_00(r2, r6);
    a3 = mm512_permuteaj256_00(r3, r7);
    a4 = mm512_permuteaj256_11(r0, r4);
    a5 = mm512_permuteaj256_11(r1, r5);
    a6 = mm512_permuteaj256_11(r2, r6);
    a7 = mm512_permuteaj256_11(r3, r7);

    r0 = mm512_permuteaj128_00(a0, a2);
    r1 = mm512_permuteaj128_11(a0, a2);
    r2 = mm512_permuteaj128_00(a1, a3);
    r3 = mm512_permuteaj128_11(a1, a3);
    r4 = mm512_permuteaj128_00(a4, a6);
    r5 = mm512_permuteaj128_11(a4, a6);
    r6 = mm512_permuteaj128_00(a5, a7);
    r7 = mm512_permuteaj128_11(a5, a7);

    a0 = mm512_permuteaj64_00(r0, r2);
    a1 = mm512_permuteaj64_11(r0, r2);
    a2 = mm512_permuteaj64_00(r1, r3);
    a3 = mm512_permuteaj64_11(r1, r3);
    a4 = mm512_permuteaj64_00(r4, r6);
    a5 = mm512_permuteaj64_11(r4, r6);
    a6 = mm512_permuteaj64_00(r5, r7);
    a7 = mm512_permuteaj64_11(r5, r7);

    r0 = a0;
    r1 = a1;
    r2 = a2;
    r3 = a3;
    r4 = a4;
    r5 = a5;
    r6 = a6;
    r7 = a7;
#endif
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void transpose_i64_8x8_forward_AVX512(
    const u64_t T[64],
    __m512i& r0, __m512i& r1, __m512i& r2, __m512i& r3,
    __m512i& r4, __m512i& r5, __m512i& r6, __m512i& r7
){
    __m512i a0, a1, a2, a3, a4, a5, a6, a7;

    r0 = mm512_splitload_si512(T +  0, T + 16);
    r1 = mm512_splitload_si512(T +  4, T + 20);
    r2 = mm512_splitload_si512(T +  8, T + 24);
    r3 = mm512_splitload_si512(T + 12, T + 28);
    r4 = mm512_splitload_si512(T + 32, T + 48);
    r5 = mm512_splitload_si512(T + 36, T + 52);
    r6 = mm512_splitload_si512(T + 40, T + 56);
    r7 = mm512_splitload_si512(T + 44, T + 60);

    a0 = _mm512_shuffle_i64x2(r0, r4, 136);
    a1 = _mm512_shuffle_i64x2(r0, r4, 221);
    a2 = _mm512_shuffle_i64x2(r1, r5, 136);
    a3 = _mm512_shuffle_i64x2(r1, r5, 221);
    a4 = _mm512_shuffle_i64x2(r2, r6, 136);
    a5 = _mm512_shuffle_i64x2(r2, r6, 221);
    a6 = _mm512_shuffle_i64x2(r3, r7, 136);
    a7 = _mm512_shuffle_i64x2(r3, r7, 221);

    r0 = _mm512_unpacklo_epi64(a0, a4);
    r1 = _mm512_unpackhi_epi64(a0, a4);
    r2 = _mm512_unpacklo_epi64(a1, a5);
    r3 = _mm512_unpackhi_epi64(a1, a5);
    r4 = _mm512_unpacklo_epi64(a2, a6);
    r5 = _mm512_unpackhi_epi64(a2, a6);
    r6 = _mm512_unpacklo_epi64(a3, a7);
    r7 = _mm512_unpackhi_epi64(a3, a7);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void transpose_i64_8x8_forward_AVX512(
    const __m512i T[8],
    __m512i& r0, __m512i& r1, __m512i& r2, __m512i& r3,
    __m512i& r4, __m512i& r5, __m512i& r6, __m512i& r7
){
#if 1
    transpose_i64_8x8_forward_AVX512((const u64_t*)T, r0, r1, r2, r3, r4, r5, r6, r7);
#else
    r0 = _mm512_load_si512(T + 0);
    r1 = _mm512_load_si512(T + 1);
    r2 = _mm512_load_si512(T + 2);
    r3 = _mm512_load_si512(T + 3);
    r4 = _mm512_load_si512(T + 4);
    r5 = _mm512_load_si512(T + 5);
    r6 = _mm512_load_si512(T + 6);
    r7 = _mm512_load_si512(T + 7);
    transpose_i64_8x8_AVX512(r0, r1, r2, r3, r4, r5, r6, r7);
#endif
}
YM_FORCE_INLINE void transpose_i64_8x8_inverse_AVX512(
    __m512i T[8],
    __m512i& r0, __m512i& r1, __m512i& r2, __m512i& r3,
    __m512i& r4, __m512i& r5, __m512i& r6, __m512i& r7
){
    transpose_i64_8x8_AVX512(r0, r1, r2, r3, r4, r5, r6, r7);
    _mm512_store_si512(T + 0, r0);
    _mm512_store_si512(T + 1, r1);
    _mm512_store_si512(T + 2, r2);
    _mm512_store_si512(T + 3, r3);
    _mm512_store_si512(T + 4, r4);
    _mm512_store_si512(T + 5, r5);
    _mm512_store_si512(T + 6, r6);
    _mm512_store_si512(T + 7, r7);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
