/* TrivialArray.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 09/06/2016
 * Last Modified    : 09/23/2016
 * 
 *      A lightweight smart array for trivial types.
 * 
 */

#pragma once
#ifndef ymp_Memory_TrivialArray_H
#define ymp_Memory_TrivialArray_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#ifdef YMP_ENABLE_HEAVY_STATIC_CHECKS
#include <type_traits>
#endif
#include "AlignedMalloc.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <typename Type>
class TrivialArray{
#ifdef YMP_ENABLE_HEAVY_STATIC_CHECKS
    static_assert(
        std::is_trivially_constructible<Type>::value,
        "Object must be trivially constructible."
    );
    static_assert(
        std::is_trivially_destructible<Type>::value,
        "Object must be trivially destructible."
    );
#endif

public:
    ~TrivialArray(){
        aligned_free(m_ptr);
    }

    TrivialArray(TrivialArray&& x)
        : m_ptr(x.m_ptr)
    {
        x.m_ptr = nullptr;
    }
    void operator=(TrivialArray&& x){
        aligned_free(m_ptr);
        m_ptr = x.m_ptr;
        x.m_ptr = nullptr;
    }

    TrivialArray(const TrivialArray& x) = delete;
    void operator=(const TrivialArray& x) = delete;


public:
    TrivialArray()
        : m_ptr(nullptr)
    {}
    TrivialArray(upL_t size, upL_t alignment = alignof(Type))
        : m_ptr((Type*)aligned_malloc(size * sizeof(Type), alignment))
    {}

    void clear(){
        aligned_free(m_ptr);
        m_ptr = nullptr;
    }
    void reset(upL_t size, upL_t alignment = alignof(Type)){
        Type* ptr = (Type*)aligned_malloc(size * sizeof(Type), alignment);
        aligned_free(m_ptr);
        m_ptr = ptr;
    }


public:
    //const Type& operator[](upL_t index) const{
    //    return m_ptr[index];
    //}
    //Type& operator[](upL_t index){
    //    return m_ptr[index];
    //}

    //  Implicit cast to same-typed pointer.
    operator const Type*() const{
        return m_ptr;
    }
    operator Type*(){
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
    Type* m_ptr;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
