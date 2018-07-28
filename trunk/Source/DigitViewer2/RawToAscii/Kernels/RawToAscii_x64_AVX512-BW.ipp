/* RawToAscii_x64_AVX512-BW.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/14/2018
 * Last Modified    : 01/14/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_RawToAscii_x64_AVX512BW_IPP
#define ydv_DigitViewer_RawToAscii_x64_AVX512BW_IPP
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <algorithm>
#include "PublicLibs/BasicLibs/Alignment/AlignmentTools.h"
#include "DigitViewer2/RawToAscii/RawToAscii.h"
#include "RawToAscii_Default.h"
#include "RawToAscii_x64_AVX512-BW.h"
namespace DigitViewer2{
namespace RawToAscii{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool dec_to_raw(char* raw, const char* dec, upL_t digits){
    bool bad = false;

    //  Align
    {
        upL_t block = Alignment::ptr_to_aligned<64>(raw);
        block = std::min(block, digits);
        bad |= dec_to_raw_Default(raw, dec, block);
        raw += block;
        dec += block;
        digits -= block;
    }
    {
        upL_t blocks = digits / 64;
        bad |= dec_to_raw_x64_AVX512BW(reinterpret_cast<__m512i*>(raw), reinterpret_cast<const __m512i*>(dec), blocks);
        raw += blocks * 64;
        dec += blocks * 64;
        digits -= blocks * 64;
    }
    bad |= dec_to_raw_Default(raw, dec, digits);

    return bad;
}
bool hex_to_raw(char* raw, const char* hex, upL_t digits){
    bool bad = false;

    //  Align
    {
        upL_t block = Alignment::ptr_to_aligned<64>(raw);
        block = std::min(block, digits);
        bad |= hex_to_raw_Default(raw, hex, block);
        raw += block;
        hex += block;
        digits -= block;
    }
    {
        upL_t blocks = digits / 64;
        bad |= hex_to_raw_x64_AVX512BW(reinterpret_cast<__m512i*>(raw), reinterpret_cast<const __m512i*>(hex), blocks);
        raw += blocks * 64;
        hex += blocks * 64;
        digits -= blocks * 64;
    }
    bad |= hex_to_raw_Default(raw, hex, digits);

    return bad;
}
bool raw_to_dec(char* dec, const char* raw, upL_t digits){
    bool bad = false;

    //  Align
    {
        upL_t block = Alignment::ptr_to_aligned<64>(dec);
        block = std::min(block, digits);
        bad |= raw_to_dec_Default(dec, raw, block);
        raw += block;
        dec += block;
        digits -= block;
    }
    {
        upL_t blocks = digits / 64;
        bad |= raw_to_dec_x64_AVX512BW(reinterpret_cast<__m512i*>(dec), reinterpret_cast<const __m512i*>(raw), blocks);
        raw += blocks * 64;
        dec += blocks * 64;
        digits -= blocks * 64;
    }
    bad |= raw_to_dec_Default(dec, raw, digits);

    return bad;
}
bool raw_to_hex(char* hex, const char* raw, upL_t digits){
    bool bad = false;

    //  Align
    {
        upL_t block = Alignment::ptr_to_aligned<64>(hex);
        block = std::min(block, digits);
        bad |= raw_to_hex_Default(hex, raw, block);
        raw += block;
        hex += block;
        digits -= block;
    }
    {
        upL_t blocks = digits / 64;
        bad |= raw_to_hex_x64_AVX512BW(reinterpret_cast<__m512i*>(hex), reinterpret_cast<const __m512i*>(raw), blocks);
        raw += blocks * 64;
        hex += blocks * 64;
        digits -= blocks * 64;
    }
    bad |= raw_to_hex_Default(hex, raw, digits);

    return bad;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
