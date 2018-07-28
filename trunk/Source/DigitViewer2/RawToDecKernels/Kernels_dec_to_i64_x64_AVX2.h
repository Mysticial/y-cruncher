/* Kernels_dec_to_i64_x64_AVX2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/11/2018
 * Last Modified    : 02/11/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_Kernels_dec_to_i64_x64_AVX2_H
#define ydv_DigitViewer_Kernels_dec_to_i64_x64_AVX2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <immintrin.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace DigitViewer2{
namespace RawToDec{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE __m256i dec_to_i64_x64_AVX2(
    __m256i& T0,
    __m256i a0, __m256i b0, __m256i c0
){
    const __m256i MIN = _mm256_set1_epi8((char)-128);
    const __m256i MAX = _mm256_set1_epi8((char)-119);

    __m256i bad = _mm256_cmpgt_epi8(_mm256_xor_si256(a0, MIN), MAX);
    bad = _mm256_or_si256(bad, _mm256_cmpgt_epi8(_mm256_xor_si256(b0, MIN), MAX));
    bad = _mm256_or_si256(bad, _mm256_cmpgt_epi8(_mm256_xor_si256(c0, MIN), MAX));

    {
        __m256i hi;

        hi = _mm256_srli_epi16(a0, 8);
        a0 = _mm256_and_si256(a0, _mm256_set1_epi16(0x00ff));
        a0 = _mm256_mullo_epi16(a0, _mm256_set1_epi16(10));
        a0 = _mm256_add_epi16(a0, hi);

        hi = _mm256_srli_epi16(b0, 8);
        b0 = _mm256_and_si256(b0, _mm256_set1_epi16(0x00ff));
        b0 = _mm256_mullo_epi16(b0, _mm256_set1_epi16(10));
        b0 = _mm256_add_epi16(b0, hi);

        hi = _mm256_srli_epi16(c0, 8);
        c0 = _mm256_and_si256(c0, _mm256_set1_epi16(0x00ff));
        c0 = _mm256_mullo_epi16(c0, _mm256_set1_epi16(10));
        c0 = _mm256_add_epi16(c0, hi);
    }
    {
        const __m256i COMPRESS0 = _mm256_setr_epi64x(
            0x0604020080808080, 0x0e0c0a0880808080,
            0x0604020080808080, 0x0e0c0a0880808080
        );
        const __m256i COMPRESS1 = _mm256_setr_epi64x(
            0x8080808006040200, 0x808080800e0c0a08,
            0x8080808006040200, 0x808080800e0c0a08
        );
        a0 = _mm256_shuffle_epi8(a0, COMPRESS0);
        b0 = _mm256_shuffle_epi8(b0, COMPRESS1);
        a0 = _mm256_or_si256(a0, b0);
    }
    {
        __m256i hi;

        hi = _mm256_srli_epi16(a0, 8);
        a0 = _mm256_and_si256(a0, _mm256_set1_epi16(0x00ff));
        a0 = _mm256_mullo_epi16(a0, _mm256_set1_epi16(100));
        a0 = _mm256_add_epi16(a0, hi);

        hi = _mm256_srli_epi32(c0, 16);
        c0 = _mm256_and_si256(c0, _mm256_set1_epi32(0x0000ffff));
        c0 = _mm256_mullo_epi16(c0, _mm256_set1_epi16(100));
        c0 = _mm256_add_epi16(c0, hi);
    }
    {
        __m256i hi;

        hi = _mm256_srli_epi32(a0, 16);
        a0 = _mm256_and_si256(a0, _mm256_set1_epi32(0x0000ffff));
        a0 = _mm256_mullo_epi32(a0, _mm256_set1_epi32(10000));
        a0 = _mm256_add_epi32(a0, hi);

        c0 = _mm256_mul_epu32(c0, _mm256_set1_epi32(400000));
    }
    {
        __m256i hi;

        hi = _mm256_srli_epi64(a0, 32);
        a0 = _mm256_and_si256(a0, _mm256_set1_epi64x(0x00000000ffffffff));
        a0 = _mm256_mul_epu32(a0, _mm256_set1_epi64x(100000000));
        a0 = _mm256_add_epi64(a0, hi);

        c0 = _mm256_mul_epu32(c0, _mm256_set1_epi32(2500000000));
    }

    T0 = _mm256_add_epi64(a0, c0);
    return bad;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
