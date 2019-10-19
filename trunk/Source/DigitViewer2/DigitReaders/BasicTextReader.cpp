/* BasicTextReader.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/14/2018
 * Last Modified    : 03/20/2018
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <vector>
#include <algorithm>
#include "PublicLibs/Exceptions/BufferTooSmallThrower.h"
#include "PublicLibs/Exceptions/InvalidParametersException.h"
#include "PublicLibs/BasicLibs/Alignment/AlignmentTools.h"
#include "PublicLibs/BasicLibs/Concurrency/BlockSplitting.h"
#include "PublicLibs/SystemLibs/FileIO/FileIO.h"
#include "PublicLibs/SystemLibs/FileIO/FileException.h"
#include "DigitViewer2/RawToAscii/RawToAscii.h"
#include "BasicTextReader.h"
namespace DigitViewer2{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
BasicTextReader::BasicTextReader(const std::string& path, char radix)
    : m_file(FileIO::DEFAULT_FILE_ALIGNMENT_K, std::move(path), FileIO::OPEN_READONLY)
{
    alignas(FILE_ALIGNMENT) char buffer[FILE_ALIGNMENT];

    upL_t bytes = m_file.load(buffer, 0, FILE_ALIGNMENT, false);
    upL_t stop = std::min(bytes, (upL_t)64);

    m_data_offset = (ufL_t)0 - 1;
    upL_t offset = 0;

    //  Auto-detect radix
    if (radix == 0){
        radix = 10;
        for (; offset < stop; offset++){
            char ch = buffer[offset];

            //  Skip the decimal place.
            if (ch == '.'){
                m_data_offset = offset + 1;
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

            throw StringException("TextReader::BasicTextReader()", "Invalid Digit");
        }
    }
    m_radix = radix;

    //  Find the decimal place
    if (m_data_offset == (ufL_t)0 - 1){
        for (; offset < stop; offset++){
            char ch = buffer[offset];
            if (ch == '.'){
                m_data_offset = offset + 1;
                break;
            }
        }
        if (m_data_offset == (ufL_t)0 - 1){
            throw FileIO::FileException(
                "BasicTextReader::BasicTextReader()",
                m_file.path(),
                "A decimal place was not found within the first 63 bytes of the file."
            );
        }
    }

    //  # of digits after decimal place.
    m_total_digits = FileIO::GetFileSize(m_file.path().c_str()) - m_data_offset;

    //  First digits
    m_first_digits = std::string(buffer, stop);

    switch (m_radix){
    case 10:
        m_fp_convert = RawToAscii::dec_to_raw;
        break;
    case 16:
        m_fp_convert = RawToAscii::hex_to_raw;
        break;
    default:
        throw InvalidParametersException("BasicTextReader::BasicTextReader()", "Invalid Radix");
    }
}
std::string BasicTextReader::first_digits(){
    return m_first_digits;
}
bool BasicTextReader::range_is_available(uiL_t offset, uiL_t digits){
    return offset + digits <= m_total_digits;
}
upL_t BasicTextReader::recommend_buffer_size(uiL_t digits, upL_t limit) const{
    limit = std::max(limit, FILE_ALIGNMENT);
    uiL_t sectors = (digits + 2*FILE_ALIGNMENT - 1) / FILE_ALIGNMENT;
    return (upL_t)std::min(sectors * FILE_ALIGNMENT, (uiL_t)limit);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class BasicTextReader::Action_process : public BasicAction{
    const BasicTextReader& m_object;
    DigitStats* m_stats;
    char* m_output;
    const char* m_txt_digits;
    upL_t m_digits;

    upL_t m_unit_L;

public:
    Action_process(
        const BasicTextReader& object,
        DigitStats* stats,
        char* output, const char* txt_digits, upL_t digits,
        upL_t unit_L
    )
        : m_object(object)
        , m_stats(stats)
        , m_output(output), m_txt_digits(txt_digits), m_digits(digits)
        , m_unit_L(unit_L)
    {}

    virtual void run(upL_t index) override{
        upL_t si = m_unit_L * index;
        upL_t ei = si + m_unit_L;
        if (si >= m_digits){
            return;
        }
        if (ei > m_digits){
            ei = m_digits;
        }

        DigitStats* stats = m_stats == nullptr
            ? nullptr
            : m_stats + index;

        m_object.process(stats, m_output + si, m_txt_digits + si, ei - si);

        if (stats != nullptr){
            stats->scale_up_hash(m_digits - ei);
        }
    }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void BasicTextReader::process(DigitStats* stats, char* output, const char* txt_digits, upL_t digits) const{
    //  Optimization: If "output" is misaligned, do enough to align it.
    upL_t align = Alignment::ptr_to_aligned<DEFAULT_ALIGNMENT>(output);
    if (digits > DEFAULT_ALIGNMENT && align != 0){
        process(stats, output, txt_digits, align);
        output += align;
        txt_digits += align;
        digits -= align;
    }

    if (stats == nullptr){
        if (m_fp_convert(output, txt_digits, digits)){
            throw InvalidParametersException("BasicTextReader::process_txt()", "Invalid Digit");
        }
        return;
    }

    while (digits > 0){
        upL_t block = std::min(digits, (upL_t)CACHE_BLOCK);

        if (m_fp_convert(output, txt_digits, block)){
            throw InvalidParametersException("BasicTextReader::process_txt()", "Invalid Digit");
        }
        if (stats != nullptr){
            stats->accumulate(output, block);
        }

        output += block;
        txt_digits += block;
        digits -= block;
    }

    if (stats == nullptr){
        return;
    }

    alignas(DEFAULT_ALIGNMENT) char cache[CACHE_BLOCK];
    while (digits > 0){
        upL_t block = std::min(digits, (upL_t)CACHE_BLOCK);

        if (m_fp_convert(cache, txt_digits, block)){
            throw InvalidParametersException("BasicTextReader::process_txt()", "Invalid Digit");
        }
        if (stats != nullptr){
            stats->accumulate(cache, block);
        }

        txt_digits += block;
        digits -= block;
    }
}
void BasicTextReader::process(
    DigitStats* stats, char* output, const char* txt_digits, upL_t digits,
    BasicParallelizer& parallelizer, upL_t tds
) const{
    //  Optimization: If "buffer" is misaligned, do enough to align it.
    upL_t align = Alignment::ptr_to_aligned<DEFAULT_ALIGNMENT>(output);
    if (digits > DEFAULT_ALIGNMENT && align != 0){
        process(stats, output, txt_digits, align);
        output += align;
        txt_digits += align;
        digits -= align;
    }

    const upL_t THRESHOLD = 10000;

    if (digits < THRESHOLD || tds <= 1){
        process(stats, output, txt_digits, digits);
        return;
    }

    upL_t unit_L = parallel_split_words_aligned<char, DEFAULT_ALIGNMENT>(
        digits, tds, THRESHOLD
    );

    std::vector<DigitStats> vstats;
    if (stats != nullptr){
        vstats.emplace_back(m_radix, stats->hash());
        for (upL_t c = 1; c < tds; c++){
            vstats.emplace_back(m_radix);
        }
    }

    Action_process action(
        *this,
        stats != nullptr ? &vstats[0] : nullptr,
        output, txt_digits, digits, unit_L
    );
    parallelizer.run_in_parallel(action, 0, tds);

    if (stats != nullptr){
        stats[0].clear_hash();
        for (const DigitStats& stat : vstats){
            stats[0] += stat;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void BasicTextReader::load_stats(
    DigitStats& stats,
    uiL_t offset, uiL_t digits,
    const AlignedBufferC<BUFFER_ALIGNMENT>& buffer,
    BasicParallelizer& parallelizer, upL_t tds
){
    //  Ends past the end.
    uiL_t end = offset + digits;
    if (end > m_total_digits){
        throw StringException("BasicTextReader::load_stats()", "Out of range.");
    }

    char* P = (char*)buffer.ptr();
    upL_t PL = Alignment::align_int_down<FILE_ALIGNMENT>(buffer.len());
    check_BufferTooSmall("BasicTextReader::load_stats()", PL, FILE_ALIGNMENT);

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
        upL_t current = (upL_t)std::min((ufL_t)PL, aligned_bytes_left);

        //  Read from file.
        upL_t bytes_read;
        {
            std::lock_guard<std::mutex> lg(m_lock);
            bytes_read = m_file.load(P, file_aligned_offset_s, current, false);
        }

        sfL_t logical_aligned_offset_s = (sfL_t)file_aligned_offset_s - (sfL_t)m_data_offset;
        sfL_t logical_aligned_offset_e = logical_aligned_offset_s + (sfL_t)current;

        //  Bottom Filter
        upL_t shift_f = (upL_t)std::max(
            (sfL_t)logical_access_offset_s - logical_aligned_offset_s,
            (sfL_t)0
        );

        //  Top Filter
        upL_t shift_e = (upL_t)std::max(
            logical_aligned_offset_e - (sfL_t)logical_access_offset_e,
            (sfL_t)0
        );
        if (bytes_read < current - shift_e){
            throw FileIO::FileException("BasicTextReader::load_stats()", m_file.path(), "Incomplete read.");
        }

        upL_t processed = current - shift_f - shift_e;
        process(
            &stats,
            P + shift_f,
            P + shift_f,
            processed,
            parallelizer, tds
        );

        logical_access_offset_s += processed;
        file_aligned_offset_s += current;
        aligned_bytes_left -= current;
    }
}
void BasicTextReader::load_digits(
    char* output,
    DigitStats* stats,
    uiL_t offset, upL_t digits,
    const AlignedBufferC<BUFFER_ALIGNMENT>& buffer,
    BasicParallelizer& parallelizer, upL_t tds
){
    //  Ends past the end.
    uiL_t end = offset + digits;
    if (end > m_total_digits){
        throw StringException("BasicTextReader::load_stats()", "Out of range.");
    }

    char* P = (char*)buffer.ptr();
    upL_t PL = Alignment::align_int_down<FILE_ALIGNMENT>(buffer.len());
    check_BufferTooSmall("BasicTextReader::load_stats()", PL, FILE_ALIGNMENT);

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
        upL_t current = (upL_t)std::min((ufL_t)PL, aligned_bytes_left);

        //  Read from file.
        upL_t bytes_read;
        {
            std::lock_guard<std::mutex> lg(m_lock);
            bytes_read = m_file.load(P, file_aligned_offset_s, current, false);
        }

        sfL_t logical_aligned_offset_s = (sfL_t)file_aligned_offset_s - (sfL_t)m_data_offset;
        sfL_t logical_aligned_offset_e = logical_aligned_offset_s + (sfL_t)current;

        //  Bottom Filter
        upL_t shift_f = (upL_t)std::max(
            (sfL_t)logical_access_offset_s - logical_aligned_offset_s,
            (sfL_t)0
        );

        //  Top Filter
        upL_t shift_e = (upL_t)std::max(
            logical_aligned_offset_e - (sfL_t)logical_access_offset_e,
            (sfL_t)0
        );
        if (bytes_read < current - shift_e){
            throw FileIO::FileException("BasicTextReader::load_digits()", m_file.path(), "Incomplete read.");
        }

        upL_t processed = current - shift_f - shift_e;
        process(
            stats,
            output,
            P + shift_f,
            processed,
            parallelizer, tds
        );

        output += processed;
        logical_access_offset_s += processed;
        file_aligned_offset_s += current;
        aligned_bytes_left -= current;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
