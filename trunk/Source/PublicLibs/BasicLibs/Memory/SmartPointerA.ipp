/* SmartPointerA.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 09/06/2016
 * Last Modified    : 05/08/2017
 * 
 */

#pragma once
#ifndef ymp_Memory_SmartPointerA_IPP
#define ymp_Memory_SmartPointerA_IPP
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <utility>
#include "SmartPointerA.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <typename Type>
SmartPointerA<Type>::~SmartPointerA(){
    if (m_ptr == nullptr){
        return;
    }
    m_ptr->~Type();
    aligned_free(m_ptr);
}
template <typename Type>
void SmartPointerA<Type>::clear(){
    if (m_ptr != nullptr){
        m_ptr->~Type();
        aligned_free(m_ptr);
        m_ptr = nullptr;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <typename Type>
template <class... Args>
SmartPointerA<Type>::SmartPointerA(SmartPointerToken, Args&&... args)
    : m_ptr(nullptr)
{
    reset_aligned_extended(alignof(Type), 0, std::forward<Args>(args)...);
}
template <typename Type>
template <class... Args>
void SmartPointerA<Type>::reset(Args&&... args){
    reset_aligned_extended(alignof(Type), 0, std::forward<Args>(args)...);
}
template <typename Type>
template <class... Args>
void SmartPointerA<Type>::reset_aligned(upL_t alignment, Args&&... args){
    reset_aligned_extended(alignment, 0, std::forward<Args>(args)...);
}
template <typename Type>
template <class... Args>
void* SmartPointerA<Type>::reset_extended(upL_t extra_bytes, Args&&... args){
    return reset_aligned_extended(alignof(Type), extra_bytes, std::forward<Args>(args)...);
}
template <typename Type>
template <class... Args>
void* SmartPointerA<Type>::reset_aligned_extended(upL_t alignment, upL_t extra_bytes, Args&&... args){
    Type* ptr = (Type*)aligned_malloc(sizeof(Type) + extra_bytes, alignment);
//    if (ptr == nullptr){
//        return nullptr;
//    }
    try{
        ::new (ptr) Type(std::forward<Args>(args)...);
    }catch (...){
        aligned_free(ptr);
//        m_ptr = nullptr;
        throw;
    }
    if (m_ptr != nullptr){
        m_ptr->~Type();
        aligned_free(m_ptr);
    }
    m_ptr = ptr;
    return ptr + 1;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
