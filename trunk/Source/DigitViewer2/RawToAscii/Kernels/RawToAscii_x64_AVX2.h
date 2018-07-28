/* RawToAscii_x64_AVX2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/14/2018
 * Last Modified    : 01/14/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_RawToAscii_x64_AVX2_H
#define ydv_DigitViewer_RawToAscii_x64_AVX2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <immintrin.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace DigitViewer2{
namespace RawToAscii{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE bool dec_to_raw_x64_AVX2(__m256i* raw, const __m256i* dec, upL_t blocks){
    if (blocks == 0){
        return false;
    }

    const __m256i LIMIT0 = _mm256_set1_epi8('0');
    const __m256i LIMIT1 = _mm256_set1_epi8('9');
    __m256i bad = _mm256_setzero_si256();

    do{
        __m256i a0, b0;

        a0 = _mm256_loadu_si256(dec);

        b0 = _mm256_cmpgt_epi8(LIMIT0, a0);
        bad = _mm256_or_si256(bad, b0);

        b0 = _mm256_cmpgt_epi8(a0, LIMIT1);
        bad = _mm256_or_si256(bad, b0);

        a0 = _mm256_sub_epi8(a0, LIMIT0);

        _mm256_store_si256(raw, a0);

        raw++;
        dec++;
    }while (--blocks);

    return _mm256_movemask_epi8(bad) != 0;
}
YM_FORCE_INLINE bool hex_to_raw_x64_AVX2(__m256i* raw, const __m256i* hex, upL_t blocks){
    if (blocks == 0){
        return false;
    }

    const __m256i LIMIT0 = _mm256_set1_epi8('0');
    const __m256i LIMIT1 = _mm256_set1_epi8('9');
    const __m256i LIMIT2 = _mm256_set1_epi8('a');
    const __m256i LIMIT3 = _mm256_set1_epi8('f');
    const __m256i HEXDIF = _mm256_set1_epi8(39); 
    __m256i bad = _mm256_setzero_si256();

    do{
        __m256i a0, b0, c0;

        a0 = _mm256_loadu_si256(hex);

        b0 = _mm256_cmpgt_epi8(LIMIT2, a0);
        c0 = _mm256_cmpgt_epi8(a0, LIMIT1);
        b0 = _mm256_and_si256(b0, c0);
        bad = _mm256_or_si256(bad, b0);

        b0 = _mm256_cmpgt_epi8(LIMIT0, a0);
        bad = _mm256_or_si256(bad, b0);

        b0 = _mm256_cmpgt_epi8(a0, LIMIT3);

        a0 = _mm256_sub_epi8(a0, LIMIT0);
        c0 = _mm256_and_si256(c0, HEXDIF);

        bad = _mm256_or_si256(bad, b0);
        a0 = _mm256_sub_epi8(a0, c0);

        _mm256_store_si256(raw, a0);

        raw++;
        hex++;
    }while (--blocks);

    return _mm256_movemask_epi8(bad) != 0;
}
YM_FORCE_INLINE bool raw_to_dec_x64_AVX2(__m256i* dec, const __m256i* raw, upL_t blocks){
    if (blocks == 0){
        return false;
    }

    __m256i bad = _mm256_setzero_si256();

    do{
        __m256i a0, b0;

        a0 = _mm256_loadu_si256(raw);

        b0 = _mm256_cmpgt_epi8(_mm256_set1_epi8(0), a0);
        bad = _mm256_or_si256(bad, b0);

        b0 = _mm256_cmpgt_epi8(a0, _mm256_set1_epi8(9));
        bad = _mm256_or_si256(bad, b0);

        a0 = _mm256_add_epi8(a0, _mm256_set1_epi8('0'));

        _mm256_store_si256(dec, a0);

        raw++;
        dec++;
    }while (--blocks);

    return _mm256_movemask_epi8(bad) != 0;
}
YM_FORCE_INLINE bool raw_to_hex_x64_AVX2(__m256i* hex, const __m256i* raw, upL_t blocks){
    if (blocks == 0){
        return false;
    }

    __m256i bad = _mm256_setzero_si256();

    do{
        __m256i a0, b0;

        a0 = _mm256_loadu_si256(raw);

        bad = _mm256_or_si256(bad, a0);

        b0 = _mm256_cmpgt_epi8(a0, _mm256_set1_epi8(9));
        a0 = _mm256_add_epi8(a0, _mm256_set1_epi8('0'));
        b0 = _mm256_and_si256(b0, _mm256_set1_epi8(39));
        a0 = _mm256_add_epi8(a0, b0);

        _mm256_store_si256(hex, a0);

        raw++;
        hex++;
    }while (--blocks);

    return _mm256_testnzc_si256(bad, _mm256_set1_epi8((unsigned char)0xf0)) != 0;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
