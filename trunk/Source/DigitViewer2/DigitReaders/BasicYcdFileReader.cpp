/* BasicYcdFileReader.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/29/2018
 * Last Modified    : 03/21/2018
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string.h>
#include <vector>
#include <algorithm>
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/Exceptions/BufferTooSmallThrower.h"
#include "PublicLibs/Exceptions/InvalidParametersException.h"
#include "PublicLibs/BasicLibs/Alignment/AlignmentTools.h"
#include "PublicLibs/BasicLibs/Concurrency/BlockSplitting.h"
#include "PublicLibs/SystemLibs/FileIO/FileException.h"
#include "DigitViewer2/Globals.h"
#include "DigitViewer2/RawToCompressed/RawToCompressed.h"
#include "ParsingTools.h"
#include "BasicYcdFileReader.h"
namespace DigitViewer2{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
BasicYcdFileReader::BasicYcdFileReader(std::string path)
    : m_file(FileIO::DEFAULT_FILE_ALIGNMENT_K, std::move(path), FileIO::OPEN_READONLY)
{
    //  Parse the file
    FileIO::BufferedReader reader(m_file);

    //  Start after the next newline.
    while (reader.next() != '\n');

    //  Parse header info
    while (1){
        std::string token = grab_until_delim(reader, '\n');

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
            m_file_version = std::move(value);
            continue;
        }

        //  radix
        if (key == YCF_CDF_TOKEN_Base){
            m_radix = (char)parse_uL(value.c_str());
            continue;
        }

        //  first_digits
        if (key == YCF_CDF_TOKEN_FirstDigits){
            m_first_digits = std::move(value);
            continue;
        }

        //  stream_end
        if (key == YCF_CDF_TOKEN_TotalDigits){
            m_stream_end = (uiL_t)parse_uL(value.c_str());
            continue;
        }

        //  digits_per_file
        if (key == YCF_CDF_TOKEN_BlockSize){
            m_digits_per_file = (ufL_t)parse_uL(value.c_str());
            continue;
        }

        //  file_id
        if (key == YCF_CDF_TOKEN_BlockID){
            m_file_id = parse_uL(value.c_str());
            continue;
        }
    }

    //  Find offset
    while (true){
        char ch = reader.next();
        if (ch == '\0'){
            m_data_offset = reader.offset();
            break;
        }
    };

    //  Check Version
    if (m_file_version.size() == 0){
        throw FileIO::FileException("BasicYcdFileReader::BasicYcdFileReader()", path, "No version # found.");
    }
    if (m_file_version != "1.0.0" && m_file_version != "1.1.0"){
        throw FileIO::FileException(
            "BasicYcdFileReader::BasicYcdFileReader()",
            path,
            "This .ycd file is of a later format version.\n"
            "This version of the digit viewer is unable to view this file."
        );
    }

    //  Other checks
    if (m_digits_per_file < 100){
        throw FileIO::FileException("BasicYcdFileReader::BasicYcdFileReader()", path, "Invalid Digits per File");
    }

    if (m_stream_end != 0 && m_digits_per_file > m_stream_end){
        m_digits_per_file = static_cast<ufL_t>(m_stream_end);
    }

    //  Boundaries
    uiL_t block_start = m_file_id * m_digits_per_file;
    uiL_t block_end   = block_start + m_digits_per_file;
    if (m_stream_end != 0){
        if (m_stream_end <= block_start){
            std::string error = "This file is out of range.\n";
            error += "Start:        ";
            error += std::to_string(block_start);
            error += "\n";
            error += "End:          ";
            error += std::to_string(block_end);
            error += "\n";
            error += "Total Digits: ";
            error += std::to_string(m_stream_end);
            error += "\n";
            throw FileIO::FileException("BasicYcdFileReader::BasicYcdFileReader()", path, error);
        }
        if (block_end > m_stream_end){
            block_end = m_stream_end;
        }
    }
    ufL_t block_digits = (ufL_t)(block_end - block_start);

    //  Radix-specific things
    switch (m_radix){
        case 10:
            m_digits_per_word = 19;
            m_words_in_this_file = (block_digits - 1) / 19 + 1;
            m_fp_convert = RawToCompressed::i64_to_dec;
            break;
        case 16:
            m_digits_per_word = 16;
            m_words_in_this_file = (block_digits - 1) / 16 + 1;
            m_fp_convert = RawToCompressed::i64_to_hex;
            break;
        default:
            throw FileIO::FileException("BasicYcdFileReader::BasicYcdFileReader()", path, "Unsupported Radix");
    }
}
bool BasicYcdFileReader::range_is_available(uiL_t offset, uiL_t digits){
    uiL_t s = m_file_id * m_digits_per_file;
    uiL_t e = s + m_digits_per_file;
    if (m_stream_end != 0){
        e = std::min(e, m_stream_end);
    }
    return offset >= s && offset + digits <= e;
}
upL_t BasicYcdFileReader::recommend_buffer_size(uiL_t digits, upL_t limit) const{
    if (digits == 0){
        return 0;
    }
    limit = std::max(limit, 2*FILE_ALIGNMENT);
    uiL_t words = (digits + 2*m_digits_per_word - 1) / m_digits_per_word;
    uiL_t sectors = (words * sizeof(u64_t) + 2*FILE_ALIGNMENT - 1) / FILE_ALIGNMENT;
    return (upL_t)std::min(sectors * FILE_ALIGNMENT, (uiL_t)limit);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void BasicYcdFileReader::print() const{
    Console::println(m_file.path());
    Console::println();

    Console::println_labelm("file_version:", m_file_version);
    Console::println_labelm("radix:", m_radix);
    Console::println_labelm("first_digits:", m_first_digits);
    Console::println_labelm_commas("stream_end:", m_stream_end);
    Console::println_labelm_commas("digits_per_file:", m_digits_per_file);
    Console::println_labelm_commas("file_id:", m_file_id);
    Console::println();

    Console::println_labelm_commas("digits_per_word:", m_digits_per_word);
    Console::println_labelm_commas("words_in_this_file:", m_words_in_this_file);
    Console::println_labelm_commas("data_offset:", m_data_offset);
    Console::println();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void BasicYcdFileReader::process_blocks(
    DigitStats& stats,
    const u64_t* B, upL_t BL
) const{
    alignas(DEFAULT_ALIGNMENT) char raw_digits[19 * CACHE_BLOCK];
    while (BL > 0){
        upL_t current_blocks = std::min(BL, CACHE_BLOCK);
        upL_t current_digits = current_blocks * m_digits_per_word;

        m_fp_convert(raw_digits, B, current_blocks);
        stats.accumulate(raw_digits, current_digits);

        B += current_blocks;
        BL -= current_blocks;
    }
}
void BasicYcdFileReader::process_blocks(
    char* raw_digits,
    DigitStats* stats,
    const u64_t* B, upL_t BL
) const{
    if (stats == nullptr){
        //  Single-pass only. No cache blocking needed.
        m_fp_convert(raw_digits, B, BL);
    }else{
        //  Cache block this.
        while (BL > 0){
            upL_t current_blocks = std::min(BL, CACHE_BLOCK);
            upL_t current_digits = current_blocks * m_digits_per_word;

            m_fp_convert(raw_digits, B, current_blocks);
            stats->accumulate(raw_digits, current_digits);

            raw_digits += current_digits;
            B += current_blocks;
            BL -= current_blocks;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class BasicYcdFileReader::Action_process : public BasicAction{
    const BasicYcdFileReader& m_object;
    char* m_raw_digits;
    DigitStats* m_stats;
    const u64_t* m_B;
    upL_t m_BL;

    upL_t m_unit_L;

public:
    Action_process(
        const BasicYcdFileReader& object,
        char* raw_digits,
        DigitStats* stats,
        const u64_t* B, upL_t BL,
        upL_t unit_L
    )
        : m_object(object)
        , m_raw_digits(raw_digits)
        , m_stats(stats)
        , m_B(B), m_BL(BL)
        , m_unit_L(unit_L)
    {}

    virtual void run(upL_t index) override{
        upL_t si = m_unit_L * index;
        upL_t ei = si + m_unit_L;
        if (si >= m_BL){
            return;
        }
        if (ei > m_BL){
            ei = m_BL;
        }

        DigitStats* stats = m_stats == nullptr
            ? nullptr
            : m_stats + index;

        if (m_raw_digits == nullptr){
            m_object.process_blocks(*stats, m_B + si, ei - si);
        }else{
            m_object.process_blocks(m_raw_digits + si * m_object.m_digits_per_word, stats, m_B + si, ei - si);
        }

        if (stats != nullptr){
            stats->scale_up_hash((m_BL - ei) * m_object.m_digits_per_word);
        }
    }
};
void BasicYcdFileReader::process_blocks(
    char* raw_digits,
    DigitStats* stats,
    const u64_t* B, upL_t BL,
    BasicParallelizer& parallelizer, upL_t tds
) const{
    const upL_t THRESHOLD = 1000;

    if (BL < THRESHOLD || tds <= 1){
        if (raw_digits == nullptr){
            process_blocks(*stats, B, BL);
        }else{
            process_blocks(raw_digits, stats, B, BL);
        }
        return;
    }

    //  Align to both DEFAULT_ALIGNMENT as well as m_digits_per_word.
    upL_t unit_L = parallel_split_aligned(
        BL, tds,
        DEFAULT_ALIGNMENT * m_digits_per_word,
        THRESHOLD
    );

//    cout << "tds    = " << tds << endl;
//    cout << "unit_L = " << unit_L << endl;

    std::vector<DigitStats> vstats;
    if (stats != nullptr){
        vstats.emplace_back(m_radix, stats->hash());
        for (upL_t c = 1; c < tds; c++){
            vstats.emplace_back(m_radix);
        }
    }

    Action_process action(
        *this,
        raw_digits,
        stats != nullptr ? &vstats[0] : nullptr,
        B, BL,
        unit_L
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
const u64_t* BasicYcdFileReader::read_words(ufL_t word_offset, upL_t words, void* P, upL_t Pbytes){
    //  Read the words in the range [word_offset, word_offset + words) from
    //  disk into somewhere in [P, P + Pbytes).
    //  Return a pointer the actual start of the data.

    //  This function handles the sector alignment.

    upL_t bytes = words * sizeof(u64_t);

    ufL_t file_access_offset_s = m_data_offset + word_offset * sizeof(u64_t);
    ufL_t file_access_offset_e = file_access_offset_s + bytes;

    ufL_t file_block_fs = file_access_offset_s / FILE_ALIGNMENT;
    ufL_t file_block_fe = (file_access_offset_e + FILE_ALIGNMENT - 1) / FILE_ALIGNMENT;

    ufL_t file_aligned_offset_s = file_block_fs * FILE_ALIGNMENT;
    ufL_t file_aligned_offset_e = file_block_fe * FILE_ALIGNMENT;

    upL_t shift = (upL_t)(file_access_offset_s - file_aligned_offset_s);
    upL_t file_bytes = (upL_t)(file_aligned_offset_e - file_aligned_offset_s);
    check_BufferTooSmall("BasicYcdFileReader::read_words()", Pbytes, file_bytes);

    upL_t bytes_read;
    {
        std::lock_guard<std::mutex> lg(m_lock);
        bytes_read = m_file.load(P, file_aligned_offset_s, file_bytes, false);
    }
    if (bytes_read < bytes + shift){
        throw FileIO::FileException(
            "BasicYcdFileReader::read_words()",
            m_file.path(),
            "Read error."
        );
    }

    return (const u64_t*)((char*)P + shift);
}
void BasicYcdFileReader::load_stats_B(
    DigitStats& stats,
    uiL_t offset, upL_t digits,
    void* P, upL_t Pbytes,
    BasicParallelizer& parallelizer, upL_t tds
){
    ufL_t end = (ufL_t)offset + digits;
    ufL_t word_s = (ufL_t)offset / m_digits_per_word;
    ufL_t word_e = end / m_digits_per_word;

    bool end_tail = word_e * m_digits_per_word < end;

    //  Read into memory.
    const u64_t* ptr;
    {
        ufL_t read_s = word_s;
        ufL_t read_e = end_tail ? word_e + 1 : word_e;
        upL_t read_L = (upL_t)(read_e - read_s);
        ptr = read_words(word_s, read_L, P, Pbytes);
//        Console::println_af(ptr, read_L);
    }

    //  Start Filter
    ufL_t read_s = word_s * m_digits_per_word;
    if (read_s != offset){
//        cout << "Start Filter" << endl;
        char buffer[19];
        m_fp_convert(buffer, ptr, 1);
        ptr += 1;

        //  Compute sizes
        upL_t diff = (upL_t)(offset - read_s);
        upL_t size = m_digits_per_word - diff;

        //  Make sure we don't overrun the end.
        size = std::min(size, digits);

        stats.accumulate(buffer + diff, size);

        digits -= size;
        word_s++;
    }

    if (digits == 0){
        return;
    }

    //  Steady State
    upL_t blocks = (upL_t)(word_e - word_s);
    if (blocks > 0){
//        cout << "Steady" << endl;
        process_blocks(nullptr, &stats, ptr, blocks, parallelizer, tds);

        upL_t current_digits = blocks * m_digits_per_word;
        ptr += blocks;
        digits -= current_digits;
        word_s += blocks;
    }

    //  End Filter
    if (end_tail){
//        cout << "End Filter" << endl;
        char buffer[19];
        m_fp_convert(buffer, ptr, 1);

        stats.accumulate(buffer, digits);
    }
}
void BasicYcdFileReader::load_digits_B(
    char* output,
    DigitStats* stats,
    uiL_t offset, upL_t digits,
    void* P, upL_t Pbytes,
    BasicParallelizer& parallelizer, upL_t tds
){
    ufL_t end = (ufL_t)offset + digits;
    ufL_t word_s = (ufL_t)offset / m_digits_per_word;
    ufL_t word_e = end / m_digits_per_word;

    bool end_tail = word_e * m_digits_per_word < end;

    //  Read into memory.
    const u64_t* ptr;
    {
        ufL_t read_s = word_s;
        ufL_t read_e = end_tail ? word_e + 1 : word_e;
        upL_t read_L = (upL_t)(read_e - read_s);
        ptr = read_words(word_s, read_L, P, Pbytes);
//        Console::println_af(ptr, read_L);
    }

    //  Start Filter
    ufL_t read_s = word_s * m_digits_per_word;
    if (read_s != offset){
//        cout << "Start Filter" << endl;
        char buffer[19];
        m_fp_convert(buffer, ptr, 1);
        ptr += 1;

        //  Compute sizes
        upL_t diff = (upL_t)(offset - read_s);
        upL_t size = m_digits_per_word - diff;

        //  Make sure we don't overrun the end.
        size = std::min(size, digits);

        //  Copy to output.
        memcpy(output, buffer + diff, size);
        if (stats != nullptr){
            stats->accumulate(output, size);
        }

        output += size;
        digits -= size;
        word_s++;
    }

    if (digits == 0){
        return;
    }

    //  Steady State
    upL_t blocks = (upL_t)(word_e - word_s);
    if (blocks > 0){
//        cout << "Steady" << endl;
        process_blocks(output, stats, ptr, blocks, parallelizer, tds);

        upL_t current_digits = blocks * m_digits_per_word;
        ptr += blocks;
        output += current_digits;
        digits -= current_digits;
        word_s += blocks;
    }

    //  End Filter
    if (end_tail){
//        cout << "End Filter" << endl;
        char buffer[19];
        m_fp_convert(buffer, ptr, 1);

        memcpy(output, buffer, digits);
        if (stats != nullptr){
            stats->accumulate(output, digits);
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
upL_t BasicYcdFileReader::start_access(
    uiL_t& offset, uiL_t digits,
    void* P, upL_t& Pbytes
) const{
    //  Get boundaries
    uiL_t block_start = m_digits_per_file * m_file_id;
    uiL_t block_end   = block_start + m_digits_per_file;
    if (m_stream_end != 0 && block_end > m_digits_per_file){
        block_end = m_stream_end;
    }

    //  Check boundaries
    if (offset + digits > block_end || offset < block_start){
        std::string error = "BasicYcdFileReader::start_access()\n";
        error += "Read out of bounds.\n";
        error += "Requested Range: ";
        error += std::to_string(offset);
        error += " - ";
        error += std::to_string(offset + digits);
        error += "\nAvailable Range: ";
        error += std::to_string(block_start);
        error += " - ";
        error += std::to_string(block_end);
        throw FileIO::FileException("BasicYcdFileReader::start_access()", m_file.path(), error);
    }

    if (Alignment::int_past_aligned<FILE_ALIGNMENT>((upL_t)P) != 0){
        throw InvalidParametersException("BasicYcdFileReader::start_access()", "Buffer is misaligned.");
    }

    Pbytes = Alignment::align_int_down<FILE_ALIGNMENT>(Pbytes);
    check_BufferTooSmall("BasicYcdFileReader::start_access()", Pbytes, 2*FILE_ALIGNMENT);

    offset -= block_start;

    //  # of words that can be safely loaded with current buffer size.
    upL_t max_words = (Pbytes - FILE_ALIGNMENT) / sizeof(u64_t);

    //  # of digits that can be safely loaded with current buffer size.
    upL_t max_digits = (max_words - 1) * m_digits_per_word;

    return max_digits;
}
void BasicYcdFileReader::load_stats(
    DigitStats& stats,
    uiL_t offset, uiL_t digits,
    const AlignedBufferC<BUFFER_ALIGNMENT>& buffer,
    BasicParallelizer& parallelizer, upL_t tds
){
    if (digits == 0){
        return;
    }

    void* P = buffer.ptr();
    upL_t Pbytes = buffer.len();

    upL_t max_digits = start_access(offset, digits, P, Pbytes);
    while (digits > 0){
        upL_t current_digits = (upL_t)std::min(digits, (uiL_t)max_digits);

        load_stats_B(stats, offset, current_digits, P, Pbytes, parallelizer, tds);

        offset += current_digits;
        digits -= current_digits;
    }
}
void BasicYcdFileReader::load_digits(
    char* output,
    DigitStats* stats,
    uiL_t offset, upL_t digits,
    const AlignedBufferC<BUFFER_ALIGNMENT>& buffer,
    BasicParallelizer& parallelizer, upL_t tds
){
    if (digits == 0){
        return;
    }

    void* P = buffer.ptr();
    upL_t Pbytes = buffer.len();

    upL_t max_digits = start_access(offset, digits, P, Pbytes);
    while (digits > 0){
        upL_t current_digits = std::min(digits, max_digits);

        load_digits_B(output, stats, offset, current_digits, P, Pbytes, parallelizer, tds);

        output += current_digits;
        offset += current_digits;
        digits -= current_digits;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
