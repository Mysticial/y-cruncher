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
YM_FORCE_INLINE upL_t int_past_aligned(LengthType x){
    static_assert(std::is_unsigned<LengthType>::value, "Length must be an unsigned integer.");
    static_assert((alignment & (alignment - 1)) == 0, "Alignment must be a power-of-two.");
    constexpr LengthType MASK = alignment - 1;
    return x & MASK;
}
template <upL_t alignment, typename LengthType>
YM_FORCE_INLINE upL_t int_to_aligned(LengthType x){
    static_assert(std::is_unsigned<LengthType>::value, "Length must be an unsigned integer.");
    static_assert((alignment & (alignment - 1)) == 0, "Alignment must be a power-of-two.");
    constexpr LengthType MASK = alignment - 1;
    return (0 - x) & MASK;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Determine word alignment of pointer.
//  "ptr" must already be aligned to its type. Otherwise the behavior is undefined.
template <upL_t alignment, typename Type>
YM_FORCE_INLINE upL_t ptr_past_aligned(Type* ptr){
    constexpr upL_t TYPE_ALIGN = SizeOf<Type>::value;
    static_assert((TYPE_ALIGN & (TYPE_ALIGN - 1)) == 0, "sizeof(Type) must be a power-of-two.");
    static_assert((alignment & (alignment - 1)) == 0, "Alignment must be a power-of-two.");
    constexpr upL_t ALIGN = TYPE_ALIGN > alignment ? TYPE_ALIGN : alignment;
    constexpr upL_t MASK = ALIGN - 1;
    return ((upL_t)ptr & MASK) / TYPE_ALIGN;
}
template <upL_t alignment, typename Type>
YM_FORCE_INLINE upL_t ptr_to_aligned(Type* ptr){
    constexpr upL_t TYPE_ALIGN = SizeOf<Type>::value;
    static_assert((TYPE_ALIGN & (TYPE_ALIGN - 1)) == 0, "sizeof(Type) must be a power-of-two.");
    static_assert((alignment & (alignment - 1)) == 0, "Alignment must be a power-of-two.");
    constexpr upL_t ALIGN = TYPE_ALIGN > alignment ? TYPE_ALIGN : alignment;
    constexpr upL_t MASK = ALIGN - 1;
    return ((0 - (upL_t)ptr) & MASK) / TYPE_ALIGN;
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
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Align a Word Length
template <typename WordType, upL_t byte_alignment, typename LengthType>
YM_FORCE_INLINE LengthType align_length_down(LengthType length){
    static_assert(std::is_unsigned<LengthType>::value, "Length must be an unsigned integer.");
    static_assert((sizeof(WordType) & (sizeof(WordType) - 1)) == 0, "sizeof(WordType) must be a power-of-two.");
    static_assert((byte_alignment & (byte_alignment - 1)) == 0, "Alignment must be a power-of-two.");
    constexpr LengthType ALIGN = sizeof(WordType) > byte_alignment ? sizeof(WordType) : byte_alignment;
    constexpr LengthType MASK = ALIGN / sizeof(WordType) - 1;
    return length & ~MASK;
}
template <typename WordType, upL_t byte_alignment, typename LengthType>
YM_FORCE_INLINE LengthType align_length_up(LengthType length){
    static_assert(std::is_unsigned<LengthType>::value, "Length must be an unsigned integer.");
    static_assert((sizeof(WordType) & (sizeof(WordType) - 1)) == 0, "sizeof(WordType) must be a power-of-two.");
    static_assert((byte_alignment & (byte_alignment - 1)) == 0, "Alignment must be a power-of-two.");
    constexpr LengthType ALIGN = sizeof(WordType) > byte_alignment ? sizeof(WordType) : byte_alignment;
    constexpr LengthType MASK = ALIGN / sizeof(WordType) - 1;
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
