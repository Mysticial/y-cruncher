/* Int128.cpp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 01/20/2024
 *  Last Modified   : 01/20/2024
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "Int128.h"
#include "Int128_Basic.h"
#include "Int128_IO.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
UInt128::UInt128(double x){
    if (x < 0){
        *this = -UInt128(-x);
        return;
    }
    if (x >= 340282366920938463463374607431768211456.){
        m_lo = 0xffffffffffffffff;
        m_hi = 0xffffffffffffffff;
        return;
    }
    m_hi = (u64_t)(x * (1. / 18446744073709551616.));
    x -= (double)m_hi * 18446744073709551616.;
    m_lo = (u64_t)x;
}
UInt128::UInt128(const char* str)
    : m_lo(0), m_hi(0)
{
    while (true){
        char ch = *str++;
        if (ch == ',')              continue;
        if (ch < '0' || ch > '9')   return; 
        *this = *this * 10;
        *this += ch - '0';
    }
}
char UInt128::divmod10(){
    if (m_hi == 0){
        u64_t q = m_lo / 10;
        u64_t r = m_lo - 10*q;
        m_lo = q;
        return (char)r;
    }

    UInt128 rem = *this;

    u64_t q = m_hi / 10;
    rem.m_hi = m_hi - 10*q;
    m_hi = q;

    q = ((rem.m_lo >> 4) | (rem.m_hi << 60)) / 10;
    q <<= 4;
    rem -= mul128(q, 10);
    m_lo = q;

    q = rem.m_lo / 10;
    rem.m_lo -= q * 10;
    m_lo += q;

    return (char)rem.m_lo;
}
void UInt128::divmod(UInt128& rem, UInt128& quo, u64_t b) const{
#if 0   //  _M_X64
    rem = *this;

    quo.m_hi = rem.m_hi / b;
    rem.m_hi -= quo.m_hi * b;

    quo.m_lo = _udiv128(rem.m_hi, rem.m_lo, b, &rem.m_lo);
    rem.m_hi = 0;

#else
    if (m_hi == 0){
        u64_t q = m_lo / b;
        u64_t r = m_lo - q * b;
        rem = r;
        quo = q;
        return;
    }

    double rcp = 1. / (double)b;
    rem = *this;

    double x = (double)rem;
    u64_t q = (u64_t)(x * rcp * (0.9999999999999995559107901499373838305473327636719 / 18446744073709551616.));
//    cout << "q = " << q << endl;

    quo = UInt128(0, q);
    rem.m_hi -= q * b;

//    cout << quo << endl;
//    cout << rem << endl;

    x = (double)rem;
    q = (u64_t)(x * rcp * (0.9999999999999995559107901499373838305473327636719 / 65536.));
//    cout << "q = " << q << endl;

    UInt128 t = (UInt128)q << 16;
    quo += t;
    rem -= t * b;

//    cout << quo << endl;
//    cout << rem << endl;

    x = (double)rem;
    q = (u64_t)(x * rcp * (0.9999999999999995559107901499373838305473327636719));
//    cout << "q = " << q << endl;

    quo += q;
    rem -= mul128(q, b);

//    cout << quo << endl;
//    cout << rem << endl;

    if (rem >= b){
        quo++;
        rem -= b;
    }
#endif
}
void UInt128::divmod(UInt128& rem, UInt128& quo, const UInt128& b) const{
    if (b.m_hi == 0){
        divmod(rem, quo, b.m_lo);
        return;
    }

    double rcp = 1. / (double)b;
    rem = *this;

    double x = (double)rem;
    u64_t q = (u64_t)(x * rcp * (0.9999999999999995559107901499373838305473327636719));
//    cout << "q = " << q << endl;

    quo = UInt128(q);
    rem -= q * b;

//    cout << quo << endl;
//    cout << rem << endl;

    x = (double)rem;
    q = (u64_t)(x * rcp * (0.9999999999999995559107901499373838305473327636719));
//    cout << "q = " << q << endl;

    quo.m_lo += q;
    rem -= q * b;

//    cout << quo << endl;
//    cout << rem << endl;

    if (rem >= b){
        quo++;
        rem -= b;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
