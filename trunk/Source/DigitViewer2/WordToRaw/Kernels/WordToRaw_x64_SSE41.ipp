/* WordToRaw_x64_SSE41.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/12/2018
 * Last Modified    : 02/12/2018
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <algorithm>
#include "PublicLibs/BasicLibs/Alignment/AlignmentTools.h"
#include "WordToRaw_LittleEndian.h"
#include "WordToRaw_x64_SSE41.h"
namespace DigitViewer2{
namespace WordToRaw{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE void word_to_hex(char* raw, const u32_t* T, upL_t words){
    raw += words * 8;
    w32_to_hex_LittleEndian((u32_t*)raw, T, words);
}
YM_NO_INLINE void word_to_dec(char* raw, const u32_t* T, upL_t words){
    raw += words * 9;
    w32_to_dec_LittleEndian(raw, T, words);
}
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE void word_to_hex(char* raw, const u64_t* T, upL_t words){
    raw += words * 16;

    //  Align
    {
        upL_t block = Alignment::ptr_to_aligned<16>(T);
        block = std::min(block, words);
        w64_to_hex_LittleEndian((u64_t*)raw, T, block);
        raw   -= block * 16;
        T     += block;
        words -= block;
    }
    {
        upL_t blocks = words / 2;
        w64_to_hex_u2_x64_SSE41((__m128i*)raw, (const u64_t*)T, blocks);
        raw   -= blocks * 2 * 16;
        T     += blocks * 2;
        words -= blocks * 2;
    }
    w64_to_hex_LittleEndian((u64_t*)raw, T, words);
}
YM_NO_INLINE void word_to_dec(char* raw, const u64_t* T, upL_t words){
    raw += words * 19;

    //  Align
    {
        upL_t block = Alignment::ptr_to_aligned<16>(T);
        block = std::min(block, words);
        w64_to_dec_Default(raw, T, block);
        raw   -= block * 19;
        T     += block;
        words -= block;
    }
    {
        upL_t blocks = words / 2;
        w64_to_dec_u2_x64_SSE41(raw, (const __m128i*)T, blocks);
        raw   -= blocks * 2 * 19;
        T     += blocks * 2;
        words -= blocks * 2;
    }
    w64_to_dec_Default(raw, T, words);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
