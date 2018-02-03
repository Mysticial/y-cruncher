/* u64d19_forward_intrinsics_AVX512.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/24/2016
 * Last Modified    : 02/24/2016
 * 
 *      Just for fun. I have no idea how fast this will be. But it is double
 *  the width of the AVX2 version done with fewer instructions.
 * 
 *  It's too early to tell, but this is also likely to be latency bound with
 *  2x unrolling. But at least 32 registers is enough to unroll 4x.
 * 
 */

#ifndef ydv_u64d19_forward_intrinsics_AVX512_H
#define ydv_u64d19_forward_intrinsics_AVX512_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <immintrin.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "PublicLibs/SystemLibs/ProcessorCapability/ProcessorCapability.h"
namespace DigitViewer{
namespace u64d19{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Stage 1: Split into 10-digit words. Store as double.
YM_FORCE_INLINE __m512d split10_get_high(__m512i x){
    //  Compute x / 10^10. The answer may overshoot by 1.

#ifdef YMP_Arch_2017_x64_Skylake
    const __m512d SCALE = _mm512_set1_pd(0.0000000001);
    const __m512d CONVERT = _mm512_set1_pd(4503599627370496.);

    __m512d f1 = _mm512_cvtepu64_pd(x);

    //  Scale by 10^-10. Round to integer.
    f1 = _mm512_fmadd_pd(f1, SCALE, CONVERT);
    f1 = _mm512_sub_pd(f1, CONVERT);

#else
    const __m512d SCALE = _mm512_set1_pd(0.0000004096);
    const __m512d CONVERT = _mm512_set1_pd(4503599627370496.);

    __m512d f1;

    //  Shift down. We don't have enough precision to directly convert the
    //  whole thing.
    x = _mm512_srli_epi64(x, 12);

    //  Convert to floating-point.
    f1 = _mm512_castsi512_pd(_mm512_or_si512(x, _mm512_castpd_si512(CONVERT)));
    f1 = _mm512_sub_pd(f1, CONVERT);

    //  Scale by 4096 * 10^-10. (This also counters the previous right-shift by 12 bits.)
    //  Round to integer.
    f1 = _mm512_fmadd_pd(f1, SCALE, CONVERT);
    f1 = _mm512_sub_pd(f1, CONVERT);

#endif

    //  At this point, "f1" holds the top 10 digits x.
    //  Note that this result may not be exact. (It may be off by 1.)

    return f1;
}
YM_FORCE_INLINE void split10_split_words(__m512d& L, __m512d& H, __m512i x){
    //  Split x into upper and lower 32-bits. Return them as floating-point.

    __m512i y;
//    __m512d r;

    //  Split x into upper and lower 32-bits.
    //  x = {0L, 0H, 1L, 1H, 2L, 2H, 3L, 3H, 4L, 4H, 5L, 5H, 6L, 6H, 7L, 7H}
    //  L = {0L, 1L, 2L, 3L, 4L, 5L, 6L, 7L}
    //  H = {0H, 1H, 2H, 3H, 4H, 5H, 6H, 7H}
    y = _mm512_maskz_shuffle_epi32(0x5555, x, _MM_PERM_CDAB);
    x = _mm512_maskz_mov_epi32(0x5555, x);

#ifdef YMP_Arch_2017_x64_Skylake
    H = _mm512_cvtepu64_pd(y);
    L = _mm512_cvtepu64_pd(x);
#else
    const __m512d CONVERT = _mm512_set1_pd(4503599627370496.);
    H = _mm512_castsi512_pd(_mm512_or_si512(y, _mm512_castpd_si512(CONVERT)));
    L = _mm512_castsi512_pd(_mm512_or_si512(x, _mm512_castpd_si512(CONVERT)));
    H = _mm512_sub_pd(H, CONVERT);
    L = _mm512_sub_pd(L, CONVERT);
#endif

    H = _mm512_mul_pd(H, _mm512_set1_pd(4294967296.));
}
YM_FORCE_INLINE void split10_finish(__m512d& L, __m512d& H, __m512d wL, __m512d wH){
    const __m512d ONE = _mm512_set1_pd(1.);
    const __m512d POW = _mm512_set1_pd(10000000000.);

    //  Extract the bottom 10 digits. (Negative implies a correction is needed.)
    wH = _mm512_fnmadd_pd(H, POW, wH);
    L = _mm512_add_pd(wL, wH);

    //  Make correction.
    __mmask8 mask = _mm512_cmp_pd_mask(L, _mm512_setzero_pd(), _MM_CMPINT_LT);
    L = _mm512_mask_add_pd(L, mask, L, POW);
    H = _mm512_mask_sub_pd(H, mask, H, ONE);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Stage 2: Split into 5-digit words. Store as float.
YM_FORCE_INLINE void split5_to_floats(__m512& L, __m512& H, __m512d a, __m512d b){
    //  Split a and b into upper and lower 5 decimal digits.
    //  The results may not be exact. The upper word may overshoot by 1 and the
    //  lower word may be short by 10^5.

    //  Return:
    //      L = {a % 10^5}{b % 10^5}
    //      H = {a / 10^5}{b / 10^5}

    const __m512d POW = _mm512_set1_pd(100000.);
    const __m512d SCALE = _mm512_set1_pd(0.00001);
    const __m512d CONVERT = _mm512_set1_pd(4503599627370496.);

    __m512d a0, a1;
    __m512d b0, b1;

    //  Split A
    a1 = _mm512_fmadd_pd(a, SCALE, CONVERT);
    a1 = _mm512_sub_pd(a1, CONVERT);
    a0 = _mm512_fnmadd_pd(a1, POW, a);

    //  Split B
    b1 = _mm512_fmadd_pd(b, SCALE, CONVERT);
    b1 = _mm512_sub_pd(b1, CONVERT);
    b0 = _mm512_fnmadd_pd(b1, POW, b);

    //  Convert to floats
    {
        __m256 a0f = _mm512_cvtpd_ps(a0);
        __m256 b0f = _mm512_cvtpd_ps(b0);
        __m256 a1f = _mm512_cvtpd_ps(a1);
        __m256 b1f = _mm512_cvtpd_ps(b1);
        L = _mm512_castpd_ps(_mm512_insertf64x4(
            _mm512_castps_pd(_mm512_castps256_ps512(a0f)),
            _mm256_castps_pd(b0f),
            1
        ));
        H = _mm512_castpd_ps(_mm512_insertf64x4(
            _mm512_castps_pd(_mm512_castps256_ps512(a1f)),
            _mm256_castps_pd(b1f),
            1
        ));
    }
}
YM_FORCE_INLINE void split5_finish(__m512& H, __m512& L, __m512 fL, __m512 fH){
    const __m512 ONE = _mm512_set1_ps(1.f);
    const __m512 POW = _mm512_set1_ps(100000.f);

    //  Make correction
    __mmask16 mask = _mm512_cmp_ps_mask(fL, _mm512_setzero_ps(), _MM_CMPINT_LT);
    fL = _mm512_mask_add_ps(fL, mask, fL, POW);
    fH = _mm512_mask_sub_ps(fH, mask, fH, ONE);

    //  Shuffle and flip the endian.
    L = _mm512_permutex2var_ps(fH, _mm512_setr_epi32(  0, 16,  1, 17,  2, 18,  3, 19,  4, 20,  5, 21,  6, 22,  7, 23), fL);
    H = _mm512_permutex2var_ps(fH, _mm512_setr_epi32(  8, 24,  9, 25, 10, 26, 11, 27, 12, 28, 13, 29, 14, 30, 15, 31), fL);

    //  L = {fH[ 0], fL[0], fH[ 1], fL[ 1], fH[ 2], fL[ 2], fH[ 3], fL[ 3], fH[ 4], fL[ 4], fH[ 5], fL[ 5], fH[ 6], fL[ 6], fH[ 7], fL[ 7]}
    //  H = {fH[ 8], fL[8], fH[ 9], fL[ 9], fH[10], fL[10], fH[11], fL[11], fH[12], fL[12], fH[13], fL[13], fH[14], fL[14], fH[15], fL[15]}
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Stage 3: Convert 5-digit words. Place into buffers.
YM_FORCE_INLINE void extract_digit(__m512i& i0, __m512i& i1, __m512& f0, __m512& f1, __m512 SCALE){
    const __m512 SHIFT = _mm512_setr_ps(1, 256, 1, 256, 1, 256, 1, 256, 1, 256, 1, 256, 1, 256, 1, 256);

    //  Additive perturbance to compensate for round-off error.
    //  Valid Range: (0.0000007, 0.00008f)
    const __m512 ADD = _mm512_set1_ps(0.000007f);

    __m512 r0, r1;

    //  Scale up and remove integer part.
    f0 = _mm512_fmadd_ps(f0, SCALE, ADD);
    f1 = _mm512_fmadd_ps(f1, SCALE, ADD);
    r0 = _mm512_roundscale_ps(f0, _MM_FROUND_TO_ZERO);
    r1 = _mm512_roundscale_ps(f1, _MM_FROUND_TO_ZERO);
    f0 = _mm512_sub_ps(f0, r0);
    f1 = _mm512_sub_ps(f1, r1);

    //  Shift integer part and convert to integer.
    r0 = _mm512_mul_ps(r0, SHIFT);
    r1 = _mm512_mul_ps(r1, SHIFT);
    i0 = _mm512_cvttps_epi32(r0);
    i1 = _mm512_cvttps_epi32(r1);
}
YM_FORCE_INLINE void extract_digit0(__m512i& s0, __m512i& s1, __m512i& s2, __m512& f0, __m512& f1){
    __m512i i0, i1;
    extract_digit(i0, i1, f0, f1, _mm512_set1_ps(0.0001f));
    s0 = _mm512_srli_epi64(i0, 8);
    s1 = _mm512_slli_epi64(i1, 8);
}
YM_FORCE_INLINE void extract_digit1(__m512i& s0, __m512i& s1, __m512i& s2, __m512& f0, __m512& f1){
    __m512i i0, i1;
    extract_digit(i0, i1, f0, f1, _mm512_set1_ps(10.f));
    s0 = _mm512_or_si512(s0, i0);
    s1 = _mm512_or_si512(s1, _mm512_slli_epi64(i1, 16));
}
YM_FORCE_INLINE void extract_digit2(__m512i& s0, __m512i& s1, __m512i& s2, __m512& f0, __m512& f1){
    __m512i i0, i1;
    extract_digit(i0, i1, f0, f1, _mm512_set1_ps(10.f));
    s0 = _mm512_or_si512(s0, _mm512_slli_epi64(i0, 8));
    s1 = _mm512_or_si512(s1, _mm512_slli_epi64(i1, 24));
    s2 = _mm512_srli_epi64(i1, 40);
}
YM_FORCE_INLINE void extract_digit3(__m512i& s0, __m512i& s1, __m512i& s2, __m512& f0, __m512& f1){
    __m512i i0, i1;
    extract_digit(i0, i1, f0, f1, _mm512_set1_ps(10.f));
    s0 = _mm512_or_si512(s0, _mm512_slli_epi64(i0, 16));
    s1 = _mm512_or_si512(s1, _mm512_slli_epi64(i1, 32));
    s2 = _mm512_or_si512(s2, _mm512_srli_epi64(i1, 32));
}
YM_FORCE_INLINE void extract_digit4(__m512i& s0, __m512i& s1, __m512i& s2, __m512& f0, __m512& f1){
    __m512i i0, i1;
    extract_digit(i0, i1, f0, f1, _mm512_set1_ps(10.f));
    s0 = _mm512_or_si512(s0, _mm512_slli_epi64(i0, 24));
    s1 = _mm512_or_si512(s1, _mm512_srli_epi64(i0, 40));
    s1 = _mm512_or_si512(s1, _mm512_slli_epi64(i1, 40));
    s2 = _mm512_or_si512(s2, _mm512_srli_epi64(i1, 24));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Wrappers
template <bool forward, bool ascii> YM_FORCE_INLINE
void convert_d19x8_forward_AVX512(char str[19*8], const u64_t dec[8]){
    __m512 fL0, fH0;

    {
        __m512d a0, b0, c0, d0;
        __m512i x0 = _mm512_loadu_si512((const __m512i*)dec);
        b0 = split10_get_high(x0);
        split10_split_words(c0, d0, x0);
        split10_finish(a0, b0, c0, d0);
        split5_to_floats(fL0, fH0, a0, b0);
        split5_finish(fL0, fH0, fL0, fH0);
    }

    __m512i s0, s1, s2;
    extract_digit0(s0, s1, s2, fL0, fH0);
    extract_digit1(s0, s1, s2, fL0, fH0);
    extract_digit2(s0, s1, s2, fL0, fH0);
    extract_digit3(s0, s1, s2, fL0, fH0);
    extract_digit4(s0, s1, s2, fL0, fH0);
    s2 = _mm512_slli_epi64(s2, 40);

    if (ascii){
        const __m512i DECSHIFT = _mm512_set1_epi8('0');
#ifdef YMP_Arch_2017_x64_Skylake
        s0 = _mm512_add_epi8(s0, DECSHIFT);
        s1 = _mm512_add_epi8(s1, DECSHIFT);
        s2 = _mm512_add_epi8(s2, DECSHIFT);
#else
        s0 = _mm512_add_epi32(s0, DECSHIFT);
        s1 = _mm512_add_epi32(s1, DECSHIFT);
        s2 = _mm512_add_epi32(s2, DECSHIFT);
#endif
    }

    //  COMPILER-BUG-GCC: AVX512 Pointer type
#ifndef __GNUC__
    if (forward){
        const __m512i INDEX = _mm512_setr_epi64(19*0, 19*1, 19*2, 19*3, 19*4, 19*5, 19*6, 19*7);
        _mm512_i64scatter_epi64(str + 11, INDEX, s2, 1);
        _mm512_i64scatter_epi64(str +  0, INDEX, s0, 1);
        _mm512_i64scatter_epi64(str +  8, INDEX, s1, 1);
    }else{
        const __m512i INDEX = _mm512_setr_epi64(19*7, 19*6, 19*5, 19*4, 19*3, 19*2, 19*1, 19*0);
        _mm512_i64scatter_epi64(str + 11, INDEX, s2, 1);
        _mm512_i64scatter_epi64(str +  0, INDEX, s0, 1);
        _mm512_i64scatter_epi64(str +  8, INDEX, s1, 1);
    }
#else
    if (forward){
        const __m512i INDEX = _mm512_setr_epi64(19*0, 19*1, 19*2, 19*3, 19*4, 19*5, 19*6, 19*7);
        _mm512_i64scatter_epi64((long long*)(str + 11), INDEX, s2, 1);
        _mm512_i64scatter_epi64((long long*)(str +  0), INDEX, s0, 1);
        _mm512_i64scatter_epi64((long long*)(str +  8), INDEX, s1, 1);
    }else{
        const __m512i INDEX = _mm512_setr_epi64(19*7, 19*6, 19*5, 19*4, 19*3, 19*2, 19*1, 19*0);
        _mm512_i64scatter_epi64((long long*)(str + 11), INDEX, s2, 1);
        _mm512_i64scatter_epi64((long long*)(str +  0), INDEX, s0, 1);
        _mm512_i64scatter_epi64((long long*)(str +  8), INDEX, s1, 1);
    }
#endif
}
template <bool forward, bool ascii> YM_FORCE_INLINE
void convert_d19x16_forward_AVX512(char str[19*16], const u64_t dec[16]){
    __m512 fL0, fH0;
    __m512 fL1, fH1;

    {
        __m512d a0, b0, c0, d0;
        __m512d a1, b1, c1, d1;

        __m512i x0 = _mm512_loadu_si512((const __m512i*)dec + 0);
        __m512i x1 = _mm512_loadu_si512((const __m512i*)dec + 1);

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

    __m512i a0, b0, c0;
    __m512i a1, b1, c1;

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

    c0 = _mm512_slli_epi64(c0, 40);
    c1 = _mm512_slli_epi64(c1, 40);

    if (ascii){
        const __m512i DECSHIFT = _mm512_set1_epi8('0');
#ifdef YMP_Arch_2017_x64_Skylake
        a0 = _mm512_add_epi8(a0, DECSHIFT);
        b0 = _mm512_add_epi8(b0, DECSHIFT);
        c0 = _mm512_add_epi8(c0, DECSHIFT);
        a1 = _mm512_add_epi8(a1, DECSHIFT);
        b1 = _mm512_add_epi8(b1, DECSHIFT);
        c1 = _mm512_add_epi8(c1, DECSHIFT);
#else
        a0 = _mm512_add_epi32(a0, DECSHIFT);
        b0 = _mm512_add_epi32(b0, DECSHIFT);
        c0 = _mm512_add_epi32(c0, DECSHIFT);
        a1 = _mm512_add_epi32(a1, DECSHIFT);
        b1 = _mm512_add_epi32(b1, DECSHIFT);
        c1 = _mm512_add_epi32(c1, DECSHIFT);
#endif
    }

    //  COMPILER-BUG-GCC: AVX512 Pointer type
#ifndef __GNUC__
    if (forward){
        const __m512i INDEX = _mm512_setr_epi64(19*0, 19*1, 19*2, 19*3, 19*4, 19*5, 19*6, 19*7);
        _mm512_i64scatter_epi64(str +  11, INDEX, c0, 1);
        _mm512_i64scatter_epi64(str +   0, INDEX, a0, 1);
        _mm512_i64scatter_epi64(str +   8, INDEX, b0, 1);
        _mm512_i64scatter_epi64(str + 163, INDEX, c1, 1);
        _mm512_i64scatter_epi64(str + 152, INDEX, a1, 1);
        _mm512_i64scatter_epi64(str + 160, INDEX, b1, 1);
    }else{
        const __m512i INDEX = _mm512_setr_epi64(19*7, 19*6, 19*5, 19*4, 19*3, 19*2, 19*1, 19*0);
        _mm512_i64scatter_epi64(str + 163, INDEX, c0, 1);
        _mm512_i64scatter_epi64(str + 152, INDEX, a0, 1);
        _mm512_i64scatter_epi64(str + 160, INDEX, b0, 1);
        _mm512_i64scatter_epi64(str +  11, INDEX, c1, 1);
        _mm512_i64scatter_epi64(str +   0, INDEX, a1, 1);
        _mm512_i64scatter_epi64(str +   8, INDEX, b1, 1);
    }
#else
    if (forward){
        const __m512i INDEX = _mm512_setr_epi64(19*0, 19*1, 19*2, 19*3, 19*4, 19*5, 19*6, 19*7);
        _mm512_i64scatter_epi64((long long*)(str +  11), INDEX, c0, 1);
        _mm512_i64scatter_epi64((long long*)(str +   0), INDEX, a0, 1);
        _mm512_i64scatter_epi64((long long*)(str +   8), INDEX, b0, 1);
        _mm512_i64scatter_epi64((long long*)(str + 163), INDEX, c1, 1);
        _mm512_i64scatter_epi64((long long*)(str + 152), INDEX, a1, 1);
        _mm512_i64scatter_epi64((long long*)(str + 160), INDEX, b1, 1);
    }else{
        const __m512i INDEX = _mm512_setr_epi64(19*7, 19*6, 19*5, 19*4, 19*3, 19*2, 19*1, 19*0);
        _mm512_i64scatter_epi64((long long*)(str + 163), INDEX, c0, 1);
        _mm512_i64scatter_epi64((long long*)(str + 152), INDEX, a0, 1);
        _mm512_i64scatter_epi64((long long*)(str + 160), INDEX, b0, 1);
        _mm512_i64scatter_epi64((long long*)(str +  11), INDEX, c1, 1);
        _mm512_i64scatter_epi64((long long*)(str +   0), INDEX, a1, 1);
        _mm512_i64scatter_epi64((long long*)(str +   8), INDEX, b1, 1);
    }
#endif
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
