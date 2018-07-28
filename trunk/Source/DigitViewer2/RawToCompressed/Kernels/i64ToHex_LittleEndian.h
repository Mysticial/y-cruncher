/* i64ToHex_LittleEndian.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/10/2018
 * Last Modified    : 02/10/2018
 * 
 *      Not sure if these fancy implementations are actually better than the
 *  scalar approach. They're not that important anyway.
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_i64ToHex_LittleEndian_H
#define ydv_DigitViewer_i64ToHex_LittleEndian_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace DigitViewer2{
namespace RawToCompressed{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE bool hex_to_i64_LittleEndian(u64_t* T, const u64_t* raw, upL_t words){
    if (words == 0){
        return false;
    }

    u64_t bad = 0;
    do{
        u64_t r0, r1, r2, r3;

        r0 = raw[0];
        r1 = raw[1];

        //  Check bad bits.
        bad |= r0;
        bad |= r1;

        r2 = r0 & 0x000f000f000f000full;
        r3 = r1 & 0x000f000f000f000full;
        r0 >>= 8;
        r1 >>= 8;
        r0 &= 0x000f000f000f000full;
        r1 &= 0x000f000f000f000full;
        r2 <<= 4;
        r3 <<= 4;
        r0 |= r2;
        r1 |= r3;

        r2 = r0 & 0x000000ff000000ffull;
        r3 = r1 & 0x000000ff000000ffull;
        r0 >>= 16;
        r1 >>= 16;
        r0 &= 0x000000ff000000ffull;
        r1 &= 0x000000ff000000ffull;
        r2 <<= 8;
        r3 <<= 8;
        r0 |= r2;
        r1 |= r3;

        r2 = r0 & 0x000000000000ffffull;
        r3 = r1 & 0x000000000000ffffull;
        r0 >>= 32;
        r1 >>= 32;
        r0 &= 0x000000000000ffffull;
        r1 &= 0x000000000000ffffull;
        r2 <<= 16;
        r3 <<= 16;
        r0 |= r2;
        r1 |= r3;

        r0 <<= 32;
        r0 |= r1;

        T[0] = r0;

        raw += 2;
        T++;
    }while (--words);

    return (bad & 0xf0f0f0f0f0f0f0f0ull) != 0;
}
YM_FORCE_INLINE void i64_to_hex_LittleEndian(u64_t* raw, const u64_t* T, upL_t words){
    if (words == 0){
        return;
    }

    do{
        u64_t a0, a1;
        u64_t b0, b1;

        a0 = T[0];
        a1 = a0 & 0xffffffff;
        a0 >>= 32;

        b0 = a0 & 0x00000000ffff0000ull;
        b1 = a1 & 0x00000000ffff0000ull;
        a0 = a0 & 0x000000000000ffffull;
        a1 = a1 & 0x000000000000ffffull;
        b0 >>= 16;
        b1 >>= 16;
        a0 <<= 32;
        a1 <<= 32;
        a0 |= b0;
        a1 |= b1;

        b0 = a0 & 0x0000ff000000ff00ull;
        b1 = a1 & 0x0000ff000000ff00ull;
        a0 = a0 & 0x000000ff000000ffull;
        a1 = a1 & 0x000000ff000000ffull;
        b0 >>= 8;
        b1 >>= 8;
        a0 <<= 16;
        a1 <<= 16;
        a0 |= b0;
        a1 |= b1;

        b0 = a0 & 0x00f000f000f000f0ull;
        b1 = a1 & 0x00f000f000f000f0ull;
        a0 = a0 & 0x000f000f000f000full;
        a1 = a1 & 0x000f000f000f000full;
        b0 >>= 4;
        b1 >>= 4;
        a0 <<= 8;
        a1 <<= 8;
        a0 |= b0;
        a1 |= b1;

        raw[0] = a0;
        raw[1] = a1;

        raw += 2;
        T++;
    }while (--words);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
