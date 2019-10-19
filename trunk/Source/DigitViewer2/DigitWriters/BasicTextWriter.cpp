/* BasicTextWriter.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/03/2018
 * Last Modified    : 03/25/2018
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <algorithm>
#include "PublicLibs/Exceptions/BufferTooSmallThrower.h"
#include "PublicLibs/Exceptions/InvalidParametersException.h"
#include "PublicLibs/BasicLibs/Alignment/AlignmentTools.h"
#include "PublicLibs/SystemLibs/FileIO/FileException.h"
#include "PublicLibs/SystemLibs/FileIO/BufferredStreamFile.h"
#include "PublicLibs/SystemLibs/Concurrency/Parallelizers.h"
#include "DigitViewer2/RawToAscii/RawToAscii.h"
#include "DigitViewer2/DigitReaders/BasicTextReader.h"
#include "BasicTextWriter.h"
namespace DigitViewer2{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
BasicTextWriter::BasicTextWriter(
    const std::string& path,
    const std::string& first_digits,
    char radix
)
    : m_file(FileIO::DEFAULT_FILE_ALIGNMENT_K, path, FileIO::CREATE)
    , m_data_offset(0)
    , m_offset_extent(0)
{
    m_radix = radix;
    switch (radix){
    case 10:
        m_fp_convert = RawToAscii::raw_to_dec;
        break;
    case 16:
        m_fp_convert = RawToAscii::raw_to_hex;
        break;
    default:
        throw InvalidParametersException("BasicTextWriter::BasicTextWriter()", "Invalid radix.");
    }

    //  Write the first digits.
    if (first_digits.empty()){
        return;
    }

    upL_t decimal_offset = first_digits.find('.');
    if (decimal_offset == std::string::npos){
        throw FileIO::FileException("TextWriter::TextWriter()", path, "No decimal place was found.");
    }
    m_data_offset = decimal_offset + 1;

    FileIO::BufferedWriter writer(m_file);
    writer.push(first_digits.c_str(), decimal_offset + 1);
}
BasicTextWriter::~BasicTextWriter(){
    if (m_file){
        m_file.close_and_set_size(m_data_offset + m_offset_extent);
    }
}
upL_t BasicTextWriter::recommend_buffer_size(uiL_t digits, upL_t limit) const{
    limit = std::max(limit, FILE_ALIGNMENT);
    uiL_t sectors = (digits + 2*FILE_ALIGNMENT - 1) / FILE_ALIGNMENT;
    return (upL_t)std::min(sectors * FILE_ALIGNMENT, (uiL_t)limit);
}
std::unique_ptr<BasicDigitReader> BasicTextWriter::close_and_get_basic_reader(){
    if (!m_file){
        return nullptr;
    }
    std::string path = m_file.path();
    m_file.close_and_set_size(m_data_offset + m_offset_extent);
    return std::make_unique<BasicTextReader>(path, m_radix);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void BasicTextWriter::store_digits(
    const char* input,
    uiL_t offset, upL_t digits,
    const AlignedBufferC<BUFFER_ALIGNMENT>& buffer,
    BasicParallelizer& parallelizer, upL_t tds
){
    if (!m_file){
        throw InvalidParametersException("BasicTextWriter::store_digits()", "File is closed.");
    }

    char* P = (char*)buffer.ptr();
    check_BufferTooSmall("BasicTextReader::load_stats()", buffer.len(), FILE_ALIGNMENT);

    //  This is the ugly sector alignment logic.

    ufL_t logical_access_offset_s = (ufL_t)offset;
    ufL_t logical_access_offset_e = logical_access_offset_s + (ufL_t)digits;

    ufL_t file_access_offset_s = m_data_offset + logical_access_offset_s;
    ufL_t file_access_offset_e = m_data_offset + logical_access_offset_e;

    ufL_t file_block_fs = file_access_offset_s / FILE_ALIGNMENT;
    ufL_t file_block_fe = (file_access_offset_e + FILE_ALIGNMENT - 1) / FILE_ALIGNMENT;

    ufL_t file_aligned_offset_s = file_block_fs * FILE_ALIGNMENT;
    ufL_t file_aligned_offset_e = file_block_fe * FILE_ALIGNMENT;

    ufL_t aligned_bytes_left = file_aligned_offset_e - file_aligned_offset_s;
    while (aligned_bytes_left > 0){
        upL_t current = (upL_t)std::min((ufL_t)buffer.len(), aligned_bytes_left);

        sfL_t logical_aligned_offset_s = (sfL_t)file_aligned_offset_s - (sfL_t)m_data_offset;
        sfL_t logical_aligned_offset_e = logical_aligned_offset_s + (sfL_t)current;

        bool locked = false;
        std::unique_lock<std::mutex> lock(m_lock, std::defer_lock);

        //  Filters
        upL_t shift_f = (upL_t)std::max(
            (sfL_t)logical_access_offset_s - logical_aligned_offset_s,
            (sfL_t)0
        );
        upL_t shift_e = (upL_t)std::max(
            logical_aligned_offset_e - (sfL_t)logical_access_offset_e,
            (sfL_t)0
        );

        //  Load boundaries
        if (shift_f != 0 || shift_e != 0){
            lock.lock();
            locked = true;
        }
        if (shift_f != 0){
            m_file.load(P, file_aligned_offset_s, FILE_ALIGNMENT, false);
        }
        if (shift_e != 0 && current > FILE_ALIGNMENT){
            upL_t shift = current - FILE_ALIGNMENT;
            m_file.load(P + shift, file_aligned_offset_s + shift, FILE_ALIGNMENT, false);
        }

        //  Process digits
        upL_t processed = current - shift_f - shift_e;
//        cout << "current = " << current << ", shift_f = " << shift_f << ", shift_e = " << shift_e << endl;
        bool bad = RawToAscii::parallel_convert(
            m_fp_convert,
            P + shift_f,
            input,
            processed,
            parallelizer, tds
        );
        if (bad){
            throw InvalidParametersException("BasicTextWriter::store_digits()", "Invalid Digit");
        }

        //  Store
        {
            if (!locked){
                lock.lock();
            }
            m_file.store(P, file_aligned_offset_s, current, true);
        }

        input += processed;
        logical_access_offset_s += processed;
        file_aligned_offset_s += current;
        aligned_bytes_left -= current;

        m_offset_extent = std::max(m_offset_extent, logical_access_offset_s);
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
