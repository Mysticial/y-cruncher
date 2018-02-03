/* SmartPointerA.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 09/06/2016
 * Last Modified    : 05/08/2017
 * 
 *      A lightweight smart pointer with support for alignment.
 * 
 */

#pragma once
#ifndef ymp_Memory_SmartPointerA_H
#define ymp_Memory_SmartPointerA_H
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
template <typename Type>
class SmartPointerA{
public:
    ~SmartPointerA(){
        if (m_ptr == nullptr){
            return;
        }
        m_ptr->~Type();
        aligned_free(m_ptr);
    }

    template <typename Child>
    SmartPointerA(SmartPointerA<Child>&& x)
        : m_ptr(x.m_ptr)
    {
        x.m_ptr = nullptr;
    }

    template <typename Child>
    void operator=(SmartPointerA<Child>&& x){
        clear();
        m_ptr = x.m_ptr;
        x.m_ptr = nullptr;
    }

    SmartPointerA(const SmartPointerA& x) = delete;
    void operator=(const SmartPointerA& x) = delete;


public:
    //  Construct pointer with no object.
    SmartPointerA() : m_ptr(nullptr) {}

    //  Construct pointer with object by emplacement.
    template <class... Args>
    SmartPointerA(Args&&... args);

    //  Static version of above. This one allows default construction of the object.
    template <class... Args>
    static SmartPointerA make(Args&&... args);

    //  Clear Pointer
    void clear(){
        if (m_ptr != nullptr){
            m_ptr->~Type();
            aligned_free(m_ptr);
            m_ptr = nullptr;
        }
    }

    //  Basic reset.
    template <class... Args>
    void reset(Args&&... args);

    template <class... Args>
    void reset_aligned(upL_t alignment, Args&&... args);


    //  Reset with extended memory.
    //  Returns the a pointer to the extended portion of memory.
    template <class... Args>
    void* reset_extended(upL_t extra_bytes, Args&&... args);

    template <class... Args>
    void* reset_aligned_extended(upL_t alignment, upL_t extra_bytes, Args&&... args);


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

private:
    Type* m_ptr;

    template <typename> friend class SmartPointerA;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
