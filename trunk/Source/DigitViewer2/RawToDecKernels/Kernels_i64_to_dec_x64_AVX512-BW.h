/* Kernels_i64_to_dec_x64_AVX512-BW.h
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
#ifndef ydv_DigitViewer_Kernels_i64_to_dec_x64_AVX512BW_H
#define ydv_DigitViewer_Kernels_i64_to_dec_x64_AVX512BW_H
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
YM_FORCE_INLINE __m512i div_100000000(__m512i x0){
    __m512i r0, r1, r2, r3;

    r3 = _mm512_shuffle_epi32(x0, _MM_PERM_CDAB);

    r0 = _mm512_mul_epu32(x0, _mm512_set1_epi32(2882303761));
    r1 = _mm512_mul_epu32(r3, _mm512_set1_epi32(2221002493));
    r2 = _mm512_mul_epu32(x0, _mm512_set1_epi32(2221002493));

    r2 = _mm512_srli_epi64(r2, 32);
    r0 = _mm512_add_epi64(r0, r2);
    r2 = _mm512_mask_blend_epi32(0xaaaa, r1, _mm512_setzero_si512());
    r0 = _mm512_add_epi64(r0, r2);

    r0 = _mm512_srli_epi64(r0, 32);
    r1 = _mm512_srli_epi64(r1, 32);

    r3 = _mm512_mul_epu32(r3, _mm512_set1_epi32(2882303761));
    r3 = _mm512_add_epi64(r3, r0);
    r3 = _mm512_add_epi64(r3, r1);

    return _mm512_srli_epi64(r3, 26);
}
YM_FORCE_INLINE void i64_to_dec_x64_AVX512BW(
    __m512i T0,
    __m512i& a0, __m512i& b0, __m512i& c0
){
    {
        __m512i r0;

        b0 = div_100000000(T0);

        r0 = _mm512_mul_epu32(b0, _mm512_set1_epi32(100000000));
        a0 = _mm512_sub_epi32(T0, r0);
    }
    {
        __m512i r0;

        c0 = div_100000000(b0);

        r0 = _mm512_mul_epu32(c0, _mm512_set1_epi32(100000000));
        b0 = _mm512_sub_epi32(b0, r0);
    }

     __m512i x0 = _mm512_mask_shuffle_epi32(b0, 0xaaaa, a0, _MM_PERM_CDAB);

    {
        __m512i lo, hi;

        //  Invariant multiply
        hi = _mm512_shuffle_epi32(x0, _MM_PERM_CDAB);
        lo = _mm512_mul_epu32(x0, _mm512_set1_epi32(3518437209));
        hi = _mm512_mul_epu32(hi, _mm512_set1_epi32(3518437209));
        hi = _mm512_mask_shuffle_epi32(hi, 0x5555, lo, _MM_PERM_CDAB);

        //  Down shift
        hi = _mm512_srli_epi32(hi, 13);

        //  Multiply + subtract
        lo = _mm512_mullo_epi32(hi, _mm512_set1_epi32(10000));
        x0 = _mm512_sub_epi32(x0, lo);

        //  Combine
        x0 = _mm512_slli_epi32(x0, 16);
        x0 = _mm512_or_si512(x0, hi);
    }
    {
        __m512i lo, hi;

        //  Divide
        hi = _mm512_srli_epi16(c0, 2);
        hi = _mm512_mulhi_epu16(hi, _mm512_set1_epi16(5243));
        hi = _mm512_srli_epi16(hi, 1);

        //  Multiply + subtract
        lo = _mm512_mullo_epi16(hi, _mm512_set1_epi16(100));
        c0 = _mm512_sub_epi16(c0, lo);

        //  Combine
        c0 = _mm512_slli_epi32(c0, 16);
        c0 = _mm512_or_si512(c0, hi);
    }
    {
        __m512i lo, hi;

        //  Divide
        hi = _mm512_srli_epi16(x0, 2);
        hi = _mm512_mulhi_epu16(hi, _mm512_set1_epi16(5243));
        hi = _mm512_srli_epi16(hi, 1);

        //  Multiply + subtract
        lo = _mm512_mullo_epi16(hi, _mm512_set1_epi16(100));
        x0 = _mm512_sub_epi16(x0, lo);

        //  Combine
        x0 = _mm512_slli_epi16(x0, 8);
        x0 = _mm512_or_si512(x0, hi);
    }

    {
        __m512i hi;

        //  Divide
        hi = _mm512_mullo_epi16(c0, _mm512_set1_epi16(205));

#if 1
        hi = _mm512_srli_epi16(hi, 3);

        //  Multiply + subtract
        c0 = _mm512_mask_mov_epi8(c0, 0xaaaaaaaaaaaaaaaa, hi);
        c0 = _mm512_maddubs_epi16(_mm512_set1_epi16((u16_t)0xf601), c0);

        //  Fix ordering
        c0 = _mm512_shuffle_epi8(
            c0,
            _mm512_setr_epi64(
                0xffffffffff020300, 0xffffffffff0a0b08,
                0xffffffffff020300, 0xffffffffff0a0b08,
                0xffffffffff020300, 0xffffffffff0a0b08,
                0xffffffffff020300, 0xffffffffff0a0b08
            )
        );
#else
        hi = _mm512_srli_epi16(hi, 11);

        //  Multiply + subtract
        lo = _mm512_mullo_epi16(hi, _mm512_set1_epi16(10));
        c0 = _mm512_sub_epi16(c0, lo);

        //  Combine
        hi = _mm512_srli_epi32(hi, 8);
        c0 = _mm512_or_si512(c0, hi);
#endif
    }
    {
        //  Split
        a0 = _mm512_shuffle_epi8(
            x0,
            _mm512_setr_epi64(
                0x8007800680058004, 0x800f800e800d800c,
                0x8007800680058004, 0x800f800e800d800c,
                0x8007800680058004, 0x800f800e800d800c,
                0x8007800680058004, 0x800f800e800d800c
            )
        );
        b0 = _mm512_shuffle_epi8(
            x0,
            _mm512_setr_epi64(
                0x8003800280018000, 0x800b800a80098008,
                0x8003800280018000, 0x800b800a80098008,
                0x8003800280018000, 0x800b800a80098008,
                0x8003800280018000, 0x800b800a80098008
            )
        );
    }

    const __m512i PERMUTE_MULTIPLY = _mm512_setr_epi64(
        0x0607040502030001, 0x0e0f0c0d0a0b0809,
        0x0607040502030001, 0x0e0f0c0d0a0b0809,
        0x0607040502030001, 0x0e0f0c0d0a0b0809,
        0x0607040502030001, 0x0e0f0c0d0a0b0809
    );
    {
        __m512i hi;

        //  Divide
        hi = _mm512_mullo_epi16(a0, _mm512_set1_epi16(205));
        hi = _mm512_srli_epi16(hi, 3);

        //  Multiply + subtract
        a0 = _mm512_mask_mov_epi8(a0, 0xaaaaaaaaaaaaaaaa, hi);
        a0 = _mm512_maddubs_epi16(_mm512_set1_epi16((u16_t)0xf601), a0);

        //  Fix ordering
        a0 = _mm512_shuffle_epi8(a0, PERMUTE_MULTIPLY);
    }
    {
        __m512i hi;

        //  Divide
        hi = _mm512_mullo_epi16(b0, _mm512_set1_epi16(205));
        hi = _mm512_srli_epi16(hi, 3);

        //  Multiply + subtract
        b0 = _mm512_mask_mov_epi8(b0, 0xaaaaaaaaaaaaaaaa, hi);
        b0 = _mm512_maddubs_epi16(_mm512_set1_epi16((u16_t)0xf601), b0);

        //  Fix ordering
        b0 = _mm512_shuffle_epi8(b0, PERMUTE_MULTIPLY);
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
