/* BufferredStreamFile.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 03/19/2018
 * Last Modified    : 07/03/2019
 * 
 *      RawFile is a file with direct and unbufferred I/O.
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string.h>
#include "FileException.h"
#include "BufferredStreamFile.h"
namespace ymp{
namespace FileIO{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
BufferedReader::BufferedReader(RawFile& file)
    : m_block((upL_t)1 << file.alignment_k())
    , m_buffer(m_block, m_block)
    , m_buffer_offset(m_block)
    , m_buffer_end(m_block)
    , m_file_offset(0)
    , m_file(file)
{}
const std::string& BufferedReader::path() const{
    return m_file.path();
}
ufL_t BufferedReader::offset() const{
    return m_file_offset - m_block + m_buffer_offset;
}
void BufferedReader::refill(){
    if (m_buffer_end < m_block){
        throw FileIO::FileException("BufferedReader()", m_file.path(), "Unexpected end of file.");
    }
    m_buffer_end = m_file.load(m_buffer, m_file_offset, m_block, false);
    if (m_buffer_end == 0){
        throw FileIO::FileException("BufferedReader()", m_file.path(), "Unexpected end of file.");
    }
    m_buffer_offset = 0;
    m_file_offset += m_block;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
BufferedWriter::BufferedWriter(RawFile& file)
    : m_block((upL_t)1 << file.alignment_k())
    , m_buffer(m_block, m_block)
    , m_buffer_offset(0)
    , m_file_offset(0)
    , m_file(file)
{
    memset(m_buffer, 0, m_block);
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
    m_file.store(m_buffer, m_file_offset, m_block, true);
}
void BufferedWriter::refill(){
    flush();
    m_buffer_offset = 0;
    m_file_offset += m_block;
    memset(m_buffer, 0, m_block);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
