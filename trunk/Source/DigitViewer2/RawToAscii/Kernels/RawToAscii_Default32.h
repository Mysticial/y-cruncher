/* RawToAscii_Default32.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/14/2018
 * Last Modified    : 01/14/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_RawToAscii_Default32_H
#define ydv_DigitViewer_RawToAscii_Default32_H
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
YM_FORCE_INLINE bool dec_to_raw_Default32(u32_t* raw, const u32_t* dec, upL_t blocks){
    if (blocks == 0){
        return false;
    }

    u32_t bad = 0;

    do{
        u32_t a0;

        a0 = dec[0];

        //  Check top bit.
        bad |= a0;

        //  Check > '9'
        bad |= a0 + 0x46464646;

        //  Check < '0'
        bad |= ~(a0 + 0x50505050);

        a0 -= 0x30303030;
        raw[0] = a0;

        raw++;
        dec++;
    }while (--blocks);

    return (bad & 0x80808080) != 0;
}
YM_FORCE_INLINE bool hex_to_raw_Default32(u32_t* raw, const u32_t* hex, upL_t blocks){
    if (blocks == 0){
        return false;
    }

    u32_t bad = 0;

    do{
        u32_t a0, b0;

        a0 = hex[0];

        //  Check top bit.
        bad |= a0;

        //  Check >= '0'
        b0 = a0 + 0x50505050;
        u32_t ge_0 = b0;

        //  Check <= '9'
        b0 = a0 + 0x46464646;
        u32_t le_0 = ~b0;

        //  Check >= 'a'
        b0 = a0 + 0x1f1f1f1f;
        u32_t ge_a = b0;

        //  Check <= 'f'
        b0 = a0 + 0x19191919;
        u32_t le_f = ~b0;

        u32_t digit = ge_0 & le_0;
        u32_t letter = ge_a & le_f & 0x80808080;
        bad |= ~(digit | letter);

        a0 -= ((letter >> 7) * 39);
        a0 -= 0x30303030;
        raw[0] = a0;

        raw++;
        hex++;
    }while (--blocks);

    return (bad & 0x80808080) != 0;
}
YM_FORCE_INLINE bool raw_to_dec_Default32(u32_t* dec, const u32_t* raw, upL_t blocks){
    if (blocks == 0){
        return false;
    }

    u32_t bad = 0;

    do{
        u32_t a0, b0;

        a0 = raw[0];

        //  Check top bit.
        bad |= a0;

        //  Check > 9
        b0 = a0 + 0x76767676;
        b0 &= 0x80808080;
        bad |= b0;

        a0 += 0x30303030;
        dec[0] = a0;

        raw++;
        dec++;
    }while (--blocks);

    return (bad & 0x80808080) != 0;
}
YM_FORCE_INLINE bool raw_to_hex_Default32(u32_t* hex, const u32_t* raw, upL_t blocks){
    if (blocks == 0){
        return false;
    }

    u32_t bad = 0;

    do{
        u32_t a0, b0;

        a0 = raw[0];

        //  Check top bits.
        bad |= a0;

        //  Check > 9
        b0 = a0 + 0x76767676;

        b0 &= 0x80808080;
        b0 >>= 7;
        b0 *= 39;

        a0 += 0x30303030;
        a0 += b0;

        hex[0] = a0;

        raw++;
        hex++;
    }while (--blocks);

    return (bad & 0xf0f0f0f0) != 0;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
