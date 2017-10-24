/* YCDWriter.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/29/2013
 * Last Modified    : 07/29/2013
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/FileIO/FileException.h"
#include "PublicLibs/Memory/AlignedMalloc.h"
#include "DigitViewer/Globals.h"
#include "DigitViewer/DigitReaders/YCDReader.h"
#include "YCDFileWriter.h"
#include "YCDWriter.h"
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
std::string YCDWriter::make_filename(uiL_t fileid){
    std::string full_path(m_path);
    full_path += m_name;
    full_path += " - ";
    full_path += std::to_string(fileid);
    full_path += ".ycd";
    return full_path;
}
void YCDWriter::create_file(uiL_t fileid){
    m_file = YCDFileWriter(
        make_filename(fileid),
        m_first_digits,
        m_digits_per_file,
        fileid,
        m_radix
    );
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YCDWriter::YCDWriter(
    std::string path,
    std::string name,
    std::string first_digits,
    ufL_t digits_per_file_,
    uiL_t start_fileid,
    int radix,
    upL_t buffer_size,
    u64_t* buffer,
    void (*deallocator)(void*)
)
    : m_path(std::move(path))
    , m_name(std::move(name))
    , m_radix(radix)
    , m_start_fileid(start_fileid)
    , m_first_digits(std::move(first_digits))
    , m_digits_per_file(digits_per_file_)
    , m_fileid(start_fileid)
    , fp_free(deallocator)
{
    if (buffer_size < 4096){
        throw FileIO::FileException(
            "YCDWriter::YCDWriter()",
            path,
            "Requested buffer size is too small."
        );
    }

    switch (m_radix){
        case 16: break;
        case 10: break;
        default:
            throw FileIO::FileException("YCDWriter::YCDWriter()", path, "Unsupported Radix");
    }

    //  Make sure path ends in a slash.
    if (m_path.size() != 0){
        char back = m_path.back();
        if (back != '/' && back != '\\'){
            m_path += '/';
        }
    }

    //  Multiple files, create a folder for them.
    if (m_digits_per_file != (uiL_t)0 - 1){
        m_path += m_name;
        m_path += '/';
        FileIO::MakeDirectory(m_path.c_str());
    }

    m_bin_buffer_L = buffer_size / sizeof(u64_t);
    if (buffer == nullptr){
        m_external_buffer = false;
        m_bin_buffer = (u64_t*)aligned_malloc(m_bin_buffer_L * sizeof(u64_t), 2*sizeof(u64_t));
    }else{
        m_external_buffer = true;
        m_bin_buffer = buffer;
    }
}
void YCDWriter::free_buffer(){
    if (m_bin_buffer == nullptr){
        return;
    }

    //  Internally allocated.
    if (!m_external_buffer){
        aligned_free(m_bin_buffer);
    }

    //  Preallocated with manual deallocator.
    if (fp_free != nullptr){
        fp_free(m_bin_buffer);
    }

    m_bin_buffer = nullptr;
}
YCDWriter::~YCDWriter(){
    flush_buffer();
    free_buffer();
}
std::unique_ptr<DigitReader> YCDWriter::close_and_get_reader(upL_t buffer_size){
    flush_buffer();
    free_buffer();
    m_file.close();
    return std::make_unique<YCDReader>(make_filename(m_start_fileid), false, buffer_size);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void YCDWriter::write(char* str, upL_t digits){
    while (true){
        //  No file is opened.
        if (!m_file.isValid()){
            create_file(m_fileid++);
        }

        //  Write digits
        upL_t written = m_file.write_chars(str, digits, m_bin_buffer, m_bin_buffer_L);
        if (written == digits){
            return;
        }

        digits -= written;
        str    += written;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
