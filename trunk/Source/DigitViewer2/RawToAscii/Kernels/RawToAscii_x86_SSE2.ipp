/* RawToAscii_x86_SSE2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/14/2018
 * Last Modified    : 01/14/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_RawToAscii_x86_SSE2_IPP
#define ydv_DigitViewer_RawToAscii_x86_SSE2_IPP
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <algorithm>
#include "PublicLibs/BasicLibs/Alignment/AlignmentTools.h"
#include "DigitViewer2/RawToAscii/RawToAscii.h"
#include "RawToAscii_Default.h"
#include "RawToAscii_x86_SSE2.h"
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
        upL_t block = Alignment::ptr_to_aligned<16>(raw);
        block = std::min(block, digits);
        bad |= dec_to_raw_Default(raw, dec, block);
        raw += block;
        dec += block;
        digits -= block;
    }
    {
        upL_t blocks = digits / 16;
        bad |= dec_to_raw_x86_SSE2(reinterpret_cast<__m128i*>(raw), reinterpret_cast<const __m128i*>(dec), blocks);
        raw += blocks * 16;
        dec += blocks * 16;
        digits -= blocks * 16;
    }
    bad |= dec_to_raw_Default(raw, dec, digits);

    return bad;
}
bool hex_to_raw(char* raw, const char* hex, upL_t digits){
    bool bad = false;

    //  Align
    {
        upL_t block = Alignment::ptr_to_aligned<16>(raw);
        block = std::min(block, digits);
        bad |= hex_to_raw_Default(raw, hex, block);
        raw += block;
        hex += block;
        digits -= block;
    }
    {
        upL_t blocks = digits / 16;
        bad |= hex_to_raw_x86_SSE2(reinterpret_cast<__m128i*>(raw), reinterpret_cast<const __m128i*>(hex), blocks);
        raw += blocks * 16;
        hex += blocks * 16;
        digits -= blocks * 16;
    }
    bad |= hex_to_raw_Default(raw, hex, digits);

    return bad;
}
bool raw_to_dec(char* dec, const char* raw, upL_t digits){
    bool bad = false;

    //  Align
    {
        upL_t block = Alignment::ptr_to_aligned<16>(dec);
        block = std::min(block, digits);
        bad |= raw_to_dec_Default(dec, raw, block);
        raw += block;
        dec += block;
        digits -= block;
    }
    {
        upL_t blocks = digits / 16;
        bad |= raw_to_dec_x86_SSE2(reinterpret_cast<__m128i*>(dec), reinterpret_cast<const __m128i*>(raw), blocks);
        raw += blocks * 16;
        dec += blocks * 16;
        digits -= blocks * 16;
    }
    bad |= raw_to_dec_Default(dec, raw, digits);

    return bad;
}
bool raw_to_hex(char* hex, const char* raw, upL_t digits){
    bool bad = false;

    //  Align
    {
        upL_t block = Alignment::ptr_to_aligned<16>(hex);
        block = std::min(block, digits);
        bad |= raw_to_hex_Default(hex, raw, block);
        raw += block;
        hex += block;
        digits -= block;
    }
    {
        upL_t blocks = digits / 16;
        bad |= raw_to_hex_x86_SSE2(reinterpret_cast<__m128i*>(hex), reinterpret_cast<const __m128i*>(raw), blocks);
        raw += blocks * 16;
        hex += blocks * 16;
        digits -= blocks * 16;
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
