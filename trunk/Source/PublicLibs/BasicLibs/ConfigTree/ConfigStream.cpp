/* ConfigStream.cpp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 02/28/2018
 *  Last Modified   : 02/28/2018
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <vector>
#include "PublicLibs/Exceptions/ParseException.h"
#include "ConfigParser.h"
#include "ConfigStream.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
ConfigStream::ConfigStream(bool json)
    : m_json(json)
{}
void ConfigStream::push(const char* buffer, upL_t bytes){
    //  This function does not provide strong exception guarantee.
    //  If parsing fails, the entire unprocessed stream will be corrupted.

    m_buffer.append(buffer, bytes);
    upL_t bytes_left = m_buffer.size();

    const char* stream = m_buffer.c_str();
    while (true){
        if (bytes_left < sizeof(u32_t)){
            break;
        }

        u32_t size;
        memcpy(&size, stream, sizeof(u32_t));
        if (bytes_left < size){
            break;
        }

        const char* tmp = stream + sizeof(u32_t);
//        cout << tmp << endl;
        ConfigValue value = m_json
            ? parse_json_string(tmp)
            : parse_acfg_string(tmp);
        m_parsed.emplace_back(std::move(value));
        stream += size;
        bytes_left -= size;
    }

    //  Commit
    if (bytes_left != m_buffer.size()){
        m_buffer = std::string(stream, bytes_left);
    }
}
ConfigValue ConfigStream::get(){
    if (m_parsed.empty()){
        return ConfigValue();
    }
    ConfigValue ret = std::move(m_parsed[0]);
    m_parsed.pop_front();
    return ret;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
