/* RawFile_Linux.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 07/31/2011
 *  Last Modified   : 10/15/2023
 * 
 */

#pragma once
#ifndef ymp_FileIO_RawFile_Linux_H
#define ymp_FileIO_RawFile_Linux_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/BasicLibs/Concurrency/AsyncThrottler.h"
#include "PublicLibs/SystemLibs/FileIO/AlignedAccessFile.h"
namespace ymp{
namespace FileIO{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
enum Mode{
    CREATE,
    OPEN_READONLY,
    OPEN_READWRITE,
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class RawFile : public AlignedAccessFile{
    static const upL_t MAX_INFLIGHT = 32;
    static const upL_t MAX_IO_BYTES = (upL_t)1 << 30;

public:
    virtual ~RawFile() override;

    RawFile(RawFile&& x) noexcept;
    void operator=(RawFile&& x) noexcept;

    RawFile(const RawFile& x) = delete;
    void operator=(const RawFile& x) = delete;

public:
    //  Empty Handle
    RawFile();

    //  Create or open file.
    RawFile(
        ukL_t alignment_k,
        std::string path, Mode mode,
        bool persistent = true,
        bool raw_io = true
    );

    //  Create a file with a specific size.
    RawFile(
        ukL_t alignment_k,
        ufL_t bytes, std::string path,
        bool persistent,
        bool raw_io,
        bool read_protect   //  Doesn't do anything.
    );

public:
    explicit operator bool() const;     //  Returns true if file is open and valid.
    const std::string& path() const{ return m_path; }
    bool persistent() const{ return m_persistent; }
    bool raw_io() const{ return m_raw_io; }

    virtual void flush() override;
    void close(bool keep_file);
    void close_and_set_size(ufL_t bytes);
    virtual void rename(std::string path, bool readonly) override;

public:
    //  All parameters must be aligned to "2^m_alignment_k".
    // 
    //  If "throw_on_partial" is true, it will throw an exception if it is
    //  unable to read/write the full requested bytes. For reads, this may imply
    //  reaching the end of the file. For stores, this almost always implies a
    //  more serious error such as disk-is-full or a hardware failure.
    //
    //  These are all thread-safe with each other provided they do not overlap
    //  in memory or the file regions. However, these are not thread-safe with
    //  everything else in this class (such as "close", "rename", etc...)
    //
    //  Not only are these thread-safe, there may be performance gain to
    //  parallelizing I/O accesses.

    virtual upL_t load (      void* data, ufL_t offset, upL_t bytes, bool throw_on_partial) override;
    virtual upL_t store(const void* data, ufL_t offset, upL_t bytes, bool throw_on_partial) override;

private:
    void open(Mode mode);
    void set_size(ufL_t bytes);

private:
    int m_filehandle;
    bool m_persistent = true;
    bool m_raw_io = true;
    AsyncThrottler m_throttler;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
