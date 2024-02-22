/* DigitViewer_RawToI64_x64_AVX512-VBMI.ipp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 01/01/2024
 *  Last Modified   : 01/01/2024
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
#include "i64ToHex_x64_AVX512-BW.h"
#include "i64ToDec_x64_AVX512-BW.h"
#include "i64ToDec_x64_AVX512-VBMI.h"
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
        upL_t blocks = words / 8;
        bad |= hex_to_i64_u8_x64_AVX512BW((__m512i*)T, (const __m512i*)raw, blocks);
        T     += blocks * 8;
        raw   += blocks * 8 * 16;
        words -= blocks * 8;
    }
    bad |= hex_to_i64_LittleEndian(T, (const u64_t*)raw, words);
    return bad;
}
YM_NO_INLINE void i64_to_hex(char* raw, const u64_t* T, upL_t words){
    {
        upL_t blocks = words / 8;
        i64_to_hex_u8_x64_AVX512BW((__m512i*)raw, (__m256i*)T, blocks);
        T     += blocks * 8;
        raw   += blocks * 8 * 16;
        words -= blocks * 8;
    }
    return i64_to_hex_LittleEndian((u64_t*)raw, T, words);
}
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE bool dec_to_i64(u64_t* T, const char* raw, upL_t words){
    bool bad = false;
    {
        upL_t blocks = words / 8;
        bad |= dec_to_i64_u8_x64_AVX512VBMI((__m512i*)T, raw, blocks);
        T     += blocks * 8;
        raw   += blocks * 8 * 19;
        words -= blocks * 8;
    }
    bad |= dec_to_i64_LittleEndian(T, raw, words);
    return bad;
}
YM_NO_INLINE void i64_to_dec(char* raw, const u64_t* T, upL_t words){
    {
        upL_t blocks = words / 8;
        i64_to_dec_u8_x64_AVX512VBMI(raw, (__m512i*)T, blocks);
        T     += blocks * 8;
        raw   += blocks * 8 * 19;
        words -= blocks * 8;
    }
    i64_to_dec_Default(raw, T, words);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
