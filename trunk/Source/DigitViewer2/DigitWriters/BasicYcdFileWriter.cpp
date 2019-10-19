/* BasicYcdFileWriter.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/04/2018
 * Last Modified    : 03/26/2018
 * 
 *      Implements a single file of a set of .ycd compressed files.
 * 
 *  When the file is created, it will have "(incomplete)" appended to its name
 *  to indicate that it has not been completely written out.
 * 
 *  Once the file is completely written out, it is automatically closed and
 *  renamed to the proper name. At this point, no more writes will be accepted.
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string.h>
#include <algorithm>
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/Exceptions/BufferTooSmallThrower.h"
#include "PublicLibs/BasicLibs/Alignment/AlignmentTools.h"
#include "PublicLibs/SystemLibs/FileIO/FileException.h"
#include "PublicLibs/SystemLibs/FileIO/FileIO.h"
#include "PublicLibs/SystemLibs/FileIO/BufferredStreamFile.h"
#include "DigitViewer2/Globals.h"
#include "DigitViewer2/RawToCompressed/RawToCompressed.h"
#include "BasicYcdFileWriter.h"
namespace DigitViewer2{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
BasicYcdFileWriter::BasicYcdFileWriter(
    const std::string& path,
    const std::string& first_digits,
    char radix,
    ufL_t digits_per_file, uiL_t stream_end,
    uiL_t fileid
)
    : m_path(path + ".ycd")
    , m_file(FileIO::DEFAULT_FILE_ALIGNMENT_K, path + " (incomplete).ycd", FileIO::CREATE)
    , m_stream_end(stream_end)
    , m_digits_per_file(digits_per_file)
    , m_file_id(fileid)
    , m_target(
        fileid * digits_per_file,
        stream_end == 0
            ? (fileid + 1) * digits_per_file
            : std::min((fileid + 1) * digits_per_file, stream_end)
    )
{
    m_radix = radix;

    uiL_t s = m_digits_per_file * m_file_id;
    if (stream_end != 0 && stream_end <= s){
        throw InvalidParametersException(
            "BasicYcdFileWriter::BasicYcdFileWriter()",
            "File ID is out of range."
        );
    }

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
    m_offset_stream_end = header.size();
//    header += to_string_max(stream_end);
    header += std::to_string(stream_end);
    header += "\r\n\r\n";

    header += YCF_CDF_TOKEN_BlockSize;
    header += '\t';
    header += std::to_string(digits_per_file);
    header += "\r\n";

    header += YCF_CDF_TOKEN_BlockID;
    header += '\t';
    header += std::to_string(m_file_id);
    header += "\r\n\r\n";

    header += YCF_CDF_TOKEN_EndHeader;
    header += "\r\n\r\n";
    header += '\0';

    //  Write the header
    FileIO::BufferedWriter writer(m_file);
    writer.push(header.c_str(), header.size());
    m_data_offset = header.size();
    m_offset_extent = m_data_offset;

    switch (radix){
        case 10:
            m_digits_per_word = 19;
            m_words_in_this_file = (digits_per_file - 1) / 19 + 1;
            m_fp_convert_forward = RawToCompressed::dec_to_i64;
            m_fp_convert_inverse = RawToCompressed::i64_to_dec;
            m_max_word = 9999999999999999999ull;
            break;
        case 16:
            m_digits_per_word = 16;
            m_words_in_this_file = (digits_per_file - 1) / 16 + 1;
            m_fp_convert_forward = RawToCompressed::hex_to_i64;
            m_fp_convert_inverse = RawToCompressed::i64_to_hex;
            m_max_word = 0xffffffffffffffffull;
            break;
        default:
            throw FileIO::FileException("BasicYcdFileWriter::BasicYcdFileWriter()", path, "Unsupported Radix");
    }
}
BasicYcdFileWriter::~BasicYcdFileWriter(){
    if (m_written == m_target){
        std::string path = m_file.path();
        m_file.close_and_set_size(m_offset_extent);
        FileIO::RenameFile(path, m_path);
    }else{
        Console::println_labelc("Warning, closing incomplete file", m_path);
        m_written.print();
        m_file.close_and_set_size(m_offset_extent);
    }
}
upL_t BasicYcdFileWriter::recommend_buffer_size(uiL_t digits, upL_t limit) const{
    if (digits == 0){
        return 0;
    }
    limit = std::max(limit, 2*FILE_ALIGNMENT);
    uiL_t words = (digits + 2*m_digits_per_word - 1) / m_digits_per_word;
    uiL_t sectors = (words * sizeof(u64_t) + 2*FILE_ALIGNMENT - 1) / FILE_ALIGNMENT;
    return (upL_t)std::min(sectors * FILE_ALIGNMENT, (uiL_t)limit);
}
std::unique_ptr<BasicDigitReader> BasicYcdFileWriter::close_and_get_basic_reader(){
    //  Not supported.
    return nullptr;
}
void BasicYcdFileWriter::print() const{
    Console::println(m_path);
    Console::println();

//    Console::println_labelm("file_version:", m_file_version);
    Console::println_labelm("radix:", m_radix);
//    Console::println_labelm("first_digits:", m_first_digits);
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
u64_t* BasicYcdFileWriter::get_range(
    ufL_t& write_offset, upL_t& write_bytes,
    ufL_t word_offset, upL_t words,
    void* P, upL_t Pbytes
){
    //  Extract an aligned region from [P, P + Pbytes) that will fit the entire
    //  region in [word_offset, word_offset + words).
    //  Return a pointer the actual start of the data.

    //  This function handles the sector alignment.
    //  If either of the end points are misaligned, it will load them.

    upL_t bytes = words * sizeof(u64_t);

    ufL_t file_access_offset_s = m_data_offset + word_offset * sizeof(u64_t);
    ufL_t file_access_offset_e = file_access_offset_s + bytes;

    ufL_t file_block_fs = file_access_offset_s / FILE_ALIGNMENT;
    ufL_t file_block_fe = (file_access_offset_e + FILE_ALIGNMENT - 1) / FILE_ALIGNMENT;

    ufL_t file_aligned_offset_s = file_block_fs * FILE_ALIGNMENT;
    ufL_t file_aligned_offset_e = file_block_fe * FILE_ALIGNMENT;

    upL_t shift = (upL_t)(file_access_offset_s - file_aligned_offset_s);
    upL_t file_bytes = (upL_t)(file_aligned_offset_e - file_aligned_offset_s);
    check_BufferTooSmall("BasicYcdFileWriter::get_range()", Pbytes, file_bytes);

    write_offset = file_aligned_offset_s;
    write_bytes = file_bytes;

    //  Don't skip the edge blocks even if it aligns properly.
    //  The edge blocks are still needed if the digit offset is mialigned
    //  with respect to the word.

    //  Read start block.
//    if (shift != 0){
        m_file.load(
            P,
            file_aligned_offset_s,
            //  Need to read two sectors if the word crosses a sector boundary.
            shift + sizeof(u64_t) <= FILE_ALIGNMENT
                ? FILE_ALIGNMENT
                : FILE_ALIGNMENT * 2,
            false
        );
//    }

    //  Read end block.
    if (file_bytes > FILE_ALIGNMENT){
//    if (file_access_offset_e < file_aligned_offset_e && file_bytes > FILE_ALIGNMENT){
        m_file.load(
            (char*)P + file_bytes - FILE_ALIGNMENT * 2, //  TODO: Don't always need to read 2 sectors.
            file_aligned_offset_e - FILE_ALIGNMENT * 2,
            FILE_ALIGNMENT,
            false
        );
    }

    return (u64_t*)((char*)P + shift);
}
void BasicYcdFileWriter::store_digits_B(
    const char* input,
    ufL_t offset, upL_t digits,
    void* P, upL_t Pbytes,
    BasicParallelizer& parallelizer, upL_t tds
){
    ufL_t end = offset + digits;
    ufL_t word_s = offset / m_digits_per_word;
    ufL_t word_e = end / m_digits_per_word;

    upL_t digits_left = digits;

    bool end_tail = word_e * m_digits_per_word < end;

    std::lock_guard<std::mutex> lg(m_lock);

    //  Get file I/O range.
    u64_t* ptr;
    ufL_t write_offset;
    upL_t write_bytes;
    {
        ufL_t access_s = word_s;
        ufL_t access_e = end_tail ? word_e + 1 : word_e;
        upL_t access_L = (upL_t)(access_e - access_s);
        ptr = get_range(
            write_offset, write_bytes,
            word_s, access_L,
            P, Pbytes
        );
    }

    //  Start Filter
    ufL_t read_s = word_s * m_digits_per_word;
    if (read_s != offset){
//        cout << "Start Filter: " << offset + digits - digits_left << endl;

        //  Load and filter
        char buffer[19];
        if (ptr[0] > m_max_word){
            memset(buffer, 0, sizeof(buffer));
        }else{
            m_fp_convert_inverse(buffer, ptr, 1);
        }

        //  Compute sizes
        upL_t diff = (upL_t)(offset - read_s);
        upL_t size = m_digits_per_word - diff;

        //  Make sure we don't overrun the end.
        size = std::min(size, digits_left);

        //  Copy to output.
        memcpy(buffer + diff, input, size);

        //  Store
        if (m_fp_convert_forward(ptr, buffer, 1)){
            throw InvalidParametersException("BasicYcdFileWriter::store_digits_B()", "Invalid Digit");
        }

        input += size;
        digits_left -= size;
        word_s++;
        ptr++;
    }

    if (digits_left == 0){
        m_file.store(P, write_offset, write_bytes, true);
        return;
    }

    //  Steady State
    upL_t blocks = (upL_t)(word_e - word_s);
    if (blocks > 0){
//        cout << "Steady" << endl;
        bool bad = RawToCompressed::raw_to_i64(
            m_fp_convert_forward, m_digits_per_word,
            ptr, input, blocks,
            parallelizer, tds
        );
        if (bad){
            throw InvalidParametersException("BasicYcdFileWriter::store_digits_B()", "Invalid Digit");
        }

        upL_t current_digits = blocks * m_digits_per_word;
        ptr += blocks;
        input += current_digits;
        digits_left -= current_digits;
        word_s += blocks;
    }

    //  End Filter
    if (word_e * m_digits_per_word < end){
//        cout << "End Filter" << endl;
        char buffer[19];
        if (ptr[0] > m_max_word){
            memset(buffer, 0, sizeof(buffer));
        }else{
            m_fp_convert_inverse(buffer, ptr, 1);
        }

        memcpy(buffer, input, digits_left);
        if(m_fp_convert_forward(ptr, buffer, 1)){
            throw InvalidParametersException("BasicYcdFileWriter::store_digits_B()", "Invalid Digit");
        }
    }

    m_file.store(P, write_offset, write_bytes, true);
    m_offset_extent = std::max(m_offset_extent, write_offset + write_bytes);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
upL_t BasicYcdFileWriter::start_access(
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
        std::string error = "BasicYcdFileWriter::start_access()\n";
        error += "Write out of bounds.\n";
        error += "Requested Range: ";
        error += std::to_string(offset);
        error += " - ";
        error += std::to_string(offset + digits);
        error += "\nAvailable Range: ";
        error += std::to_string(block_start);
        error += " - ";
        error += std::to_string(block_end);
        throw FileIO::FileException("BasicYcdFileWriter::start_access()", m_file.path(), error);
    }

    if (Alignment::int_past_aligned<FILE_ALIGNMENT>((upL_t)P) != 0){
        throw InvalidParametersException("BasicYcdFileWriter::start_access()", "Buffer is misaligned.");
    }

    Pbytes = Alignment::align_int_down<FILE_ALIGNMENT>(Pbytes);
    check_BufferTooSmall("BasicYcdFileWriter::start_access()", Pbytes, 2*FILE_ALIGNMENT);

    offset -= block_start;

    //  # of words that can be safely loaded with current buffer size.
    upL_t max_words = (Pbytes - FILE_ALIGNMENT) / sizeof(u64_t);

    //  # of digits that can be safely loaded with current buffer size.
    upL_t max_digits = (max_words - 1) * m_digits_per_word;

    return max_digits;
}
void BasicYcdFileWriter::store_digits(
    const char* input,
    uiL_t offset, upL_t digits,
    const AlignedBufferC<BUFFER_ALIGNMENT>& buffer,
    BasicParallelizer& parallelizer, upL_t tds
){
    if (digits == 0){
        return;
    }

    void* P = buffer.ptr();
    upL_t Pbytes = buffer.len();

    uiL_t s = offset;
    uiL_t e = offset + digits;

    upL_t max_digits = start_access(offset, digits, P, Pbytes);
//    cout << "max_digits = " << max_digits << endl;
    while (digits > 0){
        upL_t current_digits = std::min(digits, max_digits);

//        cout << "offset = " << offset << ", current_digits = " << current_digits << endl;
        store_digits_B(input, (ufL_t)offset, current_digits, P, Pbytes, parallelizer, tds);

        input += current_digits;
        offset += current_digits;
        digits -= current_digits;
    }

    //  Update the written-to tracker.
    std::lock_guard<std::mutex> lg(m_lock);
    m_written |= Region<uiL_t>(s, e);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
