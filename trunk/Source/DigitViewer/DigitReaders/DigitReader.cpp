/* DigitReader.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/22/2013
 * Last Modified    : 07/28/2013
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Exceptions/StringException.h"
#include "PublicLibs/FileIO/FileException.h"
#include "DigitViewer/Globals.h"
#include "DigitReader.h"
#include "TextReader.h"
#include "YCDReader.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
namespace DigitViewer{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DigitReader::DigitReader()
    : buffer_L(0)
    , iter_f_offset(0)
    , iter_b_offset(0)
{
}
void DigitReader::clear_buffer(){
    //  To clear the buffer, set the offset to the size of the buffer.
    //  This will trick the next digit load into thinking that all the digits
    //  in the buffer have been used up. Thereby, forcing it to reload the
    //  buffer.
    iter_b_offset = buffer_L;
}
void DigitReader::set_pos(uiL_t offset){
    uiL_t total_digits = get_digits();
    if (total_digits != 0 && offset > total_digits){
        throw StringException("DigitReader::set_pos()", "Out of range.");
    }

    //  Set the offset
    iter_f_offset = offset;

    //  Setting the position will invalidate the buffer.
    clear_buffer();
}
void DigitReader::read(char* str, upL_t digits){
    while (digits > 0){
        //  Buffer is empty
        if (iter_b_offset == buffer_L){
            reload();
        }

        upL_t block = buffer_L - iter_b_offset;
        if (block > digits)
            block = digits;

        memcpy(str, &buffer[0] + iter_b_offset, block);
        
        iter_f_offset += block;
        iter_b_offset += block;
        str += block;
        digits -= block;
    }
}
YM_NO_INLINE void DigitReader::reload(){
    //  Reloads the buffer.

    //  Regardless of whether this method throws, it will leave the buffer in
    //  a consistent state.

    //  Buffer isn't initialized yet.
    if (buffer_L == 0){
        upL_t buffer_size = YC_DIGITREADER_DEFAULT_BUFFER;
        buffer = std::unique_ptr<char[]>(new char[buffer_size]);

        //  Do this assignment last - just in case the above throws.
        buffer_L = buffer_size;
    }

    //  Wipe the buffer now. This will keep the object in a consistent state
    //  in the event of an exception.
    clear_buffer();

    uiL_t total_digits = get_digits();
    if (total_digits == 0){
        throw StringException(
            "DigitReader::reload()", 
            "DigitReader::reload() does not support unknown sizes.\n"
            "You must override this function when total_digits could be 0."
        );
    }

    upL_t read  = buffer_L;
    uiL_t start = iter_f_offset;
    uiL_t end   = iter_f_offset + read;
    if (start >= total_digits){
        std::string error("No more digits left: ");
        error += std::to_string(total_digits);
        throw StringException("DigitReader::reload()", std::move(error));
    }

    //  Near the end. Read the rest and fill only part of the buffer.
    upL_t current_b_offset = 0;
    if (end > total_digits){
        read = (upL_t)(total_digits - start);
        current_b_offset = buffer_L - read;
    }

    //  Read into buffer
    this->read(start, &buffer[current_b_offset], read);

    //  Do this assignment last to protect against an exception.
    iter_b_offset = current_b_offset;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Factory
std::unique_ptr<DigitReader> OpenDigitFile(std::string path, bool raw, upL_t buffer_size){
    //  Extract the extension
    size_t extension_offset = path.rfind('.');
    if (extension_offset >= path.size()){
        throw FileIO::FileException("OpenDigitFile()", path, "No Extension found.");
    }
    std::string extension = path.substr(extension_offset);

    //  Use RTTI.
    if (extension == ".txt"){
        return std::unique_ptr<DigitReader>(new TextReader(path, raw, 0));
    }else if (extension == ".ycd"){
        return std::unique_ptr<DigitReader>(new YCDReader(std::move(path), raw, buffer_size));
    }else{
        throw FileIO::FileException("OpenDigitFile()", path, "Unrecognized Extension: " + extension);
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
