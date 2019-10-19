/* Stream.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/09/2017
 * Last Modified    : 04/09/2017
 * 
 */

#pragma once
#ifndef ymp_ExportSafeLibs_Stream_H
#define ymp_ExportSafeLibs_Stream_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include "PublicLibs/Types.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
static void local_stream_deleter(char* ptr){
    delete[] ptr;
}
////////////////////////////////////////////////////////////////////////////////
class DllSafeStream{
public:
    //  Rule of 5
    ~DllSafeStream(){ clear(); }
    DllSafeStream(DllSafeStream&& x)
        : m_bytes(x.m_bytes)
        , m_ptr(x.m_ptr)
        , m_deleter(x.m_deleter)
    {
        x.m_ptr = nullptr;
    }
    void operator=(DllSafeStream&& x){
        clear();
        m_bytes = x.m_bytes;
        m_ptr = x.m_ptr;
        m_deleter = x.m_deleter;
        x.m_ptr = nullptr;
    }
    DllSafeStream(const DllSafeStream& x)
        : m_bytes(x.m_bytes)
        , m_ptr(new char[x.m_bytes])
        , m_deleter(&local_stream_deleter)
    {
        memcpy(m_ptr, x.m_ptr, m_bytes);
    }
    void operator=(const DllSafeStream& x){
        m_bytes = x.m_bytes;
        m_ptr = new char[x.m_bytes];
        m_deleter = &local_stream_deleter;
        memcpy(m_ptr, x.m_ptr, m_bytes);
    }

public:
    //  Constructors
    DllSafeStream()
        : m_bytes(0)
        , m_ptr(nullptr)
    {}
    DllSafeStream(upL_t bytes)
        : m_bytes(bytes)
        , m_ptr(new char[bytes])
        , m_deleter(&local_stream_deleter)
    {}
    void clear(){
        if (m_ptr != nullptr){
            m_deleter(m_ptr);
        }
    }

public:
    //  Getters
    upL_t size() const{ return m_bytes; }
    void* get(upL_t offset = 0){
        return m_ptr + offset;
    }
    const void* get(upL_t offset = 0) const{
        return m_ptr + offset;
    }

private:
    upL_t m_bytes;
    char* m_ptr;
    void (*m_deleter)(char*);
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
