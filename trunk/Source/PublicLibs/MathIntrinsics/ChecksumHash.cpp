/* ChecksumHash.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 12/26/2010
 * Last Modified    : 07/14/2014
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#if _MSC_VER
#include <intrin.h>
#elif __GNUC__
#include <x86intrin.h>
#endif
#include "PublicLibs/SystemLibs/ProcessorCapability/ProcessorCapability.h"
#include "PublicLibs/MathIntrinsics/LeadingZeros.h"
#include "ChecksumHash.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void hash_t::operator*=(hash_t b){
    u64_t L, H;

#if 0
#elif defined YMP_Arch_2013_x64_Haswell
    L = _mulx_u64(m_hash, b.m_hash, (unsigned long long*)&(H));

#elif defined YMP_Arch_2004_x64 && defined _WIN32
    L = _umul128(m_hash, b.m_hash, &H);

#elif defined YMP_Arch_2004_x64 && defined __GNUC__
    unsigned __int128 temp = (unsigned __int128)m_hash * b.m_hash;
    L = (u64_t)temp;
    H = (u64_t)(temp >> 64);

#else
    u32_t AL = (u32_t)m_hash;
    u32_t BL = (u32_t)b.m_hash;
    u32_t AH = (u32_t)(m_hash >> 32);
    u32_t BH = (u32_t)(b.m_hash >> 32);

    u64_t r0, r1;

    L  = (u64_t)AL * BL;
    r0 = (u64_t)AH * BL;
    r1 = (u64_t)AL * BH;
    H  = (u64_t)AH * BH;

    r0 += L >> 32;
    L  &= 0xffffffff;

    r1 += r0 & 0xffffffff;
    L  |= r1 << 32;
    H  += r0 >> 32;
    H  += r1 >> 32;
#endif

    m_hash = reduce(L, H);
}
void hash_t::operator^=(uiL_t pow){
    const uiL_t MASK = (uiL_t)1 << (INDEX_BITS - 1);

    ukL_t lzs;
    if (!Intrinsics::leading_zeros(lzs, pow)){
        m_hash = 1;
        return;
    }
    lzs++;

    upL_t c = INDEX_BITS - lzs;
    pow <<= lzs;

    hash_t b = *this;
    hash_t x = *this;
    while (c-- > 0){
        x *= x;

        if ((pow & MASK) != 0){
            x *= b;
        }

        pow <<= 1;
    }
    *this = x;
}
template <> hash_t hash_t::word_power<u32_t>(siL_t pow){
    if (pow == 0){
        return 1;
    }

    hash_t x;
    x.m_hash = 4294967296;
    if (pow < 0){
        x.m_hash = 536870912;
        pow = -pow;
    }

    x ^= pow;
    return x;
}
template <> hash_t hash_t::word_power<u64_t>(siL_t pow){
    if (pow == 0){
        return 1;
    }

    hash_t x;
    x.m_hash = 8;
    if (pow < 0){
        x.m_hash = 288230376151711744;
        pow = -pow;
    }

    x ^= pow;
    return x;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
