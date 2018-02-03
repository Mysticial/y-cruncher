/* DigitReader.h - File Reader Object Interface
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/06/2012
 * Last Modified    : 01/09/2018
 * 
 * 
 *  This is an abstract class that represents a digit reader that streams digits
 *  from a source.
 * 
 *  This the parent class of:
 *      -   TextReader  (reads the simple .txt files of plain-text digits)
 *      -   YCDReader   (reads the compressed digit .ycd format files)
 * 
 *  Each of these classes have their own constructors. But once created they
 *  share this common interface for all digit reading/streaming purposes.
 */

#pragma once
#ifndef ydv_DigitViewer_DigitReader_H
#define ydv_DigitViewer_DigitReader_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string>
#include <memory>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "DigitViewer/InjectableBuffer.h"
namespace DigitViewer{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define YC_DIGITREADER_DEFAULT_BUFFER   ((upL_t)64 << 20)
class DigitReader{
    DigitReader(const DigitReader&) = delete;
    void operator=(const DigitReader&) = delete;

public:
    DigitReader();
    virtual ~DigitReader(){}

    //  Terrible design. Blame it on y-cruncher's messed up memory micromanagement.
    virtual void set_native_buffer(InjectableBuffer<u64_t> buffer){}
    virtual void clear_native_buffer(){}

    virtual const char* get_extension() const = 0;

    /*  Iterator methods
     * 
     *  The following 3 methods act as iterators for streaming digits.
     * 
     *  Note that these iterator methods require an internal buffer that is
     *  instantiated upon first use. If these are never used, the buffer is
     *  is never made. So this no memory will be wasted if you only intend to
     *  use the raw read() method.
     */

    //  Sets the digit position of this reader.
    void set_pos(uiL_t offset);

    //  Returns the next digit.
    char next();

    //  Returns the next N digits. (Reads "digits" digits and stores them into "str".)
    //  For large N, this function is faster than calling "next()" N times.
    void read(char* str, upL_t digits);



    //  Prints out the contents of the reader. (For debugging)
    virtual void print() const = 0;

    //  Returns the radix of the digits. (10 or 16)
    virtual int get_radix() const = 0;

    //  Returns the # of digits in this digit stream. (0 = unknown)
    virtual uiL_t get_digits() const = 0;

    //  Enable/Disable raw output.
    virtual void set_raw(bool raw) = 0;

    //  Checks to see if the range [start, end) is accessible.
    virtual bool check_range(uiL_t start, uiL_t end) = 0;

    //  Returns the first few digits.
    virtual std::string get_first_digits(upL_t L) = 0;


    /*  Reads N digits starting from an offset of "pos".
     * 
     *  Note that this read function does not do internal buffering of the 
     *  decompressed digits. It reads directly from the disk into a raw buffer.
     *  Then it formats/decompresses them directly to "str".
     * 
     *  Since this function does not touch the internal buffer, it will not
     *  invalidate the iterator state.
     * 
     *  For performance reasons, the best way to use this function is:
     *      -   Grab the entire desired region of digits with a single call.
     *      -   Stream a very large number of digits with multiple calls where
     *          "digits" is large. (> ~50,000,000)
     * 
     *  This function should NOT be used to stream digits a little at a time.
     *  Use the other read() function for that purpose.
     * 
     * 
     *  Each call to this function usually involves multiple disk seeks and a
     *  large number of integer divisions. So it has very high calling overhead.
     * 
     *  But once this function is called, it has the highest steady-state
     *  conversion rate of all the read functions. So its main purpose is to
     *  stream a very large number of digits.
     * 
     *  While this function will work correctly for any value "digits", it is
     *  most efficient when "digits > ~50,000,000" - subject to the latency
     *  and sequential bandwidth of the hard drive.
     */
    virtual void read(uiL_t pos, char* str, upL_t digits) = 0;

protected:

    //  Digit Buffer
    std::unique_ptr<char[]> m_digit_buffer;
    upL_t buffer_L;

    //  Iterator
    uiL_t iter_f_offset;
    upL_t iter_b_offset;

    //  Reload the buffer
    //  If the subclass supports unknown # of digits (get_digits() == 0),
    //  then it MUST override this function.
    virtual void reload();

    //  Clear the buffer
    void clear_buffer();
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE char DigitReader::next(){
    //  This is a VERY performance critical function call.
    //  So put the implementation here to help the compiler inline it.

    //  Buffer is empty
    if (iter_b_offset == buffer_L){
        reload();
    }
    
    iter_f_offset++;
    return m_digit_buffer[iter_b_offset++];
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Digit Viewer Factory
//      Call this on any digit file and it will return the digit viewer for it.
std::unique_ptr<DigitReader> OpenDigitFile(
    std::string path,
    bool raw = false,
    upL_t buffer_size = YC_DIGITREADER_DEFAULT_BUFFER
);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
