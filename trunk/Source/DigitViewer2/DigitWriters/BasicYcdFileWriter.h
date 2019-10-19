/* BasicYcdFileWriter.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/04/2018
 * Last Modified    : 03/26/2018
 * 
 *      This writer uses raw (unbuffered) I/O.
 * 
 *  The advantages of raw disk I/O are:
 *      -   Faster for bulk transfers because it eliminates the OS memcpy().
 *      -   Saves memory since the OS doesn't need to allocate a buffer.
 *      -   Prevents the OS from doing any stupid caching that may lead to
 *          the pagefile Thrash of Death.
 *
 *  The disadvantage is that the buffer needs to be larger and must satisfy
 *  stricter alignment requirements. It is also more difficult to implement.
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_BasicYcdFileWriter_H
#define ydv_DigitViewer_BasicYcdFileWriter_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <mutex>
#include "PublicLibs/BasicLibs/SparseRegion.h"
#include "PublicLibs/SystemLibs/FileIO/RawFile/RawFile.h"
#include "BasicDigitWriter.h"
namespace DigitViewer2{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class BasicYcdFileWriter : public BasicDigitWriter{
    static const upL_t FILE_ALIGNMENT = FileIO::DEFAULT_FILE_ALIGNMENT;

public:
    BasicYcdFileWriter(
        const std::string& path,    //  UTF-8
        const std::string& first_digits,
        char radix,
        ufL_t digits_per_file, uiL_t stream_end,
        uiL_t fileid
    );
    ~BasicYcdFileWriter();

    virtual upL_t recommend_buffer_size(uiL_t digits, upL_t limit) const override;
    virtual std::unique_ptr<BasicDigitReader> close_and_get_basic_reader() override;

    uiL_t file_id() const{ return m_file_id; }
    const std::string& path() const{ return m_path; }

    virtual void store_digits(
        const char* input,
        uiL_t offset, upL_t digits,
        const AlignedBufferC<BUFFER_ALIGNMENT>& buffer,
        BasicParallelizer& parallelizer, upL_t tds
    ) override;


    void print() const; //  Debugging


private:
    u64_t* get_range(
        ufL_t& write_offset, upL_t& write_bytes,
        ufL_t word_offset, upL_t words,
        void* P, upL_t Pbytes
    );
    void store_digits_B(
        const char* input,
        ufL_t offset, upL_t digits,
        void* P, upL_t Pbytes,
        BasicParallelizer& parallelizer, upL_t tds
    );

    upL_t start_access(
        uiL_t& offset, uiL_t digits,
        void* P, upL_t& Pbytes
    ) const;


private:
    using ConvertForward = bool (*)(u64_t*, const char*, upL_t);
    using ConvertInverse = void (*)(char*, const u64_t*, upL_t);

    std::mutex m_lock;

    std::string m_path;
    FileIO::RawFile m_file;

    uiL_t m_stream_end;
    ufL_t m_digits_per_file;
    uiL_t m_file_id;

    ConvertForward m_fp_convert_forward;
    ConvertInverse m_fp_convert_inverse;
    u64_t m_max_word;

    //  File offset of where the "stream_end" field is.
    //  This is needed because this value isn't determined until the file is
    //  closed. That's when the value is written. So we need to know where to
    //  seek back to. This is also the reason why this is the only zero-padded
    //  field in the compressed digit format.
    ufL_t m_offset_stream_end;

    upL_t m_digits_per_word;
    ufL_t m_words_in_this_file;
    ufL_t m_data_offset;        //  Offset where the actual data blocks begin.
    ufL_t m_offset_extent;      //  Largest offset written + 1.

    //  Keep track of which parts of the file have been written to.
    Region<uiL_t> m_target;
    SparseRegion<uiL_t> m_written;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
