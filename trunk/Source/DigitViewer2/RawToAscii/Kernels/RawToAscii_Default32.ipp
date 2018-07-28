/* RawToAscii_Default32.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/14/2018
 * Last Modified    : 01/14/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_RawToAscii_Default32_IPP
#define ydv_DigitViewer_RawToAscii_Default32_IPP
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <algorithm>
#include "PublicLibs/BasicLibs/Alignment/AlignmentTools.h"
#include "DigitViewer2/RawToAscii/RawToAscii.h"
#include "RawToAscii_Default.h"
#include "RawToAscii_Default32.h"
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
        upL_t block = Alignment::ptr_to_aligned<4>(raw);
        block = std::min(block, digits);
        bad |= dec_to_raw_Default(raw, dec, block);
        raw += block;
        dec += block;
        digits -= block;
    }
    {
        upL_t blocks = digits / 4;
        bad |= dec_to_raw_Default32(reinterpret_cast<u32_t*>(raw), reinterpret_cast<const u32_t*>(dec), blocks);
        raw += blocks * 4;
        dec += blocks * 4;
        digits -= blocks * 4;
    }
    bad |= dec_to_raw_Default(raw, dec, digits);

    return bad;
}
bool hex_to_raw(char* raw, const char* hex, upL_t digits){
    bool bad = false;

    //  Align
    {
        upL_t block = Alignment::ptr_to_aligned<4>(raw);
        block = std::min(block, digits);
        bad |= hex_to_raw_Default(raw, hex, block);
        raw += block;
        hex += block;
        digits -= block;
    }
    {
        upL_t blocks = digits / 4;
        bad |= hex_to_raw_Default32(reinterpret_cast<u32_t*>(raw), reinterpret_cast<const u32_t*>(hex), blocks);
        raw += blocks * 4;
        hex += blocks * 4;
        digits -= blocks * 4;
    }
    bad |= hex_to_raw_Default(raw, hex, digits);

    return bad;
}
bool raw_to_dec(char* dec, const char* raw, upL_t digits){
    bool bad = false;

    //  Align
    {
        upL_t block = Alignment::ptr_to_aligned<4>(dec);
        block = std::min(block, digits);
        bad |= raw_to_dec_Default(dec, raw, block);
        raw += block;
        dec += block;
        digits -= block;
    }
    {
        upL_t blocks = digits / 4;
        bad |= raw_to_dec_Default32(reinterpret_cast<u32_t*>(dec), reinterpret_cast<const u32_t*>(raw), blocks);
        raw += blocks * 4;
        dec += blocks * 4;
        digits -= blocks * 4;
    }
    bad |= raw_to_dec_Default(dec, raw, digits);

    return bad;
}
bool raw_to_hex(char* hex, const char* raw, upL_t digits){
    bool bad = false;

    //  Align
    {
        upL_t block = Alignment::ptr_to_aligned<4>(hex);
        block = std::min(block, digits);
        bad |= raw_to_hex_Default(hex, raw, block);
        raw += block;
        hex += block;
        digits -= block;
    }
    {
        upL_t blocks = digits / 4;
        bad |= raw_to_hex_Default32(reinterpret_cast<u32_t*>(hex), reinterpret_cast<const u32_t*>(raw), blocks);
        raw += blocks * 4;
        hex += blocks * 4;
        digits -= blocks * 4;
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
