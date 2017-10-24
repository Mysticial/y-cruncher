/* EndPointTasks.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/02/2012
 * Last Modified    : 08/04/2013
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string.h>

#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/ConsoleIO/Array.h"
#include "PublicLibs/Exceptions/StringException.h"

#include "Globals.h"
#include "PrintHelpers.h"

#include "DigitReaders/DigitReader.h"
#include "DigitReaders/TextReader.h"
#include "DigitReaders/YCDReader.h"

#include "DigitWriters/TextWriter.h"
#include "DigitWriters/YCDWriter.h"

namespace DigitViewer{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Helpers
YM_FORCE_INLINE u64_t hash_mul10(u64_t hash){
    hash += hash;
    hash = (hash & 0x1fffffffffffffffull) + (hash >> 61);

    hash *= 5;
    hash = (hash & 0x1fffffffffffffffull) + (hash >> 61);

    return hash;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Features
void ViewRange(DigitReader* file){
    //  Print out digits in a specific range.
    Console::println("\n\nView a Range of Digits\n");

    uiL_t limit = file->get_digits();
    if (limit == 0){
        limit = (uiL_t)0 - 1;
    }

    Console::println();

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
    if (!file->check_range(start, start + digits)){
        Console::print("This range of digits is not accessible.");
        return;
    }

    //  Set output format.
    file->set_raw(false);

    //  Read digits
    std::string str(digits, '-');
    file->read(start, &str[0], digits);

    //  Check bounds
    limit = file->get_digits();
    if (limit){
        if (start >= limit){
            Console::print("No digits to view.");
            return;
        }
        if (start + digits > limit){
            digits = (upL_t)(limit - start);
        }
    }

    //  Display
    DisplayFancy(start, &str[0], digits);
}
void CountDigits(DigitReader* file){
    //  Count up the # of instances of each digit.

    uiL_t limit = file->get_digits();
    if (limit == 0){
        limit = (uiL_t)0 - 1;
    }

    Console::println();

    uiL_t start = Console::scan_label_uiL_range("Starting Digit: ", 1, limit);
    uiL_t end   = Console::scan_label_uiL_range("Ending Digit:   ", start, limit);
    start--;

    //  Check range
    if (!file->check_range(start, end)){
        Console::print("This range of digits is not accessible.");
        return;
    }

    file->set_pos(start);
    file->set_raw(true);

    u64_t digit[16];
    memset(digit, 0, sizeof(digit));

    Console::println();

    uiL_t pos = start;
    while (pos < end){
        upL_t block = 1000000000;
        uiL_t left = end - pos;
        if (block > left){
            block = (upL_t)left;
        }

        for (upL_t c = 0; c < block; c++){
            digit[(int)file->next()]++;
        }

        pos += block;

        //  Print Results
        Console::print_commas(start + 1);
        Console::print(" - ");
        Console::print_commas(pos);
        Console::print(" :\t");
        Console::println_af(digit, file->get_radix());
    }
}
void ComputeHash(DigitReader* file){
    //  Count up the # of instances of each digit.

    uiL_t limit = file->get_digits();
    if (limit == 0){
        limit = (uiL_t)0 - 1;
    }

    Console::println();

    uiL_t end = Console::scan_label_uiL_range("Ending Digit:   ", 1, limit);

    //  Check range
    if (!file->check_range(0, end)){
        Console::print("This range of digits is not accessible.");
        return;
    }

    file->set_pos(0);
    file->set_raw(true);

    Console::println();

    //  Digits before decimal place
    u64_t hash = 0;
    {
        std::string first_digits = file->get_first_digits(64);

        bool decimal_found = false;
        for (char ch : first_digits){
            if (ch == '.'){
                decimal_found = true;
                break;
            }

            //  Accumulate hash
            hash = hash_mul10(hash);
            hash += ch - '0';
        }
        if (!decimal_found){
            throw StringException(
                "ComputeHash()",
                "A decimal place was not found within the first 63 bytes of the file."
            );
        }
    }

    uiL_t pos = 0;
    while (pos < end){
        upL_t block = 1000000000;
        uiL_t left = end - pos;
        if (block > left)
            block = (upL_t)left;

        //  Accumulate the hash 1 digit at a time.
        //  y-cruncher can do this much more efficiently by directly
        //  accumulating entire 64-bit compressed words at a time - thereby
        //  completely bypassing the need to decompress the digits at all.

        //  But that code is not trivial. So we'll stick with a simple
        //  implementation here.

        //  In any case, this feature is used to verify that the digits were
        //  written properly and that the Digit Viewer correctly decompresses
        //  them. So we actually *want* to decompress the digits before
        //  computing the hash.

        for (upL_t c = 0; c < block; c++){
            hash = hash_mul10(hash);
            hash += (int)file->next();
        }

        //  Make final correction before printing it out.
        if (hash > 0x1fffffffffffffffull){
            hash -= 0x1fffffffffffffffull;
        }

        pos += block;

        //  Print Results
        Console::print_commas(pos);
        Console::print(" :\t");
        Console::println(hash);
    }
}
void ToTextFile(DigitReader* file){
    Console::println("\n\nWrite to Text File.\n");

    uiL_t limit = file->get_digits();
    if (limit == 0){
        limit = (uiL_t)0 - 1;
    }

    Console::println();

    uiL_t start = Console::scan_label_uiL_range("Starting Digit: ", 1, limit);
    Console::println();
    start--;
    limit -= start;

    uiL_t digits = Console::scan_label_uiL_range("Digits to Write: ", 1, limit);
    Console::println();

    //  Check range
    if (!file->check_range(start, start + digits)){
        Console::print("This range of digits is not accessible.");
        return;
    }

    //  Use plain-text output. This is faster for the text writer.
    file->set_raw(false);

    Console::println("Enter the path of the destination file:");

    //  Prompt for path
    std::string path = Console::scan_utf8();

    //  Extract the extension
    size_t extension_offset = path.rfind('.');

    //  Append extension if needed.
    if (extension_offset >= path.size() ||
        path.substr(extension_offset) != ".txt"
    ){
        path += ".txt";
    }

    //  Create Writer
    TextWriter writer(
        path,
        start == 0 ? file->get_first_digits(100) : "",
        false,
        file->get_radix()
    );

    //  Allocate Buffer
    upL_t buffer_L = YC_DIGITWRITER_DEFAULT_BUFFER;
    auto str = std::unique_ptr<char[]>(new char[buffer_L]);

    Console::println();

    //  Streaming Loop
    //  The fastest way to do this is to use our own buffer and call the
    //  no-buffer methods for both the reader and the writer.
    uiL_t written = 0;
    while (digits > 0){
        upL_t current_digits = buffer_L;
        if (current_digits > digits){
            current_digits = (upL_t)digits;
        }

        //  Read digits
        file->read(start, str.get(), current_digits);

        //  Write digits
        writer.write(str.get(), current_digits);

        //  Update Counters
        start   += current_digits;
        digits  -= current_digits;
        written += current_digits;

        //  Print
        ClearLine();
        Console::print("Streaming:  ");
        Console::print_commas(written);
        Console::print("  digits written");
    }
}
void ToYCDFileAll(DigitReader* file){
    Console::println("\n\nWrite to Compressed File.\n");

    uiL_t limit = file->get_digits();
    if (limit == 0){
        limit = (uiL_t)0 - 1;
    }

    Console::println();

    uiL_t digits = Console::scan_label_uiL_range("Digits to Write: ", 1000000, limit);
    Console::println();

    //  Check range
    if (!file->check_range(0, digits)){
        Console::print("This range of digits is not accessible.");
        return;
    }

    //  Use plain-text output. This is faster for the text writer.
    file->set_raw(true);
    
    ufL_t digits_per_file;
    do{
        digits_per_file = static_cast<ufL_t>(Console::scan_label_uiL_suffix_range("Digits per file (0 for single file): ", 0, digits));
        Console::println();
        if (digits_per_file == 0){
            digits_per_file = (ufL_t)0 - 1;
        }
        if (digits_per_file < 1000000){
            Console::Warning("Must be at least 1,000,000.\n");
            continue;
        }
        break;
    }while (1);

    Console::println("Enter a name for the digits:");
    auto name = Console::scan_labelc_utf8("Name");

    Console::println("\nEnter the destination path (SPACE for default):");
    std::string path = Console::scan_utf8();
    if (path.size() != 0 && path[0] == ' '){
        path.clear();
    }

    //  Create the writer
    YCDWriter writer(
        std::move(path),
        std::move(name),
        file->get_first_digits(64),
        digits_per_file,
        0,
        file->get_radix()
    );

    //  Allocate Buffer
    upL_t buffer_L = YC_DIGITWRITER_DEFAULT_BUFFER;
    auto str = std::unique_ptr<char[]>(new char[buffer_L]);

    Console::println();

    //  Streaming Loop
    //  The fastest way to do this is to use our own buffer and call the
    //  no-buffer methods for both the reader and the writer.
    uiL_t current_position = 0;
    uiL_t written = 0;
    while (digits > 0){
        upL_t current_digits = buffer_L;
        if (current_digits > digits){
            current_digits = (upL_t)digits;
        }

        //  Read digits
        file->read(current_position, str.get(), current_digits);

        //  Write digits
        writer.write(str.get(), current_digits);

        //  Update Counters
        current_position += current_digits;
        digits           -= current_digits;
        written          += current_digits;

        //  Print
        ClearLine();
        Console::print("Streaming:  ");
        Console::print_commas(written, 0);
        Console::print("  digits written");
    }
}
void ToYCDFilePartial(YCDReader* file){
    Console::println("\n\nRecompress to Partial Compressed File.\n");

    uiL_t limit = file->get_digits();
    if (limit == 0){
        limit = (uiL_t)0 - 1;
    }

    Console::println();

    //  Use raw output.
    file->set_raw(true);
    
    ufL_t digits_per_file = static_cast<ufL_t>(Console::scan_label_uiL_range("Digits per file: ", 0, limit));

    Console::println();
    uiL_t start_id = Console::scan_label_uiL_range("Start ID: ", 0);
    uiL_t end_id   = Console::scan_label_uiL_range("End   ID: ", start_id) + 1;

    uiL_t start_pos = digits_per_file * start_id;
    uiL_t end_pos   = digits_per_file * end_id;
    if (start_pos >= limit){
        Console::Warning("\nNo Digits to Recompress.");
        return;
    }
    if (end_pos > limit){
        end_pos = limit;
    }

    //  Check range
    if (!file->check_range(start_pos, end_pos)){
        Console::print("This range of digits is not accessible.");
        return;
    }

    Console::println("\nEnter the destination path (SPACE for default):");
    std::string path = Console::scan_utf8();
    if (path.size() != 0 && path[0] == ' '){
        path.clear();
    }

    //  Get the name
    std::string name = file->get_name();

    //  Strip off the trailing " - ".
    while (name.size() > 0 && (name.back() == ' ' || name.back() == '-')){
        name.resize(name.size() - 1);
//        name.pop_back();
    }

    //  Create the writer
    YCDWriter writer(
        std::move(path),
        std::move(name),
        file->get_first_digits(64),
        digits_per_file,
        start_id,
        file->get_radix()
    );

    //  Allocate Buffer
    upL_t buffer_L = YC_DIGITWRITER_DEFAULT_BUFFER;
    auto str = std::unique_ptr<char[]>(new char[buffer_L]);

    Console::println();

    //  Streaming Loop
    //  The fastest way to do this is to use our own buffer and call the
    //  no-buffer methods for both the reader and the writer.
    uiL_t digits = end_pos - start_pos;
    uiL_t current_position = start_pos;
    uiL_t written = 0;
    while (digits > 0){
        upL_t current_digits = buffer_L;
        if (current_digits > digits){
            current_digits = (upL_t)digits;
        }

        //  Read digits
        file->read(current_position, str.get(), current_digits);

        //  Write digits
        writer.write(str.get(), current_digits);

        //  Update Counters
        current_position += current_digits;
        digits           -= current_digits;
        written          += current_digits;

        //  Print
        ClearLine();
        Console::print("Streaming:  ");
        Console::print_commas(written);
        Console::print("  digits written");
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
