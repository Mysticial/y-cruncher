/* Array.cpp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 09/11/2014
 *  Last Modified   : 09/11/2014
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "Array.h"
namespace ymp{
namespace Console{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <typename wtype> YM_NO_INLINE
void print_af(const wtype* A, upL_t L, char color){
    ConsoleLockScope lock;
    set_color(color);

    if (L == 0){
        print("{}");
        return;
    }

    print("{");

    L--;
    for (upL_t c = 0; c < L; c++){
        print_int(A[c]);
        print(",");
    }
    print_int(A[L]);
    print("}");
}
template <> YM_NO_INLINE void print_af(const double* A, upL_t L, char color){
    ConsoleLockScope lock;
    set_color(color);

    if (L == 0){
        print("{}");
        return;
    }

    print("{");

    L--;
    for (upL_t c = 0; c < L; c++){
        print_float(A[c]);
        print(",");
    }
    print_float(A[L]);
    print("}");
}
template <typename wtype> YM_NO_INLINE
void println_af(const wtype* A, upL_t L, char color){
    ConsoleLockScope lock;
    print_af(A, L, color);
    println();
}
////////////////////////////////////////////////////////////////////////////////
template void println_af<char>(const char* A, upL_t L, char color);
template void println_af<unsigned char>(const unsigned char* A, upL_t L, char color);
template void println_af<u16_t>(const u16_t* A, upL_t L, char color);
template void println_af<u32_t>(const u32_t* A, upL_t L, char color);
template void println_af<s32_t>(const s32_t* A, upL_t L, char color);
template void println_af<u64_t>(const u64_t* A, upL_t L, char color);
template void println_af<s64_t>(const s64_t* A, upL_t L, char color);
template void println_af<double>(const double* A, upL_t L, char color);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
