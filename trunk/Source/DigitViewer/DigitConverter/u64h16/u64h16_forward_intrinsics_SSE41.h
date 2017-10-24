/* u64h16_forward_intrinsics_SSE41.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/19/2011
 * Last Modified    : 01/23/2016
 * 
 */

#ifndef ydv_u64h16_forward_intrinsics_SSE41_H
#define ydv_u64h16_forward_intrinsics_SSE41_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <smmintrin.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace DigitViewer{
namespace u64h16{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <bool ascii> YM_FORCE_INLINE
void convert_h16x1_forward_SSE41(char str[16], const u64_t hex[1]){
    const __m128i MASK = _mm_set1_epi16(0xf0);

    __m128i a0, b0;

    a0 = _mm_loadl_epi64((const __m128i*)(hex + 0));

    a0 = _mm_cvtepu8_epi16(a0);

    b0 = _mm_and_si128(a0, MASK);

    a0 = _mm_xor_si128(a0, b0);

    b0 = _mm_srli_epi16(b0, 4);
    a0 = _mm_slli_epi16(a0, 8);

    a0 = _mm_or_si128(a0, b0);

    if (ascii){
        const __m128i ADD = _mm_set1_epi8('0');
        const __m128i CMP = _mm_set1_epi8(9);
        const __m128i HEXL = _mm_set1_epi8('a' - '9' - 1);

        b0 = _mm_cmpgt_epi8(a0, CMP);

        b0 = _mm_and_si128(b0, HEXL);
        a0 = _mm_add_epi16(a0, ADD);

        a0 = _mm_add_epi16(a0, b0);
    }

    a0 = _mm_shufflelo_epi16(a0, 27);

    a0 = _mm_shufflehi_epi16(a0, 27);

    a0 = _mm_shuffle_epi32(a0, 78);

    _mm_storeu_si128((__m128i*)str + 0, a0);
}
template <bool forward, bool ascii> YM_FORCE_INLINE
void convert_h16x2_forward_SSE41(char str[16*2], const u64_t hex[2]){
    const __m128i MASK = _mm_set1_epi16(0xf0);

    __m128i a0, b0;
    __m128i a1, b1;

    if (forward){
        a0 = _mm_loadl_epi64((const __m128i*)(hex + 0));
        a1 = _mm_loadl_epi64((const __m128i*)(hex + 1));
    }else{
        a0 = _mm_loadl_epi64((const __m128i*)(hex + 1));
        a1 = _mm_loadl_epi64((const __m128i*)(hex + 0));
    }

    a0 = _mm_cvtepu8_epi16(a0);
    a1 = _mm_cvtepu8_epi16(a1);

    b0 = _mm_and_si128(a0, MASK);
    b1 = _mm_and_si128(a1, MASK);

    a0 = _mm_xor_si128(a0, b0);
    a1 = _mm_xor_si128(a1, b1);

    b0 = _mm_srli_epi16(b0, 4);
    b1 = _mm_srli_epi16(b1, 4);
    a0 = _mm_slli_epi16(a0, 8);
    a1 = _mm_slli_epi16(a1, 8);

    a0 = _mm_or_si128(a0, b0);
    a1 = _mm_or_si128(a1, b1);

    if (ascii){
        const __m128i ADD = _mm_set1_epi8('0');
        const __m128i CMP = _mm_set1_epi8(9);
        const __m128i HEXL = _mm_set1_epi8('a' - '9' - 1);

        b0 = _mm_cmpgt_epi8(a0, CMP);
        b1 = _mm_cmpgt_epi8(a1, CMP);

        b0 = _mm_and_si128(b0, HEXL);
        b1 = _mm_and_si128(b1, HEXL);
        a0 = _mm_add_epi16(a0, ADD);
        a1 = _mm_add_epi16(a1, ADD);

        a0 = _mm_add_epi16(a0, b0);
        a1 = _mm_add_epi16(a1, b1);
    }

    a0 = _mm_shufflelo_epi16(a0, 27);
    a1 = _mm_shufflelo_epi16(a1, 27);

    a0 = _mm_shufflehi_epi16(a0, 27);
    a1 = _mm_shufflehi_epi16(a1, 27);

    a0 = _mm_shuffle_epi32(a0, 78);
    a1 = _mm_shuffle_epi32(a1, 78);

    _mm_storeu_si128((__m128i*)str + 0, a0);
    _mm_storeu_si128((__m128i*)str + 1, a1);
}
template <bool forward, bool ascii> YM_FORCE_INLINE
void convert_h16x4_forward_SSE41(char str[16*4], const u64_t hex[4]){
    const __m128i MASK = _mm_set1_epi16(0xf0);

    __m128i a0, b0;
    __m128i a1, b1;
    __m128i a2, b2;
    __m128i a3, b3;

    if (forward){
        a0 = _mm_loadl_epi64((const __m128i*)(hex + 0));
        a1 = _mm_loadl_epi64((const __m128i*)(hex + 1));
        a2 = _mm_loadl_epi64((const __m128i*)(hex + 2));
        a3 = _mm_loadl_epi64((const __m128i*)(hex + 3));
    }else{
        a0 = _mm_loadl_epi64((const __m128i*)(hex + 3));
        a1 = _mm_loadl_epi64((const __m128i*)(hex + 2));
        a2 = _mm_loadl_epi64((const __m128i*)(hex + 1));
        a3 = _mm_loadl_epi64((const __m128i*)(hex + 0));
    }

    a0 = _mm_cvtepu8_epi16(a0);
    a1 = _mm_cvtepu8_epi16(a1);
    a2 = _mm_cvtepu8_epi16(a2);
    a3 = _mm_cvtepu8_epi16(a3);

    b0 = _mm_and_si128(a0, MASK);
    b1 = _mm_and_si128(a1, MASK);
    b2 = _mm_and_si128(a2, MASK);
    b3 = _mm_and_si128(a3, MASK);

    a0 = _mm_xor_si128(a0, b0);
    a1 = _mm_xor_si128(a1, b1);
    a2 = _mm_xor_si128(a2, b2);
    a3 = _mm_xor_si128(a3, b3);

    b0 = _mm_srli_epi16(b0, 4);
    b1 = _mm_srli_epi16(b1, 4);
    b2 = _mm_srli_epi16(b2, 4);
    b3 = _mm_srli_epi16(b3, 4);
    a0 = _mm_slli_epi16(a0, 8);
    a1 = _mm_slli_epi16(a1, 8);
    a2 = _mm_slli_epi16(a2, 8);
    a3 = _mm_slli_epi16(a3, 8);

    a0 = _mm_or_si128(a0, b0);
    a1 = _mm_or_si128(a1, b1);
    a2 = _mm_or_si128(a2, b2);
    a3 = _mm_or_si128(a3, b3);

    if (ascii){
        const __m128i ADD = _mm_set1_epi8('0');
        const __m128i CMP = _mm_set1_epi8(9);
        const __m128i HEXL = _mm_set1_epi8('a' - '9' - 1);

        b0 = _mm_cmpgt_epi8(a0, CMP);
        b1 = _mm_cmpgt_epi8(a1, CMP);
        b2 = _mm_cmpgt_epi8(a2, CMP);
        b3 = _mm_cmpgt_epi8(a3, CMP);

        b0 = _mm_and_si128(b0, HEXL);
        b1 = _mm_and_si128(b1, HEXL);
        b2 = _mm_and_si128(b2, HEXL);
        b3 = _mm_and_si128(b3, HEXL);
        a0 = _mm_add_epi16(a0, ADD);
        a1 = _mm_add_epi16(a1, ADD);
        a2 = _mm_add_epi16(a2, ADD);
        a3 = _mm_add_epi16(a3, ADD);

        a0 = _mm_add_epi16(a0, b0);
        a1 = _mm_add_epi16(a1, b1);
        a2 = _mm_add_epi16(a2, b2);
        a3 = _mm_add_epi16(a3, b3);
    }

    a0 = _mm_shufflelo_epi16(a0, 27);
    a1 = _mm_shufflelo_epi16(a1, 27);
    a2 = _mm_shufflelo_epi16(a2, 27);
    a3 = _mm_shufflelo_epi16(a3, 27);

    a0 = _mm_shufflehi_epi16(a0, 27);
    a1 = _mm_shufflehi_epi16(a1, 27);
    a2 = _mm_shufflehi_epi16(a2, 27);
    a3 = _mm_shufflehi_epi16(a3, 27);

    a0 = _mm_shuffle_epi32(a0, 78);
    a1 = _mm_shuffle_epi32(a1, 78);
    a2 = _mm_shuffle_epi32(a2, 78);
    a3 = _mm_shuffle_epi32(a3, 78);

    _mm_storeu_si128((__m128i*)str + 0, a0);
    _mm_storeu_si128((__m128i*)str + 1, a1);
    _mm_storeu_si128((__m128i*)str + 2, a2);
    _mm_storeu_si128((__m128i*)str + 3, a3);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
