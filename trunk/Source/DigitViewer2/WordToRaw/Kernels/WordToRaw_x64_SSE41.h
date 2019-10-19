/* WordToRaw_x64_SSE41.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/11/2018
 * Last Modified    : 02/11/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_WordToRaw_x64_SSE41_H
#define ydv_DigitViewer_WordToRaw_x64_SSE41_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "PublicLibs/ArchSpecificLibs/Shuffle/x86_128/AdjacentLanePermute_x86_128.h"
#include "DigitViewer2/RawToDecKernels/Kernels_i64_to_dec_x64_SSE41.h"
namespace DigitViewer2{
namespace WordToRaw{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void w64_to_hex_u2_x64_SSE41(__m128i* raw, const u64_t* T, upL_t blocks){
    if (blocks == 0){
        return;
    }

    const __m128i MASK = _mm_set1_epi16(0xf0);
    do{
        raw -= 2;

        __m128i a0, b0;
        __m128i a1, b1;

        a0 = _mm_loadl_epi64((const __m128i*)(T + 0));
        a1 = _mm_loadl_epi64((const __m128i*)(T + 1));

        a0 = _mm_cvtepu8_epi16(a0);
        a1 = _mm_cvtepu8_epi16(a1);

        b0 = _mm_and_si128(a0, MASK);
        b1 = _mm_and_si128(a1, MASK);

        a0 = _mm_xor_si128(a0, b0);
        a1 = _mm_xor_si128(a1, b1);

        b0 = _mm_srli_epi16(b0, 4);
        b1 = _mm_srli_epi16(b1, 4);
        a0 = _mm_slli_epi16(a0, 8);
        a1 = _mm_slli_epi16(a1, 8);

        a0 = _mm_or_si128(a0, b0);
        a1 = _mm_or_si128(a1, b1);

        a0 = _mm_shufflelo_epi16(a0, 27);
        a1 = _mm_shufflelo_epi16(a1, 27);

        a0 = _mm_shufflehi_epi16(a0, 27);
        a1 = _mm_shufflehi_epi16(a1, 27);

        a0 = _mm_shuffle_epi32(a0, 78);
        a1 = _mm_shuffle_epi32(a1, 78);

        _mm_storeu_si128(raw + 0, a1);
        _mm_storeu_si128(raw + 1, a0);

        T += 2;
    }while (--blocks);
}
YM_FORCE_INLINE void w64_to_dec_u2_x64_SSE41(char* raw, const __m128i* T, upL_t blocks){
    if (blocks == 0){
        return;
    }

    do{
        raw -= 38;

        __m128i a0, b0, c0;
        RawToDec::i64_to_dec_x64_SSE41(T[0], a0, b0, c0);

        SIMD::mm_splitstore_si128(raw + 19, raw +  0, c0);
        SIMD::mm_splitstore_si128(raw + 22, raw +  3, b0);
        SIMD::mm_splitstore_si128(raw + 30, raw + 11, a0);

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
