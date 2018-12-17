/* ParsingTools.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/30/2018
 * Last Modified    : 01/30/2018
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/SystemLibs/FileIO/FileException.h"
#include "ParsingTools.h"
namespace DigitViewer2{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
std::string grab_until_delim(FileIO::BufferedReader& file, char delim){
    //  Streams characters from "file" into a string until a deliminator is found.

    std::string out;

    char ch;
    do{
        ch = file.next();
        if (ch == '\r'){
            continue;
        }
        if (ch == delim){
            return out;
        }
        out += ch;
    }while (1);
}
std::string grab_until_delim(FileIO::BasicFile& file, char delim){
    //  Streams characters from "file" into a string until a deliminator is found.

    std::string out;

    char ch;
    do{
        if (file.read(&ch, 1) == 0){
            throw FileIO::FileException("grab_until_delim()", file.GetPath(), "Unexpected End of File");
        }
        if (ch == '\r'){
            continue;
        }
        if (ch == delim){
            return out;
        }
        out += ch;
    }while (1);
}
const char* grab_until_delim(std::string& token, const char* str, char delim){
    //  Streams characters from "str" into the builder until a deliminator is found.

    char ch;
    do{
        ch = *str++;
        if (ch == '\r'){
            continue;
        }
        if (ch == delim || ch == '\0'){
            return str;
        }
        token += ch;
    }while (1);
}
uiL_t parse_uL(const char* str){
    uiL_t x = 0;
    while (*str != '\0'){
        x = 10*x + (upL_t)(*str++ - '0');
    }
    return x;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
