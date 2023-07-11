/* cpu_x86.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 04/12/2014
 *  Last Modified   : 04/12/2014
 * 
 */

#pragma once
#ifndef cpu_x86_H
#define cpu_x86_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string>
#include "PublicLibs/Types.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
struct cpu_x86{
    //  Vendor
    bool Vendor_AMD;
    bool Vendor_Intel;

    //  OS Features
    bool OS_x64;
    bool OS_AVX;
    bool OS_AVX512;

    //  Misc.
    bool HW_MMX;        //  Pentium (1997) + Athlon (1999)
    bool HW_x64;        //  Opteron (2003) + Prescott (2004)
    bool HW_ABM;        //  K10 (2007) + Haswell (2013)
    bool HW_RDRAND;     //  Ivy Bridge (2012) + Excavator (2015)
    bool HW_RDSEED;     //  Broadwell (2014) + Zen 1 (2017)
    bool HW_BMI1;       //  Piledriver (2012) + Haswell (2013)
    bool HW_BMI2;       //  Excavator (2015) + Haswell (2013)
    bool HW_ADX;        //  Broadwell (2014) + Zen1 (2017)
//    bool HW_MPX;        //  Skylake (2015) - dead
    bool HW_PREFETCHW;  //  K6 (1998) + Broadwell (2014)
    bool HW_PREFETCHWT1;
    bool HW_RDPID;      //  Zen2 (2019)
    bool HW_SERIALIZE;
    bool HW_CLDEMOTE;   //  Sapphire Rapids (2023)

    //  SIMD: 128-bit
    bool HW_SSE;        //  P3 Katmai (1999) + Athlon XP (2001)
    bool HW_SSE2;       //  P4 Willamette (2000) + K8 (2003)
    bool HW_SSE3;       //  Prescott (2003) + K8 (2003)
    bool HW_SSSE3;      //  Conroe (2006) + Bulldozer (2011)
    bool HW_SSE41;      //  Penryn (2007) + Bulldozer (2011)
    bool HW_SSE42;      //  Nehalem (2008) + Bulldozer (2011)
    bool HW_SSE4a;      //  K10 (2007)
    bool HW_PCLMULQDQ;  //  Westmere (2010) + Bulldozer (2011)
    bool HW_AES;        //  Westmere (2010) + Bulldozer (2011)
    bool HW_SHA;        //  Zen1 (2017) + Ice Lake (2019)
    bool HW_GFNI;       //  Ice Lake (2019) + Zen4 (2022)

    //  SIMD: 256-bit
    bool HW_AVX;            //  Sandy Bridge (2011) + Bulldozer (2011)
    bool HW_FMA4;           //  Bulldozer (2011) - dead
    bool HW_XOP;            //  Bulldozer (2011) - dead
    bool HW_FMA3;           //  Piledriver (2012) + Haswell (2013)
    bool HW_AVX2;           //  Haswell (2013) + Excavator (2015)
    bool HW_VAES;           //  Ice Lake (2019) + Zen3 (2020)
    bool HW_AVX_GFNI;       //  Ice Lake (2019) + Zen4 (2022)
    bool HW_AVX_VNNI;       //  Alder Lake (2021)
    bool HW_AVX_VNNI_INT8;  //  Sierra Forest (2024?)
    bool HW_AVX_IFMA;       //  Sierra Forest (2024?)
    bool HW_AVX_NE_CONVERT; //  Sierra Forest (2024?)

    //  SIMD: 512-bit
    bool HW_AVX512_F;               //  Knights Landing (2016) + Zen4 (2022)
    bool HW_AVX512_CD;              //  Knights Landing (2016) + Zen4 (2022)
    bool HW_AVX512_PF;              //  Knights Landing (2016) - dead
    bool HW_AVX512_ER;              //  ...
    bool HW_AVX512_VL;              //  Skylake Purley (2017) + Zen4 (2022)
    bool HW_AVX512_BW;              //  ...
    bool HW_AVX512_DQ;              //  ...
    bool HW_AVX512_VPOPCNTDQ;       //  Knights Mill (2017)
    bool HW_AVX512_4FMAPS;          //  Knights Mill (2017) - dead
    bool HW_AVX512_4VNNIW;          //  ...
    bool HW_AVX512_IFMA;            //  Cannon Lake (2018) + Zen4 (2022)
    bool HW_AVX512_VBMI;            //  ...
    bool HW_AVX512_VNNI;            //  Cascade Lake (2019) + Zen4 (2022)
    bool HW_AVX512_VPCLMULQDQ;      //  Ice Lake (2019) + Zen4 (2022)
    bool HW_AVX512_VAES;            //  ...
    bool HW_AVX512_GFNI;            //  ...
    bool HW_AVX512_VBMI2;           //  ...
    bool HW_AVX512_BITALG;          //  ...
    bool HW_AVX512_BF16;            //  Cooper Lake (2020) + Zen4 (2022)
    bool HW_AVX512_VP2INTERSECT;    //  Tiger Lake (2020) - dead
    bool HW_AVX512_FP16;            //  Sapphire Rapids (2023)

    //  AMX
    bool HW_AMX_TILE;       //  Sapphire Rapids (2023)
    bool HW_AMX_INT8;       //  ...
    bool HW_AMX_BF16;       //  ...
    bool HW_AMX_FP16;       //  Granite Rapids (2024?)
    bool HW_AMX_COMPLEX;

public:
    cpu_x86();
    void detect_host();

    void print() const;
    static void print_host();

    static void cpuid(s32_t out[4], u32_t eax, u32_t ecx);
    static std::string get_vendor_string();

private:
    static bool detect_OS_x64();
    static bool detect_OS_AVX();
    static bool detect_OS_AVX512();
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
