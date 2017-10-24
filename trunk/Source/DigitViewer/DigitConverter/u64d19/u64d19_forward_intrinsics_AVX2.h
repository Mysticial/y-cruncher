/* u64d19_forward_intrinsics_AVX2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/22/2016
 * Last Modified    : 01/22/2016
 * 
 *      This is an improved version of the SSE4.1 algorithm.
 * 
 *  In addition to widening everything to 256 bits, it further splits the words
 *  into 5-digit floats rather than doing a sequential scaled remainder tree on
 *  10-digit doubles.
 * 
 *  On Haswell, this kernel appears to be latency-bound and gets slightly less
 *  than 2 instructions/cycle. Unfortunately, there are not enough registers
 *  to unroll more than 2 iterations without resorting to more complicated
 *  transformations.
 * 
 */

#ifndef ydv_u64d19_forward_intrinsics_AVX2_H
#define ydv_u64d19_forward_intrinsics_AVX2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <immintrin.h>
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
YM_FORCE_INLINE __m256d split10_get_high(__m256i x){
    //  Compute x / 10^10. The answer may overshoot by 1.

    const __m256d SCALE = _mm256_set1_pd(0.0000004096);
    const __m256d CONVERT = _mm256_set1_pd(4503599627370496.);

    __m256d f1;

    //  Shift down. We don't have enough precision to directly convert the
    //  whole thing.
    x = _mm256_srli_epi64(x, 12);

    //  Convert to floating-point.
    f1 = _mm256_castsi256_pd(x);
    f1 = _mm256_or_pd(f1, CONVERT);
    f1 = _mm256_sub_pd(f1, CONVERT);

    //  Scale by 4096 * 10^-10. (This also counters the previous right-shift by 12 bits.)
    //  Round to integer.
    f1 = _mm256_fmadd_pd(f1, SCALE, CONVERT);
    f1 = _mm256_sub_pd(f1, CONVERT);

    //  At this point, "f1" holds the top 10 digits x.
    //  Note that this result may not be exact. (It may be off by 1.)

    return f1;
}
YM_FORCE_INLINE void split10_split_words(__m256d& L, __m256d& H, __m256i x){
    //  Split x into upper and lower 32-bits. Return them as floating-point.

    const __m256d WORD = _mm256_set1_pd(4294967296.);

    //  Split x into upper and lower 32-bits.
    //  x = {0L, 0H, 1L, 1H, 2L, 2H, 3L, 3H}
    //  L = {0L, 1L, 2L, 3L}
    //  H = {0H, 1H, 2H, 3H}

    const __m256d CONVERT = _mm256_set1_pd(4503599627370496.);

    __m256i y;
    y = _mm256_srli_epi64(x, 32);
    x = _mm256_blend_epi16(x, _mm256_setzero_si256(), 0xcc);

    H = _mm256_castsi256_pd(y);
    L = _mm256_castsi256_pd(x);
    H = _mm256_or_pd(H, CONVERT);
    L = _mm256_or_pd(L, CONVERT);
    H = _mm256_sub_pd(H, CONVERT);
    L = _mm256_sub_pd(L, CONVERT);
    H = _mm256_mul_pd(H, WORD);
}
YM_FORCE_INLINE void split10_finish(__m256d& L, __m256d& H, __m256d wL, __m256d wH){
    const __m256d ONE = _mm256_set1_pd(1.);
    const __m256d POW = _mm256_set1_pd(10000000000.);

    //  Extract the bottom 10 digits. (Negative implies a correction is needed.)
    wH = _mm256_fnmadd_pd(H, POW, wH);
    L = _mm256_add_pd(wL, wH);

    //  Make correction.
    wH = _mm256_cmp_pd(L, _mm256_setzero_pd(), _CMP_LT_OS);
    wL = _mm256_and_pd(wH, POW);
    wH = _mm256_and_pd(wH, ONE);
    L = _mm256_add_pd(L, wL);
    H = _mm256_sub_pd(H, wH);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Stage 2: Split into 5-digit words. Store as float.
YM_FORCE_INLINE void split5_to_floats(__m256& L, __m256& H, __m256d a, __m256d b){
    //  Split a and b into upper and lower 5 decimal digits.
    //  The results may not be exact. The upper word may overshoot by 1 and the
    //  lower word may be short by 10^5.

    //  Return:
    //      L = {a % 10^5}{b % 10^5}
    //      H = {a / 10^5}{b / 10^5}

    const __m256d POW = _mm256_set1_pd(100000.);
    const __m256d SCALE = _mm256_set1_pd(0.00001);
    const __m256d CONVERT = _mm256_set1_pd(4503599627370496.);

    __m256d a0, a1;
    __m256d b0, b1;

    //  Split A
    a1 = _mm256_fmadd_pd(a, SCALE, CONVERT);
    a1 = _mm256_sub_pd(a1, CONVERT);
    a0 = _mm256_fnmadd_pd(a1, POW, a);

    //  Split B
    b1 = _mm256_fmadd_pd(b, SCALE, CONVERT);
    b1 = _mm256_sub_pd(b1, CONVERT);
    b0 = _mm256_fnmadd_pd(b1, POW, b);

    //  Convert to floats
    //  COMPILER-BUG-GCC: Missing AVX intrinsic: "_mm256_setr_m128()"
#ifndef __GNUC__
    L = _mm256_setr_m128(_mm256_cvtpd_ps(a0), _mm256_cvtpd_ps(b0));
    H = _mm256_setr_m128(_mm256_cvtpd_ps(a1), _mm256_cvtpd_ps(b1));
#else
    L = _mm256_insertf128_ps(_mm256_castps128_ps256(_mm256_cvtpd_ps(a0)), _mm256_cvtpd_ps(b0), 1);
    H = _mm256_insertf128_ps(_mm256_castps128_ps256(_mm256_cvtpd_ps(a1)), _mm256_cvtpd_ps(b1), 1);
#endif
}
YM_FORCE_INLINE void split5_finish(__m256& H, __m256& L, __m256 fL, __m256 fH){
    const __m256 ONE = _mm256_set1_ps(1.f);
    const __m256 POW = _mm256_set1_ps(100000.f);

    //  Make correction
    H = _mm256_cmp_ps(fL, _mm256_setzero_ps(), _CMP_LT_OS);
    L = _mm256_and_ps(H, POW);
    H = _mm256_and_ps(H, ONE);
    fL = _mm256_add_ps(fL, L);
    fH = _mm256_sub_ps(fH, H);

    //  Shuffle and flip the endian.
    fL = _mm256_permutevar8x32_ps(fL, _mm256_setr_epi32(4, 5, 0, 1, 6, 7, 2, 3));
    fH = _mm256_permutevar8x32_ps(fH, _mm256_setr_epi32(4, 5, 0, 1, 6, 7, 2, 3));
    H = _mm256_unpacklo_ps(fH, fL);
    L = _mm256_unpackhi_ps(fH, fL);

    //  L = {fH[0], fL[0], fH[1], fL[1], fH[2], fL[2], fH[3], fL[3]}
    //  H = {fH[4], fL[4], fH[5], fL[5], fH[6], fL[6], fH[7], fL[7]}
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Stage 3: Convert 5-digit words. Place into buffers.
YM_FORCE_INLINE void extract_digit(__m256i& i0, __m256i& i1, __m256& f0, __m256& f1, __m256 SCALE){
    const __m256 SHIFT = _mm256_setr_ps(1, 256, 1, 256, 1, 256, 1, 256);

    //  Additive perturbance to compensate for round-off error.
    //  Valid Range: (0.0000007, 0.00008f)
    const __m256 ADD = _mm256_set1_ps(0.000007f);

    __m256 r0, r1;

    //  Scale up and remove integer part.
    f0 = _mm256_fmadd_ps(f0, SCALE, ADD);
    f1 = _mm256_fmadd_ps(f1, SCALE, ADD);
    r0 = _mm256_floor_ps(f0);
    r1 = _mm256_floor_ps(f1);
    f0 = _mm256_sub_ps(f0, r0);
    f1 = _mm256_sub_ps(f1, r1);

    //  Shift integer part and convert to integer.
    r0 = _mm256_mul_ps(r0, SHIFT);
    r1 = _mm256_mul_ps(r1, SHIFT);
    i0 = _mm256_cvttps_epi32(r0);
    i1 = _mm256_cvttps_epi32(r1);
}
YM_FORCE_INLINE void extract_digit0(__m256i& s0, __m256i& s1, __m256i& s2, __m256& f0, __m256& f1){
    __m256i i0, i1;
    extract_digit(i0, i1, f0, f1, _mm256_set1_ps(0.0001f));
    s0 = _mm256_srli_epi64(i0, 8);
    s1 = _mm256_slli_epi64(i1, 8);
}
YM_FORCE_INLINE void extract_digit1(__m256i& s0, __m256i& s1, __m256i& s2, __m256& f0, __m256& f1){
    __m256i i0, i1;
    extract_digit(i0, i1, f0, f1, _mm256_set1_ps(10.f));
    s0 = _mm256_or_si256(s0, i0);
    s1 = _mm256_or_si256(s1, _mm256_slli_epi64(i1, 16));
}
YM_FORCE_INLINE void extract_digit2(__m256i& s0, __m256i& s1, __m256i& s2, __m256& f0, __m256& f1){
    __m256i i0, i1;
    extract_digit(i0, i1, f0, f1, _mm256_set1_ps(10.f));
    s0 = _mm256_or_si256(s0, _mm256_slli_epi64(i0, 8));
    s1 = _mm256_or_si256(s1, _mm256_slli_epi64(i1, 24));
    s2 = _mm256_srli_epi64(i1, 40);
}
YM_FORCE_INLINE void extract_digit3(__m256i& s0, __m256i& s1, __m256i& s2, __m256& f0, __m256& f1){
    __m256i i0, i1;
    extract_digit(i0, i1, f0, f1, _mm256_set1_ps(10.f));
    s0 = _mm256_or_si256(s0, _mm256_slli_epi64(i0, 16));
    s1 = _mm256_or_si256(s1, _mm256_slli_epi64(i1, 32));
    s2 = _mm256_or_si256(s2, _mm256_srli_epi64(i1, 32));
}
YM_FORCE_INLINE void extract_digit4(__m256i& s0, __m256i& s1, __m256i& s2, __m256& f0, __m256& f1){
    __m256i i0, i1;
    extract_digit(i0, i1, f0, f1, _mm256_set1_ps(10.f));
    s0 = _mm256_or_si256(s0, _mm256_slli_epi64(i0, 24));
    s1 = _mm256_or_si256(s1, _mm256_srli_epi64(i0, 40));
    s1 = _mm256_or_si256(s1, _mm256_slli_epi64(i1, 40));
    s2 = _mm256_or_si256(s2, _mm256_srli_epi64(i1, 24));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Wrappers
#define ymp_m256i_scatter4(v, p0, p1, p2, p3){  \
    __m128d _xmm = _mm256_castpd256_pd128(_mm256_castsi256_pd(v));  \
    _mm_storel_pd((double*)(p0), _xmm); \
    _mm_storeh_pd((double*)(p1), _xmm); \
    _xmm = _mm256_extractf128_pd(_mm256_castsi256_pd(v), 1);    \
    _mm_storel_pd((double*)(p2), _xmm); \
    _mm_storeh_pd((double*)(p3), _xmm); \
}
template <bool forward, bool ascii> YM_FORCE_INLINE
void convert_d19x4_forward_AVX2(char str[19*4], const u64_t dec[4]){
    __m256 fL0, fH0;

    {
        __m256d a0, b0, c0, d0;
        __m256i x0 = _mm256_lddqu_si256((const __m256i*)dec);
        b0 = split10_get_high(x0);
        split10_split_words(c0, d0, x0);
        split10_finish(a0, b0, c0, d0);
        split5_to_floats(fL0, fH0, a0, b0);
        split5_finish(fL0, fH0, fL0, fH0);
    }

    __m256i s0, s1, s2;
    extract_digit0(s0, s1, s2, fL0, fH0);
    extract_digit1(s0, s1, s2, fL0, fH0);
    extract_digit2(s0, s1, s2, fL0, fH0);
    extract_digit3(s0, s1, s2, fL0, fH0);
    extract_digit4(s0, s1, s2, fL0, fH0);
    s2 = _mm256_slli_epi64(s2, 40);

    if (ascii){
        const __m256i DECSHIFT = _mm256_set1_epi8('0');
        s0 = _mm256_add_epi8(s0, DECSHIFT);
        s1 = _mm256_add_epi8(s1, DECSHIFT);
        s2 = _mm256_add_epi8(s2, DECSHIFT);
    }

    if (forward){
        ymp_m256i_scatter4(s2, str + 11 + 19*0, str + 11 + 19*1, str + 11 + 19*2, str + 11 + 19*3);
        ymp_m256i_scatter4(s0, str +  0 + 19*0, str +  0 + 19*1, str +  0 + 19*2, str +  0 + 19*3);
        ymp_m256i_scatter4(s1, str +  8 + 19*0, str +  8 + 19*1, str +  8 + 19*2, str +  8 + 19*3);
    }else{
        ymp_m256i_scatter4(s2, str + 11 + 19*3, str + 11 + 19*2, str + 11 + 19*1, str + 11 + 19*0);
        ymp_m256i_scatter4(s0, str +  0 + 19*3, str +  0 + 19*2, str +  0 + 19*1, str +  0 + 19*0);
        ymp_m256i_scatter4(s1, str +  8 + 19*3, str +  8 + 19*2, str +  8 + 19*1, str +  8 + 19*0);
    }
}
template <bool forward, bool ascii> YM_FORCE_INLINE
void convert_d19x8_forward_AVX2(char str[19*8], const u64_t dec[8]){
    __m256  fL0, fH0;
    __m256  fL1, fH1;

    {
        __m256d a0, b0, c0, d0;
        __m256d a1, b1, c1, d1;

        __m256i x0 = _mm256_lddqu_si256((const __m256i*)dec + 0);
        __m256i x1 = _mm256_lddqu_si256((const __m256i*)dec + 1);

        b0 = split10_get_high(x0);
        b1 = split10_get_high(x1);

        split10_split_words(c0, d0, x0);
        split10_split_words(c1, d1, x1);

        split10_finish(a0, b0, c0, d0);
        split10_finish(a1, b1, c1, d1);

        split5_to_floats(fL0, fH0, a0, b0);
        split5_to_floats(fL1, fH1, a1, b1);

        split5_finish(fL0, fH0, fL0, fH0);
        split5_finish(fL1, fH1, fL1, fH1);
    }

    __m256i a0, b0, c0;
    __m256i a1, b1, c1;
    extract_digit0(a0, b0, c0, fL0, fH0);
    extract_digit0(a1, b1, c1, fL1, fH1);

    extract_digit1(a0, b0, c0, fL0, fH0);
    extract_digit1(a1, b1, c1, fL1, fH1);

    extract_digit2(a0, b0, c0, fL0, fH0);
    extract_digit2(a1, b1, c1, fL1, fH1);

    extract_digit3(a0, b0, c0, fL0, fH0);
    extract_digit3(a1, b1, c1, fL1, fH1);

    extract_digit4(a0, b0, c0, fL0, fH0);
    extract_digit4(a1, b1, c1, fL1, fH1);

    c0 = _mm256_slli_epi64(c0, 40);
    c1 = _mm256_slli_epi64(c1, 40);

    if (ascii){
        const __m256i DECSHIFT = _mm256_set1_epi8('0');
        a0 = _mm256_add_epi8(a0, DECSHIFT);
        b0 = _mm256_add_epi8(b0, DECSHIFT);
        c0 = _mm256_add_epi8(c0, DECSHIFT);
        a1 = _mm256_add_epi8(a1, DECSHIFT);
        b1 = _mm256_add_epi8(b1, DECSHIFT);
        c1 = _mm256_add_epi8(c1, DECSHIFT);
    }

    if (forward){
        ymp_m256i_scatter4(c0, str + 11 + 19*0, str + 11 + 19*1, str + 11 + 19*2, str + 11 + 19*3);
        ymp_m256i_scatter4(a0, str +  0 + 19*0, str +  0 + 19*1, str +  0 + 19*2, str +  0 + 19*3);
        ymp_m256i_scatter4(b0, str +  8 + 19*0, str +  8 + 19*1, str +  8 + 19*2, str +  8 + 19*3);
        ymp_m256i_scatter4(c1, str + 11 + 19*4, str + 11 + 19*5, str + 11 + 19*6, str + 11 + 19*7);
        ymp_m256i_scatter4(a1, str +  0 + 19*4, str +  0 + 19*5, str +  0 + 19*6, str +  0 + 19*7);
        ymp_m256i_scatter4(b1, str +  8 + 19*4, str +  8 + 19*5, str +  8 + 19*6, str +  8 + 19*7);
    }else{
        ymp_m256i_scatter4(c0, str + 11 + 19*7, str + 11 + 19*6, str + 11 + 19*5, str + 11 + 19*4);
        ymp_m256i_scatter4(a0, str +  0 + 19*7, str +  0 + 19*6, str +  0 + 19*5, str +  0 + 19*4);
        ymp_m256i_scatter4(b0, str +  8 + 19*7, str +  8 + 19*6, str +  8 + 19*5, str +  8 + 19*4);
        ymp_m256i_scatter4(c1, str + 11 + 19*3, str + 11 + 19*2, str + 11 + 19*1, str + 11 + 19*0);
        ymp_m256i_scatter4(a1, str +  0 + 19*3, str +  0 + 19*2, str +  0 + 19*1, str +  0 + 19*0);
        ymp_m256i_scatter4(b1, str +  8 + 19*3, str +  8 + 19*2, str +  8 + 19*1, str +  8 + 19*0);
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
