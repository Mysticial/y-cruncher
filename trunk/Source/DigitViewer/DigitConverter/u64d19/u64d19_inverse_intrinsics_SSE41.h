/* u64d19_inverse_intrinsics_SSE41.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/19/2011
 * Last Modified    : 01/23/2016
 * 
 */

#ifndef ydv_DigitViewer_u64d19_inverse_intrinsics_SSE41_H
#define ydv_DigitViewer_u64d19_inverse_intrinsics_SSE41_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <smmintrin.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace DigitViewer{
namespace u64d19{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void convert_d19x2_inverse_SSE41(const char str[19*2], u64_t dec[2]){
    const __m128i MASK0 = _mm_set1_epi16(0xf);
    const __m128i MASK1 = _mm_set1_epi32(0xff);
    const __m128i MASK2 = _mm_set_epi32(0, 0xffff, 0, 0xffff);
    const __m128i MASK3 = _mm_set1_epi32(0x00ffffff);
    const __m128i TEN0 = _mm_set1_epi16(10);
    const __m128i TEN1 = _mm_set1_epi32(100);
    const __m128i TEN2 = _mm_set1_epi32(10000);
    const __m128i TEN3 = _mm_set1_epi32(100000000);
    const __m128i FIVE0 = _mm_set1_epi32(78125);
    const __m128i FIVE1 = _mm_set_epi32(0, 390625, 0, 390625);

    register __m128i a0, b0, c0;
    register __m128i a1, b1;

    c0 = _mm_cvtsi32_si128(*(u32_t*)(str +  0));
    c0 = _mm_insert_epi32(c0, *(u32_t*)(str + 19), 2);
    c0 = _mm_and_si128(c0, MASK3);

    b0 = _mm_and_si128(c0, MASK0);
    c0 = _mm_srli_epi16(c0, 8);
    b0 = _mm_mullo_epi16(b0, TEN0);
    c0 = _mm_add_epi16(c0, b0);

    b0 = _mm_and_si128(c0, MASK1);
    c0 = _mm_srli_epi32(c0, 16);
    b0 = _mm_mullo_epi16(b0, TEN1);
    c0 = _mm_add_epi16(c0, b0);

    c0 = _mm_mul_epu32(c0, FIVE0);
    c0 = _mm_mul_epu32(c0, FIVE1);
    c0 = _mm_slli_epi64(c0, 15);


    a0 = _mm_lddqu_si128((__m128i*)(str +  3));
    a1 = _mm_lddqu_si128((__m128i*)(str + 22));

    b0 = _mm_and_si128(a0, MASK0);
    b1 = _mm_and_si128(a1, MASK0);
    a0 = _mm_srli_epi16(a0, 8);
    a1 = _mm_srli_epi16(a1, 8);
    b0 = _mm_mullo_epi16(b0, TEN0);
    b1 = _mm_mullo_epi16(b1, TEN0);
    a0 = _mm_add_epi16(a0, b0);
    a1 = _mm_add_epi16(a1, b1);

    b0 = _mm_and_si128(a0, MASK1);
    b1 = _mm_and_si128(a1, MASK1);
    a0 = _mm_srli_epi32(a0, 16);
    a1 = _mm_srli_epi32(a1, 16);
    b0 = _mm_mullo_epi16(b0, TEN1);
    b1 = _mm_mullo_epi16(b1, TEN1);
    a0 = _mm_add_epi16(a0, b0);
    a1 = _mm_add_epi16(a1, b1);

    b0 = _mm_and_si128(a0, MASK2);
    b1 = _mm_and_si128(a1, MASK2);
    a0 = _mm_srli_epi64(a0, 32);
    a1 = _mm_srli_epi64(a1, 32);
    b0 = _mm_mullo_epi32(b0, TEN2);
    b1 = _mm_mullo_epi32(b1, TEN2);
    a0 = _mm_add_epi32(a0, b0);
    a1 = _mm_add_epi32(a1, b1);


    b0 = _mm_unpacklo_epi64(a0, a1);
    a0 = _mm_unpackhi_epi64(a0, a1);
    b0 = _mm_mul_epu32(b0, TEN3);
    a0 = _mm_add_epi64(a0, b0);


