/* u32d9_forward_intrinsics_SSE41.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/19/2011
 * Last Modified    : 01/23/2016
 * 
 */

#ifndef ydv_u32d9_forward_intrinsics_SSE41_H
#define ydv_u32d9_forward_intrinsics_SSE41_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <smmintrin.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace DigitViewer{
namespace u32d9{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Iterate Reverse, Raw Output
YM_FORCE_INLINE void extract_digit0(__m128i& i0, __m128i& i1, __m128d& f0, __m128d& f1, __m128d SHIFT0, __m128d SHIFT1){
    const __m128d SCALE = _mm_set1_pd(0.00000001);

    __m128d d0, d1;

    f0 = _mm_mul_pd(f0, SCALE);
    f1 = _mm_mul_pd(f1, SCALE);
    d0 = _mm_floor_pd(f0);
    d1 = _mm_floor_pd(f1);
    f0 = _mm_sub_pd(f0, d0);
    f1 = _mm_sub_pd(f1, d1);
    d0 = _mm_mul_pd(d0, SHIFT0);
    d1 = _mm_mul_pd(d1, SHIFT1);
    i0 = _mm_cvttpd_epi32(d0);
    i1 = _mm_cvttpd_epi32(d1);
}
YM_FORCE_INLINE void extract_digit(__m128i& i0, __m128i& i1, __m128d& f0, __m128d& f1, __m128d SHIFT0, __m128d SHIFT1){
    register const __m128d TEN = _mm_set1_pd(10.00000002);

    __m128d d0, d1;

    f0 = _mm_mul_pd(f0, TEN);
    f1 = _mm_mul_pd(f1, TEN);
    d0 = _mm_floor_pd(f0);
    d1 = _mm_floor_pd(f1);
    f0 = _mm_sub_pd(f0, d0);
    f1 = _mm_sub_pd(f1, d1);
    d0 = _mm_mul_pd(d0, SHIFT0);
    d1 = _mm_mul_pd(d1, SHIFT1);
    i0 = _mm_cvttpd_epi32(d0);
    i1 = _mm_cvttpd_epi32(d1);
}
YM_FORCE_INLINE void convert_d9x4_forward_SSE41(char str[9*4], const u32_t dec[4]){
    const __m128d SHIFT0 = _mm_set_pd(256., 1.);
    const __m128d SHIFT1 = _mm_set_pd(65536., 256.);
    const __m128d SHIFT2 = _mm_set_pd(16777216., 65536.);
    const __m128d SHIFT3 = _mm_set_pd(1., 16777216.);

    register __m128d a0, a1;
    register __m128i i0, i1;
    register __m128i o0, o1, o2;
    register __m128i t0, t1;

    //  Load 4 words
    i1 = ((const __m128i*)dec)[0];
    i0 = _mm_shuffle_epi32(i1, 27);
    i1 = _mm_shuffle_epi32(i1, 177);
    a0 = _mm_cvtepi32_pd(i0);
    a1 = _mm_cvtepi32_pd(i1);

    //  1st Digit
    extract_digit0(o0, o1, a0, a1, SHIFT0, SHIFT2);
    o0 = _mm_shuffle_epi32(o0, 216);
    o1 = _mm_shuffle_epi32(o1, 216);

    //  2nd Digit
    extract_digit(i0, i1, a0, a1, SHIFT1, SHIFT3);
    i0 = _mm_shuffle_epi32(i0, 216);
    i1 = _mm_shuffle_epi32(i1, 120);
    o0 = _mm_or_si128(o0, i0);
    o1 = _mm_or_si128(o1, i1);

    //  3rd Digit
    extract_digit(i0, i1, a0, a1, SHIFT2, SHIFT0);
    i0 = _mm_shuffle_epi32(i0, 216);
    i1 = _mm_shuffle_epi32(i1, 114);
    o0 = _mm_or_si128(o0, i0);
    o1 = _mm_or_si128(o1, i1);

    //  4th Digit
    extract_digit(i0, i1, a0, a1, SHIFT3, SHIFT1);
    i0 = _mm_shuffle_epi32(i0, 120);
    i1 = _mm_shuffle_epi32(i1, 114);
    o0 = _mm_or_si128(o0, i0);
    o1 = _mm_or_si128(o1, i1);

    //  5th Digit
    extract_digit(i0, i1, a0, a1, SHIFT0, SHIFT2);
    i0 = _mm_shuffle_epi32(i0, 114);
    i1 = _mm_shuffle_epi32(i1, 114);
    o0 = _mm_or_si128(o0, i0);
    o1 = _mm_or_si128(o1, i1);

    //  6th Digit
    extract_digit(i0, i1, a0, a1, SHIFT1, SHIFT3);
    i0 = _mm_shuffle_epi32(i0, 114);
    o2 = _mm_shuffle_epi32(i1, 169);
    i1 = _mm_shuffle_epi32(i1, 162);
    o0 = _mm_or_si128(o0, i0);
    o1 = _mm_or_si128(o1, i1);

    //  7th Digit
    extract_digit(i0, i1, a0, a1, SHIFT2, SHIFT0);
    i0 = _mm_shuffle_epi32(i0, 114);
    t0 = _mm_shuffle_epi32(i1, 138);
    i1 = _mm_shuffle_epi32(i1, 169);
    o0 = _mm_or_si128(o0, i0);
    o1 = _mm_or_si128(o1, t0);
    o2 = _mm_or_si128(o2, i1);

    //  8th Digit
    extract_digit(i0, i1, a0, a1, SHIFT3, SHIFT1);
    t0 = _mm_shuffle_epi32(i0, 169);
    i0 = _mm_shuffle_epi32(i0, 162);
    t1 = _mm_shuffle_epi32(i1, 138);
    i1 = _mm_shuffle_epi32(i1, 169);
    o0 = _mm_or_si128(o0, i0);
    o1 = _mm_or_si128(o1, t0);
    o1 = _mm_or_si128(o1, t1);
    o2 = _mm_or_si128(o2, i1);

    //  9th Digit
    extract_digit(i0, i1, a0, a1, SHIFT0, SHIFT2);
    t0 = _mm_shuffle_epi32(i0, 138);
    i0 = _mm_shuffle_epi32(i0, 169);
    t1 = _mm_shuffle_epi32(i1, 138);
    i1 = _mm_shuffle_epi32(i1, 169);
    o0 = _mm_or_si128(o0, t0);
    o1 = _mm_or_si128(o1, i0);
    o1 = _mm_or_si128(o1, t1);
    o2 = _mm_or_si128(o2, i1);

    //  Store to destination.
    _mm_storeu_si128((__m128i*)str + 0, o0);
    _mm_storeu_si128((__m128i*)str + 1, o1);
    *(u32_t*)((__m128i*)str + 2) = _mm_extract_epi32(o2, 0);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
