/* u64d19_forward_convert_SSE41.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/19/2011
 * Last Modified    : 01/23/2016
 * 
 *  u64r_to_d19r(): Nehalem - (VS2015 ~ Ushio)
 *      convert_d19x1_forward_Default():    209     cycles/word
 *      convert_d19x2_forward_Default():    133     cycles/word
 *      convert_d19x2_forward_SSE41():      105     cycles/word
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include "u64d19_forward_intrinsics_Default.h"
#include "u64d19_forward_intrinsics_SSE41.h"
#include "u64d19.h"
namespace DigitViewer{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void u64r_to_d19r(char* T, const u64_t* A, upL_t AL){
    T += AL * 19;
    const u64_t* stopA = A + AL;

    const upL_t MASK = sizeof(__m128i) - 1;

    //  Align it
    while (A < stopA && ((upL_t)A & MASK)){
        T -= 19;
        u64d19::convert_d19x1_forward_Default<false>(T, *A);
        A += 1;
    }

    while (A < stopA - 1){
        T -= 38;
        u64d19::convert_d19x2_forward_SSE41<false, false>(T, A);
        A += 2;
    }
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

    //  Iterate until A is aligned.
    while  (A < stopA && ((upL_t)A & 15)){
        u64d19::convert_d19x1_forward_Default<false>(T, *A);
        A += 1;
        T += 19;
    }

    //  Proceed at full speed.
    while (A < stopA - 1){
        u64d19::convert_d19x2_forward_SSE41<true, false>(T, A);
        A += 2;
        T += 38;
    }
    while (A < stopA){
        u64d19::convert_d19x1_forward_Default<false>(T, *A);
        A += 1;
        T += 19;
    }
}
void u64f_to_d19a(char* T, const u64_t* A, upL_t AL){
    const u64_t* stopA = A + AL;

    //  Iterate until A is aligned.
    while (A < stopA && ((upL_t)A & 15)){
        u64d19::convert_d19x1_forward_Default<true>(T, *A);
        A += 1;
        T += 19;
    }

    //  Proceed at full speed.
    while (A < stopA - 1){
        u64d19::convert_d19x2_forward_SSE41<true, true>(T, A);
        A += 2;
        T += 38;
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
