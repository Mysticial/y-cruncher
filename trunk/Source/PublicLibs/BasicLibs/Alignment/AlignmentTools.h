/* AlignmentTools.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 06/27/2008
 * Last Modified    : 09/17/2014
 * 
 */

#pragma once
#ifndef ymp_Alignment_AlignmentTools_H
#define ymp_Alignment_AlignmentTools_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <type_traits>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "PublicLibs/BasicLibs/TemplateTools/SizeOf.h"
#include "DefaultAlignment.h"
namespace ymp{
namespace Alignment{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Determine alignment of an integer.
template <upL_t alignment, typename LengthType>
YM_FORCE_INLINE LengthType int_past_aligned(LengthType x){
    static_assert(std::is_unsigned<LengthType>::value, "Length must be an unsigned integer.");
    static_assert((alignment & (alignment - 1)) == 0, "Alignment must be a power-of-two.");
    constexpr LengthType MASK = alignment - 1;
    return x & MASK;
}
template <upL_t alignment, typename LengthType>
YM_FORCE_INLINE LengthType int_to_aligned(LengthType x){
    static_assert(std::is_unsigned<LengthType>::value, "Length must be an unsigned integer.");
    static_assert((alignment & (alignment - 1)) == 0, "Alignment must be a power-of-two.");
    constexpr LengthType MASK = alignment - 1;
    return (0 - x) & MASK;
}
template <typename LengthType>
YM_FORCE_INLINE LengthType int_past_aligned_k(ukL_t alignment_k, LengthType x){
    static_assert(std::is_unsigned<LengthType>::value, "Length must be an unsigned integer.");
    LengthType mask = ((LengthType)1 << alignment_k) - 1;
    return x & mask;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Determine word alignment of pointer.
//  "ptr" must already be aligned to its type. Otherwise the behavior is undefined.
template <upL_t alignment, typename Type>
YM_FORCE_INLINE upL_t ptr_past_aligned(Type* ptr){
    constexpr upL_t SIZE = SizeOf<Type>::value;
    static_assert((SIZE & (SIZE - 1)) == 0, "sizeof(Type) must be a power-of-two.");
    static_assert((alignment & (alignment - 1)) == 0, "Alignment must be a power-of-two.");
    constexpr upL_t ALIGN = SIZE > alignment ? SIZE : alignment;
    constexpr upL_t MASK = ALIGN - 1;
    return ((upL_t)ptr & MASK) / SIZE;
}
template <upL_t alignment, typename Type>
YM_FORCE_INLINE upL_t ptr_to_aligned(Type* ptr){
    constexpr upL_t SIZE = SizeOf<Type>::value;
    static_assert((SIZE & (SIZE - 1)) == 0, "sizeof(Type) must be a power-of-two.");
    static_assert((alignment & (alignment - 1)) == 0, "Alignment must be a power-of-two.");
    constexpr upL_t ALIGN = SIZE > alignment ? SIZE : alignment;
    constexpr upL_t MASK = ALIGN - 1;
    return ((0 - (upL_t)ptr) & MASK) / SIZE;
}
template <typename Type>
YM_FORCE_INLINE upL_t ptr_past_aligned_k(ukL_t alignment_k, Type* ptr){
    constexpr upL_t SIZE = SizeOf<Type>::value;
    static_assert((SIZE & (SIZE - 1)) == 0, "sizeof(Type) must be a power-of-two.");
    upL_t alignment = (upL_t)1 << alignment_k;
    upL_t align = SIZE > alignment ? SIZE : alignment;
    upL_t mask = align - 1;
    return ((upL_t)ptr & mask) / SIZE;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Align an Integer
template <upL_t alignment, typename LengthType>
YM_FORCE_INLINE LengthType align_int_down(LengthType x){
    static_assert(std::is_unsigned<LengthType>::value, "Length must be an unsigned integer.");
    static_assert((alignment & (alignment - 1)) == 0, "Alignment must be a power-of-two.");
    constexpr LengthType MASK = alignment - 1;
    return x & ~MASK;
}
template <upL_t alignment, typename LengthType>
YM_FORCE_INLINE LengthType align_int_up(LengthType x){
    static_assert(std::is_unsigned<LengthType>::value, "Length must be an unsigned integer.");
    static_assert((alignment & (alignment - 1)) == 0, "Alignment must be a power-of-two.");
    constexpr LengthType MASK = alignment - 1;
    return (x + MASK) & ~MASK;
}
template <typename LengthType>
YM_FORCE_INLINE LengthType align_int_down_k(ukL_t alignment_k, LengthType x){
    static_assert(std::is_unsigned<LengthType>::value, "Length must be an unsigned integer.");
    upL_t alignment = (upL_t)1 << alignment_k;
    LengthType MASK = alignment - 1;
    return x & ~MASK;
}
template <typename LengthType>
YM_FORCE_INLINE LengthType align_int_up_k(ukL_t alignment_k, LengthType x){
    static_assert(std::is_unsigned<LengthType>::value, "Length must be an unsigned integer.");
    upL_t alignment = (upL_t)1 << alignment_k;
    LengthType mask = alignment - 1;
    return (x + mask) & ~mask;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Align a Word Length
template <typename WordType, upL_t byte_alignment, typename LengthType>
YM_FORCE_INLINE LengthType align_length_down(LengthType length){
    constexpr upL_t SIZE = SizeOf<WordType>::value;
    static_assert(std::is_unsigned<LengthType>::value, "Length must be an unsigned integer.");
    static_assert((SIZE & (SIZE - 1)) == 0, "sizeof(WordType) must be a power-of-two.");
    static_assert((byte_alignment & (byte_alignment - 1)) == 0, "Alignment must be a power-of-two.");
    constexpr LengthType ALIGN = SIZE > byte_alignment ? SIZE : byte_alignment;
    constexpr LengthType MASK = ALIGN / SIZE - 1;
    return length & ~MASK;
}
template <typename WordType, upL_t byte_alignment, typename LengthType>
YM_FORCE_INLINE LengthType align_length_up(LengthType length){
    constexpr upL_t SIZE = SizeOf<WordType>::value;
    static_assert(std::is_unsigned<LengthType>::value, "Length must be an unsigned integer.");
    static_assert((SIZE & (SIZE - 1)) == 0, "sizeof(WordType) must be a power-of-two.");
    static_assert((byte_alignment & (byte_alignment - 1)) == 0, "Alignment must be a power-of-two.");
    constexpr LengthType ALIGN = SIZE > byte_alignment ? SIZE : byte_alignment;
    constexpr LengthType MASK = ALIGN / SIZE - 1;
    return (length + MASK) & ~MASK;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Align a Pointer
template <upL_t alignment, typename Type>
YM_FORCE_INLINE Type* align_ptr_down(Type* ptr){
    return (Type*)align_int_down<alignment>((upL_t)ptr);
}
template <upL_t alignment, typename Type>
YM_FORCE_INLINE Type* align_ptr_up(Type* ptr){
    return (Type*)align_int_up<alignment>((upL_t)ptr);
}
template <typename Type>
YM_FORCE_INLINE Type* align_ptr_up_k(ukL_t alignment_k, Type* ptr){
    return (Type*)align_int_up_k(alignment_k, (upL_t)ptr);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Run-time Alignment (deprecated)
template <typename T, typename A>
YM_FORCE_INLINE T align_int_up(const T& x, A align){
    static_assert(std::is_integral<T>::value, "Integer type required.");
    static_assert(std::is_integral<A>::value, "Integer type required.");
    const T MASK = align - 1;
    return (x + MASK) & ~MASK;
}
//template <typename T, typename A>
//YM_FORCE_INLINE T align_int_down(const T& x, A align){
//    static_assert(std::is_integral<T>::value, "Integer type required.");
//    static_assert(std::is_integral<A>::value, "Integer type required.");
//    const T MASK = align - 1;
//    return x & ~MASK;
//}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
