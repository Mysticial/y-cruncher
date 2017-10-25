/* u64d19_inverse_convert_SSE41.ipp
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
#include "u64d19_inverse_intrinsics_Default.h"
#include "u64d19_inverse_intrinsics_SSE41.h"
#include "u64d19.h"
namespace DigitViewer{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void d19f_to_u64r(const char* T, u64_t* A, upL_t AL){
    const u64_t* stopA = A + AL;

    while (A < stopA - 3){
        u64d19::convert_d19x4_inverse_SSE41(T, A);
        T += 76;
        A += 4;
    }
    while (A < stopA - 1){
        u64d19::convert_d19x2_inverse_SSE41(T, A);
        T += 38;
        A += 2;
    }
    if (A < stopA){
        u64d19::convert_d19x1_inverse_Default(T, A);
        T += 19;
        A += 1;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}