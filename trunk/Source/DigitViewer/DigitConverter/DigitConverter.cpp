/* DigitConverter.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/02/2012
 * Last Modified    : 01/24/2016
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/ConsoleIO/Margin.h"
#include "PublicLibs/SystemLibs/ProcessorCapability/ProcessorCapability.h"
#include "DigitConverter.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#if 0
#elif (defined YMP_Arch_2016_x64_KnightsLanding) || (defined YMP_Arch_2017_x64_Skylake)
#include "rawhex/rawhex_forward_convert_AVX2.ipp"
#include "rawdec/rawdec_forward_convert_AVX2.ipp"
#include "rawhex/rawhex_inverse_convert_AVX2.ipp"
#include "rawdec/rawdec_inverse_convert_AVX2.ipp"
#include "u32h8/u32h8_forward_convert_SSE41.ipp"
#include "u32d9/u32d9_forward_convert_SSE41.ipp"
#include "u64h16/u64h16_forward_convert_SSE41.ipp"
#include "u64h16/u64h16_inverse_convert_SSE2.ipp"
#include "u64d19/u64d19_forward_convert_AVX512.ipp"
#include "u64d19/u64d19_inverse_convert_SSE41.ipp"
namespace DigitViewer{
    void CompileOptions_DigitConverter(){
        const upL_t MARGIN = 20;
        Console::println_labelm(MARGIN, "    raw_to_hex", "AVX2", 'P');
        Console::println_labelm(MARGIN, "    raw_to_dec", "AVX2", 'P');
        Console::println_labelm(MARGIN, "    hex_to_raw", "AVX2", 'P');
        Console::println_labelm(MARGIN, "    dec_to_raw", "AVX2", 'P');
        Console::println_labelm(MARGIN, "    u32_to_h8", "SSE4.1", 'T');
        Console::println_labelm(MARGIN, "    u32_to_d8", "SSE4.1", 'T');
        Console::println_labelm(MARGIN, "    u64_to_h16", "SSE4.1", 'T');
#ifdef YMP_Arch_2017_x64_Skylake
        Console::println_labelm(MARGIN, "    u64_to_d19", "AVX512-DQ", 'r');
#else
        Console::println_labelm(MARGIN, "    u64_to_d19", "AVX512-F", 'g');
#endif
        Console::println_labelm(MARGIN, "    h16_to_u64", "SSE2", 'B');
        Console::println_labelm(MARGIN, "    d19_to_u64", "SSE4.1", 'T');
        Console::println();
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#elif defined YMP_Arch_2013_x64_Haswell
#include "rawhex/rawhex_forward_convert_AVX2.ipp"
#include "rawdec/rawdec_forward_convert_AVX2.ipp"
#include "rawhex/rawhex_inverse_convert_AVX2.ipp"
#include "rawdec/rawdec_inverse_convert_AVX2.ipp"
#include "u32h8/u32h8_forward_convert_SSE41.ipp"
#include "u32d9/u32d9_forward_convert_SSE41.ipp"
#include "u64h16/u64h16_forward_convert_SSE41.ipp"
#include "u64h16/u64h16_inverse_convert_SSE2.ipp"
#include "u64d19/u64d19_forward_convert_AVX2.ipp"
#include "u64d19/u64d19_inverse_convert_SSE41.ipp"
namespace DigitViewer{
    void CompileOptions_DigitConverter(){
        const upL_t MARGIN = 20;
        Console::println_labelm(MARGIN, "    raw_to_hex", "AVX2", 'P');
        Console::println_labelm(MARGIN, "    raw_to_dec", "AVX2", 'P');
        Console::println_labelm(MARGIN, "    hex_to_raw", "AVX2", 'P');
        Console::println_labelm(MARGIN, "    dec_to_raw", "AVX2", 'P');
        Console::println_labelm(MARGIN, "    u32_to_h8", "SSE4.1", 'T');
        Console::println_labelm(MARGIN, "    u32_to_d8", "SSE4.1", 'T');
        Console::println_labelm(MARGIN, "    u64_to_h16", "SSE4.1", 'T');
        Console::println_labelm(MARGIN, "    u64_to_d19", "AVX2", 'P');
        Console::println_labelm(MARGIN, "    h16_to_u64", "SSE2", 'B');
        Console::println_labelm(MARGIN, "    d19_to_u64", "SSE4.1", 'T');
        Console::println();
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#elif defined YMP_Arch_2007_x64_Penryn
#include "rawhex/rawhex_forward_convert_SSE2.ipp"
#include "rawdec/rawdec_forward_convert_SSE2.ipp"
#include "rawhex/rawhex_inverse_convert_SSE2.ipp"
#include "rawdec/rawdec_inverse_convert_SSE2.ipp"
#include "u32h8/u32h8_forward_convert_SSE41.ipp"
#include "u32d9/u32d9_forward_convert_SSE41.ipp"
#include "u64h16/u64h16_forward_convert_SSE41.ipp"
#include "u64h16/u64h16_inverse_convert_SSE2.ipp"
#include "u64d19/u64d19_forward_convert_SSE41.ipp"
#include "u64d19/u64d19_inverse_convert_SSE41.ipp"
namespace DigitViewer{
    void CompileOptions_DigitConverter(){
        const upL_t MARGIN = 20;
        Console::println_labelm(MARGIN, "    raw_to_hex", "SSE2", 'B');
        Console::println_labelm(MARGIN, "    raw_to_dec", "SSE2", 'B');
        Console::println_labelm(MARGIN, "    hex_to_raw", "SSE2", 'B');
        Console::println_labelm(MARGIN, "    dec_to_raw", "SSE2", 'B');
        Console::println_labelm(MARGIN, "    u32_to_h8", "SSE4.1", 'T');
        Console::println_labelm(MARGIN, "    u32_to_d8", "SSE4.1", 'T');
        Console::println_labelm(MARGIN, "    u64_to_h16", "SSE4.1", 'T');
        Console::println_labelm(MARGIN, "    u64_to_d19", "SSE4.1", 'T');
        Console::println_labelm(MARGIN, "    h16_to_u64", "SSE2", 'B');
        Console::println_labelm(MARGIN, "    d19_to_u64", "SSE4.1", 'T');
        Console::println();
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#elif (defined YMP_Arch_2001_x86_SSE2) || (defined YMP_Arch_2004_x64_SSE3)
#include "rawhex/rawhex_forward_convert_SSE2.ipp"
#include "rawdec/rawdec_forward_convert_SSE2.ipp"
#include "rawhex/rawhex_inverse_convert_SSE2.ipp"
#include "rawdec/rawdec_inverse_convert_SSE2.ipp"
#include "u32h8/u32h8_forward_convert_Default.ipp"
#include "u32d9/u32d9_forward_convert_Default.ipp"
#include "u64h16/u64h16_forward_convert_Default.ipp"
#include "u64h16/u64h16_inverse_convert_SSE2.ipp"
#include "u64d19/u64d19_forward_convert_Default.ipp"
#include "u64d19/u64d19_inverse_convert_Default.ipp"
namespace DigitViewer{
    void CompileOptions_DigitConverter(){
        const upL_t MARGIN = 20;
        Console::println_labelm(MARGIN, "    raw_to_hex", "SSE2", 'B');
        Console::println_labelm(MARGIN, "    raw_to_dec", "SSE2", 'B');
        Console::println_labelm(MARGIN, "    hex_to_raw", "SSE2", 'B');
        Console::println_labelm(MARGIN, "    dec_to_raw", "SSE2", 'B');
        Console::println_labelm(MARGIN, "    u32_to_h8", "Default", 'w');
        Console::println_labelm(MARGIN, "    u32_to_d8", "Default", 'w');
        Console::println_labelm(MARGIN, "    u64_to_h16", "Default", 'w');
        Console::println_labelm(MARGIN, "    u64_to_d19", "Default", 'w');
        Console::println_labelm(MARGIN, "    h16_to_u64", "SSE2", 'B');
        Console::println_labelm(MARGIN, "    d19_to_u64", "Default", 'w');
        Console::println();
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#else
#include "rawhex/rawhex_forward_convert_Default.ipp"
#include "rawdec/rawdec_forward_convert_Default.ipp"
#include "rawhex/rawhex_inverse_convert_Default.ipp"
#include "rawdec/rawdec_inverse_convert_Default.ipp"
#include "u32h8/u32h8_forward_convert_Default.ipp"
#include "u32d9/u32d9_forward_convert_Default.ipp"
#include "u64h16/u64h16_forward_convert_Default.ipp"
#include "u64h16/u64h16_inverse_convert_Default.ipp"
#include "u64d19/u64d19_forward_convert_Default.ipp"
#include "u64d19/u64d19_inverse_convert_Default.ipp"
namespace DigitViewer{
    void CompileOptions_DigitConverter(){
        const upL_t MARGIN = 20;
        Console::println_labelm(MARGIN, "    raw_to_hex", "Default", 'w');
        Console::println_labelm(MARGIN, "    raw_to_dec", "Default", 'w');
        Console::println_labelm(MARGIN, "    hex_to_raw", "Default", 'w');
        Console::println_labelm(MARGIN, "    dec_to_raw", "Default", 'w');
        Console::println_labelm(MARGIN, "    u32_to_h8", "Default", 'w');
        Console::println_labelm(MARGIN, "    u32_to_d8", "Default", 'w');
        Console::println_labelm(MARGIN, "    u64_to_h16", "Default", 'w');
        Console::println_labelm(MARGIN, "    u64_to_d19", "Default", 'w');
        Console::println_labelm(MARGIN, "    h16_to_u64", "Default", 'w');
        Console::println_labelm(MARGIN, "    d19_to_u64", "Default", 'w');
        Console::println();
    }
}
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
