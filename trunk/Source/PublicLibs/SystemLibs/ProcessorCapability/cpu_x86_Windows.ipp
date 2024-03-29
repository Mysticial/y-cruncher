/* cpu_x86_Windows.ipp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 04/12/2014
 *  Last Modified   : 04/12/2014
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <Windows.h>
#include <intrin.h>
#include "cpu_x86.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void cpu_x86::cpuid(s32_t out[4], u32_t eax, u32_t ecx){
    __cpuidex(out, eax, ecx);
}
__int64 xgetbv(unsigned int x){
    return _xgetbv(x);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Detect 64-bit - Note that this snippet of code for detecting 64-bit has been copied from MSDN.
using LPFN_ISWOW64PROCESS = BOOL (WINAPI*) (HANDLE, PBOOL);
BOOL IsWow64()
{
    BOOL bIsWow64 = FALSE;

    LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
        GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

    if (NULL != fnIsWow64Process)
    {
        if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
        {
            printf("Error Detecting Operating System.\n");
            printf("Defaulting to 32-bit OS.\n\n");
            bIsWow64 = FALSE;
        }
    }
    return bIsWow64;
}
bool cpu_x86::detect_OS_x64(){
#ifdef _M_X64
    return true;
#else
    return IsWow64() != 0;
#endif
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
