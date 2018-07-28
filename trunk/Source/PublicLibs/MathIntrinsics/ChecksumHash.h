/* ChecksumHash.h - Checksum Hashing
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 12/26/2010
 * Last Modified    : 07/14/2014
 * 
 *      The prime number used for this hash is 2^61 - 1.
 * 
 *  This file is intentionally lightweight since it's used in a lot of headers.
 * 
 */

#pragma once
#ifndef ymp_ChecksumHash_H
#define ymp_ChecksumHash_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class hash_t{
public:
    static const u64_t PRIME = 0x1fffffffffffffffull;

public:
    YM_FORCE_INLINE hash_t() = default;
    YM_FORCE_INLINE hash_t(int x);
    YM_FORCE_INLINE hash_t(u32_t x);
    YM_FORCE_INLINE hash_t(u64_t x);

public:
    YM_FORCE_INLINE u64_t value() const;

public:
    friend bool operator==(hash_t a, hash_t b);
    friend bool operator!=(hash_t a, hash_t b);

    friend hash_t operator+(hash_t a);
    friend hash_t operator-(hash_t a);

    friend hash_t operator+(hash_t a, hash_t b);
    friend hash_t operator-(hash_t a, hash_t b);
    friend hash_t operator*(hash_t a, hash_t b);
    friend hash_t operator^(hash_t a, uiL_t pow);
    friend hash_t operator^(hash_t a, siL_t pow);

    void operator+=(hash_t b);
    void operator-=(hash_t b);
    void operator*=(hash_t b);
    void operator^=(uiL_t pow);
    void operator^=(siL_t pow);

    template <typename wtype>
    static hash_t word_power(siL_t pow);

private:
    YM_FORCE_INLINE hash_t(u64_t x, void*) : m_hash(x) {}
    static u64_t reduce(u64_t x);
    static u64_t reduce(s64_t x);
    static u64_t reduce(u64_t L, u64_t H);

private:
    //  This internal hash is not fully reduced modulo 2^61 - 1.
    //  But it is guaranteed to be inside the range [0, 2^63).
    u64_t m_hash;

    friend hash_t bp_hash(hash_t hash_in, const u32_t* T, upL_t L);
    friend hash_t bp_hash(hash_t hash_in, const u64_t* T, upL_t L);
    friend hash_t bp_hash_radix(hash_t hash_in, u32_t radix, const u32_t* T, upL_t L);
    friend hash_t bp_hash_radix(hash_t hash_in, u64_t radix, const u64_t* T, upL_t L);
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Implementations
YM_FORCE_INLINE hash_t::hash_t(int x)
    : m_hash(x >= 0 ? x : x + PRIME)
{}
YM_FORCE_INLINE hash_t::hash_t(u32_t x)
    : m_hash(x)
{}
YM_FORCE_INLINE hash_t::hash_t(u64_t x)
    : m_hash(reduce(x))
{}
YM_FORCE_INLINE u64_t hash_t::value() const{
    u64_t x = reduce(m_hash);
    return x == PRIME ? 0 : x;
}
YM_FORCE_INLINE u64_t hash_t::reduce(u64_t x){
    //  Reduce any unsigned 64-bit integer into the range [0, 2^61 + 7).
    return (x >> 61) + (x & PRIME);
}
YM_FORCE_INLINE u64_t hash_t::reduce(s64_t x){
    //  Reduce any signed 64-bit integer into the range [-4, 2^61 + 3).
    return (x >> 61) + ((u64_t)x & PRIME);
}
YM_FORCE_INLINE u64_t hash_t::reduce(u64_t L, u64_t H){
    L = (L >> 61) + (L & PRIME);
    L += H >> 58;
    L += (H << 3) & PRIME;
    return L;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE bool operator==(hash_t a, hash_t b){
    return a.value() == b.value();
}
YM_FORCE_INLINE bool operator!=(hash_t a, hash_t b){
    return a.value() != b.value();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE hash_t operator+(hash_t a){
    return a;
}
YM_FORCE_INLINE hash_t operator-(hash_t a){
    return hash_t::reduce((u64_t)(0xfffffffffffffff8ull - a.m_hash));
}
YM_FORCE_INLINE hash_t operator+(hash_t a, hash_t b){
    a += b;
    return a;
}
YM_FORCE_INLINE hash_t operator-(hash_t a, hash_t b){
    a -= b;
    return a;
}
YM_FORCE_INLINE hash_t operator*(hash_t a, hash_t b){
    a *= b;
    return a;
}
YM_FORCE_INLINE hash_t operator^(hash_t a, uiL_t pow){
    a ^= pow;
    return a;
}
YM_FORCE_INLINE hash_t operator^(hash_t a, siL_t pow){
    a ^= pow;
    return a;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void hash_t::operator+=(hash_t b){
    m_hash = reduce(m_hash + b.m_hash);
}
YM_FORCE_INLINE void hash_t::operator-=(hash_t b){
    m_hash = reduce((s64_t)m_hash - (s64_t)b.m_hash) + PRIME;
}
YM_FORCE_INLINE void hash_t::operator^=(siL_t pow){
    if (pow < 0){
        pow += PRIME - 1;
    }
    *this ^= (uiL_t)pow;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
