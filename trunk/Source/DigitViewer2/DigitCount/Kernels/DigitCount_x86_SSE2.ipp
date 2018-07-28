/* DigitCount_x86_SSE2.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/13/2018
 * Last Modified    : 01/13/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_DigitCount_x86_SSE2_IPP
#define ydv_DigitViewer_DigitCount_x86_SSE2_IPP
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <algorithm>
#include "PublicLibs/BasicLibs/Alignment/AlignmentTools.h"
#include "DigitViewer2/DigitCount/DigitCount.h"
#include "DigitCount_Default.h"
#include "DigitCount_x86_SSE2.h"
namespace DigitViewer2{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void DigitCounts::accumulate(const char* raw_digits, upL_t digits){
    switch (m_radix){
    case 10:
        //  Align
        {
            upL_t block = Alignment::ptr_to_aligned<16>(raw_digits);
            block = std::min(block, digits);
            DigitCount::accumulate_Default(m_counts, raw_digits, block);
            raw_digits += block;
            digits -= block;
        }
        while (digits >= 16){
            upL_t blocks = std::min(digits / 16, (upL_t)128);
            DigitCount::accumulate_b16_SSE2_64x2<0, 1>(m_counts + 0, (const __m128i*)raw_digits, blocks);
            DigitCount::accumulate_b16_SSE2_64x2<2, 3>(m_counts + 2, (const __m128i*)raw_digits, blocks);
            DigitCount::accumulate_b16_SSE2_64x2<4, 5>(m_counts + 4, (const __m128i*)raw_digits, blocks);
            DigitCount::accumulate_b16_SSE2_64x2<6, 7>(m_counts + 6, (const __m128i*)raw_digits, blocks);
            DigitCount::accumulate_b16_SSE2_64x2<8, 9>(m_counts + 8, (const __m128i*)raw_digits, blocks);
            raw_digits += blocks * 16;
            digits -= blocks * 16;
        }
        DigitCount::accumulate_Default(m_counts, raw_digits, digits);
        break;

    case 16:
        //  Align
        {
            upL_t block = Alignment::ptr_to_aligned<16>(raw_digits);
            block = std::min(block, digits);
            DigitCount::accumulate_Default(m_counts, raw_digits, block);
            raw_digits += block;
            digits -= block;
        }
        while (digits >= 16){
            upL_t blocks = std::min(digits / 16, (upL_t)128);
            DigitCount::accumulate_b16_SSE2_64x2< 0,  1>(m_counts +  0, (const __m128i*)raw_digits, blocks);
            DigitCount::accumulate_b16_SSE2_64x2< 2,  3>(m_counts +  2, (const __m128i*)raw_digits, blocks);
            DigitCount::accumulate_b16_SSE2_64x2< 4,  5>(m_counts +  4, (const __m128i*)raw_digits, blocks);
            DigitCount::accumulate_b16_SSE2_64x2< 6,  7>(m_counts +  6, (const __m128i*)raw_digits, blocks);
            DigitCount::accumulate_b16_SSE2_64x2< 8,  9>(m_counts +  8, (const __m128i*)raw_digits, blocks);
            DigitCount::accumulate_b16_SSE2_64x2<10, 11>(m_counts + 10, (const __m128i*)raw_digits, blocks);
            DigitCount::accumulate_b16_SSE2_64x2<12, 13>(m_counts + 12, (const __m128i*)raw_digits, blocks);
            DigitCount::accumulate_b16_SSE2_64x2<14, 15>(m_counts + 14, (const __m128i*)raw_digits, blocks);
            raw_digits += blocks * 16;
            digits -= blocks * 16;
        }
        DigitCount::accumulate_Default(m_counts, raw_digits, digits);
        break;

    default:
        DigitCount::accumulate_Default(m_counts, raw_digits, digits);
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
