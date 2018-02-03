/* rawhex_inverse_intrinsics_AVX2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/24/2016
 * Last Modified    : 01/24/2016
 * 
 */

#ifndef ydv_DigitViewer_rawhex_inverse_intrinsics_AVX2_H
#define ydv_DigitViewer_rawhex_inverse_intrinsics_AVX2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <immintrin.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace DigitViewer{
namespace rawhex{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE __m256i convert_hex32_inverse_AVX2(char str[32]){
    __m256i* T = (__m256i*)str;

    const __m256i LIMIT0 = _mm256_set1_epi8('0');
    const __m256i LIMIT1 = _mm256_set1_epi8('9');
    const __m256i LIMIT2 = _mm256_set1_epi8('a');
    const __m256i LIMIT3 = _mm256_set1_epi8('f');
    const __m256i HEXDIF = _mm256_set1_epi8(39); 

    __m256i bad;
    __m256i a0, b0, c0;

    a0 = T[0];

    b0 = _mm256_cmpgt_epi8(LIMIT2, a0);
    c0 = _mm256_cmpgt_epi8(a0, LIMIT1);
    b0 = _mm256_and_si256(b0, c0);
    bad = b0;

    b0 = _mm256_cmpgt_epi8(LIMIT0, a0);
    bad = _mm256_or_si256(bad, b0);

    b0 = _mm256_cmpgt_epi8(a0, LIMIT3);
    bad = _mm256_or_si256(bad, b0);

    a0 = _mm256_sub_epi8(a0, LIMIT0);
    c0 = _mm256_and_si256(c0, HEXDIF);

    a0 = _mm256_sub_epi8(a0, c0);

    T[0] = a0;

    return bad;
}
YM_FORCE_INLINE __m256i convert_hex64_inverse_AVX2(char str[64]){
    __m256i* T = (__m256i*)str;

    const __m256i LIMIT0 = _mm256_set1_epi8('0');
    const __m256i LIMIT1 = _mm256_set1_epi8('9');
    const __m256i LIMIT2 = _mm256_set1_epi8('a');
    const __m256i LIMIT3 = _mm256_set1_epi8('f');
    const __m256i HEXDIF = _mm256_set1_epi8(39); 

    __m256i bad;
    __m256i a0, b0, c0;
    __m256i a1, b1, c1;

    a0 = T[0];
    a1 = T[1];

    b0 = _mm256_cmpgt_epi8(LIMIT2, a0);
    b1 = _mm256_cmpgt_epi8(LIMIT2, a1);
    c0 = _mm256_cmpgt_epi8(a0, LIMIT1);
    c1 = _mm256_cmpgt_epi8(a1, LIMIT1);
    b0 = _mm256_and_si256(b0, c0);
    b1 = _mm256_and_si256(b1, c1);
    bad = _mm256_or_si256(b0, b1);

    b0 = _mm256_cmpgt_epi8(LIMIT0, a0);
    b1 = _mm256_cmpgt_epi8(LIMIT0, a1);
    bad = _mm256_or_si256(bad, b0);
    bad = _mm256_or_si256(bad, b1);

    b0 = _mm256_cmpgt_epi8(a0, LIMIT3);
    b1 = _mm256_cmpgt_epi8(a1, LIMIT3);
    bad = _mm256_or_si256(bad, b0);
    bad = _mm256_or_si256(bad, b1);

    a0 = _mm256_sub_epi8(a0, LIMIT0);
    a1 = _mm256_sub_epi8(a1, LIMIT0);
    c0 = _mm256_and_si256(c0, HEXDIF);
    c1 = _mm256_and_si256(c1, HEXDIF);

    a0 = _mm256_sub_epi8(a0, c0);
    a1 = _mm256_sub_epi8(a1, c1);

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
