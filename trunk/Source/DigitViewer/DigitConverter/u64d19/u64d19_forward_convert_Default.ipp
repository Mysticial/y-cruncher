/* u64d19_forward_convert_Default.ipp
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
#include "u64d19_forward_intrinsics_Default.h"
#include "u64d19.h"
namespace DigitViewer{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void u64r_to_d19r(char* T, const u64_t* A, upL_t AL){
#if 0
    T += AL * 19;
    const u64_t* stopA = A + AL;

    while (A < stopA){
        T -= 19;
        u64d19::convert_d19x1_forward_Default<false>(T, *A);
        A += 1;
    }
#else
    T += AL * 19;
    const u64_t* stopA = A + AL;
    
    while (A < stopA - 1){
        T -= 38;
        u64d19::convert_d19x2_forward_Default<false, false>(T, A);
        A += 2;
    }
    if (A < stopA){
        T -= 19;
        u64d19::convert_d19x1_forward_Default<false>(T, *A);
        A += 1;
    }
#endif
}
void u64f_to_d19r(char* T, const u64_t* A, upL_t AL){
    const u64_t* stopA = A + AL;

    while (A < stopA - 1){
        u64d19::convert_d19x2_forward_Default<true, false>(T, A);
        A += 2;
        T += 38;
    }
    if (A < stopA){
        u64d19::convert_d19x1_forward_Default<false>(T, *A);
        A += 1;
        T += 19;
    }
}
void u64f_to_d19a(char* T, const u64_t* A, upL_t AL){
    const u64_t* stopA = A + AL;

    while (A < stopA - 1){
        u64d19::convert_d19x2_forward_Default<true, true>(T, A);
        A += 2;
        T += 38;
    }
    if (A < stopA){
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
