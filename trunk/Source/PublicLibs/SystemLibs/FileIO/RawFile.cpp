/* RawFile.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 03/19/2018
 * Last Modified    : 03/19/2018
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string.h>
#include "PublicLibs/BasicLibs/Alignment/AlignmentTools.h"
#if _WIN32
#include "RawFile_Windows.ipp"
#else
#include "RawFile_Linux.ipp"
#endif
namespace ymp{
namespace FileIO{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
RawIoBuffer::RawIoBuffer(void* buffer, upL_t bytes)
    : m_buffer(buffer)
    , m_bytes(bytes)
{
    if (Alignment::ptr_past_aligned<RAWIO_ALIGNMENT>(buffer) != 0){
        throw InvalidParametersException("RawIoBuffer::RawIoBuffer()", "Buffer is misaligned.");
    }
    if (Alignment::int_past_aligned<RAWIO_ALIGNMENT>(bytes) != 0){
        throw InvalidParametersException("RawIoBuffer::RawIoBuffer()", "Length is misaligned.");
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
BufferedReader::BufferedReader(RawFile& file)
    : m_buffer(RAWIO_ALIGNMENT, RAWIO_ALIGNMENT)
    , m_buffer_offset(RAWIO_ALIGNMENT)
    , m_buffer_end(RAWIO_ALIGNMENT)
    , m_file_offset(0)
    , m_file(file)
{}
const std::string& BufferedReader::path() const{
    return m_file.path();
}
ufL_t BufferedReader::offset() const{
    return m_file_offset - RAWIO_ALIGNMENT + m_buffer_offset;
}
void BufferedReader::refill(){
    if (m_buffer_end < RAWIO_ALIGNMENT){
        throw FileIO::FileException("BufferedReader()", m_file.path(), "Unexpected end of file.");
    }
    m_buffer_end = m_file.load(m_buffer, m_file_offset, RAWIO_ALIGNMENT, false);
    if (m_buffer_end == 0){
        throw FileIO::FileException("BufferedReader()", m_file.path(), "Unexpected end of file.");
    }
    m_buffer_offset = 0;
    m_file_offset += RAWIO_ALIGNMENT;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
BufferedWriter::BufferedWriter(RawFile& file)
    : m_buffer(RAWIO_ALIGNMENT, RAWIO_ALIGNMENT)
    , m_buffer_offset(0)
    , m_file_offset(0)
    , m_file(file)
{
    memset(m_buffer, 0, RAWIO_ALIGNMENT);
}
BufferedWriter::~BufferedWriter(){
    if (m_buffer_offset != 0){
        flush();
    }
}
const std::string& BufferedWriter::path() const{
    return m_file.path();
}
ufL_t BufferedWriter::offset() const{
    return m_file_offset + m_buffer_offset;
}
void BufferedWriter::push(const char* buffer, upL_t bytes){
    for (upL_t c = 0; c < bytes; c++){
        push(buffer[c]);
    }
}
void BufferedWriter::flush(){
    m_file.store(m_buffer, m_file_offset, RAWIO_ALIGNMENT);
}
void BufferedWriter::refill(){
    flush();
    m_buffer_offset = 0;
    m_file_offset += RAWIO_ALIGNMENT;
    memset(m_buffer, 0, RAWIO_ALIGNMENT);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
