/* BasicYcdSetReader.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/31/2018
 * Last Modified    : 02/01/2018
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <algorithm>
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/Exceptions/InvalidParametersException.h"
#include "PublicLibs/BasicLibs/Alignment/AlignmentTools.h"
#include "PublicLibs/BasicLibs/StringTools/ymb_STR.h"
#include "PublicLibs/BasicLibs/Alignment/AlignmentTools.h"
#include "PublicLibs/SystemLibs/FileIO/FileIO.h"
#include "InconsistentMetadataException.h"
#include "BasicYcdSetReader.h"
namespace DigitViewer2{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
BasicYcdSetReader::BasicYcdSetReader(const std::string& path)
    : m_max_id_length(0)
{
    //  Check the file name
    if (path.size() < 4){
        throw FileIO::FileException("BasicYcdSetReader::BasicYcdSetReader()", path, "File name is too short.");
    }

    //  Check extension
    if (path.substr(path.size() - 4, 4) != ".ycd"){
        throw FileIO::FileException("BasicYcdSetReader::BasicYcdSetReader()", path, "Invalid Extension");
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
//    uiL_t id = StringTools::parse_uL(&m_name[id_index]);
//    cout << "id = " << id << endl;
    m_name.resize(id_index);


    //  Open and parse the file header.
    std::unique_ptr<BasicYcdFileReader> file(new BasicYcdFileReader(path));

    uiL_t id = file->file_id();
    m_radix           = file->radix();
    m_digits_per_word = file->digits_per_word();
    m_first_digits    = file->first_digits();
    m_stream_end      = file->stream_end();
    m_digits_per_file = file->digits_per_file();
    m_blocks_per_file = (m_digits_per_file + m_digits_per_word - 1) / m_digits_per_word;

//    file->print();
    m_files.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(id),
        std::forward_as_tuple(std::move(file))
    );
}
void BasicYcdSetReader::print_paths() const{
    for (const std::string& path : m_paths){
        Console::print("    ");
        Console::println(path);
    }
}
void BasicYcdSetReader::add_search_path(std::string path){
    //  Make sure path ends in a slash.
    if (path.size() != 0){
        char back = path.back();
        if (back != '/' && back != '\\'){
            path += '/';
        }
    }
    m_paths.emplace_back(std::move(path));
}
void BasicYcdSetReader::flush_cache(){
    m_files.erase(++m_files.begin(), m_files.end());
}
BasicYcdFileReader& BasicYcdSetReader::load_file(const std::string& path, uiL_t id){
    //  Loads a new file, checks all the metadata and updates "stream_end" and
    //  "max_id_length" if possible.

    //  Get id length.
    std::string id_string = std::to_string(id);
    upL_t query_id_length = id_string.size();

    const BasicYcdFileReader& ref_file = *m_files.begin()->second;

    //  Open the file.
    std::unique_ptr<BasicYcdFileReader> new_file(new BasicYcdFileReader(path));

    //  Cross check all the metadata.
    if (new_file->file_version() != ref_file.file_version()){
        throw InconsistentMetaData("BasicYcdSetReader::load_file", path, "File version does not match.");
    }
    if (new_file->radix() != ref_file.radix()){
        throw InconsistentMetaData("BasicYcdSetReader::load_file", path, "Radix does not match.");
    }
    if (new_file->digits_per_file() != ref_file.digits_per_file()){
        throw InconsistentMetaData("BasicYcdSetReader::load_file", path, "Digits per file does not match.");
    }
    if (new_file->digits_per_word() != ref_file.digits_per_word()){
        throw InconsistentMetaData("BasicYcdSetReader::load_file", path, "Digits per word does not match.");
    }
    if (m_stream_end != 0 &&
        ref_file.stream_end() != 0 &&
        m_stream_end != ref_file.stream_end()
    ){
        throw InconsistentMetaData("BasicYcdSetReader::load_file", path, "Total digits does not match.");
    }

    //  Set new total digits.
    if (m_stream_end == 0){
        m_stream_end = new_file->stream_end();
    }

    //  Update maximum id length.
    m_max_id_length = std::max(m_max_id_length, query_id_length);

    if (new_file->file_id() != id){
        throw InconsistentMetaData("BasicYcdSetReader::load_file", path, "File id does not match.");
    }

//    new_file->print();
    auto ret = m_files.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(id),
        std::forward_as_tuple(std::move(new_file))
    );
    if (!ret.second){
        throw InconsistentMetaData("BasicYcdSetReader::load_file", path, "File id already exists.");
    }
    return *ret.first->second;
}
BasicYcdFileReader& BasicYcdSetReader::get_file(uiL_t id){
    //  Open the specified file id.
    //  This funtion will search all the search paths and will try a variety of
    //  different zero-padded number schemes. If this function successfully
    //  opens a file with an id that is longer than the current "max_id_length",
    //  it will update that value to reflect that new longest id.

    std::lock_guard<std::mutex> lg(m_files_lock);

    //  File already opened.
    auto iter = m_files.find(id);
    if (iter != m_files.end()){
        return *iter->second;
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
            if (FileIO::FileExists(full_path)){
                return load_file(full_path, id);
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
    throw FileIO::FileException("BasicYcdSetReader::load_file()", m_name, error);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool BasicYcdSetReader::range_is_available(uiL_t offset, uiL_t digits){
    //  Find file boundaries
    uiL_t start = offset;
    uiL_t end = offset + digits;
    if (m_stream_end != 0 && m_stream_end < end){
        return false;
    }

    uiL_t file_start = start / m_digits_per_file;
    uiL_t file_end   = (end + m_digits_per_file - 1) / m_digits_per_file;

    bool bad = false;

    for (uiL_t file = file_start; file < file_end; file++){
        try{
            get_file(file);
        }catch (InconsistentMetaData&){
            std::string path = m_name + std::to_string(file) + ".ycd";
            Console::Warning("Inconsistent Metadata: \"" + path + "\"");
            bad = true;
        }catch (FileIO::FileException& e){
            if (!bad){
                Console::Warning("The following needed files are missing or inaccessible:");
            }
            std::string path = m_name + std::to_string(file) + ".ycd";
            Console::println(path);
            e.print();
            bad = true;
        }
    }

    return !bad;
}
upL_t BasicYcdSetReader::recommend_buffer_size(uiL_t digits, upL_t limit) const{
    return m_files.begin()->second->recommend_buffer_size(digits, limit);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class BasicYcdSetReader::Command{
public:
    Command(
        BasicYcdFileReader& file,
        uiL_t offset, uiL_t digits,
        char* output, bool stats,
        uiL_t scale_hash
    )
        : m_file(file)
        , m_offset(offset), m_digits(digits)
        , m_output(output)
        , m_stats(stats ? new DigitStats(file.radix()) : nullptr)
        , m_scale_hash(scale_hash)
    {}

    const DigitStats& stats() const{
        return *m_stats;
    }

    void run(
        const AlignedBufferC<BUFFER_ALIGNMENT>& buffer,
        BasicParallelizer& parallelizer, upL_t tds
    ){
        if (m_output == nullptr){
            m_file.load_stats(*m_stats, m_offset, m_digits, buffer, parallelizer, tds);
        }else{
            m_file.load_digits(m_output, m_stats.get(), m_offset, (upL_t)m_digits, buffer, parallelizer, tds);
        }
        if (m_stats){
            m_stats->scale_up_hash(m_scale_hash);
        }
    }

    void print() const{
        Console::println_labelm_commas("    file = ", m_file.file_id());
        Console::println_labelm_commas("    offset = ", m_offset);
        Console::println_labelm_commas("    digits = ", m_digits);
        Console::println_labelm_commas("    output = ", (upL_t)m_output);
        Console::println_labelm_commas("    scale  = ", m_scale_hash);
        Console::println();
    }

private:
    BasicYcdFileReader& m_file;
    uiL_t m_offset;
    uiL_t m_digits;
    char* m_output;
    std::unique_ptr<DigitStats> m_stats;
    uiL_t m_scale_hash;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
std::vector<BasicYcdSetReader::Command> BasicYcdSetReader::make_commands(
    char* output, bool stats,
    uiL_t offset, uiL_t digits
){
    //  Find file boundaries
    uiL_t start = offset;
    uiL_t end = offset + digits;
    uiL_t file_start = start / m_digits_per_file;
    uiL_t file_end   = (end + m_digits_per_file - 1) / m_digits_per_file;

    bool bad = false;

    std::vector<BasicYcdSetReader::Command> commands;
    commands.reserve((upL_t)(file_end - file_start));

    for (uiL_t file = file_start; file < file_end; file++){
        try{
            uiL_t s = file * m_digits_per_file;
            uiL_t e = s + m_digits_per_file;
            s = std::max(s, start);
            e = std::min(e, end);
            commands.emplace_back(
                get_file(file),
                s, e - s,
                output == nullptr ? nullptr : output + s - start,
                stats,
                end - e
            );
        }catch (InconsistentMetaData&){
            throw;
        }catch (FileIO::FileException& e){
            if (m_stream_end != 0 && m_stream_end < end){
                throw InvalidParametersException("This digit stream does not have enough digits.");
            }
            if (!bad){
                Console::Warning("The following needed files are missing or inaccessible:");
            }
            std::string path = m_name + std::to_string(file) + ".ycd";
            Console::println(path);
            e.print();
            bad = true;
        }
    }

    if (bad){
        throw InvalidParametersException("BasicYcdSetReader::make_commands()", "Missing required files.");
    }

    return commands;
}
void BasicYcdSetReader::load_stats(
    DigitStats& stats,
    uiL_t offset, uiL_t digits,
    const AlignedBufferC<BUFFER_ALIGNMENT>& buffer,
    BasicParallelizer& parallelizer, upL_t tds
){
    if (digits == 0){
        return;
    }

    uiL_t end = offset + digits;
    if (m_stream_end != 0 && end > m_stream_end){
        throw FileIO::FileException("BasicYcdSetReader::load_stats()", m_name, "Out of range.");
    }

    std::vector<Command> commands = make_commands(nullptr, true, offset, digits);

    //  TODO: Parallelize this for files that are on different physical storage devices.
    for (Command& command : commands){
//        command.print();
        command.run(buffer, parallelizer, tds);
    }

    //  Aggregate stats
    stats.scale_up_hash(digits);
    for (Command& command : commands){
        stats += command.stats();
    }
}
void BasicYcdSetReader::load_digits(
    char* output,
    DigitStats* stats,
    uiL_t offset, upL_t digits,
    const AlignedBufferC<BUFFER_ALIGNMENT>& buffer,
    BasicParallelizer& parallelizer, upL_t tds
){
    if (digits == 0){
        return;
    }

    uiL_t end = offset + digits;
    if (m_stream_end != 0 && end > m_stream_end){
        throw FileIO::FileException("BasicYcdSetReader::load_stats()", m_name, "Out of range.");
    }

    std::vector<Command> commands = make_commands(output, stats != nullptr, offset, digits);

    //  TODO: Parallelize this for files that are on different physical storage devices.
    for (Command& command : commands){
//        command.print();
        command.run(buffer, parallelizer, tds);
    }

    //  Combine the hashes.
    if (stats != nullptr){
        stats->scale_up_hash(digits);
        for (Command& command : commands){
            *stats += command.stats();
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
