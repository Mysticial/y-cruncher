/* TextDigitReader.cpp
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
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Exceptions/StringException.h"
#include "PublicLibs/SystemLibs/FileIO/FileIO.h"
#include "PublicLibs/SystemLibs/FileIO/FileException.h"
#include "DigitViewer/DigitConverter/DigitConverter.h"
#include "DigitViewer/Globals.h"
#include "TextDigitReader.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
namespace DigitViewer{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Constructors
TextReader::TextReader(
    const std::string& path,
    bool raw,
    int radix
)
    : m_file(path)
    , m_radix(radix)
    , fp_convert(nullptr)
{
    //  Auto-detect radix
    if (m_radix == 0){
        auto_detect_radix();
    }else{
        set_raw(raw);
    }

    //  Find the decimal place
    m_file.set_ptr(0);
    ufL_t c = 0;
    while (1){
        char ch;
        if (m_file.read(&ch, 1) != 1){
            throw FileIO::FileException(
                "TextReader::TextReader()",
                path,
                "Unexpected End of File"
            );
        }

        c++;
        if (ch == '.'){
            m_dp_offset = c;
            break;
        }
        if (c == 63){
            throw FileIO::FileException(
                "TextReader::TextReader()",
                path,
                "A decimal place was not found within the first 63 bytes of the file."
            );
        }
    }

    //  # of digits after decimal place.
    m_total_digits = FileIO::GetFileSize(path.c_str()) - m_dp_offset;
}
//TextReader::~TextReader(){
//    file.Close();
//}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void TextReader::print() const{
    Console::print("File: ");
    Console::println(m_file.GetPath());
    Console::println();

    Console::print("Radix:                  ");
    Console::println(m_radix);

    Console::print("Decimal Place Offset:   ");
    Console::println(m_dp_offset);

    Console::print("Digits in File:         ");
    Console::println_commas(m_total_digits);

    Console::print("Iterator File Offset:   ");
    Console::println(iter_f_offset);

    Console::print("Iterator Buffer Offset: ");
    Console::println(iter_b_offset);
    Console::println();
}
int TextReader::get_radix() const{
    return m_radix;
}
uiL_t TextReader::get_digits() const{
    return m_total_digits;
}
void TextReader::set_raw(bool raw){
    //  Clear the cache
    iter_b_offset = buffer_L;
    
    if (raw){
        //  User wants output to be raw.
        switch (m_radix){
            case 10:
                fp_convert = dec_to_raw;
                break;
            case 16:
                fp_convert = hex_to_raw;
                break;
            default:
                throw StringException("TextReader::set_raw()", "Unsupported Radix");
        };
    }else{
        //  User wants output to be text.
        switch (m_radix){
            case 10:
                fp_convert = nullptr;
                break;
            case 16:
                fp_convert = nullptr;
                break;
            default:
                throw StringException("TextReader::set_raw()", "Unsupported Radix");
        };
    }
}
bool TextReader::check_range(uiL_t start, uiL_t end){
    if (start >= end){
        throw StringException("TextReader::check_range()", "Invalid Parameters");
    }
    if (start >= m_total_digits){
        return false;
    }
    if (end > m_total_digits){
        return false;
    }
    return true;
}
std::string TextReader::get_first_digits(upL_t L){
    if (L == 0){
        return "";
    }
    m_file.set_ptr(0);

    std::string str(L, '\0');
    m_file.read(&str[0], L);

    return str;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void TextReader::read(uiL_t pos, char* str, upL_t digits){
    uiL_t end = pos + digits;

    //  Ends past the end.
    if (end > m_total_digits){
        throw StringException("TextReader::read()", "Out of range.");
    }

    m_file.set_ptr(m_dp_offset + static_cast<ufL_t>(pos));
    if (m_file.read(str, digits) != digits){
        throw StringException("TextReader::read()", "Error Reading from File");
    }

    //  Convert
    if (fp_convert != nullptr){
        if (fp_convert(str, digits)){
            std::string error("Invalid Digit: ");
            error += std::to_string(pos);
            error += " - ";
            error += std::to_string(pos + digits);
            throw StringException("TextReader::read()", std::move(error));
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void TextReader::set_radix(int radix){
    //  Make sure the radix is valid before setting it. Don't let an exception
    //  leave the object in an invalid state.
    switch (radix){
        case 10:
        case 16:
            break;
        default:
            throw StringException("TextReader::set_radix()", "Unsupported Radix");
    }
    m_radix = radix;
    if (fp_convert != nullptr){
        set_raw(true);
    }
}
void TextReader::auto_detect_radix(){
    //  Clear the cache
    iter_b_offset = buffer_L;

    //  Assume radix 10 until a hexadecimal digit is found.
    int radix = 10;

    //  Read the first 64 bytes to guess the radix.
    m_file.set_ptr(0);

    for (upL_t c = 0; c < 64; c++){
        char ch;
        if (m_file.read(&ch, 1) != 1){
            break;
        }

        //  Skip the decimal place.
        if (ch == '.'){
            continue;
        }

        //  Decimal digit
        if ('0' <= ch && ch <= '9'){
            continue;
        }

        //  Hexadecimal digit
        if ('a' <= ch && ch <= 'f'){
            radix = 16;
            break;
        }

        throw StringException("TextReader::auto_detect_radix()", "Invalid Digit");
    }

    set_radix(radix);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
