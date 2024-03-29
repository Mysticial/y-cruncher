/* AlignedAccessFile.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 03/19/2018
 *  Last Modified   : 10/15/2023
 * 
 *      AlignedAccessFile is a random access file that requires all buffers,
 *  offsets, and lengths to be aligned to the sector. This is the basis for
 *  unbuffered raw I/O.
 * 
 */

#pragma once
#ifndef ymp_FileIO_AlignedAccessFile_H
#define ymp_FileIO_AlignedAccessFile_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string>
#include "PublicLibs/Types.h"
namespace ymp{
namespace FileIO{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const ukL_t DEFAULT_FILE_ALIGNMENT_K = 12;
const upL_t DEFAULT_FILE_ALIGNMENT = (upL_t)1 << DEFAULT_FILE_ALIGNMENT_K;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class AlignedAccessFile{
public:
    virtual ~AlignedAccessFile() = default;
    AlignedAccessFile(AlignedAccessFile&& x) noexcept;
    void operator=(AlignedAccessFile&& x) noexcept;

public:
    AlignedAccessFile() = default;
    AlignedAccessFile(ukL_t alignment_k, std::string path);

    ukL_t alignment_k() const{ return m_alignment_k; }
    const std::string& path() const{ return m_path; }

public:
    virtual void flush() = 0;
    virtual void rename(std::string path, bool readonly) = 0;

public:
    //  All parameters must be aligned to "2^m_alignment_k".
    virtual upL_t load (      void* data, ufL_t offset, upL_t bytes, bool throw_on_partial) = 0;
    virtual upL_t store(const void* data, ufL_t offset, upL_t bytes, bool throw_on_partial) = 0;

protected:
    void check_alignment(const void* data, ufL_t offset, upL_t bytes);

protected:
    ukL_t m_alignment_k = 0;
    std::string m_path;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
