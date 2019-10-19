/* BasicYcdSetWriter.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/06/2018
 * Last Modified    : 03/28/2018
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
#include "PublicLibs/BasicLibs/Alignment/DefaultAlignment.h"
#include "PublicLibs/SystemLibs/FileIO/FileIO.h"
#include "DigitViewer2/DigitReaders/InconsistentMetadataException.h"
#include "DigitViewer2/DigitReaders/BasicYcdSetReader.h"
#include "BasicYcdFileWriter.h"
#include "BasicYcdSetWriter.h"
namespace DigitViewer2{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
BasicYcdSetWriter::BasicYcdSetWriter(
    const std::string& path,
    const std::string& name,
    const std::string& first_digits,
    char radix,
    ufL_t digits_per_file, uiL_t stream_end
)
    : m_path(path), m_name(name)
    , m_first_digits(first_digits)
    , m_digits_per_file(digits_per_file)
    , m_stream_end(stream_end)
{
    m_radix = radix;
    switch (m_radix){
    case 10:
        m_digits_per_word = 19;
        break;
    case 16:
        m_digits_per_word = 16;
        break;
    default:
        throw InvalidParametersException("BasicYcdSetWriter::BasicYcdSetWriter()");
    }

    //  Make sure path ends in a slash.
    if (m_path.size() != 0){
        char back = m_path.back();
        if (back != '/' && back != '\\'){
            m_path += '/';
        }
    }

    //  Multiple files, create a folder for them.
    if (digits_per_file != (uiL_t)0 - 1){
        m_path += m_name;
        m_path += '/';
        FileIO::MakeDirectory(m_path.c_str());
    }else{
        if (stream_end == 0){
            throw InvalidParametersException(
                "BasicYcdSetWriter::BasicYcdSetWriter()",
                "Single-file is incompatible with unbounded output."
            );
        }
        if ((ufL_t)stream_end != stream_end){
            throw InvalidParametersException(
                "BasicYcdSetWriter::BasicYcdSetWriter()",
                "Too many digits for single-file compress."
            );
        }
        m_digits_per_file = (ufL_t)stream_end;
    }

}
BasicYcdFileWriter& BasicYcdSetWriter::get_file(uiL_t id){
    std::lock_guard<std::mutex> lg(m_files_lock);

    //  File already opened.
    auto iter = m_files.find(id);
    if (iter != m_files.end()){
        return iter->second;
    }

    uiL_t stream_end = m_stream_end;
    if (stream_end != 0){
        uiL_t last_file = (stream_end - 1) / m_digits_per_file;
//        cout << "stream_end       = " << stream_end << endl;
//        cout << "m_digits_per_file  = " << m_digits_per_file << endl;
//        cout << "last_file          = " << last_file << endl;
//        cout << "id = " << id << endl;
        if (!(id == last_file && (id + 1) * m_digits_per_file != stream_end)){
            stream_end = 0;
        }
//        cout << "stream_end (fixed) = " << stream_end << endl;
    }

    auto ret = m_files.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(id),
        std::forward_as_tuple(
            m_path + m_name + " - " + std::to_string(id),
            m_first_digits, m_radix,
            m_digits_per_file, stream_end,
            id
        )
    );
    return ret.first->second;
}
upL_t BasicYcdSetWriter::recommend_buffer_size(uiL_t digits, upL_t limit) const{
    if (digits == 0){
        return 0;
    }
    uiL_t words = (digits - 1) / m_digits_per_word + 1;
    upL_t bytes = (upL_t)std::min((words + 2) * sizeof(u64_t), (uiL_t)limit);
    bytes = std::max(bytes, DEFAULT_ALIGNMENT + 2 * sizeof(u64_t));
    return bytes;
}
std::unique_ptr<BasicDigitReader> BasicYcdSetWriter::close_and_get_basic_reader(){
    if (m_files.empty()){
        throw InvalidParametersException("BasicYcdSetWriter::close_and_get_basic_reader()", "No files open.");
    }
    std::string path = m_files.begin()->second.path();
    m_files.clear();
    return std::make_unique<BasicYcdSetReader>(path);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class BasicYcdSetWriter::Command{
public:
    Command(
        BasicYcdFileWriter& file,
        uiL_t offset, upL_t digits,
        const char* input
    )
        : m_file(file)
        , m_offset(offset), m_digits(digits)
        , m_input(input)
    {}

    void run(
        const AlignedBufferC<BUFFER_ALIGNMENT>& buffer,
        BasicParallelizer& parallelizer, upL_t tds
    ){
        m_file.store_digits(m_input, m_offset, m_digits, buffer, parallelizer, tds);
    }

    void print() const{
        Console::println_labelm_commas("    file = ", m_file.file_id());
        Console::println_labelm_commas("    offset = ", m_offset);
        Console::println_labelm_commas("    digits = ", m_digits);
        Console::println_labelm_commas("    input  = ", (upL_t)m_input);
        Console::println();
    }

private:
    BasicYcdFileWriter& m_file;
    uiL_t m_offset;
    upL_t m_digits;
    const char* m_input;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
std::vector<BasicYcdSetWriter::Command> BasicYcdSetWriter::make_commands(
    const char* input, uiL_t offset, upL_t digits
){
    //  Find file boundaries
    uiL_t start = offset;
    uiL_t end = offset + digits;
    uiL_t file_start = start / m_digits_per_file;
    uiL_t file_end   = (end + m_digits_per_file - 1) / m_digits_per_file;

    if (file_end <= file_start){
        throw InvalidParametersException("BasicYcdSetWriter::make_commands()", "Internal Overflow");
    }

    std::vector<BasicYcdSetWriter::Command> commands;
    commands.reserve((upL_t)(file_end - file_start));

    for (uiL_t file = file_start; file < file_end; file++){
        uiL_t s = file * m_digits_per_file;
        uiL_t e = s + m_digits_per_file;
        s = std::max(s, start);
        e = std::min(e, end);
        commands.emplace_back(
            get_file(file),
            s, (upL_t)(e - s),
            input + s - start
        );
    }

    return commands;
}
void BasicYcdSetWriter::store_digits(
    const char* input,
    uiL_t offset, upL_t digits,
    const AlignedBufferC<BUFFER_ALIGNMENT>& buffer,
    BasicParallelizer& parallelizer, upL_t tds
){
    if (digits == 0){
        return;
    }

    uiL_t end = offset + digits;
    if (m_stream_end != 0 && end > m_stream_end){
        throw FileIO::FileException("BasicYcdSetWriter::store_digits()", m_name, "Out of range.");
    }

    std::vector<Command> commands = make_commands(input, offset, digits);

    //  TODO: Parallelize this for files that are on different physical storage devices.
    for (Command& command : commands){
//        command.print();
        command.run(buffer, parallelizer, tds);
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
