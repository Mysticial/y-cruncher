/* DigitHash_x64_AVX512-VNNI.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 05/04/2023
 *  Last Modified   : 05/04/2023
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_DigitHash_x64_AVX512VNNI_H
#define ydv_DigitViewer_DigitHash_x64_AVX512VNNI_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "Settings.h"   //  REMOVE
#include "Modules/ShortVectorPrinting.h"    //  REMOVE
#include <immintrin.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "PublicLibs/ArchSpecificLibs/x86_AVX512.h"
#include "DigitHash_x64_AVX512-BW.h"
namespace DigitViewer2{
namespace DigitHash{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE inline u64_t hash_dec_raw_d128_AVX512VNNI(u64_t hash_in, const __m512i* T, upL_t blocks){
    if (blocks == 0){
        return hash_in;
    }

    const __m512i PRIME = _mm512_set1_epi64(0x1fffffffffffffffull);
    const __m512i COMPRESS = _mm512_setr_epi16(
            0,  2,  4,  6,  8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30,
        32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62
    );
    const __m512i SHIFT = _mm512_set1_epi32(2);
    const __m512i SCALE12 = _mm512_set1_epi32(((u32_t)1 << 24) | ((u32_t)10 << 16) | ((u32_t)25 << 8) | ((u32_t)250 << 0));
    const __m512i SCALE3 = _mm512_set1_epi32(((u32_t)1 << 16) | ((u32_t)10000 << 0));

    __m512i hash = _mm512_maskz_set1_epi64(0x80, hash_in);

    do{
        __m512i x0 = T[0];
        __m512i x1 = T[1];
#if 0
        {
            //  Merges 1 and 2 + compress.
            x0 = _mm512_sllv_epi16(x0, SHIFT);
            x1 = _mm512_sllv_epi16(x1, SHIFT);
            x0 = _mm512_dpbusd_epi32(_mm512_setzero_si512(), SCALE12, x0);
            x1 = _mm512_dpbusd_epi32(_mm512_setzero_si512(), SCALE12, x1);
            x0 = _mm512_permutex2var_epi16(x0, COMPRESS, x1);
        }
#else
        {
            //  1st Merge
            const __m512i SCALE1 = _mm512_set1_epi16(((u32_t)1 << 8) | ((u32_t)10 << 0));
            x0 = _mm512_maddubs_epi16(x0, SCALE1);
            x1 = _mm512_maddubs_epi16(x1, SCALE1);
        }
        {
            //  Compress
            const __m512i COMPRESS1 = _mm512_setr_epi8(
                 0,  2,  4,  6,  8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30,
                32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62,
                64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94,
                96, 98,100,102,104,106,108,110,112,114,116,118,120,122,124,126
            );
            const __m512i SCALE2 = _mm512_set1_epi16(((u32_t)1 << 8) | ((u32_t)100 << 0));
            x0 = _mm512_permutex2var_epi8(x0, COMPRESS1, x1);
            x0 = _mm512_maddubs_epi16(x0, SCALE2);
        }
#endif
        {
            //  3rd Merge
            x0 = _mm512_madd_epi16(x0, SCALE3);
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
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
