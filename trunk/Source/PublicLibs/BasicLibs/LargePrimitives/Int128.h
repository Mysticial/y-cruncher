/* Int128.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 01/20/2024
 *  Last Modified   : 05/11/2025
 * 
 */

#pragma once
#ifndef ymp_Int128_H
#define ymp_Int128_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/Types.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class UInt128{
public:
    UInt128() = default;

    UInt128(u32_t x) : m_lo(x), m_hi(0) {}
    UInt128(s32_t x) : m_lo(x), m_hi((s64_t)x >> 63) {}
    UInt128(u64_t x) : m_lo(x), m_hi(0) {}
    UInt128(s64_t x) : m_lo(x), m_hi(x >> 63) {}

    explicit UInt128(double x);
    explicit UInt128(const char* str);

    explicit operator u32_t() const{ return (u32_t)m_lo; }
    explicit operator s32_t() const{ return (s32_t)m_lo; }
    explicit operator u64_t() const{ return m_lo; }
    explicit operator s64_t() const{ return (s64_t)m_lo; }
    explicit operator double() const{ return (double)m_lo + (double)m_hi * 18446744073709551616.; }


public:
    //  Compare
    friend bool operator==(const UInt128& a, const UInt128& b){
        return a.m_lo == b.m_lo && a.m_hi == b.m_hi;
    }
    friend bool operator!=(const UInt128& a, const UInt128& b){
        return a.m_lo != b.m_lo || a.m_hi != b.m_hi;
    }
    friend bool operator<(const UInt128& a, const UInt128& b){
        return a.m_hi != b.m_hi ? a.m_hi < b.m_hi : a.m_lo < b.m_lo;
    }
    friend bool operator<=(const UInt128& a, const UInt128& b){
        return a.m_hi != b.m_hi ? a.m_hi < b.m_hi : a.m_lo <= b.m_lo;
    }
    friend bool operator>(const UInt128& a, const UInt128& b){
        return a.m_hi != b.m_hi ? a.m_hi > b.m_hi : a.m_lo > b.m_lo;
    }
    friend bool operator>=(const UInt128& a, const UInt128& b){
        return a.m_hi != b.m_hi ? a.m_hi > b.m_hi : a.m_lo >= b.m_lo;
    }


public:
    //  Logical
    UInt128 operator~() const{
        return UInt128(~m_lo, ~m_hi);
    }
    friend UInt128 operator|(const UInt128& a, const UInt128& b){
        return UInt128(a.m_lo | b.m_lo, a.m_hi | b.m_hi);
    }
    friend UInt128 operator&(const UInt128& a, const UInt128& b){
        return UInt128(a.m_lo & b.m_lo, a.m_hi & b.m_hi);
    }
    friend UInt128 operator^(const UInt128& a, const UInt128& b){
        return UInt128(a.m_lo ^ b.m_lo, a.m_hi ^ b.m_hi);
    }
    friend UInt128 operator<<(const UInt128& a, u64_t b){
        b &= 127;
        if (b == 0) return a;
        if (b < 64) return UInt128(a.m_lo << b, (a.m_hi << b) | (a.m_lo >> (64 - b)));
        return UInt128(0, a.m_lo << (b - 64));
    }
    friend UInt128 operator>>(const UInt128& a, u64_t b){
        b &= 127;
        if (b == 0) return a;
        if (b < 64) return UInt128((a.m_lo >> b) | (a.m_hi << (64 - b)), a.m_hi >> b);
        return UInt128(a.m_hi >> (b - 64), 0);
    }
    UInt128& operator<<=(u64_t b){ return *this = *this << b; }
    UInt128& operator>>=(u64_t b){ return *this = *this >> b; }


