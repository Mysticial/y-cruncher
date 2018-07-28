/* SmartBuffer.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 09/06/2016
 * Last Modified    : 09/23/2016
 * 
 *      A lightweight buffer of raw memory.
 * 
 *  All data is raw. No constructors or destructors are called. The typing is
 *  just for convenience.
 * 
 */

#pragma once
#ifndef ymp_Memory_SmartBuffer_H
#define ymp_Memory_SmartBuffer_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "AlignedMalloc.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <typename Type = void>
class SmartBuffer{
public:
    ~SmartBuffer(){
        if (m_ptr != nullptr){
            aligned_free(m_ptr);
        }
    }

    SmartBuffer(SmartBuffer&& x)
        : m_ptr(x.m_ptr)
    {
        x.m_ptr = nullptr;
    }
    void operator=(SmartBuffer&& x){
        clear();
        m_ptr = x.m_ptr;
        x.m_ptr = nullptr;
    }

    SmartBuffer(const SmartBuffer& x) = delete;
    void operator=(const SmartBuffer& x) = delete;


public:
    SmartBuffer()
        : m_ptr(nullptr)
    {}
    SmartBuffer(upL_t bytes, upL_t alignment = alignof(Type))
        : m_ptr((Type*)aligned_malloc(bytes, alignment))
    {}

    void clear(){
        aligned_free(m_ptr);
        m_ptr = nullptr;
    }
    void reset(upL_t bytes, upL_t alignment = alignof(Type)){
        Type* ptr = (Type*)aligned_malloc(bytes, alignment);
        aligned_free(m_ptr);
        m_ptr = ptr;
    }


public:
    const Type* operator->() const{
        return m_ptr;
    }
    Type* operator->(){
        return m_ptr;
    }

    operator const Type*() const{
        return m_ptr;
    }
    operator Type*(){
        return m_ptr;
    }

    const Type& operator[](upL_t index) const{
        return m_ptr[index];
    }
    Type& operator[](upL_t index){
        return m_ptr[index];
    }


private:
    Type* m_ptr;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <>
class SmartBuffer<void>{
public:
    ~SmartBuffer(){
        if (m_ptr != nullptr){
            aligned_free(m_ptr);
        }
    }

    SmartBuffer(SmartBuffer&& x)
        : m_ptr(x.m_ptr)
    {
        x.m_ptr = nullptr;
    }
    void operator=(SmartBuffer&& x){
        clear();
        m_ptr = x.m_ptr;
        x.m_ptr = nullptr;
    }

    SmartBuffer(const SmartBuffer& x) = delete;
    void operator=(const SmartBuffer& x) = delete;


public:
    SmartBuffer()
        : m_ptr(nullptr)
    {}
    SmartBuffer(upL_t bytes, upL_t alignment = DEFAULT_ALIGNMENT)
        : m_ptr(aligned_malloc(bytes, alignment))
    {}

    void clear(){
        aligned_free(m_ptr);
        m_ptr = nullptr;
    }
    void reset(upL_t bytes, upL_t alignment = DEFAULT_ALIGNMENT){
        aligned_free(m_ptr);
        m_ptr = aligned_malloc(bytes, alignment);
    }


public:
    operator const void*() const{
        return m_ptr;
    }
    operator void*(){
        return m_ptr;
    }

    //  Explicit cast to any other pointer.
    template <typename type>
    explicit operator const type*() const{
        return (type*)m_ptr;
    }
    template <typename type>
    explicit operator type*(){
        return (type*)m_ptr;
    }


private:
    void* m_ptr;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
