/* YCDDigitReader.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/13/2013
 * Last Modified    : 01/09/2018
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <algorithm>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/BasicLibs/Memory/AlignedMalloc.h"
#include "PublicLibs/SystemLibs/FileIO/FileIO.h"
#include "DigitViewer/DigitConverter/DigitConverter.h"
#include "DigitViewer/Globals.h"
#include "InconsistentMetadataException.h"
#include "YCDDigitReader.h"
namespace DigitViewer{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Constructors
YCDReader::YCDReader(
    std::string path,
    bool raw,
    InjectableBuffer<u64_t> buffer
)
    : m_max_id_length(0)
    , fp_convert(nullptr)
    , m_buffer(std::move(buffer))
{
    if (m_buffer.size() < 4096){
        m_buffer.reset(4096 / sizeof(u64_t));
    }

    //  Check the file name
    if (path.size() < 4){
        throw FileIO::FileException("YCDReader::YCDReader()", path, "File name is too short.");
    }

    //  Check extension
    if (path.substr(path.size() - 4, 4) != ".ycd"){
        throw FileIO::FileException("YCDReader::YCDReader()", path, "Invalid Extension");
    }

    //  Separate name and path.
    upL_t slash_index = path.size();
    while (slash_index > 0){
        char ch = path[slash_index - 1];
        if (ch == '/' || ch == '\\'){
            break;
        }
        slash_index--;
    }
    std::string base = path.substr(0, slash_index);
    m_name = path.substr(slash_index, path.size());

    //  Add base path to path list.
    m_paths.push_back(std::move(base));

    //  Separate name and ID #.
    upL_t id_index = m_name.size() - 4;
    while (id_index > 0){
        char ch = m_name[id_index - 1];
        if (ch < '0' || '9' < ch){
            break;
        }
        id_index--;
        m_max_id_length++;
    }
    m_name.resize(id_index);

    //  Open and parse the file header.
    current_file    = YCDFileReader(std::move(path));
    m_radix           = current_file.radix;
    m_digits_per_word = current_file.digits_per_word;
    m_first_digits    = current_file.first_digits;
    m_total_digits    = current_file.total_digits;
    m_digits_per_file = current_file.digits_per_file;
    m_blocks_per_file = (m_digits_per_file + m_digits_per_word - 1) / m_digits_per_word;

    //  Set the conversion function pointers.
    set_raw(raw);
}
void YCDReader::set_native_buffer(InjectableBuffer<u64_t> buffer){
    m_buffer = std::move(buffer);
    if (m_buffer.size() < 4096){
        m_buffer.reset(4096 / sizeof(u64_t));
    }
}
void YCDReader::clear_native_buffer(){
    m_buffer.reset(4096 / sizeof(u64_t));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void YCDReader::print() const{
    Console::println_labelm(8, "Name:", m_name);

    const upL_t MARGIN = 20;

    Console::println_labelm(MARGIN, "max_id_length:", m_max_id_length);

    //  Search Paths
    Console::println("Search Paths:");
    for (const std::string& str : m_paths){
        Console::print("    ");
        Console::println(str);
    }
    Console::println();

    Console::println_labelm         (MARGIN, "Radix:", m_radix);
    Console::println_labelm_commas  (MARGIN, "digits_per_word:", m_digits_per_word);
    Console::println_labelm         (MARGIN, "first_digits:", m_first_digits);
    Console::println_labelm_commas  (MARGIN, "total_digits:", m_total_digits);
    Console::println_labelm_commas  (MARGIN, "digits_per_file:", m_digits_per_file);
    Console::println_labelm_commas  (MARGIN, "blocks_per_file:", m_blocks_per_file);
    Console::println_labelm_commas  (MARGIN, "bin_buffer_L:", m_buffer.size());
    Console::println_labelm_commas  (MARGIN, "buffer_L:", buffer_L);
    Console::println_labelm         (24, "Iterator File Offset:", iter_f_offset);
    Console::println_labelm_commas  (24, "Iterator Buffer Offset:", iter_b_offset);
    Console::println();
}
int YCDReader::get_radix() const{
    return m_radix;
}
uiL_t YCDReader::get_digits() const{
    return m_total_digits;
}
void YCDReader::set_raw(bool raw){
    //  Clear the cache
    clear_buffer();

    switch (m_radix){
        case 10:
            fp_convert = raw ? u64f_to_d19r : u64f_to_d19a;
            break;
        case 16:
            fp_convert = raw ? u64f_to_h16r : u64f_to_h16a;
            break;
        default:;
    }
}
bool YCDReader::check_range(uiL_t start, uiL_t end){
    //  Checks to see if all the necessary files to access the range [start, end)
    //  exist and can be opened.

    //  Find file boundaries
    uiL_t file_start = start / m_digits_per_file;
    uiL_t file_end   = (end + m_digits_per_file - 1) / m_digits_per_file;

    bool ret = true;

    for (uiL_t file = file_start; file < file_end; file++){
        try{
            set_current_file(file);
        }catch (InconsistentMetaData& e){
            e.print();
            return false;
        }catch (Exception&){
            if (m_total_digits != 0 && m_total_digits < end){
                Console::Warning("This digit stream does not have enough digits.");
                return false;
            }
            if (ret){
                Console::Warning("The following needed files are missing or inaccessible:");
            }
            std::string path = m_name + std::to_string(file) + ".ycd";
            Console::println(path);
            ret = false;
        }
    }

    return ret;
}
std::string YCDReader::get_first_digits(upL_t L){
    return m_first_digits;
}
void YCDReader::read(uiL_t pos, char* str, upL_t digits){
    //  This method reads digits from the file.
    //  It reads the following region:
    //      [pos, pos + digits)

    //  "pos" is relative to the start of the constant. Thus it open as many
    //  files as needed to get that range. If any of those files are missing
    //  or fail to open, then it will throw an exception.

    if (digits == 0){
        return;
    }

    uiL_t end = pos + digits;
    if (m_total_digits != 0 && end > m_total_digits){
        throw FileIO::FileException("YCDReader::read()", m_name, "Out of range.");
    }

    //  Find file boundaries
    uiL_t file_start = pos / m_digits_per_file;
    uiL_t file_end   = (end + m_digits_per_file - 1) / m_digits_per_file;

    //  Only 1 file
    if (file_end - file_start == 1){
        set_current_file(file_start);
//        cout << file_start << "  " << pos << "   " << pos + digits << "  " << digits << endl;
        current_file.read_chars(pos, str, digits, m_buffer, m_buffer.size(), fp_convert);
        return;
    }

    //  Read from each file.
    for (uiL_t file = file_start; file < file_end; file++){
        //  Get boundaries
        uiL_t local_start = file * m_digits_per_file;
        uiL_t local_end   = local_start + m_digits_per_file;

        //  Set file
        set_current_file(file);
//        cout << "file = " << file << endl;

        //  First file may be a partial file.
        if (file == file_start){
            upL_t current = (upL_t)(local_end - pos);
            current_file.read_chars(pos, str, current, m_buffer, m_buffer.size(), fp_convert);
            str += current;
            pos += current;
            digits -= current;
            continue;
        }

        //  Top filter
        upL_t current = digits;
        if (current > m_digits_per_file){
            current = (upL_t)m_digits_per_file;
        }

        current_file.read_chars(pos, str, current, m_buffer, m_buffer.size(), fp_convert);

        str += current;
        pos += current;
        digits -= current;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void YCDReader::add_search_path(std::string path){
    //  Make sure path ends in a slash.
    if (path.size() != 0){
        char back = path.back();
        if (back != '/' && back != '\\')
            path += '/';
    }
    m_paths.push_back(std::move(path));
}
const std::string& YCDReader::get_name() const{
    return m_name;
}
ufL_t YCDReader::get_digits_per_file() const{
    return m_digits_per_file;
}
void YCDReader::print_paths() const{
    for (const std::string& str : m_paths){
        Console::print("    ");
        Console::println(str);
    }
}
upL_t YCDReader::get_num_paths() const{
    return m_paths.size();
}
void YCDReader::load_new_file(std::string path, uiL_t id){
    //  Loads a new file, checks all the metadata and updates "total_digits" and
    //  "max_id_length" if possible.

    //  Get id length.
    std::wstring id_string = std::to_wstring(id);
    upL_t query_id_length = id_string.size();

    //  Open the file.
    YCDFileReader new_file(std::move(path));

    //  Cross check all the metadata.
    if (new_file.file_version != current_file.file_version){
        throw InconsistentMetaData("YCDReader::load_new_file", path, "File version does not match.");
    }
    if (new_file.radix != current_file.radix){
        throw InconsistentMetaData("YCDReader::load_new_file", path, "Radix does not match.");
    }
    if (new_file.digits_per_file != current_file.digits_per_file){
        throw InconsistentMetaData("YCDReader::load_new_file", path, "Digits per file does not match.");
    }
    if (new_file.digits_per_word != current_file.digits_per_word){
        throw InconsistentMetaData("YCDReader::load_new_file", path, "Digits per word does not match.");
    }
    if (m_total_digits != 0 &&
        current_file.total_digits != 0 &&
        m_total_digits != current_file.total_digits
    ){
        throw InconsistentMetaData("YCDReader::load_new_file", path, "Total digit does not match.");
    }

    //  Set new total digits.
    if (m_total_digits == 0){
        m_total_digits = new_file.total_digits;
    }

    //  Update maximum id length.
    if (m_max_id_length < query_id_length){
        m_max_id_length = query_id_length;
    }

    current_file = std::move(new_file);
}
void YCDReader::set_current_file(uiL_t id){
    //  Open the specified file id.
    //  This funtion will search all the search paths and will try a variety of
    //  different zero-padded number schemes. If this function successfully
    //  opens a file with an id that is longer than the current "max_id_length",
    //  it will update that value to reflect that new longest id.

    //  Already on the right file.
    if (current_file.file_id == id){
        return;
    }

    std::string id_string = std::to_string(id);
    upL_t query_id_length = id_string.size();
    upL_t limit_id_length = std::max(query_id_length, m_max_id_length);

    //  For each search path:
    for (std::string& path : m_paths){
        //  Try all zero-padded schemes up to the maximum length.
        std::string base_path = path + m_name;
        upL_t id_length = query_id_length;
        while (true){
            //  Build the full path.
            std::string full_path = base_path + id_string + ".ycd";

            //  If the file exists, we're done. Open it.
            if (FileIO::FileExists(full_path.c_str())){
                load_new_file(std::move(full_path), id);
                return;
            }

            //  File not found after searching all zero-padded lengths.
            if (id_length == limit_id_length){
                break;
            }

            //  Add another leading zero.
            id_length++;
            base_path += '0';
        }
    }

    std::string error = "Unable to open file #";
    error += std::to_string(id);
    throw FileIO::FileException("YCDReader::set_current_file()", m_name, error);
}
YM_NO_INLINE void YCDReader::reload(){
    //  This overrides the default reload() function. This is necessary because
    //  the default reload() will try to load as much as possible.
    //
    //  However, loading too much will cause an exception due to going out of
    //  bounds. And the extended file formats mean that you don't know how
    //  many digits are left.
    //
    //  This override will load as much as possible, but it will never cross
    //  a file boundary. So it will load at most to the end of a file.


    //  The function is mostly copy/paste from DigitViewer::reload() with
    //  the file-limiting code injected into the middle.

    //  Buffer isn't initialized yet.
    if (buffer_L == 0){
        upL_t buffer_size = YC_DIGITREADER_DEFAULT_BUFFER;
        m_digit_buffer = std::unique_ptr<char[]>(new char[buffer_size]);

        //  Do this assignment last - just in case the above throws.
        buffer_L = buffer_size;
    }

    //  Wipe the buffer now. This will keep the object in a consistent state
    //  in the event of an exception.
    clear_buffer();

    upL_t read  = buffer_L;
    uiL_t start = iter_f_offset;
    uiL_t end   = iter_f_offset + read;
    if (m_total_digits != 0 && start >= m_total_digits){
        std::string error("No more digits left: ");
        error += std::to_string(m_total_digits);
        throw FileIO::FileException("YCDReader::reload()", m_name, std::move(error));
    }

    //  Get read limit.
    uiL_t read_offset_limit = end;

    //  Don't cross a file boundary.
    uiL_t file_id = start / m_digits_per_file;
    uiL_t file_limit = (file_id + 1) * m_digits_per_file;
    read_offset_limit = std::min(read_offset_limit, file_limit);

    //  Set the current file now. This will update "total_digits" in time for
    //  the next check.
    //  This very important because the required file could be an incomplete
    //  file that knows the end of this digit stream. Without that information
    //  it is possible for this function to fetch too many digits. This will
    //  cause "read()" to throw an exception since it will not tolerate any
    //  reads that are out of bounds.
    set_current_file(file_id);

    //  Don't overrun total digits.
    if (m_total_digits != 0 && read_offset_limit > m_total_digits){
        read_offset_limit = m_total_digits;
    }

    //  Calculate how much to actually read.
    upL_t current_b_offset = 0;
    if (end > read_offset_limit){
        read = (upL_t)(read_offset_limit - start);
        current_b_offset = buffer_L - read;
    }

    //  Read into buffer
    this->read(start, &m_digit_buffer[current_b_offset], read);

    //  Do this assignment last to protect against an exception.
    iter_b_offset = current_b_offset;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
