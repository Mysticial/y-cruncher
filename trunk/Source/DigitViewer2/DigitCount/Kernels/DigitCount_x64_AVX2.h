/* DigitCount_x64_AVX2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/13/2018
 * Last Modified    : 01/13/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_DigitCount_x64_AVX2_H
#define ydv_DigitViewer_DigitCount_x64_AVX2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <immintrin.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/ArchSpecificLibs/Shuffle/x86_256/Transpose_64x4x4_x86_AVX2.h"
namespace DigitViewer2{
namespace DigitCount{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE __m256i reduce_u8_to_u64_AVX2(__m256i x){
    __m256i r;

    r = _mm256_srli_epi16(x, 8);
    x = _mm256_and_si256(x, _mm256_set1_epi16(0x00ff));
    x = _mm256_add_epi16(x, r);

    r = _mm256_srli_epi32(x, 16);
    x = _mm256_and_si256(x, _mm256_set1_epi32(0x0000ffff));
    x = _mm256_add_epi32(x, r);

    r = _mm256_srli_epi64(x, 32);
    x = _mm256_and_si256(x, _mm256_set1_epi64x(0x00000000ffffffff));
    x = _mm256_add_epi64(x, r);

    return x;
}
template <int digit0, int digit1> YM_FORCE_INLINE
void accumulate_b32_AVX2_64x2(u64_t digits[2], const __m256i* raw_digits, upL_t blocks){
    const __m256i DIGIT0 = _mm256_set1_epi8(digit0);
    const __m256i DIGIT1 = _mm256_set1_epi8(digit1);

    __m256i sum0 = _mm256_setzero_si256();
    __m256i sum1 = _mm256_setzero_si256();

    do{
        __m256i r0 = _mm256_load_si256(raw_digits);
        sum0 = _mm256_sub_epi8(sum0, _mm256_cmpeq_epi8(r0, DIGIT0));
        sum1 = _mm256_sub_epi8(sum1, _mm256_cmpeq_epi8(r0, DIGIT1));
        raw_digits++;
    }while (--blocks);

    sum0 = reduce_u8_to_u64_AVX2(sum0);
    sum1 = reduce_u8_to_u64_AVX2(sum1);

    __m128i s0 = _mm_add_epi64(_mm256_castsi256_si128(sum0), _mm256_extracti128_si256(sum0, 1));
    __m128i s1 = _mm_add_epi64(_mm256_castsi256_si128(sum1), _mm256_extracti128_si256(sum1, 1));

    s0 = _mm_add_epi64(
        _mm_unpacklo_epi64(s0, s1),
        _mm_unpackhi_epi64(s0, s1)
    );
    digits[0] += _mm_cvtsi128_si32(s0);
    digits[1] += _mm_cvtsi128_si32(_mm_unpackhi_epi64(s0, s0));
}
template <int digit0, int digit1, int digit2, int digit3> YM_FORCE_INLINE
void accumulate_b32_AVX2_64x4(u64_t digits[4], const __m256i* raw_digits, upL_t blocks){
    const __m256i DIGIT0 = _mm256_set1_epi8(digit0);
    const __m256i DIGIT1 = _mm256_set1_epi8(digit1);
    const __m256i DIGIT2 = _mm256_set1_epi8(digit2);
    const __m256i DIGIT3 = _mm256_set1_epi8(digit3);

    __m256i sum0 = _mm256_setzero_si256();
    __m256i sum1 = _mm256_setzero_si256();
    __m256i sum2 = _mm256_setzero_si256();
    __m256i sum3 = _mm256_setzero_si256();

    do{
        __m256i r0 = _mm256_load_si256(raw_digits);
        sum0 = _mm256_sub_epi8(sum0, _mm256_cmpeq_epi8(r0, DIGIT0));
        sum1 = _mm256_sub_epi8(sum1, _mm256_cmpeq_epi8(r0, DIGIT1));
        sum2 = _mm256_sub_epi8(sum2, _mm256_cmpeq_epi8(r0, DIGIT2));
        sum3 = _mm256_sub_epi8(sum3, _mm256_cmpeq_epi8(r0, DIGIT3));
        raw_digits++;
    }while (--blocks);

    sum0 = reduce_u8_to_u64_AVX2(sum0);
    sum1 = reduce_u8_to_u64_AVX2(sum1);
    sum2 = reduce_u8_to_u64_AVX2(sum2);
    sum3 = reduce_u8_to_u64_AVX2(sum3);
    SIMD::transpose_i64_4x4_AVX2(sum0, sum1, sum2, sum3);

    sum0 = _mm256_add_epi64(sum0, sum1);
    sum2 = _mm256_add_epi64(sum2, sum3);
    sum0 = _mm256_add_epi64(sum0, sum2);

    sum1 = _mm256_lddqu_si256((__m256i*)digits);
    sum0 = _mm256_add_epi64(sum0, sum1);
    _mm256_storeu_si256((__m256i*)digits, sum0);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
