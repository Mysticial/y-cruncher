/* u64h16_inverse_intrinsics_SSE2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/19/2011
 * Last Modified    : 01/23/2016
 * 
 */

#ifndef ydv_DigitViewer_u64h16_inverse_intrinsics_SSE2_H
#define ydv_DigitViewer_u64h16_inverse_intrinsics_SSE2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <emmintrin.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace DigitViewer{
namespace u64h16{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void convert_h16x2_inverse_SSE2(const char str[16*2], u64_t hex[2]){
    const __m128i MASK0 = _mm_set1_epi16(0xf);
    const __m128i MASK1 = _mm_set1_epi32(0xff);

    register __m128i a0, a1;
    register __m128i b0, b1;

    a0 = _mm_loadu_si128((const __m128i*)str + 0);
    a1 = _mm_loadu_si128((const __m128i*)str + 1);

    b0 = _mm_and_si128(a0, MASK0);
    b1 = _mm_and_si128(a1, MASK0);
    a0 = _mm_srli_epi16(a0, 8);
    a1 = _mm_srli_epi16(a1, 8);

    b0 = _mm_slli_epi16(b0, 4);
    b1 = _mm_slli_epi16(b1, 4);

    a0 = _mm_or_si128(a0, b0);
    a1 = _mm_or_si128(a1, b1);

    b0 = _mm_and_si128(a0, MASK1);
    b1 = _mm_and_si128(a1, MASK1);
    a0 = _mm_srli_epi32(a0, 16);
    a1 = _mm_srli_epi32(a1, 16);

    b0 = _mm_slli_epi16(b0, 8);
    b1 = _mm_slli_epi16(b1, 8);

    a0 = _mm_or_si128(a0, b0);
    a1 = _mm_or_si128(a1, b1);

    a0 = _mm_shufflelo_epi16(a0, 114);
    a1 = _mm_shufflelo_epi16(a1, 114);

    a0 = _mm_shufflehi_epi16(a0, 114);
    a1 = _mm_shufflehi_epi16(a1, 114);

    a0 = _mm_shuffle_epi32(a0, 114);
    a1 = _mm_shuffle_epi32(a1, 39);

    a0 = _mm_or_si128(a0, a1);
    _mm_store_si128((__m128i*)hex, a0);
}
YM_FORCE_INLINE void convert_h16x4_inverse_SSE2(const char str[16*4], u64_t hex[4]){
    const __m128i MASK0 = _mm_set1_epi16(0xf);
    const __m128i MASK1 = _mm_set1_epi32(0xff);

    register __m128i a0, a1, a2, a3;
    register __m128i b0, b1, b2, b3;

    a0 = _mm_loadu_si128((const __m128i*)str + 0);
    a1 = _mm_loadu_si128((const __m128i*)str + 1);
    a2 = _mm_loadu_si128((const __m128i*)str + 2);
    a3 = _mm_loadu_si128((const __m128i*)str + 3);

    b0 = _mm_and_si128(a0, MASK0);
    b1 = _mm_and_si128(a1, MASK0);
    b2 = _mm_and_si128(a2, MASK0);
    b3 = _mm_and_si128(a3, MASK0);
    a0 = _mm_srli_epi16(a0, 8);
    a1 = _mm_srli_epi16(a1, 8);
    a2 = _mm_srli_epi16(a2, 8);
    a3 = _mm_srli_epi16(a3, 8);

    b0 = _mm_slli_epi16(b0, 4);
    b1 = _mm_slli_epi16(b1, 4);
    b2 = _mm_slli_epi16(b2, 4);
    b3 = _mm_slli_epi16(b3, 4);

    a0 = _mm_or_si128(a0, b0);
    a1 = _mm_or_si128(a1, b1);
    a2 = _mm_or_si128(a2, b2);
    a3 = _mm_or_si128(a3, b3);

    b0 = _mm_and_si128(a0, MASK1);
    b1 = _mm_and_si128(a1, MASK1);
    b2 = _mm_and_si128(a2, MASK1);
    b3 = _mm_and_si128(a3, MASK1);
    a0 = _mm_srli_epi32(a0, 16);
    a1 = _mm_srli_epi32(a1, 16);
    a2 = _mm_srli_epi32(a2, 16);
    a3 = _mm_srli_epi32(a3, 16);

    b0 = _mm_slli_epi16(b0, 8);
    b1 = _mm_slli_epi16(b1, 8);
    b2 = _mm_slli_epi16(b2, 8);
    b3 = _mm_slli_epi16(b3, 8);

    a0 = _mm_or_si128(a0, b0);
    a1 = _mm_or_si128(a1, b1);
    a2 = _mm_or_si128(a2, b2);
    a3 = _mm_or_si128(a3, b3);

    a0 = _mm_shufflelo_epi16(a0, 114);
    a1 = _mm_shufflelo_epi16(a1, 114);
    a2 = _mm_shufflelo_epi16(a2, 114);
    a3 = _mm_shufflelo_epi16(a3, 114);

    a0 = _mm_shufflehi_epi16(a0, 114);
    a1 = _mm_shufflehi_epi16(a1, 114);
    a2 = _mm_shufflehi_epi16(a2, 114);
    a3 = _mm_shufflehi_epi16(a3, 114);

    a0 = _mm_shuffle_epi32(a0, 114);
    a1 = _mm_shuffle_epi32(a1, 39);
    a2 = _mm_shuffle_epi32(a2, 114);
    a3 = _mm_shuffle_epi32(a3, 39);

    a0 = _mm_or_si128(a0, a1);
    a2 = _mm_or_si128(a2, a3);
    _mm_store_si128((__m128i*)hex + 0, a0);
    _mm_store_si128((__m128i*)hex + 1, a2);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
