/* Transpose_64x8x8_x86_AVX512.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 08/30/2015
 *  Last Modified   : 03/10/2024
 * 
 */

#pragma once
#ifndef ymp_Transpose_64x8x8_x86_AVX512_H
#define ymp_Transpose_64x8x8_x86_AVX512_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <immintrin.h>
#include "PublicLibs/CompilerSettings.h"
namespace ymp{
namespace SIMD{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void transpose_i64x8x8_x86_AVX512(
    __m512i& r0, __m512i& r1, __m512i& r2, __m512i& r3,
    __m512i& r4, __m512i& r5, __m512i& r6, __m512i& r7
){
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
}
YM_FORCE_INLINE void transpose_f64x8x8_x86_AVX512(
    __m512d& r0, __m512d& r1, __m512d& r2, __m512d& r3,
    __m512d& r4, __m512d& r5, __m512d& r6, __m512d& r7
){
    __m512d a0, a1;

    a0 = _mm512_unpackhi_pd(r0, r1);
    r1 = _mm512_unpacklo_pd(r0, r1);
    a1 = _mm512_unpackhi_pd(r2, r3);
    r2 = _mm512_unpacklo_pd(r2, r3);
    r0 = _mm512_unpackhi_pd(r4, r5);
    r4 = _mm512_unpacklo_pd(r4, r5);
    r5 = _mm512_unpacklo_pd(r6, r7);
    r7 = _mm512_unpackhi_pd(r6, r7);

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
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
