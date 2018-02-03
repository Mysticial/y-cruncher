/* u64d19_forward_intrinsics_Default.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/19/2011
 * Last Modified    : 01/22/2016
 * 
 */

#ifndef ydv_DigitViewer_u64d19_forward_intrinsics_Default_H
#define ydv_DigitViewer_u64d19_forward_intrinsics_Default_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace DigitViewer{
namespace u64d19{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <bool ascii> YM_FORCE_INLINE
void convert_d19x1_forward_Default(char str[19], u64_t dec){
    const char OFFSET = ascii ? '0' : 0;

    u64_t r0 = dec;
    str[18] = (char)(r0 % 10) + OFFSET;
    r0 /= 10;
    str[17] = (char)(r0 % 10) + OFFSET;
    r0 /= 10;
    str[16] = (char)(r0 % 10) + OFFSET;
    r0 /= 10;
    str[15] = (char)(r0 % 10) + OFFSET;
    r0 /= 10;
    str[14] = (char)(r0 % 10) + OFFSET;
    r0 /= 10;
    str[13] = (char)(r0 % 10) + OFFSET;
    r0 /= 10;
    str[12] = (char)(r0 % 10) + OFFSET;
    r0 /= 10;
    str[11] = (char)(r0 % 10) + OFFSET;
    r0 /= 10;
    str[10] = (char)(r0 % 10) + OFFSET;
    r0 /= 10;
    str[ 9] = (char)(r0 % 10) + OFFSET;
    r0 /= 10;
    str[ 8] = (char)(r0 % 10) + OFFSET;
    r0 /= 10;
    str[ 7] = (char)(r0 % 10) + OFFSET;
    r0 /= 10;
    str[ 6] = (char)(r0 % 10) + OFFSET;
    r0 /= 10;
    str[ 5] = (char)(r0 % 10) + OFFSET;
    r0 /= 10;
    str[ 4] = (char)(r0 % 10) + OFFSET;
    r0 /= 10;
    str[ 3] = (char)(r0 % 10) + OFFSET;
    r0 /= 10;
    str[ 2] = (char)(r0 % 10) + OFFSET;
    r0 /= 10;
    str[ 1] = (char)(r0 % 10) + OFFSET;
    r0 /= 10;
    str[ 0] = (char)r0 + OFFSET;
}
template <bool forward, bool ascii> YM_FORCE_INLINE
void convert_d19x2_forward_Default(char str[19*2], const u64_t dec[2]){
    const char OFFSET = ascii ? '0' : 0;

    u64_t r0 = dec[forward ? 0 : 1];
    u64_t r1 = dec[forward ? 1 : 0];

    str[18] = (char)(r0 % 10) + OFFSET;
    str[37] = (char)(r1 % 10) + OFFSET;
    r0 /= 10;
    r1 /= 10;
    str[17] = (char)(r0 % 10) + OFFSET;
    str[36] = (char)(r1 % 10) + OFFSET;
    r0 /= 10;
    r1 /= 10;
    str[16] = (char)(r0 % 10) + OFFSET;
    str[35] = (char)(r1 % 10) + OFFSET;
    r0 /= 10;
    r1 /= 10;
    str[15] = (char)(r0 % 10) + OFFSET;
    str[34] = (char)(r1 % 10) + OFFSET;
    r0 /= 10;
    r1 /= 10;
    str[14] = (char)(r0 % 10) + OFFSET;
    str[33] = (char)(r1 % 10) + OFFSET;
    r0 /= 10;
    r1 /= 10;
    str[13] = (char)(r0 % 10) + OFFSET;
    str[32] = (char)(r1 % 10) + OFFSET;
    r0 /= 10;
    r1 /= 10;
    str[12] = (char)(r0 % 10) + OFFSET;
    str[31] = (char)(r1 % 10) + OFFSET;
    r0 /= 10;
    r1 /= 10;
    str[11] = (char)(r0 % 10) + OFFSET;
    str[30] = (char)(r1 % 10) + OFFSET;
    r0 /= 10;
    r1 /= 10;
    str[10] = (char)(r0 % 10) + OFFSET;
    str[29] = (char)(r1 % 10) + OFFSET;
    r0 /= 10;
    r1 /= 10;
    str[ 9] = (char)(r0 % 10) + OFFSET;
    str[28] = (char)(r1 % 10) + OFFSET;
    r0 /= 10;
    r1 /= 10;
    str[ 8] = (char)(r0 % 10) + OFFSET;
    str[27] = (char)(r1 % 10) + OFFSET;
    r0 /= 10;
    r1 /= 10;
    str[ 7] = (char)(r0 % 10) + OFFSET;
    str[26] = (char)(r1 % 10) + OFFSET;
    r0 /= 10;
    r1 /= 10;
    str[ 6] = (char)(r0 % 10) + OFFSET;
    str[25] = (char)(r1 % 10) + OFFSET;
    r0 /= 10;
    r1 /= 10;
    str[ 5] = (char)(r0 % 10) + OFFSET;
    str[24] = (char)(r1 % 10) + OFFSET;
    r0 /= 10;
    r1 /= 10;
    str[ 4] = (char)(r0 % 10) + OFFSET;
    str[23] = (char)(r1 % 10) + OFFSET;
    r0 /= 10;
    r1 /= 10;
    str[ 3] = (char)(r0 % 10) + OFFSET;
    str[22] = (char)(r1 % 10) + OFFSET;
    r0 /= 10;
    r1 /= 10;
    str[ 2] = (char)(r0 % 10) + OFFSET;
    str[21] = (char)(r1 % 10) + OFFSET;
    r0 /= 10;
    r1 /= 10;
    str[ 1] = (char)(r0 % 10) + OFFSET;
    str[20] = (char)(r1 % 10) + OFFSET;
    r0 /= 10;
    r1 /= 10;
    str[ 0] = (char)r0 + OFFSET;
    str[19] = (char)r1 + OFFSET;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
