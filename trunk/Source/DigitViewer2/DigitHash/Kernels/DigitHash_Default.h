/* DigitHash_Default.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/14/2018
 * Last Modified    : 01/14/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_DigitHash_Default_H
#define ydv_DigitViewer_DigitHash_Default_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace DigitViewer2{
namespace DigitHash{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <int radix> YM_FORCE_INLINE
u64_t hash_raw_str_Default(u64_t hash_in, const char* raw_digits, upL_t digits){
    static_assert(0 < radix && radix <= 16, "Radix is out of range.");

    if (digits == 0){
        return hash_in;
    }
    do{
        u64_t lo = (u64_t)(u32_t)hash_in * radix;
        u64_t hi = (u64_t)(u32_t)(hash_in >> 32) * radix;
        hash_in = lo + (hi >> 29) + ((hi & 0x1fffffff) << 32) + raw_digits[0];
        raw_digits++;
    }while (--digits);
    return hash_in;
}
template <int radix> YM_NO_INLINE
u64_t hash_raw_u32d4_LittleEndian(u64_t hash_in, const u32_t* T, upL_t L){
    static_assert(0 < radix && radix <= 16, "Radix is out of range.");

    if (L == 0){
        return hash_in;
    }

    const u32_t SCALE1 = radix;
    const u32_t SCALE2 = SCALE1 * SCALE1;
    const u32_t SCALE4 = SCALE2 * SCALE2;

    do{
        u32_t word = T[0];
        {
            u32_t lo = word & 0x00ff00ff;
            u32_t hi = (word >> 8) & 0x00ff00ff;
            word = lo * SCALE1 + hi;
        }
        {
            u32_t lo = word & 0x0000ffff;
            u32_t hi = word >> 16;
            word = lo * SCALE2 + hi;
        }
        {
            u64_t lo = (u64_t)(u32_t)hash_in * SCALE4;
            u64_t hi = (u64_t)(u32_t)(hash_in >> 32) * SCALE4;
            hash_in = lo + (hi >> 29) + ((hi & 0x1fffffff) << 32) + word;
        }
        T++;
    }while (--L);
    return hash_in;
}
template <int radix> YM_NO_INLINE
u64_t hash_raw_u64d8_LittleEndian(u64_t hash_in, const u64_t* T, upL_t L){
    static_assert(0 < radix && radix <= 16, "Radix is out of range.");

    if (L == 0){
        return hash_in;
    }

    const u32_t SCALE1 = radix;
    const u32_t SCALE2 = SCALE1 * SCALE1;
    const u32_t SCALE4 = SCALE2 * SCALE2;
    const u64_t SCALE8 = (u64_t)SCALE4 * SCALE4;

    do{
        u64_t word = T[0];
        {
            u64_t lo = word & 0x00ff00ff00ff00ff;
            u64_t hi = (word >> 8) & 0x00ff00ff00ff00ff;
            word = lo * SCALE1 + hi;
        }
        {
            u64_t lo = word & 0x0000ffff0000ffff;
            u64_t hi = (word >> 16) & 0x0000ffff0000ffff;
            word = lo * SCALE2 + hi;
        }
        {
            u64_t lo = word & 0x00000000ffffffff;
            u64_t hi = word >> 32;
            word = lo * SCALE4 + hi;
        }
        {
            u64_t lo = (u64_t)(u32_t)hash_in * SCALE8;
            u64_t hi = (u64_t)(u32_t)(hash_in >> 32) * SCALE8;
            lo = (lo >> 61) + ((u64_t)lo & 0x1fffffffffffffff);
            hi = (hi >> 29) + ((hi & 0x1fffffff) << 32);
            hash_in = lo + hi + word;
        }
        T++;
    }while (--L);
    return hash_in;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
