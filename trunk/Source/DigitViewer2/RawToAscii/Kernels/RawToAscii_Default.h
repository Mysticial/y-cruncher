/* RawToAscii_Default.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/14/2018
 * Last Modified    : 01/14/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_RawToAscii_Default_H
#define ydv_DigitViewer_RawToAscii_Default_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace DigitViewer2{
namespace RawToAscii{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE bool dec_to_raw_Default(char* raw, const char* dec, upL_t digits){
    bool bad = false;
    for (upL_t c = 0; c < digits; c++){
        unsigned char digit = dec[c] - '0';
        raw[c] = digit;
        bad |= (unsigned)digit > 9;
    }
    return bad;
}
YM_FORCE_INLINE bool hex_to_raw_Default(char* raw, const char* hex, upL_t digits){
    bool bad = false;
    for (upL_t c = 0; c < digits; c++){
        char digit = hex[c] - '0';
        if ((unsigned)digit > 9){
            digit += '0' + 10 - 'a';
            bad |= (unsigned)digit < 10;
            bad |= (unsigned)digit > 15;
        }
        raw[c] = digit;
    }
    return bad;
}
YM_FORCE_INLINE bool raw_to_dec_Default(char* dec, const char* raw, upL_t digits){
    bool bad = false;
    for (upL_t c = 0; c < digits; c++){
        char ch = raw[c];
        bad |= (unsigned)ch > 9;
        dec[c] = ch + '0';
    }
    return bad;
}
YM_FORCE_INLINE bool raw_to_hex_Default(char* hex, const char* raw, upL_t digits){
    bool bad = false;
    for (upL_t c = 0; c < digits; c++){
        char ch = raw[c];
        bad |= (unsigned)ch > 15;
        ch += ch < 10 ? '0' : 'a' - 10;
        hex[c] = ch;
    }
    return bad;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
