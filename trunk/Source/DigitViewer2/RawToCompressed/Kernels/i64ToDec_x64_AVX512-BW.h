/* i64ToDec_x64_AVX512-BW.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/11/2018
 * Last Modified    : 02/11/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_i64ToDec_x64_AVX512BW_H
#define ydv_DigitViewer_i64ToDec_x64_AVX512BW_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/ArchSpecificLibs/Shuffle/x86_256/AdjacentLanePermute_x86_256.h"
#include "DigitViewer2/RawToDecKernels/Kernels_dec_to_i64_x64_AVX512-BW.h"
#include "DigitViewer2/RawToDecKernels/Kernels_i64_to_dec_x64_AVX512-BW.h"
namespace DigitViewer2{
namespace RawToCompressed{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE bool dec_to_i64_u8_x64_AVX512BW(__m512i* T, const char* raw, upL_t blocks){
    if (blocks == 0){
        return false;
    }

    const __m512i GATHER = _mm512_setr_epi64(0*19, 1*19, 2*19, 3*19, 4*19, 5*19, 6*19, 7*19);

    __mmask64 bad = 0;
    do{
        __m512i a0, b0, c0;

        c0 = _mm512_i64gather_epi64(GATHER, raw +  0, 1);
#if 0
        b0 = _mm512_i64gather_epi64(GATHER, raw +  3, 1);
        a0 = _mm512_i64gather_epi64(GATHER, raw + 11, 1);
#else
        {
            __m256i r0 = SIMD::mm256_splitload_si256(raw +  3, raw +  41);
            __m256i r1 = SIMD::mm256_splitload_si256(raw + 22, raw +  60);
            __m256i r2 = SIMD::mm256_splitload_si256(raw + 79, raw + 117);
            __m256i r3 = SIMD::mm256_splitload_si256(raw + 98, raw + 136);

            __m512i z0 = _mm512_inserti64x4(_mm512_castsi256_si512(r0), r2, 1);
            __m512i z1 = _mm512_inserti64x4(_mm512_castsi256_si512(r1), r3, 1);
            b0 = _mm512_unpacklo_epi64(z0, z1);
            a0 = _mm512_unpackhi_epi64(z0, z1);
        }
#endif

        c0 = _mm512_and_si512(c0, _mm512_set1_epi64(0x0000000000ffffff));

        __m512i t0;
        bad |= RawToDec::dec_to_i64_x64_AVX512BW(t0, a0, b0, c0);

        _mm512_storeu_si512(T, t0);

        raw += 152;
        T += 1;
    }while (--blocks);

    return bad != 0;
}
YM_FORCE_INLINE void i64_to_dec_u8_x64_AVX512BW(char* raw, const __m512i* T, upL_t blocks){
    if (blocks == 0){
        return;
    }

    const __m512i GATHER = _mm512_setr_epi64(0*19, 1*19, 2*19, 3*19, 4*19, 5*19, 6*19, 7*19);

    do{
        __m512i a0, b0, c0;
        RawToDec::i64_to_dec_x64_AVX512BW(_mm512_loadu_si512(T), a0, b0, c0);

        _mm512_i64scatter_epi64(raw +  0, GATHER, c0, 1);
#if 0
        _mm512_i64scatter_epi64(raw +  3, GATHER, b0, 1);
        _mm512_i64scatter_epi64(raw + 11, GATHER, a0, 1);
#else
        __m512i z0 = _mm512_unpacklo_epi64(b0, a0);
        __m512i z1 = _mm512_unpackhi_epi64(b0, a0);

        SIMD::mm256_splitstore_si256(raw +  3, raw +  41, _mm512_castsi512_si256(z0));
        SIMD::mm256_splitstore_si256(raw + 22, raw +  60, _mm512_castsi512_si256(z1));
        SIMD::mm256_splitstore_si256(raw + 79, raw + 117, _mm512_extracti64x4_epi64(z0, 1));
        SIMD::mm256_splitstore_si256(raw + 98, raw + 136, _mm512_extracti64x4_epi64(z1, 1));
#endif

        raw += 152;
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
