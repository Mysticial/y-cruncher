/* FileIO_WinAPI.ipp
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
#include <errno.h>
#include <windows.h>
#include <vector>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/BasicLibs/StringTools/Unicode.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "FileException.h"
//#include "FileIO_WinAPI.h"
namespace ymp{
namespace FileIO{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CompileOptions(){
    Console::println_labelm("File IO", "WinAPI", 'G');
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
upL_t SetFileLengthFailures = 0;
int GetLastErrorCode(){
    return GetLastError();
}
void PrintLastError(){
    DWORD errorcode = GetLastError();
    Console::println("", 'R');
    Console::println_labelc("Windows Error Code", errorcode);
    Console::print("    ");
    switch (errorcode){
        case ERROR_ACCESS_DENIED:
            Console::print("Access Denied");
            break;
        case ERROR_NOT_ENOUGH_MEMORY:
        case ERROR_OUTOFMEMORY:
            Console::print("Not Enough Memory");
            break;
        case ERROR_DISK_FULL:
        case ERROR_HANDLE_DISK_FULL:
            Console::print("Disk is Full");
            break;
        case ERROR_WRITE_FAULT:
            Console::print("Write Fault");
            break;
        case ERROR_READ_FAULT:
            Console::print("Read Fault");
            break;
        case ERROR_CRC:
            Console::print("Cyclic Redundancy Check - Possible Hardware Failure");
            break;
        default:
            Console::print("Unknown Error, See:\nhttp://msdn.microsoft.com/en-us/library/ms681381(VS.85).aspx");
    }
    Console::println("", 'w');
}
////////////////////////////////////////////////////////////////////////////////
void MakeDirectory(const std::string& path){
    _wmkdir(StringTools::utf8_to_wstr(path).c_str());
}
void RenameFile(const std::wstring& oldname, const std::wstring& newname){
    while (true){
        if (!_wrename(oldname.c_str(), newname.c_str())){
            return;
        }

        _wremove(newname.c_str());

        if (!_wrename(oldname.c_str(), newname.c_str())){
            return;
        }

        errno_t err;
        _get_errno(&err);
        Console::Warning("Unable to rename file.", true);
        Console::println_labelc("Error Code", err);
        Console::println(newname);
        Console::println();
        Console::println("Re-attempting...");
        Console::SetColor('w');
        Console::Pause('w');
    }
}
void RenameFile(const std::string& oldname, const std::string& newname){
    RenameFile(
        StringTools::utf8_to_wstr(oldname),
        StringTools::utf8_to_wstr(newname)
    );
}
ufL_t GetFileSize(const std::string& path){
    WIN32_FIND_DATAW filedata;
    HANDLE data = FindFirstFileW(StringTools::utf8_to_wstr(path).c_str(), &filedata);
    if (data == INVALID_HANDLE_VALUE){
        Console::Warning("Path does not exist.");
        Console::Quit(1);
    }

    ufL_t size = ((ufL_t)filedata.nFileSizeHigh << 32) + filedata.nFileSizeLow;
    
    if (!FindClose(data)){
        Console::Warning("Unable to close file.");
        Console::Quit(1);
    }

    return size;
}
bool FileExists(const std::string& path){
    FILE *file;
    if (_wfopen_s(&file, StringTools::utf8_to_wstr(path).c_str(), L"rb")){
        return false;
    }
    fclose(file);
    return true;
}
void RemoveFile(const std::string& path){
    _wremove(StringTools::utf8_to_wstr(path).c_str());
}
bool DirectoryIsWritable(const std::string& directory){
    //Returns:
    // - 0  if directory is writable
    // - 1  if directory is not writable

    //  Build the path
    std::wstring path;

    //  Push the base path
    if (!directory.empty()){
        path += StringTools::utf8_to_wstr(directory);

        //  Ensure the path ends with a slash.
        wchar_t last = path.back();
        if (last != L'/' && last != L'\\'){
            path += L'/';
        }
    }

    //  Push the name
    path += L"pathcheck.sf";

    HANDLE file = CreateFileW(
        path.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_ALWAYS,
        FILE_FLAG_WRITE_THROUGH,
//        NULL,
        NULL
    );

    if (file == INVALID_HANDLE_VALUE)
        return false;
    else{
        while (!CloseHandle(file)){
            Console::Warning("Unable to close file.");
            Console::Quit(1);
        }
    }

    while (_wremove(path.c_str())){
        Console::Warning("Unable to delete file.", true);
        Console::println();
        Console::println("Re-attempting...");
        Console::Pause('w');
        continue;
    }

    return true;
}
std::vector<std::string> list_directory(const std::string& path){
    //  Get a list of files in the specified directory.

    std::string ppath = path;
    if (!path.empty() && path.back() != '/' && path.back() != '\\'){
        ppath += "/";
    }
    ppath += "*";

    WIN32_FIND_DATAW FindFileData;
    HANDLE hFind;
    std::vector<std::string> files;

    //  Get first file
    hFind = FindFirstFileW(StringTools::utf8_to_wstr(ppath).c_str(), &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE){
        return files;
    }

    //  Get rest of files.
    do{
        files.push_back(StringTools::wstr_to_utf8(FindFileData.cFileName));
    }while (FindNextFileW(hFind, &FindFileData));

    FindClose(hFind);

    return files;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
