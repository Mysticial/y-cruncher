/* DigitHash_x64_AVX2.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/14/2018
 * Last Modified    : 01/14/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_DigitHash_x64_AVX2_IPP
#define ydv_DigitViewer_DigitHash_x64_AVX2_IPP
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <algorithm>
#include "PublicLibs/BasicLibs/Alignment/AlignmentTools.h"
#include "DigitViewer2/DigitHash/DigitHash.h"
#include "DigitHash_Default.h"
#include "DigitHash_x64_AVX2.h"
namespace DigitViewer2{
namespace DigitHash{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <int radix>
hash_t accumulate(hash_t hash_in, const char* raw_digits, upL_t digits){
    u64_t hash = hash_in.value();

    //  Align
    {
        upL_t block = Alignment::ptr_to_aligned<32>(raw_digits);
        block = std::min(block, digits);
        hash = hash_raw_str_Default<radix>(hash, raw_digits, block);
        raw_digits += block;
        digits -= block;
    }
    if (radix == 10){
        upL_t blocks = digits / 64;
        hash = hash_dec_raw_d64_AVX2(hash, reinterpret_cast<const __m256i*>(raw_digits), blocks);
        raw_digits += blocks * 64;
        digits -= blocks * 64;
    }
    if (radix == 16){
        upL_t blocks = digits / 64;
        hash = hash_hex_raw_d64_AVX2(hash, reinterpret_cast<const __m256i*>(raw_digits), blocks);
        raw_digits += blocks * 64;
        digits -= blocks * 64;
    }
    {
        upL_t blocks = digits / 8;
        hash = hash_raw_u64d8_LittleEndian<radix>(hash, reinterpret_cast<const u64_t*>(raw_digits), blocks);
        raw_digits += blocks * 8;
        digits -= blocks * 8;
    }
    hash = hash_raw_str_Default<radix>(hash, raw_digits, digits);

    return hash;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
