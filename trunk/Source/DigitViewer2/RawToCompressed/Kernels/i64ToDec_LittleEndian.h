/* i64ToDec_LittleEndian.h
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
#ifndef ydv_DigitViewer_i64ToDec_LittleEndian_H
#define ydv_DigitViewer_i64ToDec_LittleEndian_H
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
YM_FORCE_INLINE bool dec_to_i64_LittleEndian(u64_t* T, const char* raw, upL_t words){
    if (words == 0){
        return false;
    }

    u32_t bad = 0;
    do{
        u32_t r0 = *(const u32_t*)(raw +  0);
        u32_t r1 = *(const u32_t*)(raw +  4);
        u32_t r2 = *(const u32_t*)(raw +  8);
        u32_t r3 = *(const u32_t*)(raw + 12);
        u32_t r4 = *(const u32_t*)(raw + 15) & 0xffffff00;

        bad |= r0;
        bad |= r1;
        bad |= r2;
        bad |= r3;
        bad |= r4;

        bad |= r0 + 0x76767676;
        bad |= r1 + 0x76767676;
        bad |= r2 + 0x76767676;
        bad |= r3 + 0x76767676;
        bad |= r4 + 0x76767676;

        r0 = (r0 & 0x00ff00ff) * 10 + ((r0 >> 8) & 0x00ff00ff);
        r1 = (r1 & 0x00ff00ff) * 10 + ((r1 >> 8) & 0x00ff00ff);
        r2 = (r2 & 0x00ff00ff) * 10 + ((r2 >> 8) & 0x00ff00ff);
        r3 = (r3 & 0x00ff00ff) * 10 + ((r3 >> 8) & 0x00ff00ff);
        r4 = (r4 & 0x00ff00ff) * 10 + ((r4 >> 8) & 0x00ff00ff);

        r0 = (r0 & 0x0000ffff) * 100 + (r0 >> 16);
        r1 = (r1 & 0x0000ffff) * 100 + (r1 >> 16);
        r2 = (r2 & 0x0000ffff) * 100 + (r2 >> 16);
        r3 = (r3 & 0x0000ffff) * 100 + (r3 >> 16);
        r4 = (r4 & 0x0000ffff) * 100 + (r4 >> 16);

        r0 = r0 * 10000 + r1;
        r2 = r2 * 10000 + r3;

        u64_t r5 = r0 * (u64_t)100000000 + r2;
        r5 = r5 * 1000 + r4;

        T[0] = r5;

        raw += 19;
        T++;
    }while (--words);

    return (bad & 0x80808080) != 0;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
