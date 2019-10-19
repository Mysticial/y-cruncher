/* BasicTextReader.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/14/2018
 * Last Modified    : 03/20/2018
 * 
 *      This reader uses raw (unbuffered) I/O.
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
#ifndef ydv_DigitViewer_BasicTextReader_H
#define ydv_DigitViewer_BasicTextReader_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <mutex>
#include "PublicLibs/SystemLibs/FileIO/RawFile/RawFile.h"
#include "BasicDigitReader.h"
namespace DigitViewer2{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class BasicTextReader : public BasicDigitReader{
public:
    static const upL_t FILE_ALIGNMENT = FileIO::DEFAULT_FILE_ALIGNMENT;

    BasicTextReader(const std::string& path, char radix = 0);

    virtual std::string first_digits            () override;
    virtual uiL_t       stream_end              () const override{ return m_total_digits; }
    virtual bool        range_is_available      (uiL_t offset, uiL_t digits) override;
    virtual upL_t       recommend_buffer_size   (uiL_t digits, upL_t limit) const override;

    virtual void load_stats(
        DigitStats& stats,
        uiL_t offset, uiL_t digits,
        const AlignedBufferC<BUFFER_ALIGNMENT>& buffer,
        BasicParallelizer& parallelizer, upL_t tds
    ) override;
    virtual void load_digits(
        char* output,
        DigitStats* stats,
        uiL_t offset, upL_t digits,
        const AlignedBufferC<BUFFER_ALIGNMENT>& buffer,
        BasicParallelizer& parallelizer, upL_t tds
    ) override;


private:
    static const upL_t CACHE_BLOCK = 8192;

    void process(
        DigitStats* stats, char* output, const char* txt_digits, upL_t digits
    ) const;
    void process(
        DigitStats* stats, char* output, const char* txt_digits, upL_t digits,
        BasicParallelizer& parallelizer, upL_t tds
    ) const;

    class Action_process;


private:
    using ConvertFunction = bool (*)(char* raw_digits, const char* buffer, upL_t digits);

    std::mutex m_lock;
    FileIO::RawFile m_file;         //  File handle

    ConvertFunction m_fp_convert;

    ufL_t m_data_offset;            //  Offset in the file of the first digit after the decimal place.
    ufL_t m_total_digits;           //  Digits after the decimal place.
    std::string m_first_digits;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
