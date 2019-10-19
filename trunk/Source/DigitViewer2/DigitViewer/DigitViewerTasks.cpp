/* DigitViewerTasks.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/15/2018
 * Last Modified    : 02/18/2018
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <algorithm>
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Margin.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/ConsoleIO/Array.h"
#include "PublicLibs/Exceptions/InvalidParametersException.h"
#include "PublicLibs/BasicLibs/StringTools/ToString.h"
#include "PublicLibs/BasicLibs/Memory/SmartBuffer.h"
#include "PublicLibs/SystemLibs/Concurrency/Parallelizers.h"
#include "PublicLibs/SystemLibs/Environment/Environment.h"
#ifdef YMP_STANDALONE
#include "PrivateLibs/SystemLibs/ParallelFrameworks/ParallelFrameworks.h"
#endif
#include "DigitViewer2/Globals.h"
#include "DigitViewer2/PrintHelpers.h"
#include "DigitViewer2/RawToAscii/RawToAscii.h"
#include "DigitViewer2/DigitWriters/BasicDigitWriter.h"
#include "DigitViewer2/DigitWriters/BasicTextWriter.h"
#include "DigitViewer2/DigitWriters/BasicYcdSetWriter.h"
#include "DigitViewerTasks.h"
namespace DigitViewer2{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void view_range(BasicDigitReader& reader){
    //  Print out digits in a specific range.
    Console::println("\n\nView a Range of Digits");
    Console::println();

    uiL_t limit = reader.stream_end();
    if (limit == 0){
        limit = (uiL_t)0 - 1;
    }

    uiL_t start = Console::scan_label_uiL_range("Starting Digit: ", 1, limit);
    Console::println();
    start--;
    limit -= start;

    //  Don't print out more than 100k digits.
    if (limit > 100000){
        limit = 100000;
    }

    upL_t digits = Console::scan_label_upL_range("Digits to View: ", 1, (upL_t)limit);
    Console::println();

    //  Check range
    if (!reader.range_is_available(start, digits)){
        Console::print("This range of digits is not accessible.");
        return;
    }

    using DigitViewer2::BUFFER_ALIGNMENT;
    upL_t bytes = reader.recommend_buffer_size(digits);
    SmartBuffer<> buffer(bytes, BUFFER_ALIGNMENT);

    //  Read digits
    std::string str(digits, '-');
    reader.load_digits(
        &str[0], nullptr,
        start, digits,
        AlignedBufferC<BUFFER_ALIGNMENT>(buffer, bytes),
        parallelizer_none, 1
    );

    switch (reader.radix()){
    case 10:
        RawToAscii::raw_to_dec(&str[0], &str[0], digits);
        break;
    case 16:
        RawToAscii::raw_to_hex(&str[0], &str[0], digits);
        break;
    default:
        throw InvalidParametersException("view_range()", "Invalid Radix");
    }

    //  Display
    DisplayFancy(start, &str[0], digits);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void process_start(
    BasicDigitReader& reader,
    uiL_t start, uiL_t digits
){
    Console::println("Processing " + StringTools::tostr(digits, StringTools::COMMAS) + " digits...");
    Console::println("    From: " + StringTools::tostr(start + 1, StringTools::COMMAS));
    Console::println("    To  : " + StringTools::tostr(start + digits, StringTools::COMMAS));
    Console::println();
    Console::println();

    //  Check range
    if (!reader.range_is_available(start, digits)){
        Console::print("This range of digits is not accessible.");
        return;
    }

    Console::println("Hash(A - B) are the digits from A - B as an integer modulo (2^61 - 1).");
    Console::println();
    Console::println();
}
class StatsTracker{
public:
    StatsTracker(BasicDigitReader& reader, uiL_t start)
        : m_radix(reader.radix())
        , m_start(start)
        , m_offset(start)
        , m_stats(m_radix)
    {
        std::string front = reader.first_digits();
        upL_t front_digits = front.find(".");
        if (front_digits == std::string::npos){
            throw InvalidParametersException("compute_stats()", "No decimal place found.");
        }
        front = front.substr(0, front_digits);

        switch (reader.radix()){
        case 10:
            RawToAscii::dec_to_raw(&front[0], &front[0], front_digits);
            m_top_hash = DigitHash::accumulate<10>(0, &front[0], front_digits);
            break;

        case 16:
            RawToAscii::hex_to_raw(&front[0], &front[0], front_digits);
            m_top_hash = DigitHash::accumulate<16>(0, &front[0], front_digits);
            break;

        default:
            throw InvalidParametersException("top_hash()", "Invalid radix.");
        }

        Console::print_marginl(20, "Current Offset");
        if (m_start == 0){
            Console::print_marginl(23, "Hash(start - current)");
        }
        std::string label = "Hash(" + std::to_string(m_start + 1) + " - current)  ";
        m_width = std::max((upL_t)21, label.size());
        Console::print_marginl(m_width, label);
        Console::print("Digit Counts (" + std::to_string(m_start + 1) + " - current)");
        Console::println("");
    }

    DigitStats& stats(){
        return m_stats;
    }

    void print_progress(){
        Console::print_marginl_commas(20, m_offset);
        if (m_start == 0){
            hash_t hash = m_top_hash * (hash_t(m_radix) ^ (m_offset - m_start)) + m_stats.hash();
            Console::print_marginl(23, hash.value());
        }
        Console::print_marginl(m_width, m_stats.hash().value());
        Console::println(m_stats.counts().to_string());
    }

    void operator+=(upL_t digits){
        m_offset += digits;
    }

private:
    char m_radix;
    uiL_t m_start;
    uiL_t m_offset;
    upL_t m_width;
    hash_t m_top_hash;
    DigitStats m_stats;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
BasicParallelizer& get_parallelizer(){
#ifdef YMP_STANDALONE
    //  If we're compiling inside y-cruncher, we can use the (better) internal
    //  parallel frameworks. Not that it will make much of a difference anyway
    //  since the parallelism here is very simple.
    static std::unique_ptr<BasicParallelizer> parallelizer = ParallelFrameworks::get_default()->make();
    return *parallelizer;
#else
    return parallelizer_default;
#endif
}
void compute_stats(BasicDigitReader& reader){
    Console::println("\n\nCompute Digit Statistics");
    Console::println();

    uiL_t limit = reader.stream_end();
    if (limit == 0){
        limit = (uiL_t)0 - 1;
    }

    uiL_t start = Console::scan_label_uiL_range("Starting Digit: ", 1, limit);
    uiL_t end   = Console::scan_label_uiL_range("Ending Digit:   ", start, limit);
    start--;
    uiL_t digits = end - start;
    Console::println();


    process_start(reader, start, digits);

    //  Use all cores.
    upL_t tds = Environment::GetLogicalProcessors();
    BasicParallelizer& parallelizer = get_parallelizer();

    //  Don't use more than 1/4 of the remaining available memory.
    upL_t mem_limit = Environment::GetFreePhysicalMemory() / 4;

    const upL_t BLOCK_SIZE = 1000000000;

    upL_t bytes;
    {
        //  Get recommended buffer size.
        bytes = reader.recommend_buffer_size(std::min(digits, (uiL_t)BLOCK_SIZE));
        bytes = std::min(bytes, mem_limit);
    }

    //  Allocate buffers.
    using DigitViewer2::BUFFER_ALIGNMENT;
    SmartBuffer<> buffer(bytes, BUFFER_ALIGNMENT);

    //  Stats Tracker
    StatsTracker tracker(reader, start);
    tracker.print_progress();

    //  Run and print progress.
    uiL_t offset = start;
    while (digits > 0){
        upL_t block = (upL_t)std::min(digits, (uiL_t)BLOCK_SIZE);

        reader.load_stats(
            tracker.stats(),
            offset, block,
            AlignedBufferC<BUFFER_ALIGNMENT>(buffer, bytes),
            parallelizer, tds
        );

        offset += block;
        digits -= block;
        tracker += block;
        tracker.print_progress();
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void process_write(
    BasicDigitReader& reader, uiL_t read_offset, uiL_t digits,
    BasicDigitWriter& writer, uiL_t write_offset
){
    process_start(reader, read_offset, digits);

    //  Use all cores.
    upL_t tds = Environment::GetLogicalProcessors();
    BasicParallelizer& parallelizer = get_parallelizer();

    //  Don't use more than 1/4 of the remaining available memory.
    upL_t mem_limit = Environment::GetFreePhysicalMemory() / 4;

    const upL_t BLOCK_SIZE = 1000000000;

    //  Find a suitable block size.
    upL_t block_size = (upL_t)std::min(digits, (uiL_t)BLOCK_SIZE) * 2;
    upL_t bytes;
    do{
        block_size = (block_size + 1) / 2;
        bytes = std::max(
            reader.recommend_buffer_size(block_size),
            writer.recommend_buffer_size(block_size)
        );
    }while (block_size + bytes > mem_limit);
//    cout << "bytes = " << bytes << endl;

    //  Allocate buffers.
    using DigitViewer2::BUFFER_ALIGNMENT;
    SmartBuffer<> buffer(bytes, BUFFER_ALIGNMENT);
    SmartBuffer<char> raw(block_size);
    AlignedBufferC<BUFFER_ALIGNMENT> frame0(buffer, bytes);

    //  Stats Tracker
    StatsTracker tracker(reader, read_offset);
    tracker.print_progress();

    //  Run and print progress.
    while (digits > 0){
        upL_t print_block = (upL_t)std::min(digits, (uiL_t)BLOCK_SIZE);
        while (print_block > 0){
            upL_t block = std::min(print_block, block_size);

            reader.load_digits(
                raw, &tracker.stats(),
                read_offset, block,
                frame0,
                parallelizer, tds
            );
            writer.store_digits(
                raw,
                write_offset, block,
                frame0,
                parallelizer, tds
            );

            read_offset += block;
            write_offset += block;
            digits -= block;
            tracker += block;
            print_block -= block;
        }
        tracker.print_progress();
    }
}
void to_text_file(BasicDigitReader& reader){
    Console::println("\n\nWrite to Text File");
    Console::println();

    uiL_t limit = reader.stream_end();
    if (limit == 0){
        limit = (uiL_t)0 - 1;
    }

    uiL_t start = Console::scan_label_uiL_range("Starting Digit: ", 1, limit);
    uiL_t end   = Console::scan_label_uiL_range("Ending Digit:   ", start + MIN_COMPRESS_DIGITS, limit);
    start--;
    uiL_t digits = end - start;
    Console::println();

    Console::println("Enter the path of the destination file:");
    std::string path = Console::scan_utf8();
    Console::println();

    //  Extract the extension
    size_t extension_offset = path.rfind('.');

    //  Append extension if needed.
    if (extension_offset >= path.size() ||
        path.substr(extension_offset) != ".txt"
    ){
        path += ".txt";
    }

    BasicTextWriter writer(
        path,
        start == 0 ? reader.first_digits() : "",
        reader.radix()
    );
    process_write(reader, start, digits, writer, 0);
}
void to_ycd_file_all(BasicDigitReader& reader){
    Console::println("\n\nWrite to Compressed File");
    Console::println();

    uiL_t limit = reader.stream_end();
    if (limit == 0){
        limit = (uiL_t)0 - 1;
    }
    if (limit < MIN_COMPRESS_DIGITS){
        Console::println("Too few digits to compress.");
        return;
    }

    Console::println("Starting Digit: 1");
    uiL_t start = 1;
    uiL_t end;
    while (true){
        end = Console::scan_label_uiL_range("Ending Digit:   ", 0, limit);
        if (end >= MIN_COMPRESS_DIGITS){
            break;
        }
        Console::print("Please select at least ", 'Y');
        Console::print_commas(MIN_COMPRESS_DIGITS, 'G');
        Console::println(" digits.", 'Y');
    }
    start--;
    uiL_t digits = end - start;
    Console::println();

    ufL_t digits_per_file;
    do{
        digits_per_file = (ufL_t)Console::scan_label_uiL_suffix_range(
            "Digits per file (0 for single file): ", 0, digits
        );
        Console::println();
        if (digits_per_file == 0){
            digits_per_file = (ufL_t)digits;
            if (digits_per_file != digits){
                throw InvalidParametersException(
                    "to_ycd_file()",
                    "Unable to compress to a single file.\n"
                    "The number of digits exceeds the address length of a file pointer."
                );
            }
        }
        if (digits_per_file < MIN_COMPRESS_DIGITS){
            Console::Warning("Must be at least " + StringTools::tostr(MIN_COMPRESS_DIGITS) + ".\n");
            continue;
        }
        break;
    }while (1);

    Console::println("Enter a name for the digits:");
    std::string name = Console::scan_labelc_utf8("Name");

    Console::println("\nEnter the destination path (SPACE for default):");
    std::string path = Console::scan_utf8();
    if (path.size() != 0 && path[0] == ' '){
        path.clear();
    }

    BasicYcdSetWriter writer(
        path, name,
        reader.first_digits(),
        reader.radix(),
        digits_per_file,
        digits
    );
    process_write(reader, start, digits, writer, 0);
}
void to_ycd_file_partial(BasicDigitReader& reader){
    Console::println("\n\nWrite to Compressed File (partial)");
    Console::println();

    uiL_t limit = reader.stream_end();
    if (limit == 0){
        limit = (uiL_t)0 - 1;
    }
    if (limit < MIN_COMPRESS_DIGITS){
        Console::println("Too few digits to compress.");
        return;
    }

    ufL_t digits_per_file = (ufL_t)Console::scan_label_uiL_suffix_range(
        "Digits per file: ", MIN_COMPRESS_DIGITS, limit
    );

    Console::println();
    uiL_t start_id = Console::scan_label_uiL_range("Start ID: ", 0);
    uiL_t end_id   = Console::scan_label_uiL_range("End   ID: ", start_id) + 1;
    Console::println();

    uiL_t start_pos = digits_per_file * start_id;
    uiL_t end_pos   = digits_per_file * end_id;
    if (start_pos >= limit){
        Console::Warning("No Digits to Recompress.");
        return;
    }
    if (end_pos > limit){
        end_pos = limit;
    }

    Console::println("Enter a name for the digits:");
    std::string name = Console::scan_labelc_utf8("Name");

    Console::println("\nEnter the destination path (SPACE for default):");
    std::string path = Console::scan_utf8();
    if (path.size() != 0 && path[0] == ' '){
        path.clear();
    }

    BasicYcdSetWriter writer(
        path, name,
        reader.first_digits(),
        reader.radix(),
        digits_per_file,
        limit
    );
    process_write(reader, start_pos, end_pos - start_pos, writer, start_pos);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
