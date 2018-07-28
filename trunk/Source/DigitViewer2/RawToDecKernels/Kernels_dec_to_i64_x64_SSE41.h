/* Kernels_dec_to_i64_x64_SSE41.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/11/2018
 * Last Modified    : 02/11/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_Kernels_dec_to_i64_x64_SSE41_H
#define ydv_DigitViewer_Kernels_dec_to_i64_x64_SSE41_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <smmintrin.h>
#ifdef _WIN32
// Visual Studio puts _mm_set1_epi64x() in <intrin.h> instead of <emmintrin.h>.
#include <intrin.h>
#endif
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace DigitViewer2{
namespace RawToDec{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE __m128i dec_to_i64_x64_SSE41(
    __m128i& T0,
    __m128i a0, __m128i b0, __m128i c0
){
    const __m128i MIN = _mm_set1_epi8((char)-128);
    const __m128i MAX = _mm_set1_epi8((char)-119);

    __m128i bad = _mm_cmpgt_epi8(_mm_xor_si128(a0, MIN), MAX);
    bad = _mm_or_si128(bad, _mm_cmpgt_epi8(_mm_xor_si128(b0, MIN), MAX));
    bad = _mm_or_si128(bad, _mm_cmpgt_epi8(_mm_xor_si128(c0, MIN), MAX));

    {
        __m128i hi;

        hi = _mm_srli_epi16(a0, 8);
        a0 = _mm_and_si128(a0, _mm_set1_epi16(0x00ff));
        a0 = _mm_mullo_epi16(a0, _mm_set1_epi16(10));
        a0 = _mm_add_epi16(a0, hi);

        hi = _mm_srli_epi16(b0, 8);
        b0 = _mm_and_si128(b0, _mm_set1_epi16(0x00ff));
        b0 = _mm_mullo_epi16(b0, _mm_set1_epi16(10));
        b0 = _mm_add_epi16(b0, hi);

        hi = _mm_srli_epi16(c0, 8);
        c0 = _mm_and_si128(c0, _mm_set1_epi16(0x00ff));
        c0 = _mm_mullo_epi16(c0, _mm_set1_epi16(10));
        c0 = _mm_add_epi16(c0, hi);
    }
    {
        const __m128i COMPRESS0 = _mm_setr_epi32(
            0x80808080, 0x06040200, 0x80808080, 0x0e0c0a08
        );
        const __m128i COMPRESS1 = _mm_setr_epi32(
            0x06040200, 0x80808080, 0x0e0c0a08, 0x80808080
        );
        a0 = _mm_shuffle_epi8(a0, COMPRESS0);
        b0 = _mm_shuffle_epi8(b0, COMPRESS1);
        a0 = _mm_or_si128(a0, b0);
    }
    {
        __m128i hi;

        hi = _mm_srli_epi16(a0, 8);
        a0 = _mm_and_si128(a0, _mm_set1_epi16(0x00ff));
        a0 = _mm_mullo_epi16(a0, _mm_set1_epi16(100));
        a0 = _mm_add_epi16(a0, hi);

        hi = _mm_srli_epi32(c0, 16);
        c0 = _mm_and_si128(c0, _mm_set1_epi32(0x0000ffff));
        c0 = _mm_mullo_epi16(c0, _mm_set1_epi16(100));
        c0 = _mm_add_epi16(c0, hi);
    }
    {
        __m128i hi;

        hi = _mm_srli_epi32(a0, 16);
        a0 = _mm_and_si128(a0, _mm_set1_epi32(0x0000ffff));
        a0 = _mm_mullo_epi32(a0, _mm_set1_epi32(10000));
        a0 = _mm_add_epi32(a0, hi);

        c0 = _mm_mul_epu32(c0, _mm_set1_epi32(400000));
    }
    {
        __m128i hi;

        hi = _mm_srli_epi64(a0, 32);
        a0 = _mm_and_si128(a0, _mm_set1_epi64x(0x00000000ffffffff));
        a0 = _mm_mul_epu32(a0, _mm_set1_epi64x(100000000));
        a0 = _mm_add_epi64(a0, hi);

        c0 = _mm_mul_epu32(c0, _mm_set1_epi32(2500000000));
    }

    T0 = _mm_add_epi64(a0, c0);
    return bad;
}
YM_FORCE_INLINE __m128i dec_to_i64_x64_SSE41(
    __m128i& T0, __m128i& T1,
    __m128i a0, __m128i b0, __m128i c0,
    __m128i a1, __m128i b1, __m128i c1
){
    const __m128i MIN = _mm_set1_epi8((char)-128);
    const __m128i MAX = _mm_set1_epi8((char)-119);

    __m128i bad = _mm_cmpgt_epi8(_mm_xor_si128(a0, MIN), MAX);
    bad = _mm_or_si128(bad, _mm_cmpgt_epi8(_mm_xor_si128(b0, MIN), MAX));
    bad = _mm_or_si128(bad, _mm_cmpgt_epi8(_mm_xor_si128(c0, MIN), MAX));

    bad = _mm_or_si128(bad, _mm_cmpgt_epi8(_mm_xor_si128(a1, MIN), MAX));
    bad = _mm_or_si128(bad, _mm_cmpgt_epi8(_mm_xor_si128(b1, MIN), MAX));
    bad = _mm_or_si128(bad, _mm_cmpgt_epi8(_mm_xor_si128(c1, MIN), MAX));

    {
        __m128i hi;

        hi = _mm_srli_epi16(a0, 8);
        a0 = _mm_and_si128(a0, _mm_set1_epi16(0x00ff));
        a0 = _mm_mullo_epi16(a0, _mm_set1_epi16(10));
        a0 = _mm_add_epi16(a0, hi);

        hi = _mm_srli_epi16(b0, 8);
        b0 = _mm_and_si128(b0, _mm_set1_epi16(0x00ff));
        b0 = _mm_mullo_epi16(b0, _mm_set1_epi16(10));
        b0 = _mm_add_epi16(b0, hi);

        hi = _mm_srli_epi16(c0, 8);
        c0 = _mm_and_si128(c0, _mm_set1_epi16(0x00ff));
        c0 = _mm_mullo_epi16(c0, _mm_set1_epi16(10));
        c0 = _mm_add_epi16(c0, hi);

        hi = _mm_srli_epi16(a1, 8);
        a1 = _mm_and_si128(a1, _mm_set1_epi16(0x00ff));
        a1 = _mm_mullo_epi16(a1, _mm_set1_epi16(10));
        a1 = _mm_add_epi16(a1, hi);

        hi = _mm_srli_epi16(b1, 8);
        b1 = _mm_and_si128(b1, _mm_set1_epi16(0x00ff));
        b1 = _mm_mullo_epi16(b1, _mm_set1_epi16(10));
        b1 = _mm_add_epi16(b1, hi);

        hi = _mm_srli_epi16(c1, 8);
        c1 = _mm_and_si128(c1, _mm_set1_epi16(0x00ff));
        c1 = _mm_mullo_epi16(c1, _mm_set1_epi16(10));
        c1 = _mm_add_epi16(c1, hi);
    }
    {
        const __m128i COMPRESS0 = _mm_setr_epi32(
            0x80808080, 0x06040200, 0x80808080, 0x0e0c0a08
        );
        const __m128i COMPRESS1 = _mm_setr_epi32(
            0x06040200, 0x80808080, 0x0e0c0a08, 0x80808080
        );
        a0 = _mm_shuffle_epi8(a0, COMPRESS0);
        b0 = _mm_shuffle_epi8(b0, COMPRESS1);
        a0 = _mm_or_si128(a0, b0);

        a1 = _mm_shuffle_epi8(a1, COMPRESS0);
        b1 = _mm_shuffle_epi8(b1, COMPRESS1);
        a1 = _mm_or_si128(a1, b1);
    }
    {
        __m128i hi;

        hi = _mm_srli_epi16(a0, 8);
        a0 = _mm_and_si128(a0, _mm_set1_epi16(0x00ff));
        a0 = _mm_mullo_epi16(a0, _mm_set1_epi16(100));
        a0 = _mm_add_epi16(a0, hi);

        hi = _mm_srli_epi32(c0, 16);
        c0 = _mm_and_si128(c0, _mm_set1_epi32(0x0000ffff));
        c0 = _mm_mullo_epi16(c0, _mm_set1_epi16(100));
        c0 = _mm_add_epi16(c0, hi);

        hi = _mm_srli_epi16(a1, 8);
        a1 = _mm_and_si128(a1, _mm_set1_epi16(0x00ff));
        a1 = _mm_mullo_epi16(a1, _mm_set1_epi16(100));
        a1 = _mm_add_epi16(a1, hi);

        hi = _mm_srli_epi32(c1, 16);
        c1 = _mm_and_si128(c1, _mm_set1_epi32(0x0000ffff));
        c1 = _mm_mullo_epi16(c1, _mm_set1_epi16(100));
        c1 = _mm_add_epi16(c1, hi);
    }
    {
        __m128i hi;

        hi = _mm_srli_epi32(a0, 16);
        a0 = _mm_and_si128(a0, _mm_set1_epi32(0x0000ffff));
        a0 = _mm_mullo_epi32(a0, _mm_set1_epi32(10000));
        a0 = _mm_add_epi32(a0, hi);

        c0 = _mm_mul_epu32(c0, _mm_set1_epi32(400000));

        hi = _mm_srli_epi32(a1, 16);
        a1 = _mm_and_si128(a1, _mm_set1_epi32(0x0000ffff));
        a1 = _mm_mullo_epi32(a1, _mm_set1_epi32(10000));
        a1 = _mm_add_epi32(a1, hi);

        c1 = _mm_mul_epu32(c1, _mm_set1_epi32(400000));
    }
    {
        __m128i hi;

        hi = _mm_srli_epi64(a0, 32);
        a0 = _mm_and_si128(a0, _mm_set1_epi64x(0x00000000ffffffff));
        a0 = _mm_mul_epu32(a0, _mm_set1_epi64x(100000000));
        a0 = _mm_add_epi64(a0, hi);

        c0 = _mm_mul_epu32(c0, _mm_set1_epi32(2500000000));

        hi = _mm_srli_epi64(a1, 32);
        a1 = _mm_and_si128(a1, _mm_set1_epi64x(0x00000000ffffffff));
        a1 = _mm_mul_epu32(a1, _mm_set1_epi64x(100000000));
        a1 = _mm_add_epi64(a1, hi);

        c1 = _mm_mul_epu32(c1, _mm_set1_epi32(2500000000));
    }

    T0 = _mm_add_epi64(a0, c0);
    T1 = _mm_add_epi64(a1, c1);
    return bad;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
