/* i64ToRaw_x64_AVX512-BW.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/10/2018
 * Last Modified    : 02/10/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_i64ToRaw_x64_AVX512_H
#define ydv_DigitViewer_i64ToRaw_x64_AVX512_H
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
YM_FORCE_INLINE bool hex_to_i64_u8_x64_AVX512BW(__m512i* T, const __m512i* raw, upL_t blocks){
    if (blocks == 0){
        return false;
    }

    const __m512i COMPRESS = _mm512_set1_epi64(0x00020406080a0c0e);

    __m512i bad = _mm512_setzero_si512();
    do{
        __m512i x0, x1;

        x0 = _mm512_loadu_si512(raw + 0);
        x1 = _mm512_loadu_si512(raw + 1);

//        bad = _mm512_or_si512(bad, x0);
//        bad = _mm512_or_si512(bad, x1);
        bad = _mm512_ternarylogic_epi64(bad, x0, x1, 0xfe);

        {
            //  1st Merge
            x0 = _mm512_maddubs_epi16(x0, _mm512_set1_epi16(0x0110));
            x1 = _mm512_maddubs_epi16(x1, _mm512_set1_epi16(0x0110));
        }
        {
            //  Compress and Reverse
            x0 = _mm512_shuffle_epi8(x0, COMPRESS);
            x1 = _mm512_shuffle_epi8(x1, COMPRESS);
            x0 = _mm512_permutex2var_epi64(
                x0,
                _mm512_setr_epi64(0, 2, 4, 6, 8, 10, 12, 14),
                x1
            );
        }

        _mm512_storeu_si512(T, x0);

        raw += 2;
        T += 1;
    }while (--blocks);

    return _mm512_test_epi8_mask(bad, _mm512_set1_epi8((char)0xf0)) != 0;
}
YM_FORCE_INLINE void i64_to_hex_u8_x64_AVX512BW(__m512i* raw, const __m256i* T, upL_t blocks){
    if (blocks == 0){
        return;
    }

    const __m512i MASK = _mm512_set1_epi8((char)0x0f);
    const __m512i REVERSE = _mm512_setr_epi64(
        0x08090a0b0c0d0e0f, 0x0001020304050607,
        0x08090a0b0c0d0e0f, 0x0001020304050607,
        0x08090a0b0c0d0e0f, 0x0001020304050607,
        0x08090a0b0c0d0e0f, 0x0001020304050607
    );

    do{
        __m512i x0, x1;

        //  Load and decompress
        x0 = _mm512_cvtepu8_epi16(_mm256_loadu_si256(T + 0));
        x1 = _mm512_cvtepu8_epi16(_mm256_loadu_si256(T + 1));

        {
            //  Fix bytes
            __m512i lo, hi;

            hi = _mm512_slli_epi16(x0, 4);
            lo = _mm512_and_si512(x0, MASK);
            hi = _mm512_and_si512(hi, MASK);
            x0 = _mm512_or_si512(lo, hi);

            hi = _mm512_slli_epi16(x1, 4);
            lo = _mm512_and_si512(x1, MASK);
            hi = _mm512_and_si512(hi, MASK);
            x1 = _mm512_or_si512(lo, hi);
        }
        {
            //  Reverse
            x0 = _mm512_shuffle_epi8(x0, REVERSE);
            x1 = _mm512_shuffle_epi8(x1, REVERSE);
        }

        _mm512_storeu_si512(raw + 0, x0);
        _mm512_storeu_si512(raw + 1, x1);

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
