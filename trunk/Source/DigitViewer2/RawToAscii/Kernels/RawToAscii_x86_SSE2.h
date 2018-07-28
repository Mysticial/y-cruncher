/* RawToAscii_x86_SSE2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/14/2018
 * Last Modified    : 01/14/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_RawToAscii_x86_SSE2_H
#define ydv_DigitViewer_RawToAscii_x86_SSE2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "PublicLibs/ArchSpecificLibs/x86_SSE2.h"
namespace DigitViewer2{
namespace RawToAscii{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE bool dec_to_raw_x86_SSE2(__m128i* raw, const __m128i* dec, upL_t blocks){
    if (blocks == 0){
        return false;
    }

    const __m128i LIMIT0 = _mm_set1_epi8('0');
    const __m128i LIMIT1 = _mm_set1_epi8('9');
    __m128i bad = _mm_setzero_si128();

    do{
        __m128i a0, b0;

        a0 = _mm_loadu_si128(dec);

        b0 = _mm_cmplt_epi8(a0, LIMIT0);
        bad = _mm_or_si128(bad, b0);

        b0 = _mm_cmpgt_epi8(a0, LIMIT1);
        bad = _mm_or_si128(bad, b0);

        a0 = _mm_sub_epi8(a0, LIMIT0);

        _mm_store_si128(raw, a0);

        raw++;
        dec++;
    }while (--blocks);

    return _mm_movemask_epi8(bad) != 0;
}
YM_FORCE_INLINE bool hex_to_raw_x86_SSE2(__m128i* raw, const __m128i* hex, upL_t blocks){
    if (blocks == 0){
        return false;
    }

    const __m128i LIMIT0 = _mm_set1_epi8('0');
    const __m128i LIMIT1 = _mm_set1_epi8('9');
    const __m128i LIMIT2 = _mm_set1_epi8('a');
    const __m128i LIMIT3 = _mm_set1_epi8('f');
    const __m128i HEXDIF = _mm_set1_epi8(39); 
    __m128i bad = _mm_setzero_si128();

    do{
        __m128i a0, b0, c0;

        a0 = _mm_loadu_si128(hex);

        b0 = _mm_cmplt_epi8(a0, LIMIT2);
        c0 = _mm_cmpgt_epi8(a0, LIMIT1);
        b0 = _mm_and_si128(b0, c0);
        bad = _mm_or_si128(bad, b0);

        b0 = _mm_cmplt_epi8(a0, LIMIT0);
        bad = _mm_or_si128(bad, b0);

        b0 = _mm_cmpgt_epi8(a0, LIMIT3);

        a0 = _mm_sub_epi8(a0, LIMIT0);
        c0 = _mm_and_si128(c0, HEXDIF);

        bad = _mm_or_si128(bad, b0);
        a0 = _mm_sub_epi8(a0, c0);

        _mm_store_si128(raw, a0);

        raw++;
        hex++;
    }while (--blocks);

    return _mm_movemask_epi8(bad) != 0;
}
YM_FORCE_INLINE bool raw_to_dec_x86_SSE2(__m128i* dec, const __m128i* raw, upL_t blocks){
    if (blocks == 0){
        return false;
    }

    __m128i bad = _mm_setzero_si128();

    do{
        __m128i a0, b0;

        a0 = _mm_loadu_si128(raw);

        b0 = _mm_cmplt_epi8(a0, _mm_set1_epi8(0));
        bad = _mm_or_si128(bad, b0);

        b0 = _mm_cmpgt_epi8(a0, _mm_set1_epi8(9));
        bad = _mm_or_si128(bad, b0);

        a0 = _mm_add_epi8(a0, _mm_set1_epi8('0'));

        _mm_store_si128(dec, a0);

        raw++;
        dec++;
    }while (--blocks);

    return _mm_movemask_epi8(bad) != 0;
}
YM_FORCE_INLINE bool raw_to_hex_x86_SSE2(__m128i* hex, const __m128i* raw, upL_t blocks){
    if (blocks == 0){
        return false;
    }

    __m128i bad = _mm_setzero_si128();

    do{
        __m128i a0, b0;

        a0 = _mm_loadu_si128(raw);

        bad = _mm_or_si128(bad, a0);

        b0 = _mm_cmpgt_epi8(a0, _mm_set1_epi8(9));
        a0 = _mm_add_epi8(a0, _mm_set1_epi8('0'));
        b0 = _mm_and_si128(b0, _mm_set1_epi8(39));
        a0 = _mm_add_epi8(a0, b0);

        _mm_store_si128(hex, a0);

        raw++;
        hex++;
    }while (--blocks);

    return (_mm_reduce_or_epi64(bad) & 0xf0f0f0f0f0f0f0f0ull) != 0;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
