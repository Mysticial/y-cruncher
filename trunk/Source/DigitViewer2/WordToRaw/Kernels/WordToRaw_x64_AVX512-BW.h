/* WordToRaw_x64_AVX512-BW.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/11/2018
 * Last Modified    : 02/11/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_WordToRaw_x64_AVX512BW_H
#define ydv_DigitViewer_WordToRaw_x64_AVX512BW_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "PublicLibs/ArchSpecificLibs/Shuffle/x86_256/AdjacentLanePermute_x86_256.h"
#include "DigitViewer2/RawToDecKernels/Kernels_i64_to_dec_x64_AVX512-BW.h"
namespace DigitViewer2{
namespace WordToRaw{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void w64_to_hex_u8_x64_AVX512BW(__m512i* raw, const __m256i* T, upL_t blocks){
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
        raw -= 2;

        __m512i x0, x1;

        //  Load and decompress
        x0 = _mm512_cvtepu8_epi16(_mm256_permute4x64_epi64(T[0], 27));
        x1 = _mm512_cvtepu8_epi16(_mm256_permute4x64_epi64(T[1], 27));

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

        _mm512_storeu_si512(raw + 0, x1);
        _mm512_storeu_si512(raw + 1, x0);

        T += 2;
    }while (--blocks);
}
YM_FORCE_INLINE void w64_to_dec_u8_x64_AVX512BW(char* raw, const __m512i* T, upL_t blocks){
    if (blocks == 0){
        return;
    }

    const __m512i GATHER = _mm512_setr_epi64(7*19, 6*19, 5*19, 4*19, 3*19, 2*19, 1*19, 0*19);

    do{
        raw -= 152;

        __m512i a0, b0, c0;
        RawToDec::i64_to_dec_x64_AVX512BW(T[0], a0, b0, c0);

        _mm512_i64scatter_epi64(raw +  0, GATHER, c0, 1);
#if 0
        _mm512_i64scatter_epi64(raw +  3, GATHER, b0, 1);
        _mm512_i64scatter_epi64(raw + 11, GATHER, a0, 1);
#else
        __m512i z0 = _mm512_unpacklo_epi64(b0, a0);
        __m512i z1 = _mm512_unpackhi_epi64(b0, a0);

        SIMD::mm256_splitstore_si256(raw + 136, raw +  98, _mm512_castsi512_si256(z0));
        SIMD::mm256_splitstore_si256(raw + 117, raw +  79, _mm512_castsi512_si256(z1));
        SIMD::mm256_splitstore_si256(raw +  60, raw +  22, _mm512_extracti64x4_epi64(z0, 1));
        SIMD::mm256_splitstore_si256(raw +  41, raw +   3, _mm512_extracti64x4_epi64(z1, 1));
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
