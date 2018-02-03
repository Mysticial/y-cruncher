/* rawhex_forward_intrinsics_AVX2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/24/2016
 * Last Modified    : 01/24/2016
 * 
 */

#ifndef ydv_DigitViewer_rawhex_forward_intrinsics_AVX2_H
#define ydv_DigitViewer_rawhex_forward_intrinsics_AVX2_H
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
YM_FORCE_INLINE void convert_hex32_forward_AVX2(char str[32]){
    const __m256i LIMIT0 = _mm256_set1_epi8('0');
    const __m256i LIMIT1 = _mm256_set1_epi8(9 + '0');
    const __m256i HEXDIF = _mm256_set1_epi8(39);

    __m256i* T = (__m256i*)str;

    register __m256i a0, b0;

    a0 = _mm256_add_epi8(LIMIT0, T[0]);

    b0 = _mm256_cmpgt_epi8(a0, LIMIT1);

    b0 = _mm256_and_si256(b0, HEXDIF);

    a0 = _mm256_add_epi8(a0, b0);

    T[0] = a0;
}
YM_FORCE_INLINE void convert_hex64_forward_AVX2(char str[64]){
    const __m256i LIMIT0 = _mm256_set1_epi8('0');
    const __m256i LIMIT1 = _mm256_set1_epi8(9 + '0');
    const __m256i HEXDIF = _mm256_set1_epi8(39);

    __m256i* T = (__m256i*)str;

    register __m256i a0, b0;
    register __m256i a1, b1;

    a0 = _mm256_add_epi8(LIMIT0, T[0]);
    a1 = _mm256_add_epi8(LIMIT0, T[1]);

    b0 = _mm256_cmpgt_epi8(a0, LIMIT1);
    b1 = _mm256_cmpgt_epi8(a1, LIMIT1);

    b0 = _mm256_and_si256(b0, HEXDIF);
    b1 = _mm256_and_si256(b1, HEXDIF);

    a0 = _mm256_add_epi8(a0, b0);
    a1 = _mm256_add_epi8(a1, b1);

    T[0] = a0;
    T[1] = a1;
}
YM_FORCE_INLINE void convert_hex128_forward_AVX2(char str[128]){
    const __m256i LIMIT0 = _mm256_set1_epi8('0');
    const __m256i LIMIT1 = _mm256_set1_epi8(9 + '0');
    const __m256i HEXDIF = _mm256_set1_epi8(39);

    __m256i* T = (__m256i*)str;

    register __m256i a0, b0;
    register __m256i a1, b1;
    register __m256i a2, b2;
    register __m256i a3, b3;

    a0 = _mm256_add_epi8(LIMIT0, T[0]);
    a1 = _mm256_add_epi8(LIMIT0, T[1]);
    a2 = _mm256_add_epi8(LIMIT0, T[2]);
    a3 = _mm256_add_epi8(LIMIT0, T[3]);

    b0 = _mm256_cmpgt_epi8(a0, LIMIT1);
    b1 = _mm256_cmpgt_epi8(a1, LIMIT1);
    b2 = _mm256_cmpgt_epi8(a2, LIMIT1);
    b3 = _mm256_cmpgt_epi8(a3, LIMIT1);

    b0 = _mm256_and_si256(b0, HEXDIF);
    b1 = _mm256_and_si256(b1, HEXDIF);
    b2 = _mm256_and_si256(b2, HEXDIF);
    b3 = _mm256_and_si256(b3, HEXDIF);

    a0 = _mm256_add_epi8(a0, b0);
    a1 = _mm256_add_epi8(a1, b1);
    a2 = _mm256_add_epi8(a2, b2);
    a3 = _mm256_add_epi8(a3, b3);

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
