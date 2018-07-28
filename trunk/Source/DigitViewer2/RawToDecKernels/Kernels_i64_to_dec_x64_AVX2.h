/* Kernels_i64_to_dec_x64_AVX2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/11/2018
 * Last Modified    : 02/11/2018
 * 
 *      This implementation is new to DigitViewer2. While the old approach uses
 *  floating-point reciprocals, this method is an all-integer algorithm based on
 *  the invariant multiply method.
 * 
 *  The integer invariant-multiply method seems to be about 50% faster than the
 *  floating-point reciprocal method.
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_Kernels_i64_to_dec_x64_AVX2_H
#define ydv_DigitViewer_Kernels_i64_to_dec_x64_AVX2_H
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
YM_FORCE_INLINE __m256i div_100000000(__m256i x0){
    __m256i r0, r1, r2, r3;

    r3 = _mm256_shuffle_epi32(x0, 177);

    r0 = _mm256_mul_epu32(x0, _mm256_set1_epi32(2882303761));
    r1 = _mm256_mul_epu32(r3, _mm256_set1_epi32(2221002493));
    r2 = _mm256_mul_epu32(x0, _mm256_set1_epi32(2221002493));

    r2 = _mm256_srli_epi64(r2, 32);
    r0 = _mm256_add_epi64(r0, r2);
    r2 = _mm256_blend_epi32(r1, _mm256_setzero_si256(), 0xaa);
    r0 = _mm256_add_epi64(r0, r2);

    r0 = _mm256_srli_epi64(r0, 32);
    r1 = _mm256_srli_epi64(r1, 32);

    r3 = _mm256_mul_epu32(r3, _mm256_set1_epi32(2882303761));
    r3 = _mm256_add_epi64(r3, r0);
    r3 = _mm256_add_epi64(r3, r1);

    return _mm256_srli_epi64(r3, 26);
}
YM_FORCE_INLINE void i64_to_dec_x64_AVX2(
    __m256i T0,
    __m256i& a0, __m256i& b0, __m256i& c0
){
    {
        __m256i r0;

        b0 = div_100000000(T0);

        r0 = _mm256_mul_epu32(b0, _mm256_set1_epi32(100000000));
        a0 = _mm256_sub_epi32(T0, r0);
    }
    {
        __m256i r0;

        c0 = div_100000000(b0);

        r0 = _mm256_mul_epu32(c0, _mm256_set1_epi32(100000000));
        b0 = _mm256_sub_epi32(b0, r0);
    }

     __m256i x0 = _mm256_slli_epi64(a0, 32);
     x0 = _mm256_blend_epi32(x0, b0, 0x55);

    {
        __m256i lo, hi;

        //  Invariant multiply
        hi = _mm256_shuffle_epi32(x0, 177);
        lo = _mm256_mul_epu32(x0, _mm256_set1_epi32(3518437209));
        hi = _mm256_mul_epu32(hi, _mm256_set1_epi32(3518437209));

        //  Down shift
        lo = _mm256_srli_epi64(lo, 45);
        hi = _mm256_srli_epi32(hi, 13);
        hi = _mm256_blend_epi32(hi, lo, 0x55);

        //  Multiply + subtract
        lo = _mm256_mullo_epi32(hi, _mm256_set1_epi32(10000));
        x0 = _mm256_sub_epi32(x0, lo);

        //  Combine
        x0 = _mm256_slli_epi32(x0, 16);
        x0 = _mm256_or_si256(x0, hi);
    }
    {
        __m256i lo, hi;

        //  Divide
        hi = _mm256_srli_epi16(c0, 2);
        hi = _mm256_mulhi_epu16(hi, _mm256_set1_epi16(5243));
        hi = _mm256_srli_epi16(hi, 1);

        //  Multiply + subtract
        lo = _mm256_mullo_epi16(hi, _mm256_set1_epi16(100));
        c0 = _mm256_sub_epi16(c0, lo);

        //  Combine
        c0 = _mm256_slli_epi32(c0, 16);
        c0 = _mm256_or_si256(c0, hi);
    }
    {
        __m256i lo, hi;

        //  Divide
        hi = _mm256_srli_epi16(x0, 2);
        hi = _mm256_mulhi_epu16(hi, _mm256_set1_epi16(5243));
        hi = _mm256_srli_epi16(hi, 1);

        //  Multiply + subtract
        lo = _mm256_mullo_epi16(hi, _mm256_set1_epi16(100));
        x0 = _mm256_sub_epi16(x0, lo);

        //  Combine
        x0 = _mm256_slli_epi16(x0, 8);
        x0 = _mm256_or_si256(x0, hi);
    }
    {
        __m256i lo, hi;

        //  Divide
        hi = _mm256_mullo_epi16(c0, _mm256_set1_epi16(205));
        hi = _mm256_srli_epi16(hi, 11);

        //  Multiply + subtract
        lo = _mm256_mullo_epi16(hi, _mm256_set1_epi16(10));
        c0 = _mm256_sub_epi16(c0, lo);

        //  Combine
        hi = _mm256_srli_epi32(hi, 8);
        c0 = _mm256_or_si256(c0, hi);
    }
    {
        //  Split
        a0 = _mm256_shuffle_epi8(
            x0,
            _mm256_setr_epi64x(
                0x8007800680058004, 0x800f800e800d800c,
                0x8007800680058004, 0x800f800e800d800c
            )
        );
        b0 = _mm256_shuffle_epi8(
            x0,
            _mm256_setr_epi64x(
                0x8003800280018000, 0x800b800a80098008,
                0x8003800280018000, 0x800b800a80098008
            )
        );
    }
    {
        __m256i lo, hi;

        //  Divide
        hi = _mm256_mullo_epi16(a0, _mm256_set1_epi16(205));
        hi = _mm256_srli_epi16(hi, 11);

        //  Multiply + subtract
        lo = _mm256_mullo_epi16(hi, _mm256_set1_epi16(10));
        a0 = _mm256_sub_epi16(a0, lo);

        //  Combine
        a0 = _mm256_slli_epi16(a0, 8);
        a0 = _mm256_or_si256(a0, hi);
    }
    {
        __m256i lo, hi;

        //  Divide
        hi = _mm256_mullo_epi16(b0, _mm256_set1_epi16(205));
        hi = _mm256_srli_epi16(hi, 11);

        //  Multiply + subtract
        lo = _mm256_mullo_epi16(hi, _mm256_set1_epi16(10));
        b0 = _mm256_sub_epi16(b0, lo);

        //  Combine
        b0 = _mm256_slli_epi16(b0, 8);
        b0 = _mm256_or_si256(b0, hi);
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
