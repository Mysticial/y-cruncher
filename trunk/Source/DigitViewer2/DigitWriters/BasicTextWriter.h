/* BasicTextWriter.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/03/2018
 * Last Modified    : 03/25/2018
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
#ifndef ydv_DigitViewer_BasicTextWriter_H
#define ydv_DigitViewer_BasicTextWriter_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <memory>
#include <mutex>
#include "PublicLibs/SystemLibs/FileIO/RawFile/RawFile.h"
#include "BasicDigitWriter.h"
namespace DigitViewer2{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class BasicTextWriter : public BasicDigitWriter{
    static const upL_t FILE_ALIGNMENT = FileIO::DEFAULT_FILE_ALIGNMENT;

public:
    BasicTextWriter(
        const std::string& path,    //  UTF-8
        const std::string& first_digits,
        char radix
    );
    ~BasicTextWriter();

    virtual upL_t recommend_buffer_size(uiL_t digits, upL_t limit) const override;
    virtual std::unique_ptr<BasicDigitReader> close_and_get_basic_reader() override;

    virtual void store_digits(
        const char* input,
        uiL_t offset, upL_t digits,
        const AlignedBufferC<BUFFER_ALIGNMENT>& buffer,
        BasicParallelizer& parallelizer, upL_t tds
    ) override;


private:
    using ConvertFunction = bool (*)(char* dec, const char* raw, upL_t digits);

    std::mutex m_lock;
    FileIO::RawFile m_file;

    ConvertFunction m_fp_convert;

    ufL_t m_data_offset;    //  Offset in the file of the first digit after the decimal place.
    ufL_t m_offset_extent;  //  Largest offset written + 1.
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
