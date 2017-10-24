/* DigitWriter.h - File Writer Object Interface
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/03/2012
 * Last Modified    : 07/28/2013
 * 
 * 
 *  This is an abstract class that represents a digit writer that streams digits
 *  from a source.
 * 
 *  This the parent class of:
 *      -   TextWriter  (write digits to the simple .txt files)
 *      -   YCDWriter   (write digits to the compressed format .ycd files)
 * 
 *  Each of these classes have their own constructors. But once created they
 *  share this common interface for all digit writing purposes.
 * 
 * 
 * 
 *  As a general note, these writer classes do not check if the input digits are
 *  valid. If any digits are not valid, the behavior is undefined.
 * 
 * 
 * 
 *  Note that unlike the DigitReader classes, the DigitWriter classes are not
 *  fault-tolerant. If any write operation fails or throws an exception, the
 *  entire digit stream will be left in an inconsistent state. The only way to
 *  recover from this is to delete the file(s) and start over.
 * 
 *  Perhaps a future version will fix this. A possible solution is to manually
 *  keep track of the stream offsets and call setpos() at the start of every
 *  write operation. This will overwrite any partially completed write that
 *  previously failed (and triggered an exception) for whatever reason.
 */

#pragma once
#ifndef ycr_DigitWriter_H
#define ycr_DigitWriter_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string>
#include <memory>
#include "PublicLibs/Types.h"
#include "PublicLibs/FileIO/FileIO.h"
namespace DigitViewer{
    using namespace ymp;
    class DigitReader;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define YC_DIGITWRITER_DEFAULT_BUFFER   ((upL_t)64 << 20)
class DigitWriter{
    DigitWriter(const DigitWriter&) = delete;
    void operator=(const DigitWriter&) = delete;

public:
    DigitWriter();
    virtual ~DigitWriter();
    virtual std::unique_ptr<DigitReader> close_and_get_reader(upL_t buffer_size = YC_DIGITWRITER_DEFAULT_BUFFER) = 0;

    /*  Buffered Stream Methods
     * 
     *  The following 3 methods are stream write operations.
     * 
     *  These methods write to an internal buffer. This buffer is flushed
     *  automatically when it is full, or when flush() is called.
     * 
     *  This internal buffer is instantiated upon first use. If these methods
     *  are never used, the buffer is never made. So this no memory will be
     *  wasted if you only intend to use the raw write() method.
     */

    //  Write 1 digit to the file.
    void push(char digit);

    //  Write N digits to the file.
    void push(const char* str, upL_t digits);

    //  Flush the internal buffer.
    void flush_buffer();
    void make_or_flush_buffer();


    /*  Writes N digits to the file without buffering.
     * 
     *  This function does not use the internal buffer. It writes directly to
     *  the file.
     * 
     *  In order to prevent the file from entering an inconsistent state, the
     *  internal buffer will be flushed if it isn't empty. Only then will it
     *  perform this write operation.
     * 
     *  Note that "str" is not declared const. This allows the the implementation
     *  to destroy it if necessary to write the digits. This is useful if the
     *  digits need to be reformatted in-place before writing to disk.
     * 
     *  This is the DigitWriter counterpart to DigitReader's non-buffered read.
     *  So the same performance guidelines apply. Use this only when "digits"
     *  is very large.
     */
    virtual void write(char* str, upL_t digits) = 0;

private:
    void make_buffer();

protected:
    //  Digit Buffer
    //  Can't use std::vector here. We need this to be properly aligned.
    char* m_buffer;
    upL_t m_buffer_L;

    //  Iterator
    ufL_t m_iter_f_offset;
    upL_t m_iter_b_offset;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
inline void DigitWriter::push(char digit){
    //  This is a VERY performance critical function call.
    //  So put the implementation here to help the compiler inline it.

    //  Buffer is full
    if (m_iter_b_offset == m_buffer_L){
        make_or_flush_buffer();
    }

    m_iter_f_offset++;
    m_buffer[m_iter_b_offset++] = digit;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
