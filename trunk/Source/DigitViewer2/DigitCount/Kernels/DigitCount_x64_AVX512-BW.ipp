/* DigitCount_x64_AVX512-BW.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/13/2018
 * Last Modified    : 01/13/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_DigitCount_x64_AVX512BW_IPP
#define ydv_DigitViewer_DigitCount_x64_AVX512BW_IPP
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <algorithm>
#include "PublicLibs/BasicLibs/Alignment/AlignmentTools.h"
#include "DigitViewer2/DigitCount/DigitCount.h"
#include "DigitCount_Default.h"
#include "DigitCount_x64_AVX2.h"
#include "DigitCount_x64_AVX512-BW.h"
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
            upL_t block = Alignment::ptr_to_aligned<64>(raw_digits);
            block = std::min(block, digits);
            DigitCount::accumulate_Default(m_counts, raw_digits, block);
            raw_digits += block;
            digits -= block;
        }
        if (digits >= 64){
            upL_t blocks = digits / 64;
            DigitCount::accumulate_b64_AVX512_64x10(m_counts, (const __m512i*)raw_digits, blocks);
            raw_digits += blocks * 64;
            digits -= blocks * 64;
        }
        DigitCount::accumulate_Default(m_counts, raw_digits, digits);
        break;

    case 16:
        //  Align
        {
            upL_t block = Alignment::ptr_to_aligned<64>(raw_digits);
            block = std::min(block, digits);
            DigitCount::accumulate_Default(m_counts, raw_digits, block);
            raw_digits += block;
            digits -= block;
        }
        while (digits >= 64){
            upL_t blocks = std::min(digits / 64, (upL_t)128);
            DigitCount::accumulate_b64_AVX512_64x8< 0,  1,  2,  3,  4,  5,  6,  7>(m_counts + 0, (const __m512i*)raw_digits, blocks);
            DigitCount::accumulate_b64_AVX512_64x8< 8,  9, 10, 11, 12, 13, 14, 15>(m_counts + 8, (const __m512i*)raw_digits, blocks);
            raw_digits += blocks * 64;
            digits -= blocks * 64;
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
