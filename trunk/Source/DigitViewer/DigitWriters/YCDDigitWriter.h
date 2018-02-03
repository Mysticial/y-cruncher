/* YCDDigitWriter.h - .ycd Writer Object
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/27/2013
 * Last Modified    : 01/09/2018
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
#ifndef ydv_DigitViewer_YCDDigitWriter_H
#define ydv_DigitViewer_YCDDigitWriter_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "DigitViewer/InjectableBuffer.h"
#include "DigitWriter.h"
#include "YCDFileWriter.h"
namespace DigitViewer{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class YCDWriter : public DigitWriter{
public:
    virtual ~YCDWriter();
    virtual std::unique_ptr<DigitReader> close_and_get_reader() override;

    //  Create a new writer.

    //  digits_per_file:
    //      (uiL_t)0 - 1    =   All digits in one file. Don't split.
    YCDWriter(
        std::string path,   //  UTF-8
        std::string name,   //  UTF-8
        std::string first_digits,
        ufL_t digits_per_file = (ufL_t)0 - 1,
        uiL_t start_fileid = 0,
        int radix = 10,
        InjectableBuffer<u64_t> buffer = InjectableBuffer<u64_t>(YC_DIGITWRITER_DEFAULT_BUFFER / sizeof(u64_t))
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
    InjectableBuffer<u64_t> m_buffer;

    std::string make_filename(uiL_t fileid);
    void create_file(uiL_t fileid);
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
