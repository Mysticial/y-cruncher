/* i64ToHex_x86_SSE2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/10/2018
 * Last Modified    : 02/10/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_i64ToHex_x86_SSE2_H
#define ydv_DigitViewer_i64ToHex_x86_SSE2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "PublicLibs/ArchSpecificLibs/x86_SSE2.h"
namespace DigitViewer2{
namespace RawToCompressed{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE bool hex_to_i64_u4_x86_SSE2(__m128i* T, const __m128i* raw, upL_t blocks){
    if (blocks == 0){
        return false;
    }

    const __m128i MULTIPLY = _mm_set1_epi32(0x00010100);
    const __m128i MASK0 = _mm_set1_epi16(0xf);

    __m128i bad = _mm_setzero_si128();
    do{
        __m128i a0, a1, a2, a3;
        __m128i b0, b1, b2, b3;

        a0 = _mm_loadu_si128(raw + 0);
        a1 = _mm_loadu_si128(raw + 1);
        a2 = _mm_loadu_si128(raw + 2);
        a3 = _mm_loadu_si128(raw + 3);

        bad = _mm_or_si128(bad, a0);
        bad = _mm_or_si128(bad, a1);
        bad = _mm_or_si128(bad, a2);
        bad = _mm_or_si128(bad, a3);

        b0 = _mm_and_si128(a0, MASK0);
        b1 = _mm_and_si128(a1, MASK0);
        b2 = _mm_and_si128(a2, MASK0);
        b3 = _mm_and_si128(a3, MASK0);
        a0 = _mm_srli_epi16(a0, 8);
        a1 = _mm_srli_epi16(a1, 8);
        a2 = _mm_srli_epi16(a2, 8);
        a3 = _mm_srli_epi16(a3, 8);

        b0 = _mm_slli_epi16(b0, 4);
        b1 = _mm_slli_epi16(b1, 4);
        b2 = _mm_slli_epi16(b2, 4);
        b3 = _mm_slli_epi16(b3, 4);

        a0 = _mm_or_si128(a0, b0);
        a1 = _mm_or_si128(a1, b1);
        a2 = _mm_or_si128(a2, b2);
        a3 = _mm_or_si128(a3, b3);

        a0 = _mm_madd_epi16(a0, MULTIPLY);
        a1 = _mm_madd_epi16(a1, MULTIPLY);
        a2 = _mm_madd_epi16(a2, MULTIPLY);
        a3 = _mm_madd_epi16(a3, MULTIPLY);

        a0 = _mm_shufflelo_epi16(a0, 114);
        a1 = _mm_shufflelo_epi16(a1, 114);
        a2 = _mm_shufflelo_epi16(a2, 114);
        a3 = _mm_shufflelo_epi16(a3, 114);

        a0 = _mm_shufflehi_epi16(a0, 114);
        a1 = _mm_shufflehi_epi16(a1, 114);
        a2 = _mm_shufflehi_epi16(a2, 114);
        a3 = _mm_shufflehi_epi16(a3, 114);

        a0 = _mm_shuffle_epi32(a0, 114);
        a1 = _mm_shuffle_epi32(a1, 39);
        a2 = _mm_shuffle_epi32(a2, 114);
        a3 = _mm_shuffle_epi32(a3, 39);

        a0 = _mm_or_si128(a0, a1);
        a2 = _mm_or_si128(a2, a3);

        _mm_storeu_si128(T + 0, a0);
        _mm_storeu_si128(T + 1, a2);

        raw += 4;
        T += 2;
    }while (--blocks);

    return (_mm_reduce_or_epi64(bad) & 0xf0f0f0f0f0f0f0f0ull) != 0;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
