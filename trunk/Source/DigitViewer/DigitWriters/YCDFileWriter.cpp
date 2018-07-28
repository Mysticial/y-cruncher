/* YCDFileWriter.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/28/2013
 * Last Modified    : 07/29/2013
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string.h>
#include "PublicLibs/SystemLibs/FileIO/FileException.h"
#include "DigitViewer/DigitConverter/DigitConverter.h"
#include "DigitViewer/Globals.h"
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
//  Helpers
void write_uL_max(char* str, uiL_t x){
    upL_t c = YC_DIGITWRITER_MAX_DIGIT_WIDTH;
    while (x != 0){
        str[--c] = (char)(x % 10) + '0';
        x /= 10;
    }
    while (c > 0){
        str[--c] = '0';
    }
}
std::string to_string_max(uiL_t x){
    char str[YC_DIGITWRITER_MAX_DIGIT_WIDTH + 1];
    str[YC_DIGITWRITER_MAX_DIGIT_WIDTH] = '\0';
    write_uL_max(str, x);
    return std::string(str);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Rule of 5
YCDFileWriter::YCDFileWriter()
    : buffered(0)
{}
void YCDFileWriter::operator=(YCDFileWriter&& x){
    close();

    path                = std::move(x.path);
    file                = std::move(x.file);
    radix               = x.radix;
    digits_per_word     = x.digits_per_word;
    fp_convert          = x.fp_convert;
    words_per_file      = x.words_per_file;
    digits_per_file     = x.digits_per_file;
    file_id             = x.file_id;
    offset_total_digits = x.offset_total_digits;
    pos_word            = x.pos_word;
    pos_char            = x.pos_char;
    buffered            = x.buffered;
    memcpy(str_buffer, x.str_buffer, sizeof(str_buffer));
}
YCDFileWriter::~YCDFileWriter(){
    close();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Constructors
YCDFileWriter::YCDFileWriter(
    std::string path_,
    const std::string& first_digits,
    ufL_t digits_per_file_,
    uiL_t fileid_,
    int radix_
)
    : path(std::move(path_))
    , file(0, path)
    , radix(radix_)
    , digits_per_file(digits_per_file_)
    , file_id(fileid_)
    , pos_word(0)
    , pos_char(0)
    , buffered(0)
{
    //  Make the header
    std::string header;
    header += "#Compressed Digit File\r\n\r\n";

    header += YCF_CDF_TOKEN_FileVersion;
    header += '\t';
    header += YCF_CDF_FileVersion;
    header += "\r\n\r\n";

    header += YCF_CDF_TOKEN_Base;
    header += '\t';
    header += std::to_string(radix);
    header += "\r\n\r\n";

    header += YCF_CDF_TOKEN_FirstDigits;
    header += '\t';
    header += first_digits;
    header += "\r\n\r\n";

    header += YCF_CDF_TOKEN_TotalDigits;
    header += '\t';
    offset_total_digits = header.size();
    header += to_string_max(0);
    header += "\r\n\r\n";

    header += YCF_CDF_TOKEN_BlockSize;
    header += '\t';
    header += std::to_string(digits_per_file);
    header += "\r\n";

    header += YCF_CDF_TOKEN_BlockID;
    header += '\t';
    header += std::to_string(file_id);
    header += "\r\n\r\n";

    header += YCF_CDF_TOKEN_EndHeader;
    header += "\r\n\r\n";
    header += '\0';

    //  Write the header
    upL_t size = header.size();
    if (file.write(&header[0], size) != size){
        FileIO::PrintLastError();
        throw FileIO::FileException(
            "YCDFileWriter::YCDFileWriter()",
            path,
            "Error writing to file."
        );
    }

    //  Radix-specific things
    switch (radix){
        case 10:
            digits_per_word = 19;
            fp_convert = d19f_to_u64r;
            break;
        case 16:
            digits_per_word = 16;
            fp_convert = h16f_to_u64r;
            break;
        default:
            throw FileIO::FileException("YCDFileWriter::YCDFileWriter()", path_, "Unsupported Radix");
    }

    words_per_file = (digits_per_file - 1) / digits_per_word + 1;
    memset(str_buffer, 0, sizeof(str_buffer));
}
void YCDFileWriter::close(){
    //  The object isn't valid anyway.
    if (!file.is_open()){
        return;
    }

    //  Flush buffer
    flush();

    //  If the file isn't full, then it is the end-file.
    if (pos_char != digits_per_file){
        //  Set the total_digits field.
        file.set_ptr(offset_total_digits);
        std::string digit_string = to_string_max(digits_per_file * file_id + pos_char);
        if (file.write(digit_string.c_str(),
            YC_DIGITWRITER_MAX_DIGIT_WIDTH) != YC_DIGITWRITER_MAX_DIGIT_WIDTH
        ){
            //  Yes, this is dangerous if this is called from a destructor.
            //  Normally you can't recover from it anyway, but you can always
            //  call this function before you destruct.
            FileIO::PrintLastError();
            throw FileIO::FileException(
                "YCDFileWriter::close()",
                path,
                "Error writing to file."
            );
        }
    }

    //  Close the file
    file.close();
}
bool YCDFileWriter::isValid() const{
    return file.is_open();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void YCDFileWriter::write_words(u64_t* T, upL_t L){
    if (pos_word + L > words_per_file)
        throw FileIO::FileException(
            "YCDFileWriter::write_words()",
            file.GetPath(),
            "Attempted to write beyond the range of this file."
        );

    upL_t bytes = L * sizeof(u64_t);
    if (file.write(T, bytes) != bytes){
        FileIO::PrintLastError();
        throw FileIO::FileException(
            "YCDFileWriter::write_words()",
            path,
            "Error writing to file."
        );
    }
}
upL_t YCDFileWriter::write_chars(
    const char* str, upL_t digits,
    u64_t* buffer, upL_t buffer_L
){
    //  Write digits to the file.
    //  This method returns the number of digits that are actually written.
    //  If the return value is less than "digits", this implies that the end of
    //  the file has been reached. So the caller must continue by creating the
    //  next file and calling with (str + return value) and (digits - return value).

    //  If the end of the file is reached, the file is closed.

    if (!file.is_open()){
        throw FileIO::FileException(
            "YCDFileWriter::write_chars()",
            path,
            "This file is already closed."
        );
    }

    upL_t start_digits = digits;

    while (digits > 0){
        //  The file is full
        if (pos_char == digits_per_file){
            close();
            break;
        }

        //  Under some conditions, we must push 1 digit at a time.
        //  Entering this statement will push exactly 1 digit and continue the loop.
        uiL_t file_left = digits_per_file - pos_char;
        if (buffered != 0 ||                //  Buffer is not empty
            digits < digits_per_word ||     //  Digits is less then the word-size
            file_left < digits_per_word     //  The remainder of the file is less than the word-size
        ){
            //  Push the digit
            str_buffer[buffered++] = *str++;
            digits--;
            pos_char++;

            //  Buffer is full. Flush it.
            if (buffered == digits_per_word){
                u64_t tmp;
                fp_convert(str_buffer, &tmp, 1);
                write_words(&tmp, 1);
                buffered = 0;
            }
            continue;
        }

        //  At this point, the buffer is empty. This means that it is aligned
        //  to the word. Furthermore, there is more than one word to process.

        //  This makes it possible to run the fast converter.

        //  Convert this many digits this iteration.
        upL_t current_digits = digits;

        //  Don't overrun the file.
        if (current_digits > file_left){
            current_digits = (upL_t)file_left;
        }

        //  Get # of words.
        upL_t words = current_digits / digits_per_word;

        //  Don't overrun the buffer.
        if (words > buffer_L){
            words = buffer_L;
        }

        current_digits = words * digits_per_word;

        //  Convert
        fp_convert(str, buffer, words);
        write_words(buffer, words);

        str      += current_digits;
        digits   -= current_digits;
        pos_char += current_digits;
    }

    return start_digits - digits;
}
void YCDFileWriter::flush(){
    //  Should only be called at the end of a file.

    //  Nothing buffered
    if (buffered == 0){
        return;
    }

    //  Write zeros to rest of buffer
    while (buffered < digits_per_word){
        str_buffer[buffered++] = 0;
    }

    //  Flush
    u64_t tmp;
    fp_convert(str_buffer, &tmp, 1);
    write_words(&tmp, 1);
    buffered = 0;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
