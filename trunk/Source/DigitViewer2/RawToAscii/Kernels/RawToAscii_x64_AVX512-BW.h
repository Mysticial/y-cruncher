/* RawToAscii_x64_AVX512-BW.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/14/2018
 * Last Modified    : 01/14/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_RawToAscii_x64_AVX512BW_H
#define ydv_DigitViewer_RawToAscii_x64_AVX512BW_H
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
YM_FORCE_INLINE bool dec_to_raw_x64_AVX512BW(__m512i* raw, const __m512i* dec, upL_t blocks){
    if (blocks == 0){
        return false;
    }

    __mmask64 bad = 0;

    do{
        __m512i a0;

        a0 = _mm512_loadu_si512(dec);

        a0 = _mm512_sub_epi8(a0, _mm512_set1_epi8('0'));
        bad |= _mm512_cmpgt_epu8_mask(a0, _mm512_set1_epi8(9));

        _mm512_store_si512(raw, a0);

        raw++;
        dec++;
    }while (--blocks);

    return bad != 0;
}
YM_FORCE_INLINE bool hex_to_raw_x64_AVX512BW(__m512i* raw, const __m512i* hex, upL_t blocks){
    if (blocks == 0){
        return false;
    }

    const __m512i LIMIT0 = _mm512_set1_epi8('0');
    const __m512i LIMIT1 = _mm512_set1_epi8('9');
    const __m512i LIMIT2 = _mm512_set1_epi8('a');
    const __m512i LIMIT3 = _mm512_set1_epi8('f');
    const __m512i HEXDIF = _mm512_set1_epi8(39); 
    __mmask64 bad = 0;

    do{
        __m512i a0;

        a0 = _mm512_loadu_si512(hex);

        __mmask64 gt_9 = _mm512_cmpgt_epi8_mask(a0, LIMIT1);
        bad |= gt_9 & _mm512_cmplt_epi8_mask(a0, LIMIT2);
        bad |= _mm512_cmplt_epi8_mask(a0, LIMIT0);
        bad |= _mm512_cmpgt_epi8_mask(a0, LIMIT3);

        a0 = _mm512_sub_epi8(a0, LIMIT0);
        a0 = _mm512_mask_sub_epi8(a0, gt_9, a0, HEXDIF);

        _mm512_store_si512(raw, a0);

        raw++;
        hex++;
    }while (--blocks);

    return bad != 0;
}
YM_FORCE_INLINE bool raw_to_dec_x64_AVX512BW(__m512i* dec, const __m512i* raw, upL_t blocks){
    if (blocks == 0){
        return false;
    }

    __mmask64 bad = 0;

    do{
        __m512i a0;

        a0 = _mm512_loadu_si512(raw);

        bad |= _mm512_cmpgt_epu8_mask(a0, _mm512_set1_epi8(9));
        a0 = _mm512_add_epi8(a0, _mm512_set1_epi8('0'));

        _mm512_store_si512(dec, a0);

        raw++;
        dec++;
    }while (--blocks);

    return bad != 0;
}
YM_FORCE_INLINE bool raw_to_hex_x64_AVX512BW(__m512i* hex, const __m512i* raw, upL_t blocks){
    if (blocks == 0){
        return false;
    }

    __m512i bad = _mm512_setzero_si512();

    do{
        __m512i a0;

        a0 = _mm512_loadu_si512(raw);

        bad = _mm512_or_si512(bad, a0);

        __mmask64 letter = _mm512_cmpgt_epi8_mask(a0, _mm512_set1_epi8(9));
        a0 = _mm512_add_epi8(a0, _mm512_set1_epi8('0'));
        a0 = _mm512_mask_add_epi8(a0, letter, a0, _mm512_set1_epi8(39));

        _mm512_store_si512(hex, a0);

        raw++;
        hex++;
    }while (--blocks);

    return _mm512_test_epi8_mask(bad, _mm512_set1_epi8((char)0xf0)) != 0;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
