/* Kernels_dec_to_i64_x64_AVX512-BW.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/11/2018
 * Last Modified    : 02/11/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_Kernels_dec_to_i64_x64_AVX512BW_H
#define ydv_DigitViewer_Kernels_dec_to_i64_x64_AVX512BW_H
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
YM_FORCE_INLINE __mmask64 dec_to_i64_x64_AVX512BW(
    __m512i& T0,
    __m512i a0, __m512i b0, __m512i c0
){
    const __m512i MAX = _mm512_set1_epi8((char)9);

    __mmask64 bad = _mm512_cmpgt_epu8_mask(a0, MAX);
    bad |= _mm512_cmpgt_epu8_mask(b0, MAX);
    bad |= _mm512_cmpgt_epu8_mask(c0, MAX);

    const __mmask64 MASK = 0x5555555555555555;
    {
        __m512i hi;

        hi = _mm512_srli_epi16(a0, 8);
        a0 = _mm512_maskz_mov_epi8(MASK, a0);
        a0 = _mm512_mullo_epi16(a0, _mm512_set1_epi16(10));
        a0 = _mm512_add_epi16(a0, hi);

        hi = _mm512_srli_epi16(b0, 8);
        b0 = _mm512_maskz_mov_epi8(MASK, b0);
        b0 = _mm512_mullo_epi16(b0, _mm512_set1_epi16(10));
        b0 = _mm512_add_epi16(b0, hi);

        hi = _mm512_srli_epi16(c0, 8);
        c0 = _mm512_maskz_mov_epi8(MASK, c0);
        c0 = _mm512_mullo_epi16(c0, _mm512_set1_epi16(10));
        c0 = _mm512_add_epi16(c0, hi);
    }
     {
        const __m512i COMPRESS0 = _mm512_setr_epi64(
            0x0604020080808080, 0x0e0c0a0880808080,
            0x0604020080808080, 0x0e0c0a0880808080,
            0x0604020080808080, 0x0e0c0a0880808080,
            0x0604020080808080, 0x0e0c0a0880808080
        );
        const __m512i COMPRESS1 = _mm512_setr_epi64(
            0x8080808006040200, 0x808080800e0c0a08,
            0x8080808006040200, 0x808080800e0c0a08,
            0x8080808006040200, 0x808080800e0c0a08,
            0x8080808006040200, 0x808080800e0c0a08
        );
        a0 = _mm512_shuffle_epi8(a0, COMPRESS0);
        b0 = _mm512_shuffle_epi8(b0, COMPRESS1);
        a0 = _mm512_or_si512(a0, b0);
    }
    {
        __m512i hi;

        hi = _mm512_srli_epi16(a0, 8);
        a0 = _mm512_maskz_mov_epi8(MASK, a0);
        a0 = _mm512_mullo_epi16(a0, _mm512_set1_epi16(100));
        a0 = _mm512_add_epi16(a0, hi);

        hi = _mm512_srli_epi32(c0, 16);
        c0 = _mm512_maskz_mov_epi16((__mmask32)MASK, c0);
        c0 = _mm512_mullo_epi16(c0, _mm512_set1_epi16(100));
        c0 = _mm512_add_epi16(c0, hi);
    }
    {
        __m512i hi;

        hi = _mm512_srli_epi32(a0, 16);
        a0 = _mm512_maskz_mov_epi16((__mmask32)MASK, a0);
        a0 = _mm512_mullo_epi32(a0, _mm512_set1_epi32(10000));
        a0 = _mm512_add_epi32(a0, hi);

        c0 = _mm512_mul_epu32(c0, _mm512_set1_epi32(400000));
    }
    {
        __m512i hi;

        hi = _mm512_srli_epi64(a0, 32);
        a0 = _mm512_maskz_mov_epi32((__mmask16)MASK, a0);
        a0 = _mm512_mul_epu32(a0, _mm512_set1_epi64(100000000));
        a0 = _mm512_add_epi64(a0, hi);

        c0 = _mm512_mul_epu32(c0, _mm512_set1_epi32(2500000000));
    }

    T0 = _mm512_add_epi64(a0, c0);
    return bad;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
