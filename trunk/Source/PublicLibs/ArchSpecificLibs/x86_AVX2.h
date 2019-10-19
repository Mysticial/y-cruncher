/* x86_AVX2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/13/2018
 * Last Modified    : 02/13/2018
 * 
 */

#pragma once
#ifndef ymp_ArchSpecific_x86_AVX2_H
#define ymp_ArchSpecific_x86_AVX2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <immintrin.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  COMPILER-BUG-GCC: Missing AVX2 intrinsics
#if defined __GNUC__ && __GNUC__ < 8
YM_FORCE_INLINE __m256i _mm256_setr_m128i(__m128i L, __m128i H){
    return _mm256_inserti128_si256(_mm256_castsi128_si256(L), (H), 1);
}
#endif
#ifdef __GNUC__
YM_FORCE_INLINE __m256i _mm256_loadu2_m128i(const __m128i* H, const __m128i* L){
    return _mm256_inserti128_si256(
        _mm256_castsi128_si256(_mm_loadu_si128(L)),
        _mm_loadu_si128(H),
        1
    );
}
YM_FORCE_INLINE void _mm256_storeu2_m128i(__m128i* H, __m128i* L, __m256i x){
    _mm_storeu_si128(L, _mm256_castsi256_si128(x));
    _mm_storeu_si128(H, _mm256_extracti128_si256(x, 1));
}
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE __m256i _mm256_cvtsi64_si256(u64_t x){
#ifndef __INTEL_COMPILER
    __m256i r0 = _mm256_castsi128_si256(_mm_cvtsi64_si128(x));
    return _mm256_blend_epi32(r0, _mm256_setzero_si256(), 0xfc);
#else
    return _mm256_zextsi128_si256(_mm_cvtsi64_si128(x));
#endif
}
YM_FORCE_INLINE __m256i _mm256_setl_epi64(u64_t x){
#ifndef __INTEL_COMPILER
    __m256i r0 = _mm256_castsi128_si256(_mm_cvtsi64_si128(x));
    return _mm256_blend_epi32(r0, _mm256_setzero_si256(), 0xfc);
#else
    return _mm256_zextsi128_si256(_mm_cvtsi64_si128(x));
#endif
}
YM_FORCE_INLINE __m256i _mm256_loadl_epi64(const void* ptr){
#ifndef __INTEL_COMPILER
    __m256i r0 = _mm256_castsi128_si256(_mm_loadl_epi64((const __m128i*)ptr));
    return _mm256_blend_epi32(r0, _mm256_setzero_si256(), 0xfc);
#else
    return _mm256_zextsi128_si256(_mm_loadl_epi64((const __m128i*)ptr));
#endif
}
YM_FORCE_INLINE ymp::s64_t _mm256_reduce_or_epi64(__m256i y){
    __m128i x = _mm_or_si128(
        _mm256_castsi256_si128(y),
        _mm256_extracti128_si256(y, 1)
    );
    x = _mm_or_si128(x, _mm_unpackhi_epi64(x, x));
    return _mm_cvtsi128_si64(x);
}
YM_FORCE_INLINE ymp::s64_t _mm256_reduce_add_epi64(__m256i y){
    __m128i x = _mm_add_epi64(
        _mm256_castsi256_si128(y),
        _mm256_extracti128_si256(y, 1)
    );
    x = _mm_add_epi64(x, _mm_unpackhi_epi64(x, x));
    return _mm_cvtsi128_si64(x);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
