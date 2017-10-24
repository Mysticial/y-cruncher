/* u64d19_forward_convert_AVX512.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/24/2016
 * Last Modified    : 02/24/2016
 * 
 *  u64r_to_d19r(): Knights Landing - (ICC15 ~ TBA)
 *      convert_d19x1_forward_Default():    ??.?    cycles/word
 *      convert_d19x2_forward_Default():    ??.?    cycles/word
 *      convert_d19x2_forward_SSE41():      ??.?    cycles/word
 *      convert_d19x4_forward_AVX2():       ??.?    cycles/word
 *      convert_d19x8_forward_AVX2():       ??.?    cycles/word
 *      convert_d19x8_forward_AVX512():     ??.?    cycles/word
 *      convert_d19x16_forward_AVX512():    ??.?    cycles/word
 * 
 *  u64r_to_d19r(): Skylake Xeon - (ICC15 ~ TBA)
 *      convert_d19x1_forward_Default():    ??.?    cycles/word
 *      convert_d19x2_forward_Default():    ??.?    cycles/word
 *      convert_d19x2_forward_SSE41():      ??.?    cycles/word
 *      convert_d19x4_forward_AVX2():       ??.?    cycles/word
 *      convert_d19x8_forward_AVX2():       ??.?    cycles/word
 *      convert_d19x8_forward_AVX512():     ??.?    cycles/word
 *      convert_d19x16_forward_AVX512():    ??.?    cycles/word
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include "PublicLibs/Types.h"
#include "u64d19_forward_intrinsics_Default.h"
#include "u64d19_forward_intrinsics_SSE41.h"
#include "u64d19_forward_intrinsics_AVX2.h"
#include "u64d19_forward_intrinsics_AVX512.h"
#include "u64d19.h"
namespace DigitViewer{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void u64r_to_d19r(char* T, const u64_t* A, upL_t AL){
    T += AL * 19;
    const u64_t* stopA = A + AL;

    while (A < stopA - 15){
        T -= 304;
        u64d19::convert_d19x16_forward_AVX512<false, false>(T, A);
        A += 16;
    }
    while (A < stopA - 7){
        T -= 152;
        u64d19::convert_d19x8_forward_AVX512<false, false>(T, A);
        A += 8;
    }
    //while (A < stopA - 7){
    //    T -= 152;
    //    u64d19::convert_d19x8_forward_AVX2<false, false>(T, A);
    //    A += 8;
    //}
    //while (A < stopA - 3){
    //    T -= 76;
    //    u64d19::convert_d19x4_forward_AVX2<false, false>(T, A);
    //    A += 4;
    //}
    //while (A < stopA - 1){
    //    T -= 38;
    //    u64d19::convert_d19x2_forward_SSE41<false, false>(T, A);
    //    A += 2;
    //}
    //while (A < stopA - 1){
    //    T -= 38;
    //    u64d19::convert_d19x2_forward_Default<false, false>(T, A);
    //    A += 2;
    //}
    while (A < stopA){
        T -= 19;
        u64d19::convert_d19x1_forward_Default<false>(T, *A);
        A += 1;
    }
}
void u64f_to_d19r(char* T, const u64_t* A, upL_t AL){
    const u64_t* stopA = A + AL;

    while (A < stopA - 15){
        u64d19::convert_d19x16_forward_AVX512<true, false>(T, A);
        A += 16;
        T += 304;
    }
    while (A < stopA - 7){
        u64d19::convert_d19x8_forward_AVX512<true, false>(T, A);
        A += 8;
        T += 152;
    }
    while (A < stopA){
        u64d19::convert_d19x1_forward_Default<false>(T, *A);
        A += 1;
        T += 19;
    }
}
void u64f_to_d19a(char* T, const u64_t* A, upL_t AL){
    const u64_t* stopA = A + AL;

    while (A < stopA - 15){
        u64d19::convert_d19x16_forward_AVX512<true, true>(T, A);
        A += 16;
        T += 304;
    }
    while (A < stopA - 7){
        u64d19::convert_d19x8_forward_AVX512<true, true>(T, A);
        A += 8;
        T += 152;
    }
    while (A < stopA){
        u64d19::convert_d19x1_forward_Default<true>(T, *A);
        A += 1;
        T += 19;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
