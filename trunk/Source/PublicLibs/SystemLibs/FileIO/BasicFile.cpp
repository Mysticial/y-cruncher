/* BasicFile.cpp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 12/30/2015
 *  Last Modified   : 12/30/2015
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "FileIO.h"
#include "FileException.h"
#if _WIN32
#include "BaseFile/BaseFile_Windows.ipp"
#else
#include "BaseFile/BaseFile_Default.ipp"
#endif
#include "BasicFile.h"
namespace ymp{
namespace FileIO{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Constructors
BasicFile::BasicFile(const std::string& path, bool retry){
    do{
        if (open(path)){
            return;
        }

        if (!retry){
            FileException(GetLastErrorCode(), nullptr, path, "Unable to open file.").fire();
        }

        Console::warning("Unable to open file.", true);
        Console::println(path);
        PrintLastError();
        Console::println();
        Console::println("Re-attempting...");
        Console::pause('w');
    }while (true);
}
BasicFile::BasicFile(ufL_t bytes, const std::string& path, bool retry){
    do{
        if (create(path, bytes)){
            return;
        }

        if (!retry){
            FileException(GetLastErrorCode(), nullptr, path, "Unable to create file.").fire();
        }

        Console::warning("Unable to create file.", true);
        Console::println(path);
        PrintLastError();
        Console::println();
        Console::println("Re-attempting...");
        Console::pause('w');
    }while (true);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
