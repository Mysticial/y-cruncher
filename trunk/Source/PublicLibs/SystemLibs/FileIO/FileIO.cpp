/* FileIO.cpp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 01/24/2015
 *  Last Modified   : 12/30/2015
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/BasicLibs/StringTools/Unicode.h"
#include "FileIO.h"
#ifdef _WIN32
#include "FileIO_WinAPI.ipp"
#else
#include "FileIO_Default.ipp"
#endif
namespace ymp{
namespace FileIO{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
