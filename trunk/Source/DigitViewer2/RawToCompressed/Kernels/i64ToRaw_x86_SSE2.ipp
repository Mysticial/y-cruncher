/* i64ToRaw_x86_SSE2.ipp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 02/10/2018
 *  Last Modified   : 02/10/2018
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <algorithm>
#include "i64ToDec_Default.h"
#include "i64ToHex_LittleEndian.h"
#include "i64ToDec_LittleEndian.h"
#include "i64ToHex_x86_SSE2.h"
namespace DigitViewer2{
namespace RawToCompressed{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE bool hex_to_i64(u64_t* T, const char* raw, upL_t words){
    bool bad = false;
    {
        upL_t blocks = words / 4;
        bad |= hex_to_i64_u4_x86_SSE2((__m128i*)T, (const __m128i*)raw, blocks);
        T     += blocks * 4;
        raw   += blocks * 4 * 16;
        words -= blocks * 4;
    }
    bad |= hex_to_i64_LittleEndian(T, (const u64_t*)raw, words);
    return bad;
}
YM_NO_INLINE void i64_to_hex(char* raw, const u64_t* T, upL_t words){
    return i64_to_hex_LittleEndian((u64_t*)raw, T, words);
}
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE bool dec_to_i64(u64_t* T, const char* raw, upL_t words){
    return dec_to_i64_LittleEndian(T, raw, words);
}
YM_NO_INLINE void i64_to_dec(char* raw, const u64_t* T, upL_t words){
    {
        upL_t blocks = words / 2;
        i64_to_dec_u2_Default(raw, T, blocks);
        T     += blocks * 2;
        raw   += blocks * 2 * 19;
        words -= blocks * 2;
    }
    i64_to_dec_Default(raw, T, words);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
