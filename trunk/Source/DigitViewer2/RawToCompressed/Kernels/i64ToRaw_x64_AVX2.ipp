/* i64ToRaw_x64_AVX2.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/10/2018
 * Last Modified    : 02/10/2018
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
#include "i64ToDec_x64_SSE41.h"
#include "i64ToHex_x64_AVX2.h"
#include "i64ToDec_x64_AVX2.h"
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
        bad |= hex_to_i64_u4_x64_AVX2((__m256i*)T, (const __m256i*)raw, blocks);
        T     += blocks * 4;
        raw   += blocks * 4 * 16;
        words -= blocks * 4;
    }
    bad |= hex_to_i64_LittleEndian(T, (const u64_t*)raw, words);
    return bad;
}
YM_NO_INLINE void i64_to_hex(char* raw, const u64_t* T, upL_t words){
    {
        upL_t blocks = words / 4;
        i64_to_hex_u4_x64_AVX2((__m256i*)raw, (__m128i*)T, blocks);
        T     += blocks * 4;
        raw   += blocks * 4 * 16;
        words -= blocks * 4;
    }
    return i64_to_hex_LittleEndian((u64_t*)raw, T, words);
}
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE bool dec_to_i64(u64_t* T, const char* raw, upL_t words){
    bool bad = false;
    {
        upL_t blocks = words / 4;
        bad |= dec_to_i64_u4_x64_AVX2((__m256i*)T, raw, blocks);
        T     += blocks * 4;
        raw   += blocks * 4 * 19;
        words -= blocks * 4;
    }
    bad |= dec_to_i64_LittleEndian(T, raw, words);
    return bad;
}
YM_NO_INLINE void i64_to_dec(char* raw, const u64_t* T, upL_t words){
    {
        upL_t blocks = words / 4;
        i64_to_dec_u4_x64_AVX2(raw, (__m256i*)T, blocks);
        T     += blocks * 4;
        raw   += blocks * 4 * 19;
        words -= blocks * 4;
    }
    i64_to_dec_Default(raw, T, words);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
