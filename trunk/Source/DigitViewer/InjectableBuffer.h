/* InjectableBuffer.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/09/2018
 * Last Modified    : 01/09/2018
 * 
 *      This is some hack to resolve the conflicting buffer situation between
 *  the Digit Viewer and the way y-cruncher wants to use it.
 *  
 */

#pragma once
#ifndef ydv_DigitViewer_InjectableBuffer_H
#define ydv_DigitViewer_InjectableBuffer_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/BasicLibs/Memory/AlignedMalloc.h"
namespace DigitViewer{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <typename wtype>
class InjectableBuffer{
public:
    //  Rule of 5.
    ~InjectableBuffer(){
        if (m_buffer != nullptr){
            fp_free(m_buffer);
        }
    }
    InjectableBuffer(InjectableBuffer&& x)
        : m_size(x.m_size)
        , m_buffer(x.m_buffer)
        , fp_free(x.fp_free)
    {
        x.m_size = 0;
        x.m_buffer = nullptr;
        x.fp_free = nullptr;
    }
    void operator=(InjectableBuffer&& x){
        if (m_buffer != nullptr){
            fp_free(m_buffer);
        }
        m_size = x.m_size;
        m_buffer = x.m_buffer;
        fp_free = x.fp_free;
        x.m_size = 0;
        x.m_buffer = nullptr;
        x.fp_free = nullptr;
    }
    InjectableBuffer(const InjectableBuffer&) = delete;
    void operator=(const InjectableBuffer&) = delete;

public:
    //  Constructors
    InjectableBuffer()
        : m_size(0)
        , m_buffer(nullptr)
        , fp_free(nullptr)
    {}
    InjectableBuffer(upL_t size)
        : m_size(size)
        , m_buffer((wtype*)aligned_malloc(size * sizeof(wtype)))
        , fp_free(aligned_free)
    {}
    InjectableBuffer(upL_t size, wtype* buffer, void (*deallocator)(void*))
        : m_size(size)
        , m_buffer(buffer)
        , fp_free(deallocator)
    {}

    void clear(){
        if (m_buffer == nullptr){
            return;
        }
        fp_free(m_buffer);
        m_size = 0;
        m_buffer = nullptr;
        fp_free = nullptr;
    }
    void reset(upL_t size){
        if (m_buffer != nullptr){
            fp_free(m_buffer);
        }
        m_buffer = (wtype*)aligned_malloc(size * sizeof(wtype));
        fp_free = aligned_free;
        m_size = size;
    }
    void reset(upL_t size, wtype* buffer, void (*deallocator)(void*)){
        if (m_buffer != nullptr){
            fp_free(m_buffer);
        }
        m_buffer = buffer;
        m_size = size;
        fp_free = deallocator;
    }

public:
    upL_t size() const{
        return m_size;
    }
    operator wtype*(){
        return m_buffer;
    }

private:
    upL_t m_size;
    wtype* m_buffer;
    void (*fp_free)(void*);
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
