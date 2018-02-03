/* BasicMemory.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 05/06/2017
 * Last Modified    : 05/06/2017
 * 
 *      A lightweight allocator + RAII memory type for POD arrays.
 * 
 *  The lifetime of an allocation must be contained within the lifetime of the
 *  allocator that produced it.
 * 
 *  It is considered an error to destroy (or move) an allocator while it has
 *  any live allocations. How this is handled is left to the implementation
 *  of the allocator.
 * 
 *  The name "BasicMemory" is to distinguish it from the other types of "Memory"
 *  in y-cruncher that have a similar interface. (such as "SwapMemory")
 * 
 */

#pragma once
#ifndef ymp_BasicMemory_BasicMemory_H
#define ymp_BasicMemory_BasicMemory_H
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
class BasicAllocator{
public:
    BasicAllocator() = default;
    virtual ~BasicAllocator() = default;

    virtual void* malloc(upL_t bytes, upL_t alignment = alignof(upL_t)) = 0;
    virtual void free(void* ptr) = 0;

private:
    BasicAllocator(const BasicAllocator&) = delete;
    void operator=(const BasicAllocator&) = delete;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <typename Type = void>
class BasicArray{
public:
    //  Rule of 5.
    YM_FORCE_INLINE ~BasicArray(){
        if (m_ptr != nullptr){
            m_allocator->free(m_ptr);
        }
    }

    YM_FORCE_INLINE BasicArray(BasicArray&& x)
        : m_ptr(x.m_ptr)
        , m_allocator(x.m_allocator)
    {
        x.m_ptr = nullptr;
        x.m_allocator = nullptr;
    }
    YM_FORCE_INLINE void operator=(BasicArray&& x){
        if (m_ptr != nullptr){
            m_allocator->free(m_ptr);
        }
        m_ptr = x.m_ptr;
        m_allocator = x.m_allocator;
        x.m_ptr = nullptr;
        x.m_allocator = nullptr;
    }

    BasicArray(const BasicArray&) = delete;
    void operator=(const BasicArray&) = delete;

public:
    YM_FORCE_INLINE BasicArray()
        : m_ptr(nullptr)
        , m_allocator(nullptr)
    {}
    YM_FORCE_INLINE BasicArray(BasicAllocator& allocator, upL_t objects, upL_t alignment = alignof(Type))
        : m_ptr((Type*)allocator.malloc(objects * sizeof(Type), alignment))
        , m_allocator(&allocator)
    {}

    YM_FORCE_INLINE void clear(){
        if (m_ptr != nullptr){
            m_allocator->free(m_ptr);
        }
        m_ptr = nullptr;
    }
    YM_FORCE_INLINE void reset(BasicAllocator& allocator, upL_t objects, upL_t alignment = alignof(Type)){
        if (m_ptr != nullptr){
            m_allocator->free(m_ptr);
        }
        m_ptr = (Type*)allocator.malloc(objects * sizeof(Type), alignment);
        m_allocator = &allocator;
    }

public:
    YM_FORCE_INLINE explicit operator bool() const{
        return m_ptr != nullptr;
    }

    YM_FORCE_INLINE const Type& operator[](upL_t index) const{
        return m_ptr[index];
    }
    YM_FORCE_INLINE Type& operator[](upL_t index){
        return m_ptr[index];
    }

public:
    //  Implicit conversions to pointer types.
    YM_FORCE_INLINE operator const Type*() const{
        return m_ptr;
    }
    YM_FORCE_INLINE operator Type*(){
        return m_ptr;
    }

private:
    Type* m_ptr;
    BasicAllocator* m_allocator;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <>
class BasicArray<void>{
public:
    //  Rule of 5.
    YM_FORCE_INLINE ~BasicArray(){
        if (m_ptr != nullptr){
            m_allocator->free(m_ptr);
        }
    }

    YM_FORCE_INLINE BasicArray(BasicArray&& x)
        : m_ptr(x.m_ptr)
        , m_allocator(x.m_allocator)
    {
        x.m_ptr = nullptr;
        x.m_allocator = nullptr;
    }
    YM_FORCE_INLINE void operator=(BasicArray&& x){
        if (m_ptr != nullptr){
            m_allocator->free(m_ptr);
        }
        m_ptr = x.m_ptr;
        m_allocator = x.m_allocator;
        x.m_ptr = nullptr;
        x.m_allocator = nullptr;
    }

    BasicArray(const BasicArray&) = delete;
    void operator=(const BasicArray&) = delete;

public:
    YM_FORCE_INLINE BasicArray()
        : m_ptr(nullptr)
        , m_allocator(nullptr)
    {}
    YM_FORCE_INLINE BasicArray(BasicAllocator& allocator, upL_t objects, upL_t alignment)
        : m_ptr(allocator.malloc(objects, alignment))
        , m_allocator(&allocator)
    {}

    YM_FORCE_INLINE void clear(){
        if (m_ptr != nullptr){
            m_allocator->free(m_ptr);
        }
        m_ptr = nullptr;
    }
    YM_FORCE_INLINE void reset(BasicAllocator& allocator, upL_t objects, upL_t alignment){
        if (m_ptr != nullptr){
            m_allocator->free(m_ptr);
        }
        m_ptr = allocator.malloc(objects, alignment);
        m_allocator = &allocator;
    }

public:
    YM_FORCE_INLINE explicit operator bool() const{
        return m_ptr != nullptr;
    }

public:
    //  Implicit conversions to pointer types.
    YM_FORCE_INLINE operator const void*() const{
        return m_ptr;
    }
    YM_FORCE_INLINE operator void*(){
        return m_ptr;
    }

private:
    void* m_ptr;
    BasicAllocator* m_allocator;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
