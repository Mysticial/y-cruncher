/* i64ToHex_x64_AVX2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/10/2018
 * Last Modified    : 02/10/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_i64ToHex_x64_AVX2_H
#define ydv_DigitViewer_i64ToHex_x64_AVX2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <immintrin.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace DigitViewer2{
namespace RawToCompressed{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE bool hex_to_i64_u4_x64_AVX2(__m256i* T, const __m256i* raw, upL_t blocks){
    if (blocks == 0){
        return false;
    }

    const __m256i COMPRESS = _mm256_setr_epi8(
       14, 12, 10,  8,  6,  4,  2,  0, 15, 13, 11,  9,  7,  5,  3,  1,
       14, 12, 10,  8,  6,  4,  2,  0, 15, 13, 11,  9,  7,  5,  3,  1
    );

    __m256i bad = _mm256_setzero_si256();
    do{
        __m256i x0, x1;

        x0 = _mm256_loadu_si256(raw + 0);
        x1 = _mm256_loadu_si256(raw + 1);

        bad = _mm256_or_si256(bad, x0);
        bad = _mm256_or_si256(bad, x1);

        {
            //  1st Merge
            x0 = _mm256_maddubs_epi16(x0, _mm256_set1_epi16(0x0110));
            x1 = _mm256_maddubs_epi16(x1, _mm256_set1_epi16(0x0110));
        }
        {
            //  Compress and Reverse
            x0 = _mm256_shuffle_epi8(x0, COMPRESS);
            x1 = _mm256_shuffle_epi8(x1, COMPRESS);

            x0 = _mm256_permute4x64_epi64(x0, 216);
            x1 = _mm256_permute4x64_epi64(x1, 141);

            x0 = _mm256_or_si256(x0, x1);
        }

        _mm256_storeu_si256(T, x0);

        raw += 2;
        T += 1;
    }while (--blocks);

    return _mm256_testnzc_si256(bad, _mm256_set1_epi8((unsigned char)0xf0)) != 0;
}
YM_FORCE_INLINE void i64_to_hex_u4_x64_AVX2(__m256i* raw, const __m128i* T, upL_t blocks){
    if (blocks == 0){
        return;
    }

    const __m256i MASK = _mm256_set1_epi8((char)0x0f);
    const __m256i REVERSE = _mm256_setr_epi8(
       15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0,
       15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0
    );

    do{
        __m256i x0, x1;

        //  Load and decompress
        x0 = _mm256_cvtepu8_epi16(_mm_loadu_si128(T + 0));
        x1 = _mm256_cvtepu8_epi16(_mm_loadu_si128(T + 1));

        {
            //  Fix bytes
            __m256i lo, hi;

            hi = _mm256_slli_epi16(x0, 4);
            lo = _mm256_and_si256(x0, MASK);
            hi = _mm256_and_si256(hi, MASK);
            x0 = _mm256_or_si256(lo, hi);

            hi = _mm256_slli_epi16(x1, 4);
            lo = _mm256_and_si256(x1, MASK);
            hi = _mm256_and_si256(hi, MASK);
            x1 = _mm256_or_si256(lo, hi);
        }
        {
            //  Reverse
            x0 = _mm256_shuffle_epi8(x0, REVERSE);
            x1 = _mm256_shuffle_epi8(x1, REVERSE);
        }

        _mm256_storeu_si256(raw + 0, x0);
        _mm256_storeu_si256(raw + 1, x1);

        raw += 2;
        T += 2;
    }while (--blocks);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
