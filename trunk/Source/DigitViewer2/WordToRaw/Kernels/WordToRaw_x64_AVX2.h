/* WordToRaw_x64_AVX2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/11/2018
 * Last Modified    : 02/11/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_WordToRaw_x64_AVX2_H
#define ydv_DigitViewer_WordToRaw_x64_AVX2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "PublicLibs/ArchSpecificLibs/Shuffle/x86_128/AdjacentLanePermute_x86_128.h"
#include "PublicLibs/ArchSpecificLibs/Shuffle/x86_256/AdjacentLanePermute_x86_256.h"
#include "DigitViewer2/RawToDecKernels/Kernels_i64_to_dec_x64_AVX2.h"
namespace DigitViewer2{
namespace WordToRaw{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void w64_to_hex_u4_x64_AVX2(__m256i* raw, const __m128i* T, upL_t blocks){
    if (blocks == 0){
        return;
    }

    const __m256i MASK = _mm256_set1_epi8((char)0x0f);
    const __m256i REVERSE = _mm256_setr_epi8(
       15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0,
       15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0
    );

    do{
        raw -= 2;

        __m256i x0, x1;

        //  Load and decompress
        x0 = _mm256_cvtepu8_epi16(_mm_shuffle_epi32(T[0], 78));
        x1 = _mm256_cvtepu8_epi16(_mm_shuffle_epi32(T[1], 78));

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

        _mm256_storeu_si256(raw + 0, x1);
        _mm256_storeu_si256(raw + 1, x0);

        T += 2;
    }while (--blocks);
}
YM_FORCE_INLINE void w64_to_dec_u4_x64_AVX2(char* raw, const __m256i* T, upL_t blocks){
    if (blocks == 0){
        return;
    }

    do{
        raw -= 76;

        __m256i a0, b0, c0;
        RawToDec::i64_to_dec_x64_AVX2(T[0], a0, b0, c0);

        SIMD::mm_splitstore_si128(raw + 57, raw + 38, _mm256_castsi256_si128(c0));
        SIMD::mm_splitstore_si128(raw + 19, raw +  0, _mm256_extracti128_si256(c0, 1));
#if 0
        SIMD::mm_splitstore_si128(raw + 60, raw + 41, _mm256_castsi256_si128(b0));
        SIMD::mm_splitstore_si128(raw + 22, raw +  3, _mm256_extracti128_si256(b0, 1));

        SIMD::mm_splitstore_si128(raw + 68, raw + 49, _mm256_castsi256_si128(a0));
        SIMD::mm_splitstore_si128(raw + 30, raw + 11, _mm256_extracti128_si256(a0, 1));
#else
        __m256i y0 = _mm256_unpacklo_epi64(b0, a0);
        __m256i y1 = _mm256_unpackhi_epi64(b0, a0);
        SIMD::mm256_splitstore_si256(raw + 60, raw + 22, y0);
        SIMD::mm256_splitstore_si256(raw + 41, raw +  3, y1);
#endif

        T += 1;
    }while (--blocks);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
