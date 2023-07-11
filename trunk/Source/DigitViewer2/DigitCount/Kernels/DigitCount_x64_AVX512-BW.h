/* DigitCount_x64_AVX512-BW.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 01/13/2018
 *  Last Modified   : 01/13/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_DigitCount_x64_AVX512BW_H
#define ydv_DigitViewer_DigitCount_x64_AVX512BW_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <immintrin.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/ArchSpecificLibs/x86_AVX512.h"
#include "PublicLibs/ArchSpecificLibs/Shuffle/x86_512/Transpose_64x8x8_x86_AVX512.h"
namespace DigitViewer2{
namespace DigitCount{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE __m512i reduce_u8_to_u64_AVX512(__m512i x){
    __m512i r;

    r = _mm512_srli_epi16(x, 8);
    x = _mm512_and_si512(x, _mm512_set1_epi16(0x00ff));
    x = _mm512_add_epi16(x, r);

    r = _mm512_srli_epi32(x, 16);
    x = _mm512_and_si512(x, _mm512_set1_epi32(0x0000ffff));
    x = _mm512_add_epi32(x, r);

    r = _mm512_srli_epi64(x, 32);
    x = _mm512_and_si512(x, _mm512_set1_epi64(0x00000000ffffffff));
    x = _mm512_add_epi64(x, r);

    return x;
}
YM_FORCE_INLINE void accumulate_b64_AVX512_64x10(u64_t digits[10], const __m512i* raw_digits, upL_t blocks){
    const __m512i DIGIT0 = _mm512_set1_epi8(0);
    const __m512i DIGIT1 = _mm512_set1_epi8(1);
    const __m512i DIGIT2 = _mm512_set1_epi8(2);
    const __m512i DIGIT3 = _mm512_set1_epi8(3);
    const __m512i DIGIT4 = _mm512_set1_epi8(4);
    const __m512i DIGIT5 = _mm512_set1_epi8(5);
    const __m512i DIGIT6 = _mm512_set1_epi8(6);
    const __m512i DIGIT7 = _mm512_set1_epi8(7);
    const __m512i DIGIT8 = _mm512_set1_epi8(8);
    const __m512i DIGIT9 = _mm512_set1_epi8(9);

    __m512i sum0 = _mm512_setzero_si512();
    __m512i sum1 = _mm512_setzero_si512();
    __m512i sum2 = _mm512_setzero_si512();
    __m512i sum3 = _mm512_setzero_si512();
    __m512i sum4 = _mm512_setzero_si512();
    __m512i sum5 = _mm512_setzero_si512();
    __m512i sum6 = _mm512_setzero_si512();
    __m512i sum7 = _mm512_setzero_si512();
    __m512i sum8 = _mm512_setzero_si512();
    __m512i sum9 = _mm512_setzero_si512();

    const __m512i ONE = _mm512_set1_epi8(1);
    do{
        __m512i r0 = _mm512_load_si512(raw_digits);
        sum0 = _mm512_mask_add_epi8(sum0, _mm512_cmpeq_epi8_mask(r0, DIGIT0), sum0, ONE);
        sum1 = _mm512_mask_add_epi8(sum1, _mm512_cmpeq_epi8_mask(r0, DIGIT1), sum1, ONE);
        sum2 = _mm512_mask_add_epi8(sum2, _mm512_cmpeq_epi8_mask(r0, DIGIT2), sum2, ONE);
        sum3 = _mm512_mask_add_epi8(sum3, _mm512_cmpeq_epi8_mask(r0, DIGIT3), sum3, ONE);
        sum4 = _mm512_mask_add_epi8(sum4, _mm512_cmpeq_epi8_mask(r0, DIGIT4), sum4, ONE);
        sum5 = _mm512_mask_add_epi8(sum5, _mm512_cmpeq_epi8_mask(r0, DIGIT5), sum5, ONE);
        sum6 = _mm512_mask_add_epi8(sum6, _mm512_cmpeq_epi8_mask(r0, DIGIT6), sum6, ONE);
        sum7 = _mm512_mask_add_epi8(sum7, _mm512_cmpeq_epi8_mask(r0, DIGIT7), sum7, ONE);
        sum8 = _mm512_mask_add_epi8(sum8, _mm512_cmpeq_epi8_mask(r0, DIGIT8), sum8, ONE);
        sum9 = _mm512_mask_add_epi8(sum9, _mm512_cmpeq_epi8_mask(r0, DIGIT9), sum9, ONE);
        raw_digits++;
    }while (--blocks);

    sum0 = reduce_u8_to_u64_AVX512(sum0);
    sum1 = reduce_u8_to_u64_AVX512(sum1);
    sum2 = reduce_u8_to_u64_AVX512(sum2);
    sum3 = reduce_u8_to_u64_AVX512(sum3);
    sum4 = reduce_u8_to_u64_AVX512(sum4);
    sum5 = reduce_u8_to_u64_AVX512(sum5);
    sum6 = reduce_u8_to_u64_AVX512(sum6);
    sum7 = reduce_u8_to_u64_AVX512(sum7);
    sum8 = reduce_u8_to_u64_AVX512(sum8);
    sum9 = reduce_u8_to_u64_AVX512(sum9);

    SIMD::transpose_i64_8x8_AVX512(sum0, sum1, sum2, sum3, sum4, sum5, sum6, sum7);

    sum0 = _mm512_add_epi64(sum0, sum1);
    sum2 = _mm512_add_epi64(sum2, sum3);
    sum4 = _mm512_add_epi64(sum4, sum5);
    sum6 = _mm512_add_epi64(sum6, sum7);

    sum0 = _mm512_add_epi64(sum0, sum2);
    sum4 = _mm512_add_epi64(sum4, sum6);

    sum0 = _mm512_add_epi64(sum0, sum4);

    sum1 = _mm512_loadu_si512(digits);
    sum0 = _mm512_add_epi64(sum0, sum1);
    _mm512_storeu_si512(digits, sum0);

    digits[8] += _mm512_reduce_add_epi64(sum8);
    digits[9] += _mm512_reduce_add_epi64(sum9);
}
template <
    int digit0, int digit1, int digit2, int digit3,
    int digit4, int digit5, int digit6, int digit7
> YM_FORCE_INLINE
void accumulate_b64_AVX512_64x8(u64_t digits[8], const __m512i* raw_digits, upL_t blocks){
    const __m512i DIGIT0 = _mm512_set1_epi8(digit0);
    const __m512i DIGIT1 = _mm512_set1_epi8(digit1);
    const __m512i DIGIT2 = _mm512_set1_epi8(digit2);
    const __m512i DIGIT3 = _mm512_set1_epi8(digit3);
    const __m512i DIGIT4 = _mm512_set1_epi8(digit4);
    const __m512i DIGIT5 = _mm512_set1_epi8(digit5);
    const __m512i DIGIT6 = _mm512_set1_epi8(digit6);
    const __m512i DIGIT7 = _mm512_set1_epi8(digit7);

    __m512i sum0 = _mm512_setzero_si512();
    __m512i sum1 = _mm512_setzero_si512();
    __m512i sum2 = _mm512_setzero_si512();
    __m512i sum3 = _mm512_setzero_si512();
    __m512i sum4 = _mm512_setzero_si512();
    __m512i sum5 = _mm512_setzero_si512();
    __m512i sum6 = _mm512_setzero_si512();
    __m512i sum7 = _mm512_setzero_si512();

    const __m512i ONE = _mm512_set1_epi8(1);
    do{
        __m512i r0 = _mm512_load_si512(raw_digits);
        sum0 = _mm512_mask_add_epi8(sum0, _mm512_cmpeq_epi8_mask(r0, DIGIT0), sum0, ONE);
        sum1 = _mm512_mask_add_epi8(sum1, _mm512_cmpeq_epi8_mask(r0, DIGIT1), sum1, ONE);
        sum2 = _mm512_mask_add_epi8(sum2, _mm512_cmpeq_epi8_mask(r0, DIGIT2), sum2, ONE);
        sum3 = _mm512_mask_add_epi8(sum3, _mm512_cmpeq_epi8_mask(r0, DIGIT3), sum3, ONE);
        sum4 = _mm512_mask_add_epi8(sum4, _mm512_cmpeq_epi8_mask(r0, DIGIT4), sum4, ONE);
        sum5 = _mm512_mask_add_epi8(sum5, _mm512_cmpeq_epi8_mask(r0, DIGIT5), sum5, ONE);
        sum6 = _mm512_mask_add_epi8(sum6, _mm512_cmpeq_epi8_mask(r0, DIGIT6), sum6, ONE);
        sum7 = _mm512_mask_add_epi8(sum7, _mm512_cmpeq_epi8_mask(r0, DIGIT7), sum7, ONE);
        raw_digits++;
    }while (--blocks);

    sum0 = reduce_u8_to_u64_AVX512(sum0);
    sum1 = reduce_u8_to_u64_AVX512(sum1);
    sum2 = reduce_u8_to_u64_AVX512(sum2);
    sum3 = reduce_u8_to_u64_AVX512(sum3);
    sum4 = reduce_u8_to_u64_AVX512(sum4);
    sum5 = reduce_u8_to_u64_AVX512(sum5);
    sum6 = reduce_u8_to_u64_AVX512(sum6);
    sum7 = reduce_u8_to_u64_AVX512(sum7);
    SIMD::transpose_i64_8x8_AVX512(sum0, sum1, sum2, sum3, sum4, sum5, sum6, sum7);

    sum0 = _mm512_add_epi64(sum0, sum1);
    sum2 = _mm512_add_epi64(sum2, sum3);
    sum4 = _mm512_add_epi64(sum4, sum5);
    sum6 = _mm512_add_epi64(sum6, sum7);

    sum0 = _mm512_add_epi64(sum0, sum2);
    sum4 = _mm512_add_epi64(sum4, sum6);

    sum0 = _mm512_add_epi64(sum0, sum4);

    sum1 = _mm512_loadu_si512(digits);
    sum0 = _mm512_add_epi64(sum0, sum1);
    _mm512_storeu_si512(digits, sum0);
}
YM_FORCE_INLINE void accumulate_b64_AVX512_64x16(u64_t digits[16], const __m512i* raw_digits, upL_t blocks){
    //  AVX512 has enough registers to do this.

    const __m512i DIGIT_0 = _mm512_set1_epi8( 0);
    const __m512i DIGIT_1 = _mm512_set1_epi8( 1);
    const __m512i DIGIT_2 = _mm512_set1_epi8( 2);
    const __m512i DIGIT_3 = _mm512_set1_epi8( 3);
    const __m512i DIGIT_4 = _mm512_set1_epi8( 4);
    const __m512i DIGIT_5 = _mm512_set1_epi8( 5);
    const __m512i DIGIT_6 = _mm512_set1_epi8( 6);
    const __m512i DIGIT_7 = _mm512_set1_epi8( 7);
    const __m512i DIGIT_8 = _mm512_set1_epi8( 8);
    const __m512i DIGIT_9 = _mm512_set1_epi8( 9);
    const __m512i DIGIT_A = _mm512_set1_epi8(10);
    const __m512i DIGIT_B = _mm512_set1_epi8(11);
    const __m512i DIGIT_C = _mm512_set1_epi8(12);
    const __m512i DIGIT_D = _mm512_set1_epi8(13);
    const __m512i DIGIT_E = _mm512_set1_epi8(14);
    const __m512i DIGIT_F = _mm512_set1_epi8(15);

    __m512i sum0 = _mm512_setzero_si512();
    __m512i sum1 = _mm512_setzero_si512();
    __m512i sum2 = _mm512_setzero_si512();
    __m512i sum3 = _mm512_setzero_si512();
    __m512i sum4 = _mm512_setzero_si512();
    __m512i sum5 = _mm512_setzero_si512();
    __m512i sum6 = _mm512_setzero_si512();
    __m512i sum7 = _mm512_setzero_si512();
    __m512i sum8 = _mm512_setzero_si512();
    __m512i sum9 = _mm512_setzero_si512();
    __m512i sumA = _mm512_setzero_si512();
    __m512i sumB = _mm512_setzero_si512();
    __m512i sumC = _mm512_setzero_si512();
    __m512i sumD = _mm512_setzero_si512();
    __m512i sumE = _mm512_setzero_si512();
    __m512i sumF = _mm512_setzero_si512();

    const __m512i NEG = _mm512_set1_epi8(-1);
    do{
        __m512i r0 = _mm512_load_si512(raw_digits);
        sum0 = _mm512_mask_sub_epi8(sum0, _mm512_cmpeq_epi8_mask(r0, DIGIT_0), sum0, NEG);
        sum1 = _mm512_mask_sub_epi8(sum1, _mm512_cmpeq_epi8_mask(r0, DIGIT_1), sum1, NEG);
        sum2 = _mm512_mask_sub_epi8(sum2, _mm512_cmpeq_epi8_mask(r0, DIGIT_2), sum2, NEG);
        sum3 = _mm512_mask_sub_epi8(sum3, _mm512_cmpeq_epi8_mask(r0, DIGIT_3), sum3, NEG);
        sum4 = _mm512_mask_sub_epi8(sum4, _mm512_cmpeq_epi8_mask(r0, DIGIT_4), sum4, NEG);
        sum5 = _mm512_mask_sub_epi8(sum5, _mm512_cmpeq_epi8_mask(r0, DIGIT_5), sum5, NEG);
        sum6 = _mm512_mask_sub_epi8(sum6, _mm512_cmpeq_epi8_mask(r0, DIGIT_6), sum6, NEG);
        sum7 = _mm512_mask_sub_epi8(sum7, _mm512_cmpeq_epi8_mask(r0, DIGIT_7), sum7, NEG);
        sum8 = _mm512_mask_sub_epi8(sum8, _mm512_cmpeq_epi8_mask(r0, DIGIT_8), sum8, NEG);
        sum9 = _mm512_mask_sub_epi8(sum9, _mm512_cmpeq_epi8_mask(r0, DIGIT_9), sum9, NEG);
        sumA = _mm512_mask_sub_epi8(sumA, _mm512_cmpeq_epi8_mask(r0, DIGIT_A), sumA, NEG);
        sumB = _mm512_mask_sub_epi8(sumB, _mm512_cmpeq_epi8_mask(r0, DIGIT_B), sumB, NEG);
        sumC = _mm512_mask_sub_epi8(sumC, _mm512_cmpeq_epi8_mask(r0, DIGIT_C), sumC, NEG);
        sumD = _mm512_mask_sub_epi8(sumD, _mm512_cmpeq_epi8_mask(r0, DIGIT_D), sumD, NEG);
        sumE = _mm512_mask_sub_epi8(sumE, _mm512_cmpeq_epi8_mask(r0, DIGIT_E), sumE, NEG);
        sumF = _mm512_mask_sub_epi8(sumF, _mm512_cmpeq_epi8_mask(r0, DIGIT_F), sumF, NEG);
        raw_digits++;
    }while (--blocks);

    sum0 = reduce_u8_to_u64_AVX512(sum0);
    sum1 = reduce_u8_to_u64_AVX512(sum1);
    sum2 = reduce_u8_to_u64_AVX512(sum2);
    sum3 = reduce_u8_to_u64_AVX512(sum3);
    sum4 = reduce_u8_to_u64_AVX512(sum4);
    sum5 = reduce_u8_to_u64_AVX512(sum5);
    sum6 = reduce_u8_to_u64_AVX512(sum6);
    sum7 = reduce_u8_to_u64_AVX512(sum7);
    SIMD::transpose_i64_8x8_AVX512(sum0, sum1, sum2, sum3, sum4, sum5, sum6, sum7);
    sum8 = reduce_u8_to_u64_AVX512(sum8);
    sum9 = reduce_u8_to_u64_AVX512(sum9);
    sumA = reduce_u8_to_u64_AVX512(sumA);
    sumB = reduce_u8_to_u64_AVX512(sumB);
    sumC = reduce_u8_to_u64_AVX512(sumC);
    sumD = reduce_u8_to_u64_AVX512(sumD);
    sumE = reduce_u8_to_u64_AVX512(sumE);
    sumF = reduce_u8_to_u64_AVX512(sumF);
    SIMD::transpose_i64_8x8_AVX512(sum8, sum9, sumA, sumB, sumC, sumD, sumE, sumF);

    sum0 = _mm512_add_epi64(sum0, sum1);
    sum2 = _mm512_add_epi64(sum2, sum3);
    sum4 = _mm512_add_epi64(sum4, sum5);
    sum6 = _mm512_add_epi64(sum6, sum7);
    sum8 = _mm512_add_epi64(sum8, sum9);
    sumA = _mm512_add_epi64(sumA, sumB);
    sumC = _mm512_add_epi64(sumC, sumD);
    sumE = _mm512_add_epi64(sumE, sumF);

    sum0 = _mm512_add_epi64(sum0, sum2);
    sum4 = _mm512_add_epi64(sum4, sum6);
    sum8 = _mm512_add_epi64(sum8, sumA);
    sumC = _mm512_add_epi64(sumC, sumE);

    sum0 = _mm512_add_epi64(sum0, sum4);
    sum8 = _mm512_add_epi64(sum8, sumC);

    sum1 = _mm512_loadu_si512(digits + 0);
    sum9 = _mm512_loadu_si512(digits + 8);
    sum0 = _mm512_add_epi64(sum0, sum1);
    sum8 = _mm512_add_epi64(sum8, sum9);
    _mm512_storeu_si512(digits + 0, sum0);
    _mm512_storeu_si512(digits + 8, sum8);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
