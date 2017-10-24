/* YCDWriter.h - .ycd Writer Object
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/27/2013
 * Last Modified    : 07/27/2013
 * 
 * 
 *      The YCDWriter object needs yet another internal buffer that is
 *  independent of the digit buffer in DigitWriter.
 * 
 *  This buffer is needed to load the raw compressed data from the file. Then
 *  it is decompressed into the destination.
 *  
 */

#pragma once
#ifndef ycr_YCDWriter_H
#define ycr_YCDWriter_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "DigitWriter.h"
#include "YCDFileWriter.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
namespace DigitViewer{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class YCDWriter : public DigitWriter{
public:
    virtual ~YCDWriter();
    virtual std::unique_ptr<DigitReader> close_and_get_reader(upL_t buffer_size) override;

    //  Create a new writer.

    //  digits_per_file:
    //      (uiL_t)0 - 1    =   All digits in one file. Don't split.

    //  buffer_size:    Size of internal buffer.
    //  buffer:         Use a preallocated buffer?
    //      If you pass anything non-NULL for this parameter, it will use it as
    //      the internal buffer with size buffer_size bytes. It will not be
    //      freed upon destruction of the object.
    //  deallocator:    Deallocator. This will we called on "buffer" if it
    //      is preallocated. It will not be called if it is NULL, or if
    //      "buffer" is not preallocated.
    YCDWriter(
        std::string path,   //  UTF-8
        std::string name,   //  UTF-8
        std::string first_digits,
        ufL_t digits_per_file = (ufL_t)0 - 1,
        uiL_t start_fileid = 0,
        int radix = 10,
        upL_t buffer_size = YC_DIGITWRITER_DEFAULT_BUFFER,
        u64_t* buffer = nullptr,
        void (*deallocator)(void*) = nullptr
    );

    virtual void write(char* str, upL_t digits) override;

private:
    std::string m_path;                 //  Full path (including the id #)
    std::string m_name;                 //  File name

    int m_radix;                        //  Radix of the digits. (10 or 16)
//    upL_t m_digits_per_word;
    uiL_t m_start_fileid;

    std::string m_first_digits;         //  First digits

    ufL_t m_digits_per_file;            //  # of digits per compressed file

    //  File handle
    uiL_t m_fileid;
    YCDFileWriter m_file;

    //  Binary Buffer
    bool m_external_buffer;
    u64_t* m_bin_buffer;
    upL_t m_bin_buffer_L;
    void (*fp_free)(void*);

    void free_buffer();
    std::string make_filename(uiL_t fileid);
    void create_file(uiL_t fileid);
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
