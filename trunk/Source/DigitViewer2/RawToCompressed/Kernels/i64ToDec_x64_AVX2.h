/* i64ToDec_x64_AVX2.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 02/11/2018
 *  Last Modified   : 02/11/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_i64ToDec_x64_AVX2_H
#define ydv_DigitViewer_i64ToDec_x64_AVX2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/ArchSpecificLibs/AdjacentLanePermute/AdjacentLanePermute_256_x86_AVX2.h"
#include "DigitViewer2/RawToDecKernels/Kernels_dec_to_i64_x64_AVX2.h"
#include "DigitViewer2/RawToDecKernels/Kernels_i64_to_dec_x64_AVX2.h"
namespace DigitViewer2{
namespace RawToCompressed{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE bool dec_to_i64_u4_x64_AVX2(__m256i* T, const char* raw, upL_t blocks){
    if (blocks == 0){
        return false;
    }

    __m256i bad = _mm256_setzero_si256();
    do{
        __m256i a0, b0, c0;

        {
            __m128i x0, x1;
            SIMD::splitload(x0, raw +  0, raw + 19);
            SIMD::splitload(x1, raw + 38, raw + 57);
            c0 = _mm256_setr_m128i(x0, x1);
        }

        __m256i r0;
        SIMD::splitload(r0, raw +  3, raw + 41);
        SIMD::splitload(a0, raw + 22, raw + 60);

        b0 = _mm256_unpacklo_epi64(r0, a0);
        a0 = _mm256_unpackhi_epi64(r0, a0);

        c0 = _mm256_and_si256(c0, _mm256_set1_epi64x(0x0000000000ffffff));

        __m256i t0;
        __m256i ret = RawToDec::dec_to_i64_x64_AVX2(t0, a0, b0, c0);

        bad = _mm256_or_si256(bad, ret);
        _mm256_storeu_si256(T, t0);

        raw += 76;
        T += 1;
    }while (--blocks);

    return _mm256_movemask_epi8(bad) != 0;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void i64_to_dec_u4_x64_AVX2(char* raw, const __m256i* T, upL_t blocks){
    if (blocks == 0){
        return;
    }

    do{
        __m256i a0, b0, c0;
        RawToDec::i64_to_dec_x64_AVX2(_mm256_loadu_si256(T), a0, b0, c0);

        SIMD::splitstore(_mm256_castsi256_si128(c0), raw +  0, raw + 19);
        SIMD::splitstore(_mm256_extracti128_si256(c0, 1), raw + 38, raw + 57);

        __m256i y0 = _mm256_unpacklo_epi64(b0, a0);
        __m256i y1 = _mm256_unpackhi_epi64(b0, a0);
        SIMD::splitstore(y0, raw +  3, raw + 41);
        SIMD::splitstore(y1, raw + 22, raw + 60);

        raw += 76;
        T += 1;
    }while (--blocks);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
