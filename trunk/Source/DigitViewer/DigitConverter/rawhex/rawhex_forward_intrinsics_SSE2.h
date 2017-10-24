/* rawhex_forward_intrinsics_SSE2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/06/2012
 * Last Modified    : 01/24/2016
 * 
 */

#ifndef ydv_rawhex_forward_intrinsics_SSE2_H
#define ydv_rawhex_forward_intrinsics_SSE2_H
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
YM_FORCE_INLINE void convert_hex16_forward_SSE2(char str[16]){
    const __m128i LIMIT0 = _mm_set1_epi8('0');
    const __m128i LIMIT1 = _mm_set1_epi8(9);
    const __m128i HEXDIF = _mm_set1_epi8(39);

    __m128i* T = (__m128i*)str;

    register __m128i a0, b0;

    a0 = T[0];

    b0 = _mm_cmpgt_epi8(a0, LIMIT1);

    a0 = _mm_add_epi8(a0, LIMIT0);

    b0 = _mm_and_si128(b0, HEXDIF);

    a0 = _mm_add_epi8(a0, b0);

    T[0] = a0;
}
YM_FORCE_INLINE void convert_hex32_forward_SSE2(char str[32]){
    const __m128i LIMIT0 = _mm_set1_epi8('0');
    const __m128i LIMIT1 = _mm_set1_epi8(9);
    const __m128i HEXDIF = _mm_set1_epi8(39);

    __m128i* T = (__m128i*)str;

    register __m128i a0, b0;
    register __m128i a1, b1;

    a0 = T[0];
    a1 = T[1];

    b0 = _mm_cmpgt_epi8(a0, LIMIT1);
    b1 = _mm_cmpgt_epi8(a1, LIMIT1);

    a0 = _mm_add_epi8(a0, LIMIT0);
    a1 = _mm_add_epi8(a1, LIMIT0);

    b0 = _mm_and_si128(b0, HEXDIF);
    b1 = _mm_and_si128(b1, HEXDIF);

    a0 = _mm_add_epi8(a0, b0);
    a1 = _mm_add_epi8(a1, b1);

    T[0] = a0;
    T[1] = a1;
}
YM_FORCE_INLINE void convert_hex64_forward_SSE2(char str[64]){
    const __m128i LIMIT0 = _mm_set1_epi8('0');
    const __m128i LIMIT1 = _mm_set1_epi8(9);
    const __m128i HEXDIF = _mm_set1_epi8(39);

    __m128i* T = (__m128i*)str;

    register __m128i a0, b0;
    register __m128i a1, b1;
    register __m128i a2, b2;
    register __m128i a3, b3;

    a0 = T[0];
    a1 = T[1];
    a2 = T[2];
    a3 = T[3];

    b0 = _mm_cmpgt_epi8(a0, LIMIT1);
    b1 = _mm_cmpgt_epi8(a1, LIMIT1);
    b2 = _mm_cmpgt_epi8(a2, LIMIT1);
    b3 = _mm_cmpgt_epi8(a3, LIMIT1);

    a0 = _mm_add_epi8(a0, LIMIT0);
    a1 = _mm_add_epi8(a1, LIMIT0);
    a2 = _mm_add_epi8(a2, LIMIT0);
    a3 = _mm_add_epi8(a3, LIMIT0);

    b0 = _mm_and_si128(b0, HEXDIF);
    b1 = _mm_and_si128(b1, HEXDIF);
    b2 = _mm_and_si128(b2, HEXDIF);
    b3 = _mm_and_si128(b3, HEXDIF);

    a0 = _mm_add_epi8(a0, b0);
    a1 = _mm_add_epi8(a1, b1);
    a2 = _mm_add_epi8(a2, b2);
    a3 = _mm_add_epi8(a3, b3);

    T[0] = a0;
    T[1] = a1;
    T[2] = a2;
    T[3] = a3;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