    a0 = _mm_add_epi64(a0, c0);
    _mm_store_si128((__m128i*)dec, a0);
}
YM_FORCE_INLINE void convert_d19x4_inverse_SSE41(const char str[19*4], u64_t dec[4]){
    const __m128i MASK0 = _mm_set1_epi16(0xf);
    const __m128i MASK1 = _mm_set1_epi32(0xff);
    const __m128i MASK2 = _mm_set_epi32(0, 0xffff, 0, 0xffff);
    const __m128i MASK3 = _mm_set1_epi32(0x00ffffff);
    const __m128i TEN0 = _mm_set1_epi16(10);
    const __m128i TEN1 = _mm_set1_epi32(100);
    const __m128i TEN2 = _mm_set1_epi32(10000);
    const __m128i TEN3 = _mm_set1_epi32(100000000);
    const __m128i FIVE0 = _mm_set1_epi32(78125);
    const __m128i FIVE1 = _mm_set_epi32(0, 390625, 0, 390625);

    register __m128i a0, b0, c0;
    register __m128i a1, b1;
    register __m128i a2, b2, c2;
    register __m128i a3, b3;

    a0 = _mm_lddqu_si128((__m128i*)(str +  3));

    c0 = _mm_cvtsi32_si128(*(u32_t*)(str +  0));
    c0 = _mm_insert_epi32(c0, *(u32_t*)(str + 19), 2);
    c0 = _mm_insert_epi32(c0, *(u32_t*)(str + 38), 1);
    c0 = _mm_insert_epi32(c0, *(u32_t*)(str + 57), 3);
    c0 = _mm_and_si128(c0, MASK3);

    a1 = _mm_lddqu_si128((__m128i*)(str + 22));
    c2 = _mm_and_si128(c0, MASK0);
    c0 = _mm_srli_epi16(c0, 8);
    c2 = _mm_mullo_epi16(c2, TEN0);
    c0 = _mm_add_epi16(c0, c2);

    a2 = _mm_lddqu_si128((__m128i*)(str + 41));
    c2 = _mm_and_si128(c0, MASK1);
    c0 = _mm_srli_epi32(c0, 16);
    c2 = _mm_mullo_epi16(c2, TEN1);
    c0 = _mm_add_epi16(c0, c2);

    a3 = _mm_lddqu_si128((__m128i*)(str + 60));
    c0 = _mm_mullo_epi32(c0, FIVE0);
    c2 = _mm_shuffle_epi32(c0, 177);
    c0 = _mm_mul_epu32(c0, FIVE1);
    c2 = _mm_mul_epu32(c2, FIVE1);
    c0 = _mm_slli_epi64(c0, 15);
    c2 = _mm_slli_epi64(c2, 15);


    b0 = _mm_and_si128(a0, MASK0);
    b1 = _mm_and_si128(a1, MASK0);
    b2 = _mm_and_si128(a2, MASK0);
    b3 = _mm_and_si128(a3, MASK0);
    a0 = _mm_srli_epi16(a0, 8);
    a1 = _mm_srli_epi16(a1, 8);
    a2 = _mm_srli_epi16(a2, 8);
    a3 = _mm_srli_epi16(a3, 8);
    b0 = _mm_mullo_epi16(b0, TEN0);
    b1 = _mm_mullo_epi16(b1, TEN0);
    b2 = _mm_mullo_epi16(b2, TEN0);
    b3 = _mm_mullo_epi16(b3, TEN0);
    a0 = _mm_add_epi16(a0, b0);
    a1 = _mm_add_epi16(a1, b1);
    a2 = _mm_add_epi16(a2, b2);
    a3 = _mm_add_epi16(a3, b3);

    b0 = _mm_and_si128(a0, MASK1);
    b1 = _mm_and_si128(a1, MASK1);
    b2 = _mm_and_si128(a2, MASK1);
    b3 = _mm_and_si128(a3, MASK1);
    a0 = _mm_srli_epi32(a0, 16);
    a1 = _mm_srli_epi32(a1, 16);
    a2 = _mm_srli_epi32(a2, 16);
    a3 = _mm_srli_epi32(a3, 16);
    b0 = _mm_mullo_epi16(b0, TEN1);
    b1 = _mm_mullo_epi16(b1, TEN1);
    b2 = _mm_mullo_epi16(b2, TEN1);
    b3 = _mm_mullo_epi16(b3, TEN1);
    a0 = _mm_add_epi16(a0, b0);
    a1 = _mm_add_epi16(a1, b1);
    a2 = _mm_add_epi16(a2, b2);
    a3 = _mm_add_epi16(a3, b3);

    b0 = _mm_and_si128(a0, MASK2);
    b1 = _mm_and_si128(a1, MASK2);
    b2 = _mm_and_si128(a2, MASK2);
    b3 = _mm_and_si128(a3, MASK2);
    a0 = _mm_srli_epi64(a0, 32);
    a1 = _mm_srli_epi64(a1, 32);
    a2 = _mm_srli_epi64(a2, 32);
    a3 = _mm_srli_epi64(a3, 32);
    b0 = _mm_mullo_epi32(b0, TEN2);
    b1 = _mm_mullo_epi32(b1, TEN2);
    b2 = _mm_mullo_epi32(b2, TEN2);
    b3 = _mm_mullo_epi32(b3, TEN2);
    a0 = _mm_add_epi32(a0, b0);
    a1 = _mm_add_epi32(a1, b1);
    a2 = _mm_add_epi32(a2, b2);
    a3 = _mm_add_epi32(a3, b3);


    b0 = _mm_unpacklo_epi64(a0, a1);
    b2 = _mm_unpacklo_epi64(a2, a3);
    a0 = _mm_unpackhi_epi64(a0, a1);
    a2 = _mm_unpackhi_epi64(a2, a3);
    b0 = _mm_mul_epu32(b0, TEN3);
    b2 = _mm_mul_epu32(b2, TEN3);
    a0 = _mm_add_epi64(a0, b0);
    a2 = _mm_add_epi64(a2, b2);


    a0 = _mm_add_epi64(a0, c0);
    a2 = _mm_add_epi64(a2, c2);
    _mm_store_si128((__m128i*)dec + 0, a0);
    _mm_store_si128((__m128i*)dec + 1, a2);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
