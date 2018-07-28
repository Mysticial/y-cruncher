/* WordToRaw_LittleEndian.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/11/2018
 * Last Modified    : 02/11/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_WordToRaw_LittleEndian_H
#define ydv_DigitViewer_WordToRaw_LittleEndian_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace DigitViewer2{
namespace WordToRaw{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void w32_to_hex_LittleEndian(u32_t* raw, const u32_t* T, upL_t words){
    if (words == 0){
        return;
    }

    do{
        raw -= 2;

        u32_t a0, a1;
        u32_t b0, b1;

        a0 = T[0];
        a1 = a0 & 0x0000ffff;
        a0 >>= 16;

        b0 = a0 & 0x0000ff00ull;
        b1 = a1 & 0x0000ff00ull;
        a0 = a0 & 0x000000ffull;
        a1 = a1 & 0x000000ffull;
        b0 >>= 8;
        b1 >>= 8;
        a0 <<= 16;
        a1 <<= 16;
        a0 |= b0;
        a1 |= b1;

        b0 = a0 & 0x00f000f0ull;
        b1 = a1 & 0x00f000f0ull;
        a0 = a0 & 0x000f000full;
        a1 = a1 & 0x000f000full;
        b0 >>= 4;
        b1 >>= 4;
        a0 <<= 8;
        a1 <<= 8;
        a0 |= b0;
        a1 |= b1;

        raw[0] = a0;
        raw[1] = a1;

        T++;
    }while (--words);
}
YM_FORCE_INLINE void w32_to_dec_LittleEndian(char* raw, const u32_t* T, upL_t words){
    if (words == 0){
        return;
    }

    do{
        raw -= 9;

        u32_t r0 = T[0];

        raw[8] = (char)(r0 % 10);
        r0 /= 10;
        raw[7] = (char)(r0 % 10);
        r0 /= 10;
        raw[6] = (char)(r0 % 10);
        r0 /= 10;
        raw[5] = (char)(r0 % 10);
        r0 /= 10;
        raw[4] = (char)(r0 % 10);
        r0 /= 10;
        raw[3] = (char)(r0 % 10);
        r0 /= 10;
        raw[2] = (char)(r0 % 10);
        r0 /= 10;
        raw[1] = (char)(r0 % 10);
        r0 /= 10;
        raw[0] = (char)r0;

        T++;
    }while (--words);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void w64_to_hex_LittleEndian(u64_t* raw, const u64_t* T, upL_t words){
    if (words == 0){
        return;
    }

    do{
        raw -= 2;

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

        T++;
    }while (--words);
}
YM_FORCE_INLINE void w64_to_dec_Default(char* raw, const u64_t* T, upL_t words){
    if (words == 0){
        return;
    }

    do{
        raw -= 19;

        u64_t r0 = T[0];

        raw[18] = (char)(r0 % 10);
        r0 /= 10;
        raw[17] = (char)(r0 % 10);
        r0 /= 10;
        raw[16] = (char)(r0 % 10);
        r0 /= 10;
        raw[15] = (char)(r0 % 10);
        r0 /= 10;
        raw[14] = (char)(r0 % 10);
        r0 /= 10;
        raw[13] = (char)(r0 % 10);
        r0 /= 10;
        raw[12] = (char)(r0 % 10);
        r0 /= 10;
        raw[11] = (char)(r0 % 10);
        r0 /= 10;
        raw[10] = (char)(r0 % 10);
        r0 /= 10;
        raw[ 9] = (char)(r0 % 10);
        r0 /= 10;
        raw[ 8] = (char)(r0 % 10);
        r0 /= 10;
        raw[ 7] = (char)(r0 % 10);
        r0 /= 10;
        raw[ 6] = (char)(r0 % 10);
        r0 /= 10;
        raw[ 5] = (char)(r0 % 10);
        r0 /= 10;
        raw[ 4] = (char)(r0 % 10);
        r0 /= 10;
        raw[ 3] = (char)(r0 % 10);
        r0 /= 10;
        raw[ 2] = (char)(r0 % 10);
        r0 /= 10;
        raw[ 1] = (char)(r0 % 10);
        r0 /= 10;
        raw[ 0] = (char)r0;

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
