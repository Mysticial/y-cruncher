/* BasicYcdSetWriter.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/06/2018
 * Last Modified    : 03/28/2018
 * 
 *      This write does not officially support multiple writes to the same
 *  digit. This is because individual .ycd files that have been completely
 *  written out are automatically closed and renamed.
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_BasicYcdSetWriter_H
#define ydv_DigitViewer_BasicYcdSetWriter_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <vector>
#include <map>
#include <mutex>
#include "BasicDigitWriter.h"
#include "BasicYcdFileWriter.h"
namespace DigitViewer2{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class BasicYcdSetWriter : public BasicDigitWriter{
public:
    BasicYcdSetWriter(
        const std::string& path,
        const std::string& name,
        const std::string& first_digits,
        char radix,
        ufL_t digits_per_file, uiL_t stream_end
    );

    virtual upL_t recommend_buffer_size(uiL_t digits, upL_t limit) const override;
    virtual std::unique_ptr<BasicDigitReader> close_and_get_basic_reader() override;

    virtual void store_digits(
        const char* input,
        uiL_t offset, upL_t digits,
        const AlignedBufferC<BUFFER_ALIGNMENT>& buffer,
        BasicParallelizer& parallelizer, upL_t tds
    ) override;


public:
    BasicYcdFileWriter& get_file(uiL_t id);

    class Command;

    std::vector<Command> make_commands(const char* input, uiL_t offset, upL_t digits);


private:
    std::string m_path;
    std::string m_name;

    std::string m_first_digits;

    upL_t m_digits_per_word;
    ufL_t m_digits_per_file;
    uiL_t m_stream_end;

    std::mutex m_files_lock;
    std::map<uiL_t, BasicYcdFileWriter> m_files;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
