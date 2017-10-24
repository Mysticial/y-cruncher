/* String.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 05/31/2015
 * Last Modified    : 05/31/2015
 * 
 */

#pragma once
#ifndef ymp_ExportSafeLibs_String_H
#define ymp_ExportSafeLibs_String_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include "Array.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <typename type>
class dll_str{
    size_t m_len;
    dll_uarr<type> m_ptr;

public:
    dll_str(dll_str&& x)
        : m_len(x.m_len)
        , m_ptr(std::move(x.m_ptr))
    {
        x.m_len = 0;
    }
    void operator=(dll_str&& x){
        m_len = x.m_len;
        m_ptr = std::move(x.m_ptr);
        x.m_len = 0;
    }

    dll_str() : m_len(0) {}
    dll_str(const type* str)
        : m_len(strlen(str))
        , m_ptr(new type[m_len + 1])
    {
        memcpy(m_ptr.get(), str, (m_len + 1) * sizeof(type));
    }
    dll_str(const std::basic_string<type>& str)
        : m_len(str.size())
        , m_ptr(new type[m_len + 1])
    {
        memcpy(m_ptr.get(), str.c_str(), (m_len + 1) * sizeof(type));
    }

    //operator const type*() const{
    //    return std::basic_string<type>(m_ptr.get(), m_len);
    //}
    operator std::basic_string<type>() const{
        return std::basic_string<type>(m_ptr.get(), m_len);
    }
    const type* c_str() const{
        return m_ptr.get();
    }
    const size_t size() const{
        return m_len;
    }

    type& operator[](size_t index){
        return m_ptr[index];
    }
    type operator[](size_t index) const{
        return m_ptr[index];
    }

    friend std::basic_ostream<type>& operator<<(std::basic_ostream<type>& os, const dll_str& str){
        return os << str.c_str();
    }

private:
    static size_t strlen(const type* str){
        size_t L = 0;
        while (str[L] != '\0') L++;
        return L;
    }
};
////////////////////////////////////////////////////////////////////////////////
using dll_astr = dll_str<char>;
using dll_wstr = dll_str<wchar_t>;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
