/* DigitCount_x86_SSE2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/13/2018
 * Last Modified    : 01/13/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_DigitCount_x86_SSE2_H
#define ydv_DigitViewer_DigitCount_x86_SSE2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <emmintrin.h>
#include "PublicLibs/CompilerSettings.h"
namespace DigitViewer2{
namespace DigitCount{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE __m128i reduce_u8_to_u64_SSE2(__m128i x){
    __m128i r;

    r = _mm_srli_epi16(x, 8);
    x = _mm_and_si128(x, _mm_set1_epi16(0x00ff));
    x = _mm_add_epi16(x, r);

    r = _mm_srli_epi32(x, 16);
    x = _mm_and_si128(x, _mm_set1_epi32(0x0000ffff));
    x = _mm_add_epi32(x, r);

    r = _mm_srli_epi64(x, 32);
    x = _mm_and_si128(x, _mm_set1_epi64x(0x00000000ffffffff));
    x = _mm_add_epi64(x, r);

    return x;
}
template <int digit0, int digit1> YM_FORCE_INLINE
void accumulate_b16_SSE2_64x2(u64_t digits[2], const __m128i* raw_digits, upL_t blocks){
    const __m128i DIGIT0 = _mm_set1_epi8(digit0);
    const __m128i DIGIT1 = _mm_set1_epi8(digit1);

    __m128i sum0 = _mm_setzero_si128();
    __m128i sum1 = _mm_setzero_si128();
    __m128i r0;

    do{
        r0 = _mm_load_si128(raw_digits);
        sum0 = _mm_sub_epi8(sum0, _mm_cmpeq_epi8(r0, DIGIT0));
        sum1 = _mm_sub_epi8(sum1, _mm_cmpeq_epi8(r0, DIGIT1));
        raw_digits++;
    }while (--blocks);

    sum0 = reduce_u8_to_u64_SSE2(sum0);
    sum1 = reduce_u8_to_u64_SSE2(sum1);

    r0 = _mm_add_epi64(
        _mm_unpacklo_epi64(sum0, sum1),
        _mm_unpackhi_epi64(sum0, sum1)
    );
    digits[0] += _mm_cvtsi128_si32(r0);
    digits[1] += _mm_cvtsi128_si32(_mm_unpackhi_epi64(r0, r0));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
