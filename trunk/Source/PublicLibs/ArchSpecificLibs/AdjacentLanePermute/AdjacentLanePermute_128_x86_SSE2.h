/* AdjacentLanePermute_128_x86_SSE2.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 08/30/2015
 *  Last Modified   : 03/08/2024
 * 
 *      New interface for Adjacent Lane Permutes.
 * 
 */

#pragma once
#ifndef ymp_AdjacentLanePermute_128_x86_SSE2_H
#define ymp_AdjacentLanePermute_128_x86_SSE2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <immintrin.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "PublicLibs/SystemLibs/ProcessorCapability/ProcessorCapability.h"
namespace ymp{
namespace SIMD{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Integer
template <int bit_granularity> YM_FORCE_INLINE __m128i permuteaj_00(__m128i a, __m128i b);
template <int bit_granularity> YM_FORCE_INLINE __m128i permuteaj_11(__m128i a, __m128i b);
template <int bit_granularity> YM_FORCE_INLINE __m128i permuteaj_01(__m128i a, __m128i b);
template <int bit_granularity> YM_FORCE_INLINE __m128i permuteaj_10(__m128i a, __m128i b);
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m128i permuteaj_00<16>(__m128i a, __m128i b){
#ifdef YMP_Arch_2007_x64_Penryn
    b = _mm_shuffle_epi8(b, _mm_setr_epi8(
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13
    ));
    return _mm_blend_epi16(a, b, 0xaa);
#else
    a = _mm_and_si128(a, _mm_setr_epi16(-1, 0, -1, 0, -1, 0, -1, 0));
    b = _mm_slli_epi32(b, 16);
    return _mm_or_si128(a, b);
#endif
}
template <> YM_FORCE_INLINE
__m128i permuteaj_11<16>(__m128i a, __m128i b){
#ifdef YMP_Arch_2007_x64_Penryn
    a = _mm_shuffle_epi8(a, _mm_setr_epi8(
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13
    ));
    return _mm_blend_epi16(a, b, 0xaa);
#else
    a = _mm_srli_epi32(a, 16);
    b = _mm_andnot_si128(_mm_setr_epi16(-1, 0, -1, 0, -1, 0, -1, 0), b);
    return _mm_or_si128(a, b);
#endif
}
template <> YM_FORCE_INLINE
__m128i permuteaj_01<16>(__m128i a, __m128i b){
#ifdef YMP_Arch_2007_x64_Penryn
    return _mm_blend_epi16(a, b, 0xaa);
#else
    a = _mm_and_si128(a, _mm_setr_epi16(-1, 0, -1, 0, -1, 0, -1, 0));
    b = _mm_andnot_si128(_mm_setr_epi16(-1, 0, -1, 0, -1, 0, -1, 0), b);
    return _mm_or_si128(a, b);
#endif
}
template <> YM_FORCE_INLINE
__m128i permuteaj_10<16>(__m128i a, __m128i b){
#ifdef YMP_Arch_2007_x64_Penryn
    a = _mm_blend_epi16(a, b, 0x55);
    a = _mm_shuffle_epi8(a, _mm_setr_epi8(
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13
    ));
    return a;
#else
    a = _mm_srli_epi32(a, 16);
    b = _mm_slli_epi32(b, 16);
    return _mm_or_si128(a, b);
#endif
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m128i permuteaj_00<32>(__m128i a, __m128i b){
#ifdef YMP_Arch_2007_x64_Penryn
    b = _mm_shuffle_epi32(b, 177);
    return _mm_blend_epi16(a, b, 0xcc);
#else
    __m128 t = _mm_shuffle_ps(_mm_castsi128_ps(a), _mm_castsi128_ps(b), 136);
    return _mm_castps_si128(_mm_shuffle_ps(t, t, 216));
#endif
}
template <> YM_FORCE_INLINE
__m128i permuteaj_11<32>(__m128i a, __m128i b){
#ifdef YMP_Arch_2007_x64_Penryn
    a = _mm_shuffle_epi32(a, 177);
    return _mm_blend_epi16(a, b, 0xcc);
#else
    __m128 t = _mm_shuffle_ps(_mm_castsi128_ps(a), _mm_castsi128_ps(b), 221);
    return _mm_castps_si128(_mm_shuffle_ps(t, t, 216));
#endif
}
template <> YM_FORCE_INLINE
__m128i permuteaj_01<32>(__m128i a, __m128i b){
#ifdef YMP_Arch_2007_x64_Penryn
    return _mm_blend_epi16(a, b, 0xcc);
#else
    __m128 t = _mm_shuffle_ps(_mm_castsi128_ps(a), _mm_castsi128_ps(b), 216);
    return _mm_castps_si128(_mm_shuffle_ps(t, t, 216));
#endif
}
template <> YM_FORCE_INLINE
__m128i permuteaj_10<32>(__m128i a, __m128i b){
#ifdef YMP_Arch_2007_x64_Penryn
    a = _mm_blend_epi16(a, b, 0x33);
    return _mm_shuffle_epi32(a, 177);
#else
    __m128 t = _mm_shuffle_ps(_mm_castsi128_ps(a), _mm_castsi128_ps(b), 141);
    return _mm_castps_si128(_mm_shuffle_ps(t, t, 216));
#endif
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m128i permuteaj_00<64>(__m128i a, __m128i b){
    return _mm_unpacklo_epi64(a, b);
}
template <> YM_FORCE_INLINE
__m128i permuteaj_11<64>(__m128i a, __m128i b){
    return _mm_unpackhi_epi64(a, b);
}
template <> YM_FORCE_INLINE
__m128i permuteaj_01<64>(__m128i a, __m128i b){
#ifdef YMP_Arch_2007_x64_Penryn
    return _mm_blend_epi16(a, b, 0xf0);
#else
    return _mm_castpd_si128(_mm_shuffle_pd(_mm_castsi128_pd(a), _mm_castsi128_pd(b), 2));
#endif
}
template <> YM_FORCE_INLINE
__m128i permuteaj_10<64>(__m128i a, __m128i b){
    return _mm_castpd_si128(_mm_shuffle_pd(_mm_castsi128_pd(a), _mm_castsi128_pd(b), 1));
}
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void splitload(__m128i& x, const void* L, const void* H){
    x = _mm_castpd_si128(_mm_loadh_pd(_mm_load_sd((const double*)(L)), (const double*)(H)));
}
YM_FORCE_INLINE void splitstore(__m128i x, void* L, void* H){
    _mm_storel_pd((double*)(L), _mm_castsi128_pd(x));
    _mm_storeh_pd((double*)(H), _mm_castsi128_pd(x));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Float64
template <int bit_granularity> YM_FORCE_INLINE __m128d permuteaj_00(__m128d a, __m128d b);
template <int bit_granularity> YM_FORCE_INLINE __m128d permuteaj_11(__m128d a, __m128d b);
template <int bit_granularity> YM_FORCE_INLINE __m128d permuteaj_01(__m128d a, __m128d b);
template <int bit_granularity> YM_FORCE_INLINE __m128d permuteaj_10(__m128d a, __m128d b);
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m128d permuteaj_00<64>(__m128d a, __m128d b){
    return _mm_unpacklo_pd(a, b);
}
template <> YM_FORCE_INLINE
__m128d permuteaj_11<64>(__m128d a, __m128d b){
    return _mm_unpackhi_pd(a, b);
}
template <> YM_FORCE_INLINE
__m128d permuteaj_01<64>(__m128d a, __m128d b){
#ifdef YMP_Arch_2007_x64_Penryn
    return _mm_blend_pd(a, b, 2);
#else
    return _mm_shuffle_pd(a, b, 2);
#endif
}
template <> YM_FORCE_INLINE
__m128d permuteaj_10<64>(__m128d a, __m128d b){
    return _mm_shuffle_pd(a, b, 1);
}
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void splitload(__m128d& x, const void* L, const void* H){
    x = _mm_loadh_pd(_mm_load_sd((const double*)(L)), (const double*)(H));
}
YM_FORCE_INLINE void splitstore(__m128d x, void* L, void* H){
    _mm_storel_pd((double*)(L), x);
    _mm_storeh_pd((double*)(H), x);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Float32
template <int bit_granularity> YM_FORCE_INLINE __m128 permuteaj_00(__m128 a, __m128 b);
template <int bit_granularity> YM_FORCE_INLINE __m128 permuteaj_11(__m128 a, __m128 b);
template <int bit_granularity> YM_FORCE_INLINE __m128 permuteaj_01(__m128 a, __m128 b);
template <int bit_granularity> YM_FORCE_INLINE __m128 permuteaj_10(__m128 a, __m128 b);
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m128 permuteaj_00<32>(__m128 a, __m128 b){
    return _mm_castsi128_ps(permuteaj_00<32>(_mm_castps_si128(a), _mm_castps_si128(b)));
}
template <> YM_FORCE_INLINE
__m128 permuteaj_11<32>(__m128 a, __m128 b){
    return _mm_castsi128_ps(permuteaj_11<32>(_mm_castps_si128(a), _mm_castps_si128(b)));
}
template <> YM_FORCE_INLINE
__m128 permuteaj_01<32>(__m128 a, __m128 b){
#ifdef YMP_Arch_2007_x64_Penryn
    return _mm_blend_ps(a, b, 0xa);
#else
    a = _mm_shuffle_ps(a, b, 216);
    return _mm_shuffle_ps(a, a, 216);
#endif
}
template <> YM_FORCE_INLINE
__m128 permuteaj_10<32>(__m128 a, __m128 b){
    return _mm_castsi128_ps(permuteaj_10<32>(_mm_castps_si128(a), _mm_castps_si128(b)));
}
////////////////////////////////////////////////////////////////////////////////
template <> YM_FORCE_INLINE
__m128 permuteaj_00<64>(__m128 a, __m128 b){
    return _mm_shuffle_ps(a, b, 68);
}
template <> YM_FORCE_INLINE
__m128 permuteaj_11<64>(__m128 a, __m128 b){
    return _mm_shuffle_ps(a, b, 238);
}
template <> YM_FORCE_INLINE
__m128 permuteaj_01<64>(__m128 a, __m128 b){
#ifdef YMP_Arch_2007_x64_Penryn
    return _mm_blend_ps(a, b, 0xc);
#else
    return _mm_shuffle_ps(a, b, 228);
#endif
}
template <> YM_FORCE_INLINE
__m128 permuteaj_10<64>(__m128 a, __m128 b){
    return _mm_shuffle_ps(a, b, 78);
}
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void splitload(__m128& x, const void* L, const void* H){
    x = _mm_castpd_ps(_mm_loadh_pd(_mm_load_sd((const double*)(L)), (const double*)(H)));
}
YM_FORCE_INLINE void splitstore(__m128 x, void* L, void* H){
    _mm_storel_pd((double*)(L), _mm_castps_pd(x));
    _mm_storeh_pd((double*)(H), _mm_castps_pd(x));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
