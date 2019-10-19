/* i64ToDec_x64_SSE41.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/11/2018
 * Last Modified    : 02/11/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_i64ToDec_x64_SSE41_H
#define ydv_DigitViewer_i64ToDec_x64_SSE41_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/ArchSpecificLibs/Shuffle/x86_128/AdjacentLanePermute_x86_128.h"
#include "DigitViewer2/RawToDecKernels/Kernels_dec_to_i64_x64_SSE41.h"
#include "DigitViewer2/RawToDecKernels/Kernels_i64_to_dec_x64_SSE41.h"
namespace DigitViewer2{
namespace RawToCompressed{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE bool dec_to_i64_u2_x64_SSE41(__m128i* T, const char* raw, upL_t blocks){
    if (blocks == 0){
        return false;
    }

    __m128i bad = _mm_setzero_si128();
    do{
        __m128i c0 = SIMD::mm_splitload_si128(raw +  0, raw + 19);
        __m128i b0 = SIMD::mm_splitload_si128(raw +  3, raw + 22);
        __m128i a0 = SIMD::mm_splitload_si128(raw + 11, raw + 30);
        c0 = _mm_and_si128(c0, _mm_set1_epi64x(0x0000000000ffffff));

        __m128i t0;
        __m128i ret = RawToDec::dec_to_i64_x64_SSE41(t0, a0, b0, c0);

        bad = _mm_or_si128(bad, ret);
        _mm_storeu_si128(T, t0);

        raw += 38;
        T += 1;
    }while (--blocks);

    return _mm_movemask_epi8(bad) != 0;
}
YM_FORCE_INLINE bool dec_to_i64_u4_x64_SSE41(__m128i* T, const char* raw, upL_t blocks){
    if (blocks == 0){
        return false;
    }

    __m128i bad = _mm_setzero_si128();
    do{
        __m128i c0 = SIMD::mm_splitload_si128(raw +  0, raw + 19);
        __m128i b0 = SIMD::mm_splitload_si128(raw +  3, raw + 22);
        __m128i a0 = SIMD::mm_splitload_si128(raw + 11, raw + 30);
        __m128i c1 = SIMD::mm_splitload_si128(raw + 38, raw + 57);
        __m128i b1 = SIMD::mm_splitload_si128(raw + 41, raw + 60);
        __m128i a1 = SIMD::mm_splitload_si128(raw + 49, raw + 68);

        c0 = _mm_and_si128(c0, _mm_set1_epi64x(0x0000000000ffffff));
        c1 = _mm_and_si128(c1, _mm_set1_epi64x(0x0000000000ffffff));

        __m128i t0, t1;
        __m128i ret = RawToDec::dec_to_i64_x64_SSE41(
            t0, t1,
            a0, b0, c0,
            a1, b1, c1
        );

        bad = _mm_or_si128(bad, ret);
        _mm_storeu_si128(T + 0, t0);
        _mm_storeu_si128(T + 1, t1);

        raw += 76;
        T += 2;
    }while (--blocks);

    return _mm_movemask_epi8(bad) != 0;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void i64_to_dec_u2_x64_SSE41(char* raw, const __m128i* T, upL_t blocks){
    if (blocks == 0){
        return;
    }

    do{
        __m128i a0, b0, c0;
        RawToDec::i64_to_dec_x64_SSE41(_mm_loadu_si128(T), a0, b0, c0);

        SIMD::mm_splitstore_si128(raw +  0, raw + 19, c0);
        SIMD::mm_splitstore_si128(raw +  3, raw + 22, b0);
        SIMD::mm_splitstore_si128(raw + 11, raw + 30, a0);

        raw += 38;
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
