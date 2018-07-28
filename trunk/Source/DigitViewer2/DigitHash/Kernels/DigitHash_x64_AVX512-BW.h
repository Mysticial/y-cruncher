/* DigitHash_x64_AVX512-BW.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/14/2018
 * Last Modified    : 01/14/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_DigitHash_x64_AVX512BW_H
#define ydv_DigitViewer_DigitHash_x64_AVX512BW_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <immintrin.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "PublicLibs/ArchSpecificLibs/x86_AVX512.h"
namespace DigitViewer2{
namespace DigitHash{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE inline u64_t hash_dec_raw_d128_AVX512BW(u64_t hash_in, const __m512i* T, upL_t blocks){
    if (blocks == 0){
        return hash_in;
    }

    const __m512i COMPRESS = _mm512_set1_epi64(0x0e0c0a0806040200);
    const __m512i PRIME = _mm512_set1_epi64(0x1fffffffffffffffull);

    __m512i hash0 = _mm512_maskz_set1_epi64(0x80, hash_in);

    do{
        __m512i x0 = T[0];
        __m512i x1 = T[1];
        {
            //  1st Merge
            __m512i lo, hi;

            lo = _mm512_and_si512(x0, _mm512_set1_epi16(0x00ff));
            hi = _mm512_srli_epi16(x0, 8);
            lo = _mm512_mullo_epi16(lo, _mm512_set1_epi16(10));
            x0 = _mm512_add_epi16(lo, hi);

            lo = _mm512_and_si512(x1, _mm512_set1_epi16(0x00ff));
            hi = _mm512_srli_epi16(x1, 8);
            lo = _mm512_mullo_epi16(lo, _mm512_set1_epi16(10));
            x1 = _mm512_add_epi16(lo, hi);
        }
        {
            //  Compress
            x0 = _mm512_shuffle_epi8(x0, COMPRESS);
            x1 = _mm512_shuffle_epi8(x1, COMPRESS);
            x0 = _mm512_permutex2var_epi64(
                x0,
                _mm512_setr_epi64(0, 2, 4, 6, 8, 10, 12, 14),
                x1
            );
        }
        {
            //  2nd Merge
            __m512i lo, hi;
            lo = _mm512_and_si512(x0, _mm512_set1_epi16(0x00ff));
            hi = _mm512_srli_epi16(x0, 8);
            lo = _mm512_mullo_epi16(lo, _mm512_set1_epi16(100));
            x0 = _mm512_add_epi16(lo, hi);
        }
        {
            //  3rd Merge
            __m512i lo, hi;
            lo = _mm512_and_si512(x0, _mm512_set1_epi32(0x0000ffff));
            hi = _mm512_srli_epi32(x0, 16);
            lo = _mm512_mullo_epi32(lo, _mm512_set1_epi32(10000));
            x0 = _mm512_add_epi32(lo, hi);
        }
        {
            //  4th Merge
            __m512i lo, hi;
            lo = _mm512_and_si512(x0, _mm512_set1_epi64(0x00000000ffffffff));
            hi = _mm512_srli_epi64(x0, 32);
            lo = _mm512_mul_epu32(lo, _mm512_set1_epi64(100000000));
            x0 = _mm512_add_epi64(lo, hi);
        }
        {
            //  Multiply by scaling
            const __m512i SCALE_X = _mm512_set1_epi64(1745057488448534351); //  10^128 mod (2^61 - 1)
            const __m512i SCALE_H = _mm512_set1_epi64(406302858);           //  SCALE_X / 2^32

            __m512i hash1 = _mm512_shuffle_epi32(hash0, _MM_PERM_CDAB);
            __m512i L0 = _mm512_mul_epu32(hash0, SCALE_X);
            __m512i M0 = _mm512_mul_epu32(hash0, SCALE_H);
            __m512i M1 = _mm512_mul_epu32(hash1, SCALE_X);
            __m512i H0 = _mm512_mul_epu32(hash1, SCALE_H);

            hash0 = _mm512_slli_epi64(H0, 3);

            hash0 = _mm512_or_si512(hash0, _mm512_srli_epi64(L0, 61));
            hash0 = _mm512_add_epi64(hash0, _mm512_and_si512(L0, PRIME));

#if 1
            M0 = _mm512_add_epi64(M0, M1);
            hash0 = _mm512_add_epi64(hash0, _mm512_srli_epi64(M0, 29));
            M0 = _mm512_and_si512(_mm512_slli_epi64(M0, 32), PRIME);
            hash0 = _mm512_add_epi64(hash0, M0);
#else
            hash0 = _mm512_add_epi64(hash0, _mm512_srli_epi64(M0, 29));
            hash0 = _mm512_add_epi64(hash0, _mm512_srli_epi64(M1, 29));

            M0 = _mm512_and_si512(_mm512_slli_epi64(M0, 32), PRIME);
            M1 = _mm512_and_si512(_mm512_slli_epi64(M1, 32), PRIME);
            hash0 = _mm512_add_epi64(hash0, M0);
            hash0 = _mm512_add_epi64(hash0, M1);
#endif
        }
        {
            //  Add and Reduce
            hash0 = _mm512_add_epi64(hash0, x0);
            hash0 = _mm512_add_epi64(_mm512_and_si512(hash0, PRIME), _mm512_srli_epi64(hash0, 61));
        }
        T += 2;
    }while (--blocks);


    //  Collapse

    {
        //  Multiply by scaling
        const __m512i SCALE_X = _mm512_setr_epi64(
            179635585850575169, 554646312237268812, 426790256480816659, 127763649872067382,
            1251348550687181471, 409008371600591580, 10000000000000000, 1
        );
        const __m512i SCALE_H = _mm512_setr_epi64(
            41824669, 129138657, 99369850, 29747292,
            291352288, 95229682, 2328306, 0
        );

        __m512i hash1 = _mm512_shuffle_epi32(hash0, _MM_PERM_CDAB);
        __m512i L0 = _mm512_mul_epu32(hash0, SCALE_X);
        __m512i M0 = _mm512_mul_epu32(hash0, SCALE_H);
        __m512i M1 = _mm512_mul_epu32(hash1, SCALE_X);
        __m512i H0 = _mm512_mul_epu32(hash1, SCALE_H);

        hash0 = _mm512_slli_epi64(H0, 3);

        hash0 = _mm512_or_si512(hash0, _mm512_srli_epi64(L0, 61));
        hash0 = _mm512_add_epi64(hash0, _mm512_and_si512(L0, PRIME));

#if 1
        M0 = _mm512_add_epi64(M0, M1);
        hash0 = _mm512_add_epi64(hash0, _mm512_srli_epi64(M0, 29));
        M0 = _mm512_and_si512(_mm512_slli_epi64(M0, 32), PRIME);
        hash0 = _mm512_add_epi64(hash0, M0);
#else
        hash0 = _mm512_add_epi64(hash0, _mm512_srli_epi64(M0, 29));
        hash0 = _mm512_add_epi64(hash0, _mm512_srli_epi64(M1, 29));

        M0 = _mm512_and_si512(_mm512_slli_epi64(M0, 32), PRIME);
        M1 = _mm512_and_si512(_mm512_slli_epi64(M1, 32), PRIME);
        hash0 = _mm512_add_epi64(hash0, M0);
        hash0 = _mm512_add_epi64(hash0, M1);
#endif
    }

    //  Mod Reduce
    hash0 = _mm512_add_epi64(_mm512_and_si512(hash0, PRIME), _mm512_srli_epi64(hash0, 61));
    hash0 = _mm512_min_epu64(hash0, _mm512_sub_epi64(hash0, PRIME));

    //  Vector Add Reduce
    u64_t r = _mm512_reduce_add_epi64(hash0);

    //  Partial Mod Reduce
    return (r & 0x1fffffffffffffffull) + (r >> 61);
}
YM_NO_INLINE inline u64_t hash_hex_raw_d128_AVX512BW(u64_t hash_in, const __m512i* T, upL_t blocks){
    if (blocks == 0){
        return hash_in;
    }

    const __m512i COMPRESS = _mm512_set1_epi64(0x00020406080a0c0e);
    const __m512i PRIME = _mm512_set1_epi64(0x1fffffffffffffffull);

    __m512i hash0 = _mm512_maskz_set1_epi64(0x80, hash_in);
    do{
        __m512i x0 = T[0];
        __m512i x1 = T[1];
        {
            //  1st Merge
            __m512i lo, hi;

            lo = _mm512_and_si512(x0, _mm512_set1_epi16(0x00ff));
            hi = _mm512_srli_epi16(x0, 8);
            lo = _mm512_slli_epi16(lo, 4);
            x0 = _mm512_or_si512(lo, hi);

            lo = _mm512_and_si512(x1, _mm512_set1_epi16(0x00ff));
            hi = _mm512_srli_epi16(x1, 8);
            lo = _mm512_slli_epi16(lo, 4);
            x1 = _mm512_or_si512(lo, hi);
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
        {
            //  Reduce
            __m512i hi;
            hi = _mm512_srli_epi64(x0, 61);
            x0 = _mm512_and_si512(x0, PRIME);
            x0 = _mm512_add_epi64(x0, hi);
        }
        {
            //  Multiply
            __m512i hi = _mm512_srli_epi64(hash0, 37);
            hash0 = _mm512_slli_epi64(hash0, 24);
            hash0 = _mm512_and_si512(hash0, PRIME);
            hash0 = _mm512_add_epi64(hash0, hi);

            //  Add
            hash0 = _mm512_add_epi64(hash0, x0);
        }
        T += 2;
    }while (--blocks);

    //  Collapse
    {
        __m512i hi;
        hi = _mm512_srlv_epi64(hash0, _mm512_setr_epi64(40, 43, 46, 49, 52, 55, 58, 61));
        hash0 = _mm512_sllv_epi64(hash0, _mm512_setr_epi64(21, 18, 15, 12,  9,  6,  3,  0));
        hash0 = _mm512_and_si512(hash0, PRIME);
        hash0 = _mm512_add_epi64(hash0, hi);
    }

    //  Mod Reduce
    hash0 = _mm512_add_epi64(_mm512_and_si512(hash0, PRIME), _mm512_srli_epi64(hash0, 61));
    hash0 = _mm512_min_epu64(hash0, _mm512_sub_epi64(hash0, PRIME));

    //  Vector Add Reduce
    u64_t r = _mm512_reduce_add_epi64(hash0);

    //  Partial Mod Reduce
    return (r & 0x1fffffffffffffffull) + (r >> 61);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
