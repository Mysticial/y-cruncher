/* u64d19_forward_convert_AVX2.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/19/2011
 * Last Modified    : 01/23/2016
 * 
 *  u64r_to_d19r(): Haswell - (ICC15 ~ Airi)
 *      convert_d19x1_forward_Default():    84.0    cycles/word
 *      convert_d19x2_forward_Default():    59.5    cycles/word
 *      convert_d19x2_forward_SSE41():      110     cycles/word
 *      convert_d19x4_forward_AVX2():       35.1    cycles/word
 *      convert_d19x8_forward_AVX2():       22.9    cycles/word
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include "u64d19_forward_intrinsics_Default.h"
#include "u64d19_forward_intrinsics_SSE41.h"
#include "u64d19_forward_intrinsics_AVX2.h"
#include "u64d19.h"
namespace DigitViewer{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void u64r_to_d19r(char* T, const u64_t* A, upL_t AL){
    T += AL * 19;
    const u64_t* stopA = A + AL;

    while (A < stopA - 7){
        T -= 152;
        u64d19::convert_d19x8_forward_AVX2<false, false>(T, A);
        A += 8;
    }
    while (A < stopA - 3){
        T -= 76;
        u64d19::convert_d19x4_forward_AVX2<false, false>(T, A);
        A += 4;
    }
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

    while (A < stopA - 7){
        u64d19::convert_d19x8_forward_AVX2<true, false>(T, A);
        A += 8;
        T += 152;
    }
    while (A < stopA - 3){
        u64d19::convert_d19x4_forward_AVX2<true, false>(T, A);
        A += 4;
        T += 76;
    }
    while (A < stopA){
        u64d19::convert_d19x1_forward_Default<false>(T, *A);
        A += 1;
        T += 19;
    }
}
void u64f_to_d19a(char* T, const u64_t* A, upL_t AL){
    const u64_t* stopA = A + AL;

    while (A < stopA - 7){
        u64d19::convert_d19x8_forward_AVX2<true, true>(T, A);
        A += 8;
        T += 152;
    }
    while (A < stopA - 3){
        u64d19::convert_d19x4_forward_AVX2<true, true>(T, A);
        A += 4;
        T += 76;
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
