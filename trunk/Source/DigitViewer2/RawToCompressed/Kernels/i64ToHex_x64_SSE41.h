/* i64ToHex_x64_SSE41.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/10/2018
 * Last Modified    : 02/10/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_i64ToHex_x64_SSE41_H
#define ydv_DigitViewer_i64ToHex_x64_SSE41_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <smmintrin.h>
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
YM_FORCE_INLINE bool hex_to_i64_u4_x86_SSSE3(__m128i* T, const __m128i* raw, upL_t blocks){
    if (blocks == 0){
        return false;
    }

    const __m128i MULTIPLY = _mm_set1_epi16(0x0110);
    const __m128i PERMUTE = _mm_setr_epi8(14, 12, 10, 8, 6, 4, 2, 0, 14, 12, 10, 8, 6, 4, 2, 0);

    __m128i bad = _mm_setzero_si128();
    do{
        __m128i a0, a1, a2, a3;

        a0 = _mm_loadu_si128(raw + 0);
        a1 = _mm_loadu_si128(raw + 1);
        a2 = _mm_loadu_si128(raw + 2);
        a3 = _mm_loadu_si128(raw + 3);

        bad = _mm_or_si128(bad, a0);
        bad = _mm_or_si128(bad, a1);
        bad = _mm_or_si128(bad, a2);
        bad = _mm_or_si128(bad, a3);

        a0 = _mm_maddubs_epi16(a0, MULTIPLY);
        a1 = _mm_maddubs_epi16(a1, MULTIPLY);
        a2 = _mm_maddubs_epi16(a2, MULTIPLY);
        a3 = _mm_maddubs_epi16(a3, MULTIPLY);

        a0 = _mm_shuffle_epi8(a0, PERMUTE);
        a1 = _mm_shuffle_epi8(a1, PERMUTE);
        a2 = _mm_shuffle_epi8(a2, PERMUTE);
        a3 = _mm_shuffle_epi8(a3, PERMUTE);

        a0 = _mm_blend_epi16(a0, a1, 0xf0);
        a2 = _mm_blend_epi16(a2, a3, 0xf0);

        _mm_storeu_si128(T + 0, a0);
        _mm_storeu_si128(T + 1, a2);

        raw += 4;
        T += 2;
    }while (--blocks);

    return (_mm_reduce_or_epi64(bad) & 0xf0f0f0f0f0f0f0f0ull) != 0;
}
YM_FORCE_INLINE void i64_to_hex_u2_x64_SSE41(__m128i* raw, const u64_t* T, upL_t blocks){
    if (blocks == 0){
        return;
    }

    const __m128i MASK = _mm_set1_epi16(0xf0);
    do{
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

        _mm_storeu_si128(raw + 0, a0);
        _mm_storeu_si128(raw + 1, a1);

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
