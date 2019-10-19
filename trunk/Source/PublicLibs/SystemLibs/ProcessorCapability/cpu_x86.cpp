/* cpu_x86.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/12/2014
 * Last Modified    : 04/12/2014
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string.h>
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "cpu_x86.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#if _WIN32
#include "cpu_x86_Windows.ipp"
#elif (defined __linux) && (defined __GNUC__)
#include "cpu_x86_Linux.ipp"
#else
#error "No cpuid intrinsic defined."
#endif
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void print_bool(int x){
    if (x){
        Console::print("Yes", 'G');
    }else{
        Console::print("No", 'R');
    }
    Console::SetColor('w');
    Console::println();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
cpu_x86::cpu_x86(){
    memset(this, 0, sizeof(*this));
}
bool cpu_x86::detect_OS_AVX(){
    //  Copied from: http://stackoverflow.com/a/22521619/922184

    bool avxSupported = false;

    int cpuInfo[4];
    cpuid(cpuInfo, 1, 0);

    bool osUsesXSAVE_XRSTORE = (cpuInfo[2] & (1 << 27)) != 0;
    bool cpuAVXSuport = (cpuInfo[2] & (1 << 28)) != 0;

    if (osUsesXSAVE_XRSTORE && cpuAVXSuport)
    {
        u64_t xcrFeatureMask = xgetbv(_XCR_XFEATURE_ENABLED_MASK);
        avxSupported = (xcrFeatureMask & 0x6) == 0x6;
    }

    return avxSupported;
}
bool cpu_x86::detect_OS_AVX512(){
    if (!detect_OS_AVX())
        return false;

    u64_t xcrFeatureMask = xgetbv(_XCR_XFEATURE_ENABLED_MASK);
    return (xcrFeatureMask & 0xe6) == 0xe6;
}
std::string cpu_x86::get_vendor_string(){
    s32_t CPUInfo[4];
    char name[13];

    cpuid(CPUInfo, 0, 0);
    memcpy(name + 0, &CPUInfo[1], 4);
    memcpy(name + 4, &CPUInfo[3], 4);
    memcpy(name + 8, &CPUInfo[2], 4);
    name[12] = '\0';

    return name;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void cpu_x86::detect_host(){
    //  OS Features
    OS_x64 = detect_OS_x64();
    OS_AVX = detect_OS_AVX();
    OS_AVX512 = detect_OS_AVX512();

    //  Vendor
    std::string vendor(get_vendor_string());
    if (vendor == "GenuineIntel"){
        Vendor_Intel = true;
    }else if (vendor == "AuthenticAMD"){
        Vendor_AMD = true;
    }

    int info[4];
    cpuid(info, 0, 0);
    int nIds = info[0];

    cpuid(info, 0x80000000, 0);
    u32_t nExIds = info[0];

    //  Detect Features
    if (nIds >= 0x00000001){
        cpuid(info, 0x00000001, 0);
        HW_MMX    = (info[3] & ((int)1 << 23)) != 0;
        HW_SSE    = (info[3] & ((int)1 << 25)) != 0;
        HW_SSE2   = (info[3] & ((int)1 << 26)) != 0;
        HW_SSE3   = (info[2] & ((int)1 <<  0)) != 0;

        HW_SSSE3  = (info[2] & ((int)1 <<  9)) != 0;
        HW_SSE41  = (info[2] & ((int)1 << 19)) != 0;
        HW_SSE42  = (info[2] & ((int)1 << 20)) != 0;
        HW_AES    = (info[2] & ((int)1 << 25)) != 0;

        HW_AVX    = (info[2] & ((int)1 << 28)) != 0;
        HW_FMA3   = (info[2] & ((int)1 << 12)) != 0;

        HW_RDRAND = (info[2] & ((int)1 << 30)) != 0;
    }
    if (nIds >= 0x00000007){
        cpuid(info, 0x00000007, 0);
        HW_AVX2         = (info[1] & ((int)1 <<  5)) != 0;

        HW_BMI1         = (info[1] & ((int)1 <<  3)) != 0;
        HW_BMI2         = (info[1] & ((int)1 <<  8)) != 0;
        HW_ADX          = (info[1] & ((int)1 << 19)) != 0;
        HW_MPX          = (info[1] & ((int)1 << 14)) != 0;
        HW_SHA          = (info[1] & ((int)1 << 29)) != 0;
        HW_RDSEED       = (info[1] & ((int)1 << 18)) != 0;
        HW_PREFETCHWT1  = (info[2] & ((int)1 <<  0)) != 0;
        HW_RDPID        = (info[2] & ((int)1 << 22)) != 0;

        HW_AVX512_F     = (info[1] & ((int)1 << 16)) != 0;
        HW_AVX512_CD    = (info[1] & ((int)1 << 28)) != 0;
        HW_AVX512_PF    = (info[1] & ((int)1 << 26)) != 0;
        HW_AVX512_ER    = (info[1] & ((int)1 << 27)) != 0;

        HW_AVX512_VL    = (info[1] & ((int)1 << 31)) != 0;
        HW_AVX512_BW    = (info[1] & ((int)1 << 30)) != 0;
        HW_AVX512_DQ    = (info[1] & ((int)1 << 17)) != 0;

        HW_AVX512_IFMA  = (info[1] & ((int)1 << 21)) != 0;
        HW_AVX512_VBMI  = (info[2] & ((int)1 <<  1)) != 0;

        HW_AVX512_VPOPCNTDQ = (info[2] & ((int)1 << 14)) != 0;
        HW_AVX512_4FMAPS    = (info[3] & ((int)1 <<  2)) != 0;
        HW_AVX512_4VNNIW    = (info[3] & ((int)1 <<  3)) != 0;

        HW_AVX512_VNNI      = (info[2] & ((int)1 << 11)) != 0;

        HW_AVX512_VBMI2     = (info[2] & ((int)1 <<  6)) != 0;
        HW_GFNI             = (info[2] & ((int)1 <<  8)) != 0;
        HW_VAES             = (info[2] & ((int)1 <<  9)) != 0;
        HW_AVX512_VPCLMUL   = (info[2] & ((int)1 << 10)) != 0;
        HW_AVX512_BITALG    = (info[2] & ((int)1 << 12)) != 0;


        cpuid(info, 0x00000007, 1);
        HW_AVX512_BF16      = (info[0] & ((int)1 <<  5)) != 0;

    }
    if (nExIds >= 0x80000001){
        cpuid(info, 0x80000001, 0);
        HW_x64          = (info[3] & ((int)1 << 29)) != 0;
        HW_ABM          = (info[2] & ((int)1 <<  5)) != 0;
        HW_SSE4a        = (info[2] & ((int)1 <<  6)) != 0;
        HW_PREFETCHW    = (info[2] & ((int)1 <<  8)) != 0;
        HW_XOP          = (info[2] & ((int)1 << 11)) != 0;
        HW_FMA4         = (info[2] & ((int)1 << 16)) != 0;
    }
}
void cpu_x86::print() const{
    Console::println("CPU Vendor:");
    Console::print("    AMD         = "); print_bool(Vendor_AMD);
    Console::print("    Intel       = "); print_bool(Vendor_Intel);
    Console::println();

    Console::println("OS Features:");
    Console::print("  * 64-bit      = "); print_bool(OS_x64);
    Console::print("  * OS AVX      = "); print_bool(OS_AVX);
    Console::print("  * OS AVX512   = "); print_bool(OS_AVX512);
    Console::println();

    Console::println("Hardware Features:");
    Console::print("    MMX         = "); print_bool(HW_MMX);
    Console::print("  * x64         = "); print_bool(HW_x64);
    Console::print("  * ABM         = "); print_bool(HW_ABM);
    Console::print("    RDRAND      = "); print_bool(HW_RDRAND);
    Console::print("    RDSEED      = "); print_bool(HW_RDSEED);
    Console::print("    BMI1        = "); print_bool(HW_BMI1);
    Console::print("  * BMI2        = "); print_bool(HW_BMI2);
    Console::print("  * ADX         = "); print_bool(HW_ADX);
    Console::print("    MPX         = "); print_bool(HW_MPX);
    Console::print("    PREFETCHW   = "); print_bool(HW_PREFETCHW);
    Console::print("    PREFETCHWT1 = "); print_bool(HW_PREFETCHWT1);
    Console::print("    RDPID       = "); print_bool(HW_RDPID);
    Console::print("    GFNI        = "); print_bool(HW_GFNI);
    Console::print("    VAES        = "); print_bool(HW_VAES);
    Console::println();

    Console::println("SIMD: 128-bit");
    Console::print("  * SSE         = "); print_bool(HW_SSE);
    Console::print("  * SSE2        = "); print_bool(HW_SSE2);
    Console::print("  * SSE3        = "); print_bool(HW_SSE3);
    Console::print("  * SSSE3       = "); print_bool(HW_SSSE3);
    Console::print("    SSE4a       = "); print_bool(HW_SSE4a);
    Console::print("  * SSE4.1      = "); print_bool(HW_SSE41);
    Console::print("  * SSE4.2      = "); print_bool(HW_SSE42);
    Console::print("    AES-NI      = "); print_bool(HW_AES);
    Console::print("    SHA         = "); print_bool(HW_SHA);
    Console::println();

    Console::println("SIMD: 256-bit");
    Console::print("  * AVX         = "); print_bool(HW_AVX);
    Console::print("    XOP         = "); print_bool(HW_XOP);
    Console::print("  * FMA3        = "); print_bool(HW_FMA3);
    Console::print("  * FMA4        = "); print_bool(HW_FMA4);
    Console::print("  * AVX2        = "); print_bool(HW_AVX2);
    Console::println();

    Console::println("SIMD: 512-bit");
    Console::print("  * AVX512-F         = "); print_bool(HW_AVX512_F);
    Console::print("    AVX512-CD        = "); print_bool(HW_AVX512_CD);
    Console::print("    AVX512-PF        = "); print_bool(HW_AVX512_PF);
    Console::print("    AVX512-ER        = "); print_bool(HW_AVX512_ER);
    Console::print("  * AVX512-VL        = "); print_bool(HW_AVX512_VL);
    Console::print("  * AVX512-BW        = "); print_bool(HW_AVX512_BW);
    Console::print("  * AVX512-DQ        = "); print_bool(HW_AVX512_DQ);
    Console::print("  * AVX512-IFMA      = "); print_bool(HW_AVX512_IFMA);
    Console::print("  * AVX512-VBMI      = "); print_bool(HW_AVX512_VBMI);
    Console::println();

    Console::println("Alright Intel, how many drinks have you had tonight?");
    Console::print("    AVX512-VPOPCNTDQ = "); print_bool(HW_AVX512_VPOPCNTDQ);
    Console::print("    AVX512-4FMAPS    = "); print_bool(HW_AVX512_4FMAPS);
    Console::print("    AVX512-4VNNIW    = "); print_bool(HW_AVX512_4VNNIW);
    Console::print("    AVX512-VBMI2     = "); print_bool(HW_AVX512_VBMI2);
    Console::print("    AVX512-VPCLMUL   = "); print_bool(HW_AVX512_VPCLMUL);
    Console::print("    AVX512-VNNI      = "); print_bool(HW_AVX512_VNNI);
    Console::print("    AVX512-BITALG    = "); print_bool(HW_AVX512_BITALG);
    Console::print("    AVX512-BF16      = "); print_bool(HW_AVX512_BF16);
    Console::println();
}
void cpu_x86::print_host(){
    cpu_x86 features;
    features.detect_host();
    features.print();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
