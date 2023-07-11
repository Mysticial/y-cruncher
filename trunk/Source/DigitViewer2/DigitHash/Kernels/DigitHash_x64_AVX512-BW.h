/* DigitHash_x64_AVX512-BW.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 01/14/2018
 *  Last Modified   : 01/14/2018
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
YM_FORCE_INLINE __m512i reduce_partial(__m512i hash){
    const __m512i PRIME = _mm512_set1_epi64(0x1fffffffffffffffull);
    return _mm512_add_epi64(_mm512_and_si512(hash, PRIME), _mm512_srli_epi64(hash, 61));
}
YM_FORCE_INLINE __m512i multiply_by_scaling(__m512i hash0, __m512i SCALE_X, __m512i SCALE_H){
    const __m512i PRIME = _mm512_set1_epi64(0x1fffffffffffffffull);

    __m512i hash1 = _mm512_shuffle_epi32(hash0, _MM_PERM_CDAB);
    __m512i L0 = _mm512_mul_epu32(hash0, SCALE_X);
    __m512i M0 = _mm512_mul_epu32(hash0, SCALE_H);
    __m512i M1 = _mm512_mul_epu32(hash1, SCALE_X);
    __m512i H0 = _mm512_mul_epu32(hash1, SCALE_H);

    hash0 = _mm512_slli_epi64(H0, 3);

    hash0 = _mm512_or_si512(hash0, _mm512_srli_epi64(L0, 61));
    hash0 = _mm512_add_epi64(hash0, _mm512_and_si512(L0, PRIME));

    M0 = _mm512_add_epi64(M0, M1);
    hash0 = _mm512_add_epi64(hash0, _mm512_srli_epi64(M0, 29));
    M0 = _mm512_and_si512(_mm512_slli_epi64(M0, 32), PRIME);
    hash0 = _mm512_add_epi64(hash0, M0);

    return hash0;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE inline u64_t hash_dec_raw_d128_AVX512BW(u64_t hash_in, const __m512i* T, upL_t blocks){
    if (blocks == 0){
        return hash_in;
    }

    const __m512i PRIME = _mm512_set1_epi64(0x1fffffffffffffffull);
    const __m512i SCALE1 = _mm512_set1_epi16(((u32_t)1 << 8) | ((u32_t)10 << 0));
    const __m512i SCALE2 = _mm512_set1_epi16(((u32_t)1 << 8) | ((u32_t)100 << 0));
    const __m512i SCALE3 = _mm512_set1_epi32(((u32_t)1 << 16) | ((u32_t)10000 << 0));

    __m512i hash = _mm512_maskz_set1_epi64(0x80, hash_in);

    do{
        __m512i x0 = T[0];
        __m512i x1 = T[1];
        {
            //  1st Merge
            x0 = _mm512_maddubs_epi16(x0, SCALE1);
            x1 = _mm512_maddubs_epi16(x1, SCALE1);
        }
        {
            //  Compress
#ifdef YMP_Arch_2018_x64_CannonLake
            const __m512i COMPRESS = _mm512_setr_epi8(
                 0,  2,  4,  6,  8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30,
                32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62,
                64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94,
                96, 98,100,102,104,106,108,110,112,114,116,118,120,122,124,126
            );
            x0 = _mm512_permutex2var_epi8(x0, COMPRESS, x1);
#else
            const __m512i COMPRESS = _mm512_set1_epi64(0x0e0c0a0806040200);
            x0 = _mm512_shuffle_epi8(x0, COMPRESS);
            x1 = _mm512_shuffle_epi8(x1, COMPRESS);
            x0 = _mm512_permutex2var_epi64(
                x0,
                _mm512_setr_epi64(0, 2, 4, 6, 8, 10, 12, 14),
                x1
            );
#endif
        }
        {
            //  2nd + 3rd Merge
            x0 = _mm512_maddubs_epi16(x0, SCALE2);
            x0 = _mm512_madd_epi16(x0, SCALE3);
        }
        {
            //  4th Merge
            __m512i hi;
            hi = _mm512_srli_epi64(x0, 32);
            x0 = _mm512_mul_epu32(x0, _mm512_set1_epi64(100000000));
            x0 = _mm512_add_epi64(x0, hi);
        }
        {
            //  Multiply by scaling
            hash = multiply_by_scaling(
                hash,
                _mm512_set1_epi64(1745057488448534351), //  10^128 mod (2^61 - 1)
                _mm512_set1_epi64(406302858)            //  SCALE_X / 2^32
            );
        }
        {
            //  Add and Reduce
            hash = _mm512_add_epi64(hash, x0);
            hash = reduce_partial(hash);
        }
        T += 2;
    }while (--blocks);


    //  Collapse

    hash = multiply_by_scaling(
        hash,
        _mm512_setr_epi64(
            179635585850575169, 554646312237268812, 426790256480816659, 127763649872067382,
            1251348550687181471, 409008371600591580, 10000000000000000, 1
        ),
        _mm512_setr_epi64(
            41824669, 129138657, 99369850, 29747292,
            291352288, 95229682, 2328306, 0
        )
    );

    //  Mod Reduce
    hash = reduce_partial(hash);
    hash = _mm512_min_epu64(hash, _mm512_sub_epi64(hash, PRIME));

    //  Vector Add Reduce
    u64_t r = _mm512_reduce_add_epi64(hash);

    //  Partial Mod Reduce
    return (r & 0x1fffffffffffffffull) + (r >> 61);
}
YM_NO_INLINE inline u64_t hash_hex_raw_d128_AVX512BW(u64_t hash_in, const __m512i* T, upL_t blocks){
    if (blocks == 0){
        return hash_in;
    }

    const __m512i PRIME = _mm512_set1_epi64(0x1fffffffffffffffull);
    const __m512i SCALE1 = _mm512_set1_epi16(((u32_t)1 << 8) | ((u32_t)16 << 0));

    __m512i hash = _mm512_maskz_set1_epi64(0x80, hash_in);
    do{
        __m512i x0 = T[0];
        __m512i x1 = T[1];
        {
            //  1st Merge
            x0 = _mm512_maddubs_epi16(x0, SCALE1);
            x1 = _mm512_maddubs_epi16(x1, SCALE1);
        }
        {
            //  Compress and Reverse
#ifdef YMP_Arch_2018_x64_CannonLake
            const __m512i COMPRESS = _mm512_setr_epi8(
                14, 12, 10,  8,  6,  4,  2,  0, 30, 28, 26, 24, 22, 20, 18, 16,
                46, 44, 42, 40, 38, 36, 34, 32, 62, 60, 58, 56, 54, 52, 50, 48,
                78, 76, 74, 72, 70, 68, 66, 64, 94, 92, 90, 88, 86, 84, 82, 80,
               110,108,106,104,102,100, 98, 96,126,124,122,120,118,116,114,112
            );
            x0 = _mm512_permutex2var_epi8(x0, COMPRESS, x1);
#else
            const __m512i COMPRESS = _mm512_set1_epi64(0x00020406080a0c0e);
            x0 = _mm512_shuffle_epi8(x0, COMPRESS);
            x1 = _mm512_shuffle_epi8(x1, COMPRESS);
            x0 = _mm512_permutex2var_epi64(
                x0,
                _mm512_setr_epi64(0, 2, 4, 6, 8, 10, 12, 14),
                x1
            );
#endif
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
            __m512i hi = _mm512_srli_epi64(hash, 37);
            hash = _mm512_slli_epi64(hash, 24);
            hash = _mm512_and_si512(hash, PRIME);
            hash = _mm512_add_epi64(hash, hi);

            //  Add
            hash = _mm512_add_epi64(hash, x0);
        }
        T += 2;
    }while (--blocks);

    //  Collapse
    {
        __m512i hi;
        hi = _mm512_srlv_epi64(hash, _mm512_setr_epi64(40, 43, 46, 49, 52, 55, 58, 61));
        hash = _mm512_sllv_epi64(hash, _mm512_setr_epi64(21, 18, 15, 12,  9,  6,  3,  0));
        hash = _mm512_and_si512(hash, PRIME);
        hash = _mm512_add_epi64(hash, hi);
    }

    //  Mod Reduce
    hash = reduce_partial(hash);
    hash = _mm512_min_epu64(hash, _mm512_sub_epi64(hash, PRIME));

    //  Vector Add Reduce
    u64_t r = _mm512_reduce_add_epi64(hash);

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
