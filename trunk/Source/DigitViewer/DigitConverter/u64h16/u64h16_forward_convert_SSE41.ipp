/* u64h16_forward_convert_SSE41.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/20/2011
 * Last Modified    : 01/23/2016
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include "u64h16_forward_intrinsics_Default.h"
#include "u64h16_forward_intrinsics_SSE41.h"
#include "u64h16.h"
namespace DigitViewer{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void u64r_to_h16r(char* T, const u64_t* A, upL_t AL){
    T += AL * 16;
    const u64_t* stopA = A + AL;

    while (A < stopA - 3){
        T -= 64;
        u64h16::convert_h16x4_forward_SSE41<false, false>(T, A);
        A += 4;
    }
    while (A < stopA - 1){
        T -= 32;
        u64h16::convert_h16x2_forward_SSE41<false, false>(T, A);
        A += 2;
    }
    while (A < stopA){
        T -= 16;
        u64h16::convert_h16x1_forward_SSE41<false>(T, A);
        A += 1;
    }
}
void u64f_to_h16r(char* T, const u64_t* A, upL_t AL){
    const u64_t* stopA = A + AL;

    while (A < stopA - 3){
        u64h16::convert_h16x4_forward_SSE41<true, false>(T, A);
        A += 4;
        T += 64;
    }
    while (A < stopA - 1){
        u64h16::convert_h16x2_forward_SSE41<true, false>(T, A);
        A += 2;
        T += 32;
    }
    while (A < stopA){
        u64h16::convert_h16x1_forward_SSE41<false>(T, A);
        A += 1;
        T += 16;
    }
}
void u64f_to_h16a(char* T, const u64_t* A, upL_t AL){
    const u64_t* stopA = A + AL;

    while (A < stopA - 3){
        u64h16::convert_h16x4_forward_SSE41<true, true>(T, A);
        A += 4;
        T += 64;
    }
    while (A < stopA - 1){
        u64h16::convert_h16x2_forward_SSE41<true, true>(T, A);
        A += 2;
        T += 32;
    }
    while (A < stopA){
        u64h16::convert_h16x1_forward_SSE41<true>(T, A);
        A += 1;
        T += 16;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
