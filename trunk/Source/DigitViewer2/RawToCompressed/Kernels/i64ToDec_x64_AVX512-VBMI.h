/* i64ToDec_x64_AVX512-VBMI.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 01/01/2024
 *  Last Modified   : 01/01/2024
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_i64ToDec_x64_AVX512VBMI_H
#define ydv_DigitViewer_i64ToDec_x64_AVX512VBMI_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/ArchSpecificLibs/x86_AVX512.h"
#include "DigitViewer2/RawToDecKernels/Kernels_dec_to_i64_x64_AVX512-BW.h"
#include "DigitViewer2/RawToDecKernels/Kernels_i64_to_dec_x64_AVX512-BW.h"
namespace DigitViewer2{
namespace RawToCompressed{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE bool dec_to_i64_u8_x64_AVX512VBMI(__m512i* T, const char* raw, upL_t blocks){
    //  This implementation using AVX512-VBMI eliminates the gather instruction
    //  that was present in the baseline AVX512 codepath.

    if (blocks == 0){
        return false;
    }

    const __m512i INDEX0 = _mm512_setr_epi8(
         0,  1,  2, -1, -1, -1, -1, -1,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
        19, 20, 21, -1, -1, -1, -1, -1, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37,
        38, 39, 40, -1, -1, -1, -1, -1, 41, 42, 43, 44, 45, 46, 47, 48
    );
    const __m512i INDEX1 = _mm512_setr_epi8(
                                                                        49, 50, 51, 52, 53, 54, 55, 56,
        57, 58, 59, -1, -1, -1, -1, -1, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75,
        76, 77, 78, -1, -1, -1, -1, -1, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94,
        95, 96, 97, -1, -1, -1, -1, -1
    );
    const __m512i INDEX2 = _mm512_setr_epi8(
                                        34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
        50, 51, 52, -1, -1, -1, -1, -1, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68,
        69, 70, 71, -1, -1, -1, -1, -1, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87
    );

    __mmask64 bad = 0;
    do{
        __m512i a0, b0, c0;
        __m512i r0, r1, r2;

        r0 = _mm512_loadu_si512(raw +   0);
        r1 = _mm512_loadu_si512(raw +  64);
        r2 = _mm512_maskz_loadu_epi64(0x07, raw + 128);

        c0 = _mm512_maskz_permutexvar_epi8(0xff07ffff07ffff07, INDEX0, r0);
        b0 = _mm512_maskz_permutex2var_epi8(0x07ffff07ffff07ff, r0, INDEX1, r1);
        a0 = _mm512_permutex2var_epi8(r1, INDEX2, r2);

        //  Transpose 3 x 8
        r0 = _mm512_permutex2var_epi64(c0, _mm512_setr_epi64( 0,  3,  6,  9, 12, 15,  0,  0), b0);
        r1 = _mm512_permutex2var_epi64(c0, _mm512_setr_epi64( 1,  4,  7, 10, 13,  0,  0,  0), b0);
        r2 = _mm512_permutex2var_epi64(c0, _mm512_setr_epi64( 2,  5,  8, 11, 14,  0,  0,  0), b0);
        c0 = _mm512_permutex2var_epi64(r0, _mm512_setr_epi64( 0,  1,  2,  3,  4,  5, 10, 13), a0);
        b0 = _mm512_permutex2var_epi64(r1, _mm512_setr_epi64( 0,  1,  2,  3,  4,  8, 11, 14), a0);
        a0 = _mm512_permutex2var_epi64(r2, _mm512_setr_epi64( 0,  1,  2,  3,  4,  9, 12, 15), a0);

//        c0 = _mm512_and_si512(c0, _mm512_set1_epi64(0x0000000000ffffff));

        __m512i t0;
        bad |= RawToDec::dec_to_i64_x64_AVX512BW(t0, a0, b0, c0);

        _mm512_storeu_si512(T, t0);

        raw += 152;
        T += 1;
    }while (--blocks);

    return bad != 0;
}
YM_FORCE_INLINE void i64_to_dec_u8_x64_AVX512VBMI(char* raw, const __m512i* T, upL_t blocks){
    //  This implementation using AVX512-VBMI eliminates the scatter instruction
    //  that was present in the baseline AVX512 codepath.

    if (blocks == 0){
        return;
    }

    const __m512i INDEX0 = _mm512_setr_epi8(
         0,  1,  2,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
        24, 25, 26, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
        48, 49, 50, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,
        72, 73, 74, 80, 81, 82, 83
    );
    const __m512i INDEX1 = _mm512_setr_epi8(
                                    20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
        32, 33, 34, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55,
        56, 57, 58, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
        80, 81, 82, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98
    );
    const __m512i INDEX2 = _mm512_setr_epi8(
                                                                35, 36, 37, 38, 39,
        40, 41, 42, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
         0,  0
    );

    do{
        __m512i a0, b0, c0;
        __m512i r0, r1, r2;

        RawToDec::i64_to_dec_x64_AVX512BW(_mm512_loadu_si512(T), a0, b0, c0);

        //  Transpose 3 x 8
        r0 = _mm512_permutex2var_epi64(c0, _mm512_setr_epi64( 0,  8,  0,  1,  9,  0,  2, 10), b0);
        r1 = _mm512_permutex2var_epi64(c0, _mm512_setr_epi64( 0,  3, 11,  0,  4, 12,  0,  5), b0);
        r2 = _mm512_permutex2var_epi64(c0, _mm512_setr_epi64(13,  0,  6, 14,  0,  7, 15,  0), b0);
        c0 = _mm512_permutex2var_epi64(r0, _mm512_setr_epi64( 0,  1,  8,  3,  4,  9,  6,  7), a0);
        b0 = _mm512_permutex2var_epi64(r1, _mm512_setr_epi64(10,  1,  2, 11,  4,  5, 12,  7), a0);
        a0 = _mm512_permutex2var_epi64(r2, _mm512_setr_epi64( 0, 13,  2,  3, 14,  5,  6, 15), a0);

        _mm512_storeu_si512(raw +   0, _mm512_permutex2var_epi8(c0, INDEX0, b0));
        _mm512_storeu_si512(raw +  64, _mm512_permutex2var_epi8(b0, INDEX1, a0));
        _mm256_mask_storeu_epi64(raw + 128, 0x07, _mm512_castsi512_si256(_mm512_permutexvar_epi8(INDEX2, a0)));

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
