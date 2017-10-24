/* u64d19_forward_intrinsics_SSE41.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/19/2011
 * Last Modified    : 01/22/2016
 * 
 *      This file implements a word-sized radix conversion from binary to decimal.
 *  The algorithm used here is a linearized scaled remainder tree.
 * 
 *      The factor "0.00000000000001" is an additive purturbance to correct for
 *  roundoff error. This is verified by brute-forcing all possibilities.
 *  There are only 10 billion combinations: (0 - 9, 999, 999, 999)
 * 
 *  Values in the range of 10^-11 - 10^-15 have been verified to work. The
 *  current implementation uses 10^-13.
 * 
 * 
 *  This implementation isn't always faster than the default version with
 *  integer divisions. It's faster on Nehalem, but slower on Haswell.
 * 
 */

#ifndef ydv_u64d19_forward_intrinsics_SSE41_H
#define ydv_u64d19_forward_intrinsics_SSE41_H
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
//  Stage 1: Split into 10-digit words. Store as double.
YM_FORCE_INLINE __m128i split10_get_high(__m128i x){
    //  Compute x / 10^10. The answer may overshoot by 1.

    const __m128d ONE = _mm_set1_pd(1.);
    const __m128d SCALE = _mm_set1_pd(0.000004096);
    const __m128d MASK0 = _mm_set1_pd(4503599627370496.);
    const __m128d MASK1 = _mm_castsi128_pd(_mm_set_epi32(0x000fffff, 0xffffffff, 0x000fffff, 0xffffffff));

    __m128d d;

    //  x = 1234567895987654321
    x = _mm_srli_epi64(x, 12);

    //  Convert to floating-point.
    d = _mm_castsi128_pd(x);
    d = _mm_or_pd(d, MASK0);
    d = _mm_sub_pd(d, MASK0);

    //  Scale it by 4096 * 10^-9.
    //  This also counters the previous right-shift by 12 bits.
    d = _mm_mul_pd(d, SCALE);

    //  Purturb the value down by 1. Don't let it go negative.
    d = _mm_sub_pd(d, ONE);
    d = _mm_max_pd(d, _mm_setzero_pd());

    //  Convert back to integer.
    d = _mm_add_pd(d, MASK0);
    d = _mm_and_pd(d, MASK1);
    x = _mm_castpd_si128(d);

    //  At this point, "x" holds the top 10 digits of each word.
    //  x = 1234567895---------
    //  Note that this result may not be exact. (It may be off by 1.)

    return x;
}
YM_FORCE_INLINE void split10_finish(__m128d& L, __m128d& H, __m128i iL, __m128i iH){
    const __m128d MASK = _mm_set1_pd(4503599627370496.);
    const __m128i CMP0 = _mm_set_epi32(0, 999999999, 0, 999999999);
    const __m128i CMP1 = _mm_set_epi32(0, 1000000000, 0, 1000000000);
    const __m128i CMP2 = _mm_set_epi32(0, 1, 0, 1);

    __m128i r0, r1;
    u64_t i0, i1;

    //  Multiply "H" by 10^9.
    i0 = _mm_cvtsi128_si64(iH);
    i1 = _mm_extract_epi64(iH, 1);
    i0 = i0 * 1000000000ul;
    i1 = i1 * 1000000000ul;
    r0 = _mm_cvtsi64_si128(i0);
    r0 = _mm_insert_epi64(r0, i1, 1);

    //  Subtract from "iL". This will expose the bottom 9 digits.
    //  iL = ----------987654321
    iL = _mm_sub_epi64(iL, r0);

    //  Verify the split. Make corrections.
    r0 = _mm_cmpgt_epi32(iL, CMP0);
    r1 = _mm_and_si128(r0, CMP1);
    r0 = _mm_and_si128(r0, CMP2);
    iL = _mm_sub_epi64(iL, r1);
    iH = _mm_add_epi64(iH, r0);

    //  At this point:
    //  "iL" holds the bottom 9 digits of each word.
    //  "iH" holds the top 10 digits of each word.
    //  iL = ----------987654321
    //  iH = 1234567895---------
    //  This result is provably exact.

    //  Convert to floating-point.
    L = _mm_castsi128_pd(iL);
    H = _mm_castsi128_pd(iH);
    L = _mm_or_pd(L, MASK);
    H = _mm_or_pd(H, MASK);
    L = _mm_sub_pd(L, MASK);
    H = _mm_sub_pd(H, MASK);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Stage 2: Convert 10-digit words. Place into buffers.
YM_FORCE_INLINE void extract_digit0(__m128i& i0, __m128i& i1, __m128d& f0, __m128d& f1, __m128d SHIFT0, __m128d SHIFT1){
    const __m128d ADD = _mm_set1_pd(0.00000000000001);

    __m128d d0, d1;

    f1 = _mm_mul_pd(f1, _mm_set1_pd(0.000000001));
    f0 = _mm_mul_pd(f0, _mm_set1_pd(0.00000001));

    //  Purturb up. This will force rounding in the desired direction.
    f1 = _mm_add_pd(f1, ADD);
    f0 = _mm_add_pd(f0, ADD);

    //  Keep the first digit.
    //  _r3d = 1.
    //  _r2d = 9.
    d1 = _mm_floor_pd(f1);
    d0 = _mm_floor_pd(f0);

    //  Subtract to eliminate that first digit.
    //  f1 = 0.234567895
    //  f0 = 0.87654321
    f1 = _mm_sub_pd(f1, d1);
    f0 = _mm_sub_pd(f0, d0);

    //  Shift
    d1 = _mm_mul_pd(d1, SHIFT0);
    d0 = _mm_mul_pd(d0, SHIFT1);
    i1 = _mm_cvttpd_epi32(d1);
    i0 = _mm_cvttpd_epi32(d0);
}
YM_FORCE_INLINE void extract_digit(__m128i& i0, __m128i& i1, __m128d& f0, __m128d& f1, __m128d SHIFT0, __m128d SHIFT1){
    const __m128d TEN = _mm_set1_pd(10.);
    const __m128d ADD = _mm_set1_pd(0.00000000000001);

    __m128d d0, d1;

    f1 = _mm_mul_pd(f1, TEN);
    f0 = _mm_mul_pd(f0, TEN);

    //  Purturb up. This will force rounding in the desired direction.
    f1 = _mm_add_pd(f1, ADD);
    f0 = _mm_add_pd(f0, ADD);

    //  Keep the first digit.
    //  _r3d = 1.
    //  _r2d = 9.
    d1 = _mm_floor_pd(f1);
    d0 = _mm_floor_pd(f0);

    //  Subtract to eliminate that first digit.
    //  f1 = 0.234567895
    //  f0 = 0.87654321
    f1 = _mm_sub_pd(f1, d1);
    f0 = _mm_sub_pd(f0, d0);

    //  Shift
    d1 = _mm_mul_pd(d1, SHIFT0);
    d0 = _mm_mul_pd(d0, SHIFT1);
    i1 = _mm_cvttpd_epi32(d1);
    i0 = _mm_cvttpd_epi32(d0);
}
template <bool forward, bool ascii> YM_FORCE_INLINE
void convert_d19x2_forward_SSE41(char str[19*2], const u64_t dec[2]){
    const __m128d SHIFT0 = _mm_set_pd(16777216., 1.);
    const __m128d SHIFT1 = _mm_set_pd(1., 256.);
    const __m128d SHIFT2 = _mm_set_pd(256., 65536.);
    const __m128d SHIFT3 = _mm_set_pd(65536., 16777216.);

    __m128d f0, f1;
    {
        __m128i i0, i1;

        //  Load 2 words
        //  _r1i = 1234567895987654321
        i0 = ((const __m128i*)dec)[0];

        if (!forward){
            i0 = _mm_shuffle_epi32(i0, 78);
        }

        i1 = split10_get_high(i0);
        split10_finish(f0, f1, i0, i1);
    }

    __m128i o0, o1, o2;

    //  1st digit
    {
        __m128i d0, d1;
        __m128i i2, i3;
        extract_digit0(d0, d1, f0, f1, SHIFT0, SHIFT2);
        o0 = _mm_shuffle_epi32(d1, 168);
        o1 = _mm_shuffle_epi32(d1, 169);
        i2 = _mm_shuffle_epi32(d0, 138);
        i3 = _mm_shuffle_epi32(d0, 106);
        o0 = _mm_or_si128(o0, i2);
        o1 = _mm_or_si128(o1, i3);
    }

    //  2nd Digit
    {
        __m128i d0, d1;
        __m128i i0, i1, i2, i3;
        extract_digit(d0, d1, f0, f1, SHIFT1, SHIFT3);
        i0 = _mm_shuffle_epi32(d1, 168);
        i1 = _mm_shuffle_epi32(d1, 166);
        i2 = _mm_shuffle_epi32(d0, 138);
        i3 = _mm_shuffle_epi32(d0, 106);
        o0 = _mm_or_si128(o0, i0);
        o1 = _mm_or_si128(o1, i1);
        o0 = _mm_or_si128(o0, i2);
        o1 = _mm_or_si128(o1, i3);
    }

    //  3rd Digit
    {
        __m128i d0, d1;
        __m128i i0, i1, i2, i3;
        extract_digit(d0, d1, f0, f1, SHIFT2, SHIFT0);
        i0 = _mm_shuffle_epi32(d1, 168);
        i1 = _mm_shuffle_epi32(d1, 166);
        i2 = _mm_shuffle_epi32(d0, 42);
        i3 = _mm_shuffle_epi32(d0, 106);
        o0 = _mm_or_si128(o0, i0);
        o1 = _mm_or_si128(o1, i1);
        o0 = _mm_or_si128(o0, i2);
        o1 = _mm_or_si128(o1, i3);
    }

    //  4th Digit
    {
        __m128i d0, d1;
        __m128i i0, i1, i3;
        extract_digit(d0, d1, f0, f1, SHIFT3, SHIFT1);
        i0 = _mm_shuffle_epi32(d1, 168);
        i1 = _mm_shuffle_epi32(d1, 166);
        o2 = _mm_shuffle_epi32(d0, 169);
        i3 = _mm_shuffle_epi32(d0, 42);
        o0 = _mm_or_si128(o0, i0);
        o1 = _mm_or_si128(o1, i1);
        o0 = _mm_or_si128(o0, i3);
    }

    //  5th Digit
    {
        __m128i d0, d1;
        __m128i i0, i1, i2, i3;
        extract_digit(d0, d1, f0, f1, SHIFT0, SHIFT2);
        i0 = _mm_shuffle_epi32(d1, 162);
        i1 = _mm_shuffle_epi32(d1, 166);
        i2 = _mm_shuffle_epi32(d0, 42);
        i3 = _mm_shuffle_epi32(d0, 169);
        o0 = _mm_or_si128(o0, i0);
        o1 = _mm_or_si128(o1, i1);
        o0 = _mm_or_si128(o0, i2);
        o2 = _mm_or_si128(o2, i3);
    }

    //  6th Digit
    {
        __m128i d0, d1;
        __m128i i0, i1, i2, i3;
        extract_digit(d0, d1, f0, f1, SHIFT1, SHIFT3);
        i0 = _mm_shuffle_epi32(d1, 162);
        i1 = _mm_shuffle_epi32(d1, 154);
        i2 = _mm_shuffle_epi32(d0, 42);
        i3 = _mm_shuffle_epi32(d0, 169);
        o0 = _mm_or_si128(o0, i0);
        o1 = _mm_or_si128(o1, i1);
        o0 = _mm_or_si128(o0, i2);
        o2 = _mm_or_si128(o2, i3);
    }

    //  7th Digit
    {
        __m128i d0, d1;
        __m128i i0, i1, i2, i3;
        extract_digit(d0, d1, f0, f1, SHIFT2, SHIFT0);
        i0 = _mm_shuffle_epi32(d1, 162);
        i1 = _mm_shuffle_epi32(d1, 154);
        i2 = _mm_shuffle_epi32(d0, 168);
        i3 = _mm_shuffle_epi32(d0, 169);
        o0 = _mm_or_si128(o0, i0);
        o1 = _mm_or_si128(o1, i1);
        o1 = _mm_or_si128(o1, i2);
        o2 = _mm_or_si128(o2, i3);
    }

    //  8th Digit
    {
        __m128i d0, d1;
        __m128i i0, i1, i2, i3;
        extract_digit(d0, d1, f0, f1, SHIFT3, SHIFT1);
        i0 = _mm_shuffle_epi32(d1, 162);
        i1 = _mm_shuffle_epi32(d1, 154);
        i2 = _mm_shuffle_epi32(d0, 168);
        i3 = _mm_shuffle_epi32(d0, 166);
        o0 = _mm_or_si128(o0, i0);
        o1 = _mm_or_si128(o1, i1);
        o1 = _mm_or_si128(o1, i2);
        o2 = _mm_or_si128(o2, i3);
    }


    //  9th Digit
    {
        __m128i d0, d1;
        __m128i i0, i1, i2, i3;
        extract_digit(d0, d1, f0, f1, SHIFT0, SHIFT2);
        i0 = _mm_shuffle_epi32(d1, 138);
        i1 = _mm_shuffle_epi32(d1, 154);
        i2 = _mm_shuffle_epi32(d0, 168);
        i3 = _mm_shuffle_epi32(d0, 166);
        o0 = _mm_or_si128(o0, i0);
        o1 = _mm_or_si128(o1, i1);
        o1 = _mm_or_si128(o1, i2);
        o2 = _mm_or_si128(o2, i3);
    }

    //  10th Digit
    {
        const __m128d TEN = _mm_set1_pd(10.);
        __m128d d1;
        f1 = _mm_mul_pd(f1, TEN);
        d1 = _mm_round_pd(f1, 0);
        f1 = _mm_sub_pd(f1, d1);
        d1 = _mm_mul_pd(d1, SHIFT1);

        __m128i i0, i1;
        i1 = _mm_cvttpd_epi32(d1);
        i0 = _mm_shuffle_epi32(i1, 138);
        i1 = _mm_shuffle_epi32(i1, 106);
        o0 = _mm_or_si128(o0, i0);
        o1 = _mm_or_si128(o1, i1);
    }

    //  Add ASCII offsets.
    if (ascii){
        const __m128i DECSHIFT = _mm_set1_epi8('0');
        o0 = _mm_add_epi8(o0, DECSHIFT);
        o1 = _mm_add_epi8(o1, DECSHIFT);
        o2 = _mm_add_epi8(o2, DECSHIFT);
    }

    //  Store to destination.
    _mm_storeu_si128((__m128i*)(str) + 0, o0);
    _mm_storeu_si128((__m128i*)(str) + 1, o1);
    u64_t x0 = _mm_cvtsi128_si64(o2);
    *((u32_t*)(str) +  8) = (u32_t)x0;
    *((u16_t*)(str) + 18) = (u16_t)(x0 >> 32);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
