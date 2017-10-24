/* rawhex_inverse_intrinsics_SSE2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/30/2011
 * Last Modified    : 01/24/2016
 * 
 */

#ifndef ydv_rawhex_inverse_intrinsics_SSE2_H
#define ydv_rawhex_inverse_intrinsics_SSE2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <emmintrin.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace DigitViewer{
namespace rawhex{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE __m128i convert_hex16_inverse_SSE2(char str[16]){
    __m128i* T = (__m128i*)str;

    const __m128i LIMIT0 = _mm_set1_epi8('0');
    const __m128i LIMIT1 = _mm_set1_epi8('9');
    const __m128i LIMIT2 = _mm_set1_epi8('a');
    const __m128i LIMIT3 = _mm_set1_epi8('f');
    const __m128i HEXDIF = _mm_set1_epi8(39); 

    __m128i bad;
    __m128i a0, b0, c0;

    a0 = T[0];

    b0 = _mm_cmplt_epi8(a0, LIMIT2);
    c0 = _mm_cmpgt_epi8(a0, LIMIT1);
    b0 = _mm_and_si128(b0, c0);
    bad = b0;

    b0 = _mm_cmplt_epi8(a0, LIMIT0);
    bad = _mm_or_si128(bad, b0);

    b0 = _mm_cmpgt_epi8(a0, LIMIT3);

    a0 = _mm_sub_epi8(a0, LIMIT0);
    c0 = _mm_and_si128(c0, HEXDIF);

    bad = _mm_or_si128(bad, b0);
    a0 = _mm_sub_epi8(a0, c0);

    T[0] = a0;

    return bad;
}
YM_FORCE_INLINE __m128i convert_hex32_inverse_SSE2(char str[32]){
    __m128i* T = (__m128i*)str;

    const __m128i LIMIT0 = _mm_set1_epi8('0');
    const __m128i LIMIT1 = _mm_set1_epi8('9');
    const __m128i LIMIT2 = _mm_set1_epi8('a');
    const __m128i LIMIT3 = _mm_set1_epi8('f');
    const __m128i HEXDIF = _mm_set1_epi8(39); 

    __m128i bad;
    __m128i a0, b0, c0;
    __m128i a1, b1, c1;

    a0 = T[0];
    a1 = T[1];

    b0 = _mm_cmplt_epi8(a0, LIMIT2);
    b1 = _mm_cmplt_epi8(a1, LIMIT2);
    c0 = _mm_cmpgt_epi8(a0, LIMIT1);
    c1 = _mm_cmpgt_epi8(a1, LIMIT1);
    b0 = _mm_and_si128(b0, c0);
    b1 = _mm_and_si128(b1, c1);
    bad = _mm_or_si128(b0, b1);

    b0 = _mm_cmplt_epi8(a0, LIMIT0);
    b1 = _mm_cmplt_epi8(a1, LIMIT0);
    bad = _mm_or_si128(bad, b0);
    bad = _mm_or_si128(bad, b1);

    b0 = _mm_cmpgt_epi8(a0, LIMIT3);
    b1 = _mm_cmpgt_epi8(a1, LIMIT3);

    a0 = _mm_sub_epi8(a0, LIMIT0);
    a1 = _mm_sub_epi8(a1, LIMIT0);
    c0 = _mm_and_si128(c0, HEXDIF);
    c1 = _mm_and_si128(c1, HEXDIF);

    bad = _mm_or_si128(bad, b0);
    bad = _mm_or_si128(bad, b1);
    a0 = _mm_sub_epi8(a0, c0);
    a1 = _mm_sub_epi8(a1, c1);

    T[0] = a0;
    T[1] = a1;

    return bad;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