public:
    //  Arithmetic
    friend UInt128 operator+(const UInt128& a, const UInt128& b);
    friend UInt128 operator-(const UInt128& a, const UInt128& b);
    //friend UInt128 operator*(const UInt128& a, u64_t b){
    //    UInt128 ret = mul128(a.m_lo, b);
    //    ret.m_hi += a.m_hi * b;
    //    return ret;
    //}
    //friend UInt128 operator*(u64_t a, const UInt128& b){
    //    return b * a;
    //}
    friend UInt128 operator*(const UInt128& a, const UInt128& b){
        UInt128 ret = mul128(a.m_lo, b.m_lo);
        ret.m_hi += a.m_hi * b.m_lo;
        ret.m_hi += a.m_lo * b.m_hi;
        return ret;
    }
    friend UInt128 operator/(const UInt128& a, u64_t b){
        UInt128 rem, quo;
        a.divmod(rem, quo, b);
        return quo;
    }
    friend UInt128 operator%(const UInt128& a, u64_t b){
        UInt128 rem, quo;
        a.divmod(rem, quo, b);
        return rem;
    }
    friend UInt128 operator/(const UInt128& a, const UInt128& b){
        UInt128 rem, quo;
        a.divmod(rem, quo, b);
        return quo;
    }
    friend UInt128 operator%(const UInt128& a, const UInt128& b){
        UInt128 rem, quo;
        a.divmod(rem, quo, b);
        return rem;
    }

    UInt128 operator+() const{ return *this; }
    UInt128 operator-() const{ return 0 - *this; }
    UInt128& operator++(){ return *this = *this + 1; }
    UInt128& operator--(){ return *this = *this - 1; }
    UInt128 operator++(int){
        UInt128 ret = *this;
        *this = *this + 1;
        return ret;
    }
    UInt128 operator--(int){
        UInt128 ret = *this;
        *this = *this - 1;
        return ret;
    }
    UInt128& operator+=(const UInt128& b){ return *this = *this + b; }
    UInt128& operator-=(const UInt128& b){ return *this = *this - b; }
    UInt128& operator*=(const UInt128& b){ return *this = *this * b; }
    UInt128& operator/=(u64_t b){ return *this = *this / b; }
    UInt128& operator%=(u64_t b){ return *this = *this % b; }
    UInt128& operator/=(const UInt128& b){ return *this = *this / b; }
    UInt128& operator%=(const UInt128& b){ return *this = *this % b; }


public:
    static UInt128 mul128(u64_t a, u64_t b);
    char divmod10();
    void divmod(UInt128& rem, UInt128& quo, u64_t b) const;
    void divmod(UInt128& rem, UInt128& quo, const UInt128& b) const;


