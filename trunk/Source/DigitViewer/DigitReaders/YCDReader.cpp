/* YCDReader.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/13/2013
 * Last Modified    : 07/31/2013
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/Memory/AlignedMalloc.h"
#include "PublicLibs/FileIO/FileIO.h"
#include "PublicLibs/FileIO/FileException.h"
#include "DigitViewer/DigitConverter/DigitConverter.h"
#include "DigitViewer/Globals.h"
#include "YCDReader.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
namespace DigitViewer{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Exception
class InconsistentMetaData : public FileIO::FileException{
public:
    static const char TYPENAME[];

    using FileException::FileException;

public:
    virtual void fire() const override{
        throw *this;
    }
    virtual const char* get_typename() const override{
        return TYPENAME;
    }
    virtual Exception* clone() const override{
        return new InconsistentMetaData(*this);
    }
};
const char InconsistentMetaData::TYPENAME[] = "InconsistentMetaData";
ExceptionFactoryT<InconsistentMetaData> InconsistentMetaData_Instance;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Constructors
YCDReader::YCDReader(std::string path_, bool raw, upL_t buffer_size)
    : max_id_length(0)
    , fp_convert(nullptr)
{
    if (buffer_size < 4096){
        buffer_size = 4096;
    }

    //  Check the file name
    if (path_.size() < 4){
        throw FileIO::FileException("YCDReader::YCDReader()", path_, "File name is too short.");
    }

    //  Check extension
    if (path_.substr(path_.size() - 4, 4) != ".ycd"){
        throw FileIO::FileException("YCDReader::YCDReader()", path_, "Invalid Extension");
    }

    //  Separate name and path.
    upL_t slash_index = path_.size();
    while (slash_index > 0){
        char ch = path_[slash_index - 1];
        if (ch == '/' || ch == '\\'){
            break;
        }
        slash_index--;
    }
    std::string base = path_.substr(0, slash_index);
    name = path_.substr(slash_index, path_.size());

    //  Add base path to path list.
    paths.push_back(std::move(base));

    //  Separate name and ID #.
    upL_t id_index = name.size() - 4;
    while (id_index > 0){
        char ch = name[id_index - 1];
        if (ch < '0' || '9' < ch){
            break;
        }
        id_index--;
        max_id_length++;
    }
    name.resize(id_index);

    //  Open and parse the file header.
    current_file    = YCDFileReader(std::move(path_));
    radix           = current_file.radix;
    digits_per_word = current_file.digits_per_word;
    first_digits    = current_file.first_digits;
    total_digits    = current_file.total_digits;
    digits_per_file = current_file.digits_per_file;
    blocks_per_file = (digits_per_file + digits_per_word - 1) / digits_per_word;

    //  Set the conversion function pointers.
    set_raw(raw);

    bin_buffer_L = buffer_size / sizeof(u64_t);
    bin_buffer = (u64_t*)aligned_malloc(bin_buffer_L * sizeof(u64_t), 2*sizeof(u64_t));
}
YCDReader::~YCDReader(){
    aligned_free(bin_buffer);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void YCDReader::print() const{
    Console::println_labelm(8, "Name:", name);

    const upL_t MARGIN = 20;

    Console::println_labelm(MARGIN, "max_id_length:", max_id_length);

    //  Search Paths
    Console::println("Search Paths:");
    for (const std::string& str : paths){
        Console::print("    ");
        Console::println(str);
    }
    Console::println();

    Console::println_labelm         (MARGIN, "Radix:", radix);
    Console::println_labelm_commas  (MARGIN, "digits_per_word:", digits_per_word);
    Console::println_labelm         (MARGIN, "first_digits:", first_digits);
    Console::println_labelm_commas  (MARGIN, "total_digits:", total_digits);
    Console::println_labelm_commas  (MARGIN, "digits_per_file:", digits_per_file);
    Console::println_labelm_commas  (MARGIN, "blocks_per_file:", blocks_per_file);
    Console::println_labelm_commas  (MARGIN, "bin_buffer_L:", bin_buffer_L);
    Console::println_labelm_commas  (MARGIN, "buffer_L:", buffer_L);
    Console::println_labelm         (24, "Iterator File Offset:", iter_f_offset);
    Console::println_labelm_commas  (24, "Iterator Buffer Offset:", iter_b_offset);
    Console::println();
}
int YCDReader::get_radix() const{
    return radix;
}
uiL_t YCDReader::get_digits() const{
    return total_digits;
}
void YCDReader::set_raw(bool raw){
    //  Clear the cache
    clear_buffer();

    switch (radix){
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
    uiL_t file_start = start / digits_per_file;
    uiL_t file_end   = (end + digits_per_file - 1) / digits_per_file;

    bool ret = true;

    for (uiL_t file = file_start; file < file_end; file++){
        try{
            set_current_file(file);
        }catch (InconsistentMetaData& e){
            e.print();
            return false;
        }catch (Exception&){
            if (total_digits != 0 && total_digits < end){
                Console::Warning("This digit stream does not have enough digits.");
                return false;
            }
            if (ret){
                Console::Warning("The following needed files are missing or inaccessible:");
            }
            std::string path = name + std::to_string(file) + ".ycd";
            Console::println(path);
            ret = false;
        }
    }

    return ret;
}
std::string YCDReader::get_first_digits(upL_t L){
    return first_digits;
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
    if (total_digits != 0 && end > total_digits){
        throw FileIO::FileException("YCDReader::read()", name, "Out of range.");
    }

    //  Find file boundaries
    uiL_t file_start = pos / digits_per_file;
    uiL_t file_end   = (end + digits_per_file - 1) / digits_per_file;

    //  Only 1 file
    if (file_end - file_start == 1){
        set_current_file(file_start);
//        cout << file_start << "  " << pos << "   " << pos + digits << "  " << digits << endl;
        current_file.read_chars(pos, str, digits, bin_buffer, bin_buffer_L, fp_convert);
        return;
    }

    //  Read from each file.
    for (uiL_t file = file_start; file < file_end; file++){
        //  Get boundaries
        uiL_t local_start = file * digits_per_file;
        uiL_t local_end   = local_start + digits_per_file;

        //  Set file
        set_current_file(file);
//        cout << "file = " << file << endl;

        //  First file may be a partial file.
        if (file == file_start){
            upL_t current = (upL_t)(local_end - pos);
            current_file.read_chars(pos, str, current, bin_buffer, bin_buffer_L, fp_convert);
            str += current;
            pos += current;
            digits -= current;
            continue;
        }

        //  Top filter
        upL_t current = digits;
        if (current > digits_per_file)
            current = (upL_t)digits_per_file;

        current_file.read_chars(pos, str, current, bin_buffer, bin_buffer_L, fp_convert);

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
    paths.push_back(std::move(path));
}
const std::string& YCDReader::get_name() const{
    return name;
}
ufL_t YCDReader::get_digits_per_file() const{
    return digits_per_file;
}
void YCDReader::print_paths() const{
    for (const std::string& str : paths){
        Console::print("    ");
        Console::println(str);
    }
}
upL_t YCDReader::get_num_paths() const{
    return paths.size();
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
    if (total_digits != 0 &&
        current_file.total_digits != 0 &&
        total_digits != current_file.total_digits
    ){
        throw InconsistentMetaData("YCDReader::load_new_file", path, "Total digit does not match.");
    }

    //  Set new total digits.
    if (total_digits == 0){
        total_digits = new_file.total_digits;
    }

    //  Update maximum id length.
    if (max_id_length < query_id_length){
        max_id_length = query_id_length;
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
    upL_t limit_id_length = query_id_length > max_id_length ? query_id_length : max_id_length;

    //  For each search path:
    for (std::string& path : paths){
        //  Try all zero-padded schemes up to the maximum length.
        std::string base_path = path + name;
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
    throw FileIO::FileException("YCDReader::set_current_file()", name, error);
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
        buffer = std::unique_ptr<char[]>(new char[buffer_size]);

        //  Do this assignment last - just in case the above throws.
        buffer_L = buffer_size;
    }

    //  Wipe the buffer now. This will keep the object in a consistent state
    //  in the event of an exception.
    clear_buffer();

    upL_t read  = buffer_L;
    uiL_t start = iter_f_offset;
    uiL_t end   = iter_f_offset + read;
    if (total_digits != 0 && start >= total_digits){
        std::string error("No more digits left: ");
        error += std::to_string(total_digits);
        throw FileIO::FileException("YCDReader::reload()", name, std::move(error));
    }

    //  Get read limit.
    uiL_t read_offset_limit = end;

    //  Don't cross a file boundary.
    uiL_t file_id = start / digits_per_file;
    uiL_t file_limit = (file_id + 1) * digits_per_file;
    if (read_offset_limit > file_limit){
        read_offset_limit = file_limit;
    }

    //  Set the current file now. This will update "total_digits" in time for
    //  the next check.
    //  This very important because the required file could be an incomplete
    //  file that knows the end of this digit stream. Without that information
    //  it is possible for this function to fetch too many digits. This will
    //  cause "read()" to throw an exception since it will not tolerate any
    //  reads that are out of bounds.
    set_current_file(file_id);

    //  Don't overrun total digits.
    if (total_digits != 0 && read_offset_limit > total_digits){
        read_offset_limit = total_digits;
    }

    //  Calculate how much to actually read.
    upL_t current_b_offset = 0;
    if (end > read_offset_limit){
        read = (upL_t)(read_offset_limit - start);
        current_b_offset = buffer_L - read;
    }

    //  Read into buffer
    this->read(start, &buffer[current_b_offset], read);

    //  Do this assignment last to protect against an exception.
    iter_b_offset = current_b_offset;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
