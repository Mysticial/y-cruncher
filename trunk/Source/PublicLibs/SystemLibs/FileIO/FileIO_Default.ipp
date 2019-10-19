/* FileIO_Default.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/05/2010
 * Last Modified    : 07/26/2011
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <vector>
#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#ifdef _MSC_VER
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif
#include "PublicLibs/BasicLibs/StringTools/ToString.h"
#include "PublicLibs/BasicLibs/StringTools/Unicode.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "FileException.h"
//#include "FileIO_Default.h"
#include "BasicFile.h"
namespace ymp{
namespace FileIO{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CompileOptions(){
    Console::println_labelm("File IO", "Default", 'R');
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
upL_t SetFileLengthFailures = 0;
int GetLastErrorCode(){
    return errno;
}
void PrintLastError(){
    Console::println_labelc("errno", errno);
}
////////////////////////////////////////////////////////////////////////////////
void MakeDirectory(const std::string& path){
#ifdef _WIN32
    _mkdir(path.c_str());
#else
    mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
}
void RenameFile(const std::string& oldname, const std::string& newname){
    while (true){
        if (!rename(oldname.c_str(), newname.c_str())){
            return;
        }

        remove(newname.c_str());

        if (!rename(oldname.c_str(), newname.c_str())){
            return;
        }

        Console::Warning("Unable to rename file.", true);
        Console::println(newname);
        Console::println();
        Console::println("Re-attempting...");
        Console::Pause('w');
    }
}
ufL_t GetFileSize(const std::string& path){
    BasicFile file(path);

    char buf;

    ufL_t L;
    ufL_t H = 1;

    //  Find upper bound for file size.
    while (1){
        if (!file.set_ptr(H) || file.read(&buf, 1) == 0){
            break;
        }
        H <<= 1;
    }
    L = H >> 1;

    //  Binary Search the file size
    while (H - L > 1){
        ufL_t m = (L + H) >> 1;
        if (!file.set_ptr(m) || file.read(&buf, 1) == 0){
            H = m;
        }else{
            L = m;
        }
    }

    if (L == 0){
        if (!file.set_ptr(0) || file.read(&buf, 1) == 0){
            return 0;
        }
    }

    return H;
}
bool FileExists(const std::string& path){
    FILE *file = fopen(path.c_str(), "rb");
    if (file != nullptr){
        fclose(file);
        return true;
    }
    int err = errno;
    if (err == ENOENT){
        return false;
    }
    throw FileException("FileExists()", path, "Unable to open file. errno = " + std::to_string(err));
}
void RemoveFile(const std::string& path){
    remove(path.c_str());
}
bool DirectoryIsWritable(const std::string& directory){
    //  Build the path
    std::string path;

    //  Push the base path
    if (!directory.empty()){
        path += directory;

        //  Ensure the path ends with a slash.
        char last = path.back();
        if (last != '/' && last != '\\'){
            path += '/';
        }
    }

    //  Push the name
    path += "pathcheck.sf";

    try{
        BasicFile file(0, path, false);
        file.close();
        RemoveFile(path);
    }catch (FileException&){
        return false;
    }

    return true;
}
std::vector<std::string> list_directory(const std::string& path){
    //  Get a list of files in the specified directory.
    std::vector<std::string> files;

    DIR *directory;
    struct dirent *entry;

    if ((directory = opendir(path.c_str())) != nullptr){
        while ((entry = readdir(directory)) != nullptr){
            files.push_back(entry->d_name);
        }
        closedir(directory);
    }

    return files;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
