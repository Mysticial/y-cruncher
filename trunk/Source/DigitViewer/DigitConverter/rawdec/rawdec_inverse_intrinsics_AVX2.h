/* rawdec_inverse_intrinsics_AVX2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/24/2016
 * Last Modified    : 01/24/2016
 * 
 */

#ifndef ydv_rawdec_inverse_intrinsics_AVX2_H
#define ydv_rawdec_inverse_intrinsics_AVX2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <immintrin.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace DigitViewer{
namespace rawdec{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE __m256i convert_dec32_inverse_AVX2(char str[32]){
    const __m256i LIMIT0 = _mm256_set1_epi8('0');
    const __m256i LIMIT1 = _mm256_set1_epi8('9');
    __m256i* T = (__m256i*)str;

    __m256i bad;
    __m256i a0, b0;

    a0 = T[0];

    b0 = _mm256_cmpgt_epi8(LIMIT0, a0);
    bad = b0;

    b0 = _mm256_cmpgt_epi8(a0, LIMIT1);
    bad = _mm256_or_si256(bad, b0);

    a0 = _mm256_sub_epi8(a0, LIMIT0);

    T[0] = a0;

    return bad;
}
YM_FORCE_INLINE __m256i convert_dec64_inverse_AVX2(char str[64]){
    const __m256i LIMIT0 = _mm256_set1_epi8('0');
    const __m256i LIMIT1 = _mm256_set1_epi8('9');
    __m256i* T = (__m256i*)str;

    __m256i bad;
    __m256i a0, b0;
    __m256i a1, b1;

    a0 = T[0];
    a1 = T[1];

    b0 = _mm256_cmpgt_epi8(LIMIT0, a0);
    b1 = _mm256_cmpgt_epi8(LIMIT0, a1);
    bad = b0;
    bad = b1;

    b0 = _mm256_cmpgt_epi8(a0, LIMIT1);
    b1 = _mm256_cmpgt_epi8(a1, LIMIT1);
    bad = _mm256_or_si256(bad, b0);
    bad = _mm256_or_si256(bad, b1);

    a0 = _mm256_sub_epi8(a0, LIMIT0);
    a1 = _mm256_sub_epi8(a1, LIMIT0);

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