private:
    friend class SInt128;
    UInt128(u64_t lo, u64_t hi) : m_lo(lo), m_hi(hi) {}

    u64_t m_lo;
    u64_t m_hi;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class SInt128{
public:
    SInt128() = default;

    SInt128(u32_t x) : m_lo(x), m_hi(0) {}
    SInt128(s32_t x) : m_lo(x), m_hi((s64_t)x >> 63) {}
    SInt128(u64_t x) : m_lo(x), m_hi(0) {}
    SInt128(s64_t x) : m_lo(x), m_hi(x >> 63) {}

    explicit SInt128(double x) : SInt128(UInt128(x)) {}
    explicit SInt128(const char* str) : SInt128(str[0] == '-' ? -UInt128(str + 1) : UInt128(str)) {}

    explicit SInt128(const UInt128& x) : m_lo(x.m_lo), m_hi(x.m_hi) {}

    explicit operator u32_t() const{ return (u32_t)m_lo; }
    explicit operator s32_t() const{ return (s32_t)m_lo; }
    explicit operator u64_t() const{ return m_lo; }
    explicit operator s64_t() const{ return (s64_t)m_lo; }
    explicit operator double() const{ return (double)m_lo + (double)m_hi * 18446744073709551616.; }

    explicit operator UInt128() const{ return UInt128(m_lo, m_hi); }


public:
    //  Compare
    friend bool operator==(const SInt128& a, const SInt128& b){
        return a.m_lo == b.m_lo && a.m_hi == b.m_hi;
    }
    friend bool operator!=(const SInt128& a, const SInt128& b){
        return a.m_lo != b.m_lo || a.m_hi != b.m_hi;
    }
    friend bool operator<(const SInt128& a, const SInt128& b){
        return a.m_hi != b.m_hi ? a.m_hi < b.m_hi : a.m_lo < b.m_lo;
    }
    friend bool operator<=(const SInt128& a, const SInt128& b){
        return a.m_hi != b.m_hi ? a.m_hi < b.m_hi : a.m_lo <= b.m_lo;
    }
    friend bool operator>(const SInt128& a, const SInt128& b){
        return a.m_hi != b.m_hi ? a.m_hi > b.m_hi : a.m_lo > b.m_lo;
    }
    friend bool operator>=(const SInt128& a, const SInt128& b){
        return a.m_hi != b.m_hi ? a.m_hi > b.m_hi : a.m_lo >= b.m_lo;
    }


public:
    //  Logical
    SInt128 operator~() const{
        return SInt128(~m_lo, ~m_hi);
    }
    friend SInt128 operator|(const SInt128& a, const SInt128& b){
        return SInt128(a.m_lo | b.m_lo, a.m_hi | b.m_hi);
    }
    friend SInt128 operator&(const SInt128& a, const SInt128& b){
        return SInt128(a.m_lo & b.m_lo, a.m_hi & b.m_hi);
    }
    friend SInt128 operator^(const SInt128& a, const SInt128& b){
        return SInt128(a.m_lo ^ b.m_lo, a.m_hi ^ b.m_hi);
    }
    friend SInt128 operator<<(const SInt128& a, u64_t b){
        b &= 127;
        if (b == 0) return a;
        if (b < 64) return SInt128(a.m_lo << b, (a.m_hi << b) | (a.m_lo >> (64 - b)));
        return SInt128(0, a.m_lo << (b - 64));
    }
    friend SInt128 operator>>(const SInt128& a, u64_t b){
        b &= 127;
        if (b == 0) return a;
        if (b < 64) return SInt128((a.m_lo >> b) | (a.m_hi << (64 - b)), a.m_hi >> b);
        return SInt128(a.m_hi >> (b - 64), a.m_hi >> 63);
    }
    SInt128& operator<<=(u64_t b){ return *this = *this << b; }
    SInt128& operator>>=(u64_t b){ return *this = *this >> b; }


public:
    //  Arithmetic
    friend SInt128 operator+(const SInt128& a, const SInt128& b){
        return (SInt128)((UInt128)a + (UInt128)b);
    }
    friend SInt128 operator-(const SInt128& a, const SInt128& b){
        return (SInt128)((UInt128)a - (UInt128)b);
    }
    //friend SInt128 operator*(const SInt128& a, u64_t b){
    //    return (SInt128)((UInt128)a * b);
    //}
    //friend SInt128 operator*(u64_t a, const SInt128& b){
    //    return (SInt128)(a * (UInt128)b);
    //}
    friend SInt128 operator*(const SInt128& a, const SInt128& b){
        return (SInt128)((UInt128)a * (UInt128)b);
    }

    SInt128 operator+() const{ return *this; }
    SInt128 operator-() const{ return 0 - *this; }
    SInt128& operator++(){ return *this = *this + 1; }
    SInt128& operator--(){ return *this = *this - 1; }
    SInt128 operator++(int){
        SInt128 ret = *this;
        *this = *this + 1;
        return ret;
    }
    SInt128 operator--(int){
        SInt128 ret = *this;
        *this = *this - 1;
        return ret;
    }
    SInt128& operator+=(const SInt128& b){ return *this = *this + b; }
    SInt128& operator-=(const SInt128& b){ return *this = *this - b; }
    SInt128& operator*=(const SInt128& b){ return *this = *this * b; }
//    UInt128& operator/=(u64_t b){ return *this = *this / b; }
//    UInt128& operator%=(u64_t b){ return *this = *this % b; }
//    UInt128& operator/=(const UInt128& b){ return *this = *this / b; }
//    UInt128& operator%=(const UInt128& b){ return *this = *this % b; }


private:
    SInt128(u64_t lo, s64_t hi) : m_lo(lo), m_hi(hi) {}

    u64_t m_lo;
    s64_t m_hi;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
using u128_t = UInt128;
using s128_t = SInt128;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
