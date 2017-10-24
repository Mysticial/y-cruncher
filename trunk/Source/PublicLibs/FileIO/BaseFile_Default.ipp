/* BaseFile_Default.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 12/30/2015
 * Last Modified    : 12/30/2015
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/StringTools/Unicode.h"
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "BaseFile_Default.h"
namespace ymp{
namespace FileIO{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  BaseFile
bool BaseFile::open(std::string path){
    close();

    m_filehandle = fopen(path.c_str(), "rb");
    if (m_filehandle == nullptr)
        return false;

    m_path = std::move(path);
    return true;
}
bool BaseFile::create(std::string path, ufL_t bytes){
    close();

    m_filehandle = fopen(path.c_str(), "wb+");
    if (m_filehandle == nullptr)
        return false;

    if (bytes != 0){
        //  TODO
    }

    m_path = std::move(path);
    return true;
}
void BaseFile::close(bool delete_file){
    if (m_path.empty())
        return;

    fclose(m_filehandle);
    m_filehandle = nullptr;

    if (delete_file){
        remove(m_path.c_str());
    }

    m_path.clear();
}
bool BaseFile::set_ptr(ufL_t offset){
#ifdef _MSC_VER
    return _fseeki64(m_filehandle, offset, SEEK_SET) == 0;
#else
    return fseeko(m_filehandle, offset, SEEK_SET) == 0;
#endif
}
void BaseFile::flush(){
    fflush(m_filehandle);
}
upL_t BaseFile::read(void* T, upL_t bytes){
    return fread(T, 1, (size_t)bytes, m_filehandle);
}
upL_t BaseFile::write(const void* T, upL_t bytes){
    return fwrite(T, 1, (size_t)bytes, m_filehandle);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
