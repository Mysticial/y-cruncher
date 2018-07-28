/* WordToRaw_x64_AVX2.ipp
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
#include "WordToRaw_x64_AVX2.h"
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
        upL_t block = Alignment::ptr_to_aligned<32>(T);
        block = std::min(block, words);
        w64_to_hex_LittleEndian((u64_t*)raw, T, block);
        raw   -= block * 16;
        T     += block;
        words -= block;
    }
    {
        upL_t blocks = words / 4;
        w64_to_hex_u4_x64_AVX2((__m256i*)raw, (const __m128i*)T, blocks);
        raw   -= blocks * 4 * 16;
        T     += blocks * 4;
        words -= blocks * 4;
    }
    w64_to_hex_LittleEndian((u64_t*)raw, T, words);
}
YM_NO_INLINE void word_to_dec(char* raw, const u64_t* T, upL_t words){
    raw += words * 19;

    //  Align
    {
        upL_t block = Alignment::ptr_to_aligned<32>(T);
        block = std::min(block, words);
        w64_to_dec_Default(raw, T, block);
        raw   -= block * 19;
        T     += block;
        words -= block;
    }
    {
        upL_t blocks = words / 4;
        w64_to_dec_u4_x64_AVX2(raw, (const __m256i*)T, blocks);
        raw   -= blocks * 4 * 19;
        T     += blocks * 4;
        words -= blocks * 4;
    }
    w64_to_dec_Default(raw, T, words);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
