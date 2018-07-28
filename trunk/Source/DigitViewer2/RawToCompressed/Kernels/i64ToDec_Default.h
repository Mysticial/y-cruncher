/* i64ToDec_Default.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/11/2018
 * Last Modified    : 02/11/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_i64ToDec_Default_H
#define ydv_DigitViewer_i64ToDec_Default_H
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
YM_FORCE_INLINE void i64_to_dec_Default(char* raw, const u64_t* T, upL_t words){
    if (words == 0){
        return;
    }

    do{
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

        raw += 19;
        T++;
    }while (--words);
}
YM_FORCE_INLINE void i64_to_dec_u2_Default(char* raw, const u64_t* T, upL_t blocks){
    if (blocks == 0){
        return;
    }

    do{
        u64_t r0 = T[0];
        u64_t r1 = T[1];

        raw[18] = (char)(r0 % 10);
        raw[37] = (char)(r1 % 10);
        r0 /= 10;
        r1 /= 10;
        raw[17] = (char)(r0 % 10);
        raw[36] = (char)(r1 % 10);
        r0 /= 10;
        r1 /= 10;
        raw[16] = (char)(r0 % 10);
        raw[35] = (char)(r1 % 10);
        r0 /= 10;
        r1 /= 10;
        raw[15] = (char)(r0 % 10);
        raw[34] = (char)(r1 % 10);
        r0 /= 10;
        r1 /= 10;
        raw[14] = (char)(r0 % 10);
        raw[33] = (char)(r1 % 10);
        r0 /= 10;
        r1 /= 10;
        raw[13] = (char)(r0 % 10);
        raw[32] = (char)(r1 % 10);
        r0 /= 10;
        r1 /= 10;
        raw[12] = (char)(r0 % 10);
        raw[31] = (char)(r1 % 10);
        r0 /= 10;
        r1 /= 10;
        raw[11] = (char)(r0 % 10);
        raw[30] = (char)(r1 % 10);
        r0 /= 10;
        r1 /= 10;
        raw[10] = (char)(r0 % 10);
        raw[29] = (char)(r1 % 10);
        r0 /= 10;
        r1 /= 10;
        raw[ 9] = (char)(r0 % 10);
        raw[28] = (char)(r1 % 10);
        r0 /= 10;
        r1 /= 10;
        raw[ 8] = (char)(r0 % 10);
        raw[27] = (char)(r1 % 10);
        r0 /= 10;
        r1 /= 10;
        raw[ 7] = (char)(r0 % 10);
        raw[26] = (char)(r1 % 10);
        r0 /= 10;
        r1 /= 10;
        raw[ 6] = (char)(r0 % 10);
        raw[25] = (char)(r1 % 10);
        r0 /= 10;
        r1 /= 10;
        raw[ 5] = (char)(r0 % 10);
        raw[24] = (char)(r1 % 10);
        r0 /= 10;
        r1 /= 10;
        raw[ 4] = (char)(r0 % 10);
        raw[23] = (char)(r1 % 10);
        r0 /= 10;
        r1 /= 10;
        raw[ 3] = (char)(r0 % 10);
        raw[22] = (char)(r1 % 10);
        r0 /= 10;
        r1 /= 10;
        raw[ 2] = (char)(r0 % 10);
        raw[21] = (char)(r1 % 10);
        r0 /= 10;
        r1 /= 10;
        raw[ 1] = (char)(r0 % 10);
        raw[20] = (char)(r1 % 10);
        r0 /= 10;
        r1 /= 10;
        raw[ 0] = (char)r0;
        raw[19] = (char)r1;

        raw += 38;
        T += 2;
    }while (--blocks);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
