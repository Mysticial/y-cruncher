/* YCDFile.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/13/2013
 * Last Modified    : 07/28/2013
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string.h>
#include <algorithm>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/Exceptions/StringException.h"
#include "PublicLibs/SystemLibs/FileIO/FileIO.h"
#include "PublicLibs/SystemLibs/FileIO/FileException.h"
#include "DigitViewer/Globals.h"
#include "DigitViewer/DigitReaders/ParsingTools.h"
#include "YCDFileReader.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
namespace DigitViewer{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Rule of 5
void YCDFileReader::operator=(YCDFileReader&& x){
    file.close();
    path                = std::move(x.path);
    file                = std::move(x.file);
    file_version        = std::move(x.file_version);
    radix               = x.radix;
    first_digits        = std::move(x.first_digits);
    total_digits        = x.total_digits;
    digits_per_file     = x.digits_per_file;
    file_id             = x.file_id;
    digits_per_word     = x.digits_per_word;
    words_in_this_file  = x.words_in_this_file;
    data_offset         = x.data_offset;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Constructors
YCDFileReader::YCDFileReader(std::string path_)
    : path(std::move(path_))
    , file(path)
{
    //  Parse the file

    //  Start after the next newline.
    {
        char ch;
        do{
            if (file.read(&ch, 1) == 0){
                FileIO::PrintLastError();
                throw FileIO::FileException("YCDFileReader::YCDFileReader()", path, "Invalid File Format");
            }
        }while (ch != '\n');
    }

    //  Parse header info
    while (1){
        std::string token = grab_until_delim(file, '\n');

        //  Empty line
        if (token.size() == 0){
            continue;
        }

        //  End of header section
        if (token == YCF_CDF_TOKEN_EndHeader){
            break;
        }

        //  Break up the token.
        std::string key, value;

        //  Parse the key
        const char* ptr = token.c_str();
        ptr = grab_until_delim(key, ptr, '\t');

        //  Parse the value
        grab_until_delim(value, ptr, '\n');

        //  file_version
        if (key == YCF_CDF_TOKEN_FileVersion){
            file_version = std::move(value);
            continue;
        }

        //  radix
        if (key == YCF_CDF_TOKEN_Base){
            radix = (int)parse_uL(value.c_str());
            continue;
        }

        //  first_digits
        if (key == YCF_CDF_TOKEN_FirstDigits){
            first_digits = std::move(value);
            continue;
        }

        //  total_digits
        if (key == YCF_CDF_TOKEN_TotalDigits){
            total_digits = parse_uL(value.c_str());
            continue;
        }

        //  digits_per_file
        if (key == YCF_CDF_TOKEN_BlockSize){
            digits_per_file = (ufL_t)parse_uL(value.c_str());
            continue;
        }

        //  file_id
        if (key == YCF_CDF_TOKEN_BlockID){
            file_id = parse_uL(value.c_str());
            continue;
        }
    }

    //  Find offset
    //  This isn't efficient. It sets the file pointer back to zero and counts
    //  to the first null character.
    file.set_ptr(0);
    char ch;
    ufL_t c = 0;
    while (1){
        if (file.read(&ch, 1) == 0){
            throw FileIO::FileException("YCDFileReader::YCDFileReader()", path, "Error Reading File");
        }
        c++;
        if (ch == '\0')
            break;
    };
    data_offset = c;

    //  Check Version
    if (file_version.size() == 0){
        throw FileIO::FileException("YCDFileReader::YCDFileReader()", path, "No version # found.");
    }
    if (file_version != "1.0.0" && file_version != "1.1.0"){
        throw FileIO::FileException(
            "YCDFileReader::YCDFileReader()",
            path,
            "This .ycd file is of a later format version.\n"
            "This version of the digit viewer is unable to view this file."
        );
    }

    //  Other checks
    if (digits_per_file < 100){
        throw FileIO::FileException("YCDFileReader::YCDFileReader()", path, "Invalid Digits per File");
    }

    if (total_digits != 0 && digits_per_file > total_digits)
        digits_per_file = static_cast<ufL_t>(total_digits);

    //  Boundaries
    uiL_t block_start = file_id * digits_per_file;
    uiL_t block_end   = block_start + digits_per_file;
    if (total_digits != 0){
        if (total_digits <= block_start){
            std::string error = "This file is out of range.\n";
            error += "Start:        ";
            error += std::to_string(block_start);
            error += "\n";
            error += "End:          ";
            error += std::to_string(block_end);
            error += "\n";
            error += "Total Digits: ";
            error += std::to_string(total_digits);
            error += "\n";
            throw FileIO::FileException("YCDFileReader::YCDFileReader()", path, error);
        }
        if (block_end > total_digits){
            block_end = total_digits;
        }
    }
    ufL_t block_digits = (ufL_t)(block_end - block_start);

    //  Radix-specific things
    switch (radix){
        case 10:
            digits_per_word = 19;
            words_in_this_file = (block_digits - 1) / 19 + 1;
            break;
        case 16:
            digits_per_word = 16;
            words_in_this_file = (block_digits - 1) / 16 + 1;
            break;
        default:
            throw FileIO::FileException("YCDFileReader::YCDFileReader()", path, "Unsupported Radix");
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void YCDFileReader::print() const{
    Console::println(path);
    Console::println();

    Console::println_labelm("file_version:", file_version);
    Console::println_labelm("radix:", radix);
    Console::println_labelm("first_digits:", first_digits);
    Console::println_labelm_commas("total_digits:", total_digits);
    Console::println_labelm_commas("digits_per_file:", digits_per_file);
    Console::println_labelm_commas("file_id:", file_id);
    Console::println();

    Console::println_labelm_commas("digits_per_word:", digits_per_word);
    Console::println_labelm_commas("words_in_this_file:", words_in_this_file);
    Console::println_labelm_commas("data_offset:", data_offset);
    Console::println();
}
void YCDFileReader::read_words(ufL_t pos, u64_t* T, upL_t L){
    //  This method reads L words starting at offset "pos".
    //  "pos" is measured in 64-bit words. It is relative to the start of the
    //  data section of the .ycd file.

    //  Thus the byte region of the file that is read is:
    //      [
    //          (data section offset) + pos * sizeof(u64_t),
    //          (data section offset) + (pos + L) * sizeof(u64_t)
    //      )

    //  Note the [start, end). Thus it reads from "start" and ends 1 byte before
    //  "end".

    //  If any portion is out of range of the file, it throw an exception.

    if (pos + L > words_in_this_file){
        Console::Warning("Internal Error: Read out of Bounds");
        std::string error = "YCDFileReader::read_words(ufL_t pos, u64_t* T, upL_t L)\n";
        error += "Read out of bounds.\n";
        error += "Requested Range: ";
        error += std::to_string(pos);
        error += " - ";
        error += std::to_string(pos + L);
        error += "\nAvailable Range: ";
        error += std::to_string(0);
        error += " - ";
        error += std::to_string(words_in_this_file);
        throw FileIO::FileException("YCDFileReader::read_words()", path, error);
    }

    //  Set file pointer
    file.set_ptr(data_offset + pos * sizeof(u64_t));

    //  Read
    upL_t words_read = file.read(T, L * sizeof(u64_t)) / sizeof(u64_t);
    if (words_read != L){
        throw FileIO::FileException("YCDFileReader::read_words()", path, "Error Reading File");
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void YCDFileReader::read_chars(
    uiL_t pos, char* str, upL_t digits,
    u64_t* buffer, upL_t buffer_L,
    void (*fp_convert)(char*, const u64_t*, upL_t)
){
    //  This method reads digits from the file.
    //  It reads the following region:
    //      [pos, pos + digits)

    //  "pos" is relative to the start of the constant. Thus it is required that
    //  the region fits entirely inside this file. Otherwise it will throw an
    //  exception.

    //Parameters:
    //  -   pos                 -   Starting digit position of the constant.
    //  -   (str, digits)       -   Output buffer
    //  -   (buffer, buffer_L)  -   Scratch memory
    //  -   fp_convert          -   Function pointer for binary -> char conversion.

    if (digits == 0){
        return;
    }

    //  Get boundaries
    uiL_t block_start = digits_per_file * file_id;
    uiL_t block_end   = block_start + digits_per_file;
    if (total_digits != 0 && block_end > digits_per_file){
        block_end = total_digits;
    }

    //  Check boundaries
    if (pos + digits > block_end || pos < block_start){
        std::string error = "void YCDFileReader::read_chars()\n";
        error += "Read out of bounds.\n";
        error += "Requested Range: ";
        error += std::to_string(pos);
        error += " - ";
        error += std::to_string(pos + digits);
        error += "\nAvailable Range: ";
        error += std::to_string(block_start);
        error += " - ";
        error += std::to_string(block_end);
        throw FileIO::FileException("YCDFileReader::read_chars()", path, error);
    }

    //  Local digit range.
    ufL_t local_start = (ufL_t)(pos - block_start);
    ufL_t local_end   = local_start + digits;

    //  Get word boundaries
    ufL_t word_start  = local_start / digits_per_word;
    ufL_t word_end    = (local_end + digits_per_word - 1) / digits_per_word;
    upL_t word_length = (upL_t)(word_end - word_start);

    //  Edge offsets
    upL_t bot_offset = (upL_t)(local_start - word_start  * digits_per_word);
    upL_t top_offset = (upL_t)(local_end - (word_end - 1) * digits_per_word);

    //  Need special case handling for only 1 word.
    //  This is because each of the edge handlers after this will only handle
    //  its own edge and write the rest of the word. So if a request fits
    //  entirely into a single word, each edge handler will do the same one
    //  and lead to writing out-of-bounds on the destination array.
    if (word_length == 1){
//        cout << "Single Word" << endl;
        char tmp[19];
        read_words(word_start, buffer, 1);
        fp_convert(tmp, buffer, 1);
        memcpy(str, tmp + bot_offset, top_offset - bot_offset);
        return;
    }

    //  Work loop
    while (word_length > 0){
        //  Select a block size
        upL_t current_block = std::min(buffer_L, word_length);
//        cout << "current_block = " << current_block << endl;

        u64_t* current_buffer = buffer;

        //  Load words
        read_words(word_start, current_buffer, current_block);

        //  Bottom edge
        if (bot_offset != 0){
//            cout << "Bottom Edge: " << bot_offset << endl;
            char tmp[19];
            fp_convert(tmp, current_buffer, 1);
            memcpy(str, tmp + bot_offset, digits_per_word - bot_offset);
            word_start++;
            word_length--;
            current_block--;
            current_buffer++;
            str += digits_per_word - bot_offset;

            //  This if-statement is only meant to be entered once on the very
            //  first iteration. This will prevent it from entering again.
            bot_offset = 0;
        }

        //  Top edge
        //  If this is the last iteration and the top edge is split, then
        //  we stop one word short.
        bool handle_top_edge =
            current_block == word_length &&
            top_offset != digits_per_word;
        if (handle_top_edge){
            current_block--;
        }

        //  Convert the middle words.
        if (current_block != 0){
//            cout << "Middle: " << current_block << endl;
            fp_convert(str, current_buffer, current_block);
            word_start += current_block;
            word_length -= current_block;
            current_buffer += current_block;
            str += current_block * digits_per_word;
        }

        //  Top edge
        if (handle_top_edge){
//            cout << "Top Edge: " << top_offset << endl;
            char tmp[19];
            fp_convert(tmp, current_buffer, 1);
            memcpy(str, tmp, top_offset);
            word_start++;
            word_length--;
            str += top_offset;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
