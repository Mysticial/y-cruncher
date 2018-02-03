/* rawdec_inverse_intrinsics_SSE2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/30/2011
 * Last Modified    : 01/24/2016
 * 
 */

#ifndef ydv_DigitViewer_rawdec_inverse_intrinsics_SSE2_H
#define ydv_DigitViewer_rawdec_inverse_intrinsics_SSE2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <emmintrin.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace DigitViewer{
namespace rawdec{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE __m128i convert_dec16_inverse_SSE2(char str[16]){
    const __m128i LIMIT0 = _mm_set1_epi8('0');
    const __m128i LIMIT1 = _mm_set1_epi8('9');
    __m128i* T = (__m128i*)str;

    __m128i bad;
    __m128i a0, b0;

    a0 = T[0];

    b0 = _mm_cmplt_epi8(a0, LIMIT0);
    bad = b0;

    b0 = _mm_cmpgt_epi8(a0, LIMIT1);
    bad = _mm_or_si128(bad, b0);

    a0 = _mm_sub_epi8(a0, LIMIT0);

    T[0] = a0;

    return bad;
}
YM_FORCE_INLINE __m128i convert_dec32_inverse_SSE2(char str[32]){
    const __m128i LIMIT0 = _mm_set1_epi8('0');
    const __m128i LIMIT1 = _mm_set1_epi8('9');
    __m128i* T = (__m128i*)str;

    __m128i bad;
    __m128i a0, b0;
    __m128i a1, b1;

    a0 = T[0];
    a1 = T[1];

    b0 = _mm_cmplt_epi8(a0, LIMIT0);
    b1 = _mm_cmplt_epi8(a1, LIMIT0);
    bad = _mm_or_si128(b0, b1);

    b0 = _mm_cmpgt_epi8(a0, LIMIT1);
    b1 = _mm_cmpgt_epi8(a1, LIMIT1);
    bad = _mm_or_si128(bad, b0);
    bad = _mm_or_si128(bad, b1);

    a0 = _mm_sub_epi8(a0, LIMIT0);
    a1 = _mm_sub_epi8(a1, LIMIT0);

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
