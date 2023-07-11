/* Serialization.h - Serialization
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 10/28/2012
 *  Last Modified   : 09/26/2014
 * 
 *      Mostly used for checkpoint restart.
 * 
 */

#pragma once
#ifndef ymp_Serialization_H
#define ymp_Serialization_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace ymp{
namespace Serialization{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE    void            write_line  (std::string& stream);
YM_NO_INLINE    void            parse_line  (const char*& stream);
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE    void            write_siL   (std::string& stream, const std::string& label, siL_t x);
YM_NO_INLINE    siL_t           parse_siL   (const char*& stream);
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE    void            write_float (std::string& stream, const std::string& label, double x);
YM_NO_INLINE    double          parse_float (const char*& stream);
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE    void            write_str   (std::string& stream, const std::string& label, const std::string& x);
YM_NO_INLINE    std::string     parse_str   (const char*& stream);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
