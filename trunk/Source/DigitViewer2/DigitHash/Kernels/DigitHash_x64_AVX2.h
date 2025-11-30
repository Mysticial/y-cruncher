/* DigitHash_x64_AVX2.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 01/14/2018
 *  Last Modified   : 01/14/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_DigitHash_x64_AVX2_H
#define ydv_DigitViewer_DigitHash_x64_AVX2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <immintrin.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "PublicLibs/ArchSpecificLibs/x86_AVX2.h"
namespace DigitViewer2{
namespace DigitHash{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE __m256i reduce_partial(__m256i hash){
    const __m256i PRIME = _mm256_set1_epi64x(0x1fffffffffffffffull);
    return _mm256_add_epi64(_mm256_and_si256(hash, PRIME), _mm256_srli_epi64(hash, 61));
}
YM_FORCE_INLINE __m256i multiply_by_scaling(__m256i hash0, __m256i SCALE_X, __m256i SCALE_H){
    const __m256i PRIME = _mm256_set1_epi64x(0x1fffffffffffffffull);

    __m256i hash1 = _mm256_shuffle_epi32(hash0, 177);
    __m256i L0 = _mm256_mul_epu32(hash0, SCALE_X);
    __m256i M0 = _mm256_mul_epu32(hash0, SCALE_H);
    __m256i M1 = _mm256_mul_epu32(hash1, SCALE_X);
    __m256i H0 = _mm256_mul_epu32(hash1, SCALE_H);

    hash0 = _mm256_slli_epi64(H0, 3);

    hash0 = _mm256_or_si256(hash0, _mm256_srli_epi64(L0, 61));
    hash0 = _mm256_add_epi64(hash0, _mm256_and_si256(L0, PRIME));

    M0 = _mm256_add_epi64(M0, M1);
    hash0 = _mm256_add_epi64(hash0, _mm256_srli_epi64(M0, 29));
    M0 = _mm256_and_si256(_mm256_slli_epi64(M0, 32), PRIME);
    hash0 = _mm256_add_epi64(hash0, M0);

    return hash0;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE inline u64_t hash_dec_raw_d64_AVX2(u64_t hash_in, const __m256i* T, upL_t blocks){
    if (blocks == 0){
        return hash_in;
    }

//    const __m256i PRIME = _mm256_set1_epi64x(0x1fffffffffffffffull);
    const __m256i COMPRESS = _mm256_setr_epi8(
        0,  2,  4,  6,  8, 10, 12, 14,  1,  3,  5,  7,  9, 11, 13, 15,
        0,  2,  4,  6,  8, 10, 12, 14,  1,  3,  5,  7,  9, 11, 13, 15
    );
    const __m256i SCALE1 = _mm256_set1_epi16(((u32_t)1 << 8) | ((u32_t)10 << 0));
    const __m256i SCALE2 = _mm256_set1_epi16(((u32_t)1 << 8) | ((u32_t)100 << 0));
    const __m256i SCALE3 = _mm256_set1_epi32(((u32_t)1 << 16) | ((u32_t)10000 << 0));

    __m256i hash0 = _mm256_set1_epi64x(hash_in);
    hash0 = _mm256_and_si256(hash0, _mm256_setr_epi64x(0, 0, 0, (u64_t)0 - 1));

    do{
        __m256i x0 = T[0];
        __m256i x1 = T[1];
        {
            //  1st Merge
            x0 = _mm256_maddubs_epi16(x0, SCALE1);
            x1 = _mm256_maddubs_epi16(x1, SCALE1);
        }
        {
            //  Compress
            x0 = _mm256_shuffle_epi8(x0, COMPRESS);
            x1 = _mm256_shuffle_epi8(x1, COMPRESS);

            x0 = _mm256_permute4x64_epi64(x0, 216);
            x1 = _mm256_permute4x64_epi64(x1, 141);

            x0 = _mm256_or_si256(x0, x1);
        }
        {
            //  2nd + 3rd Merge
            x0 = _mm256_maddubs_epi16(x0, SCALE2);
            x0 = _mm256_madd_epi16(x0, SCALE3);
        }
        {
            //  4th Merge
            __m256i hi;
            hi = _mm256_srli_epi64(x0, 32);
            x0 = _mm256_mul_epu32(x0, _mm256_set1_epi64x(100000000));
            x0 = _mm256_add_epi64(x0, hi);
        }
        {
            //  Multiply by scaling
            hash0 = multiply_by_scaling(
                hash0,
                _mm256_set1_epi64x(127763649872067382), //  10^64 mod (2^61 - 1)
                _mm256_set1_epi64x(29747292)            //  SCALE_X / 2^32
            );
        }
        {
            //  Add and Reduce
            hash0 = _mm256_add_epi64(hash0, x0);
            hash0 = reduce_partial(hash0);
        }
        T += 2;
    }while (--blocks);


    //  Collapse
    {
        //  Multiply by scaling
        hash0 = multiply_by_scaling(
            hash0,
            _mm256_setr_epi64x(1251348550687181471, 409008371600591580, 10000000000000000, 1),
            _mm256_setr_epi64x(291352288, 95229682, 2328306, 0)
        );
    }

    //  Partial Mod Reduce
    hash0 = reduce_partial(hash0);

    //  Vector Add Reduce
    u64_t r = _mm256_reduce_add_epi64(hash0);

    //  Partial Mod Reduce
    return (r & 0x1fffffffffffffffull) + (r >> 61);
}
YM_NO_INLINE inline u64_t hash_hex_raw_d64_AVX2(u64_t hash_in, const __m256i* T, upL_t blocks){
    if (blocks == 0){
        return hash_in;
    }

    const __m256i PRIME = _mm256_set1_epi64x(0x1fffffffffffffffull);
    const __m256i COMPRESS = _mm256_setr_epi8(
       14, 12, 10,  8,  6,  4,  2,  0, 15, 13, 11,  9,  7,  5,  3,  1,
       14, 12, 10,  8,  6,  4,  2,  0, 15, 13, 11,  9,  7,  5,  3,  1
    );
    const __m256i SCALE1 = _mm256_set1_epi16(((u32_t)1 << 8) | ((u32_t)16 << 0));

    __m256i hash0 = _mm256_set1_epi64x(hash_in);
    hash0 = _mm256_and_si256(hash0, _mm256_setr_epi64x(0, 0, 0, (u64_t)0 - 1));

    do{
        __m256i x0 = T[0];
        __m256i x1 = T[1];
        {
            //  1st Merge
            x0 = _mm256_maddubs_epi16(x0, SCALE1);
            x1 = _mm256_maddubs_epi16(x1, SCALE1);
        }
        {
            //  Compress and Reverse
            x0 = _mm256_shuffle_epi8(x0, COMPRESS);
            x1 = _mm256_shuffle_epi8(x1, COMPRESS);

            x0 = _mm256_permute4x64_epi64(x0, 216);
            x1 = _mm256_permute4x64_epi64(x1, 141);

            x0 = _mm256_or_si256(x0, x1);
        }
        {
            //  Reduce
            __m256i hi;
            hi = _mm256_srli_epi64(x0, 61);
            x0 = _mm256_and_si256(x0, PRIME);
            x0 = _mm256_add_epi64(x0, hi);
        }
        {
            //  Multiply
            __m256i hi = _mm256_srli_epi64(hash0, 49);
            hash0 = _mm256_slli_epi64(hash0, 12);
            hash0 = _mm256_and_si256(hash0, PRIME);
            hash0 = _mm256_add_epi64(hash0, hi);

            //  Add
            hash0 = _mm256_add_epi64(hash0, x0);
        }
        T += 2;
    }while (--blocks);

    //  Collapse
    {
        __m256i hi;
        hi = _mm256_srlv_epi64(hash0, _mm256_setr_epi64x(52, 55, 58, 61));
        hash0 = _mm256_sllv_epi64(hash0, _mm256_setr_epi64x(9, 6, 3, 0));
        hash0 = _mm256_and_si256(hash0, PRIME);
        hash0 = _mm256_add_epi64(hash0, hi);
    }

    //  Vector Add Reduce
    u64_t r = _mm256_reduce_add_epi64(hash0);

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
