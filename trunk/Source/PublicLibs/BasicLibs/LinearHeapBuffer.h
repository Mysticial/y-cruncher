/* LinearHeapBuffer.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 09/08/2018
 *  Last Modified   : 09/08/2018
 * 
 */

#pragma once
#ifndef ymp_LinearBuffers_LinearHeapBuffer_H
#define ymp_LinearBuffers_LinearHeapBuffer_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/Exceptions/BufferTooSmallThrower.h"
#include "PublicLibs/Exceptions/InvalidParametersThrower.h"
#include "PublicLibs/BasicLibs/Alignment/DefaultAlignment.h"
#include "PublicLibs/BasicLibs/TemplateTools/SizeOf.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Words only. No alignment enforcement.
template <typename Type>
class LinearWordBuffer{
public:
    YM_FORCE_INLINE LinearWordBuffer()
        : m_ptr(nullptr)
        , m_len(0)
    {}
    YM_FORCE_INLINE LinearWordBuffer(Type* ptr, upL_t L)
        : m_ptr(ptr)
        , m_len(L)
    {}

    YM_FORCE_INLINE operator Type*() const{ return m_ptr; }
    YM_FORCE_INLINE Type* ptr() const{ return m_ptr; }
    YM_FORCE_INLINE Type* end() const{ return m_ptr + m_len; }
    YM_FORCE_INLINE upL_t len() const{ return m_len; }


public:
    //  Cut out a new buffer from the bottom of the current one.
    //  The current buffer will be shrunk to remove that bottom portion.
    template <typename SizeType>
    LinearWordBuffer cut_from_bottom(SizeType words){
        check_BufferTooSmall("LinearWordBuffer::cut_from_bottom()", m_len, words);
        upL_t length = (upL_t)words;
        Type* ptr = (Type*)m_ptr;
        m_ptr += length;
        m_len -= length;
        return LinearWordBuffer(ptr, length);
    }


private:
    Type* m_ptr;
    upL_t m_len;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Compile-time Alignment
template <upL_t ALIGNMENT = DEFAULT_ALIGNMENT>
class AlignedBufferC{
    static_assert((ALIGNMENT & (ALIGNMENT - 1)) == 0, "Alignment must be a power-of-two.");
    static const upL_t MASK = ALIGNMENT - 1;

public:
    YM_FORCE_INLINE AlignedBufferC()
        : m_ptr(nullptr)
        , m_len(0)
    {}
    YM_FORCE_INLINE AlignedBufferC(void* ptr, upL_t L)
        : m_ptr((char*)ptr)
        , m_len(L)
    {
        if ((upL_t)ptr & MASK){
            throw_InvalidParametersException("AlignedBufferC::AlignedBufferC()", "Pointer is misaligned.");
        }
    }

    template <typename Type>
    YM_FORCE_INLINE explicit operator Type*() const{ return (Type*)m_ptr; }
    YM_FORCE_INLINE operator void*() const{ return m_ptr; }
//    YM_FORCE_INLINE explicit operator void*() const{ return m_ptr; }
    YM_FORCE_INLINE void* ptr() const{ return m_ptr; }
    YM_FORCE_INLINE void* end() const{ return m_ptr + m_len; }
    YM_FORCE_INLINE upL_t bytes() const{ return m_len; }


public:
    //  Convert this buffer into a typed raw-pointer with size checking.
    template <typename WordType, typename SizeType>
    WordType* extract(SizeType words) const{
        check_BufferTooSmall("AlignedBufferC::extract()", m_len, words * sizeof(WordType));
        return (WordType*)m_ptr;
    }

    //  Return a new AlignedBuffer with a new alignment.
    template <upL_t NEW_ALIGNMENT>
    AlignedBufferC<NEW_ALIGNMENT> align() const{
        static_assert((NEW_ALIGNMENT & (NEW_ALIGNMENT - 1)) == 0, "Alignment must be a power-of-two.");
        if constexpr (NEW_ALIGNMENT < ALIGNMENT){
            return AlignedBufferC<NEW_ALIGNMENT>(m_ptr, m_len);
        }
        const upL_t NEW_MASK = NEW_ALIGNMENT - 1;
        char* ptr = (char*)(((upL_t)m_ptr + NEW_MASK) & ~NEW_MASK);
        char* end = m_ptr + m_len;
        if (ptr > end){
            throw_BufferTooSmallException("AlignedBufferC::extract_aligned()", m_len, ptr - m_ptr);
        }
        return AlignedBufferC<NEW_ALIGNMENT>(ptr, end - ptr);
    }


public:
    //  Cut out a new buffer from the bottom of the current one.
    //  The current buffer will be shrunk to remove that bottom portion.

    template <typename SizeType>
    AlignedBufferC spawn(SizeType bytes){
        check_BufferTooSmall("AlignedBufferC::spawn()", m_len, bytes);
        upL_t length = (upL_t)bytes;
        if (length & MASK){
            throw_InvalidParametersException("AlignedBufferC::spawn()", "Size is misaligned.");
        }
        char* ptr = m_ptr;
        m_ptr = ptr + length;
        m_len -= length;
        return AlignedBufferC(ptr, length);
    }

    template <typename SubType, typename SizeType>
    LinearWordBuffer<SubType> spawn_words(SizeType words){
        static_assert(alignof(SubType) <= ALIGNMENT, "SubType has greater alignment than buffer type.");
        upL_t length = (upL_t)words;
        upL_t bytes = length * sizeof(SubType);
        check_BufferTooSmall("AlignedBufferC::spawn_words()", m_len, bytes);
        bytes = (bytes + MASK) & ~MASK;
        bytes = bytes > m_len ? m_len : bytes;
        SubType* ptr = (SubType*)m_ptr;
        m_ptr = (char*)m_ptr + bytes;
        m_len -= bytes;
        return LinearWordBuffer<SubType>(ptr, length);
    }

    template <typename SubType, typename SizeType>
    SubType* spawn_objects(SizeType objects){
        upL_t bytes = (upL_t)objects * sizeof(SubType);
        bytes = (bytes + MASK) & ~MASK;
        check_BufferTooSmall("AlignedBufferC::spawn_words()", m_len, bytes);
        SubType* ptr = (SubType*)m_ptr;
        m_ptr = (char*)m_ptr + bytes;
        m_len -= bytes;
        return ptr;
    }

    template <typename SubType>
    LinearWordBuffer<SubType> to_words() const{
        AlignedBufferC copy = *this;
        return copy.spawn_words<SubType>(m_len / sizeof(SubType));
    }


private:
    char* m_ptr;
    upL_t m_len;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
