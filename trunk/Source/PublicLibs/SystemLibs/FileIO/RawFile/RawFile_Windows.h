/* RawFile_Windows.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/20/2011
 * Last Modified    : 03/24/2018
 * 
 */

#pragma once
#ifndef ymp_FileIO_RawFile_Windows_H
#define ymp_FileIO_RawFile_Windows_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
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
    static const upL_t MAX_IO_BYTES = (upL_t)1 << 30;
    static const upL_t CHECK_MEM_THRESHOLD = (upL_t)64 << 20;

public:
    virtual ~RawFile() override;

    RawFile(const RawFile& x) = delete;
    void operator=(const RawFile& x) = delete;

public:
    RawFile(      //  Create or open file.
        ukL_t alignment_k,
        std::string path, Mode mode,
        bool persistent = true,
        bool raw_io = true
    );
    RawFile(      //  Create a file with a specific size.
        ukL_t alignment_k,
        ufL_t bytes, std::string path,
        bool persistent = false,
        bool raw_io = true
    );

    operator bool() const;
    const std::string& path() const{ return m_path; }
    bool persistent() const{ return m_persistent; }

    virtual void flush() override;
    void close(bool keep_file);
    void close_and_set_size(ufL_t bytes);
    virtual void rename(std::string path, bool readonly) override;

public:
    //  All parameters must be aligned to "2^m_alignment_k".
    //  If "throw_on_partial" is true, it will throw an exception if it is
    //  unable to read/write the full requested bytes. For reads, this may imply
    //  reaching the end of the file. For stores, this almost always implies a
    //  more serious error such as disk-is-full or a hardware failure.
    upL_t load (      void* data, ufL_t offset, upL_t bytes, bool throw_on_partial);
    upL_t store(const void* data, ufL_t offset, upL_t bytes, bool throw_on_partial);
    virtual void load (      void* data, ufL_t offset, upL_t bytes, void* P, upL_t PL) override;
    virtual void store(const void* data, ufL_t offset, upL_t bytes, void* P, upL_t PL) override;

private:
    void open(Mode mode);
    void set_size(ufL_t bytes);
    upL_t pick_buffer_size(upL_t bytes) const;

private:
    void* m_filehandle;
    std::wstring m_wpath;
    bool m_persistent;
    bool m_raw_io;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
