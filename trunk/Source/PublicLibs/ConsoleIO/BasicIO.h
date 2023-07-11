/* BasicIO.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 08/31/2014
 *  Last Modified   : 08/31/2014
 * 
 */

#pragma once
#ifndef ymp_Console_BasicIO_H
#define ymp_Console_BasicIO_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "ColorStrings.h"
namespace ymp{
namespace Console{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const int DEFAULT_PRECISION = 6;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void compile_options();
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Core
uiL_t sequence_number();
YM_NO_INLINE    upL_t   print   (const std::string& str, char color = ' ');
YM_NO_INLINE    upL_t   print   (const std::wstring& str, char color = ' ');
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE    std::string     scan_utf8   (char color = 'T');
YM_NO_INLINE    std::wstring    scan_wstr   (char color = 'T');
YM_NO_INLINE    void            pause       (char color = ' ');
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE    void    set_color               (char color);
YM_NO_INLINE    void    set_color_default       ();
YM_NO_INLINE    bool    set_console_window_size (int width = 80, int height = 25);
extern bool enable_colors;
////////////////////////////////////////////////////////////////////////////////
//  Basic Derived
YM_NO_INLINE    upL_t   println     ();
YM_NO_INLINE    upL_t   println     (std::string str, char color = ' ');
YM_NO_INLINE    upL_t   println     (std::wstring str, char color = ' ');
YM_NO_INLINE    void    clear_line  (int characters = 79);
YM_NO_INLINE    void    warning     (std::string str, bool sticky = false);
////////////////////////////////////////////////////////////////////////////////
extern bool pause_on_error; //  Temporary hack. TODO: Get rid of this entirely.
[[noreturn]] YM_NO_INLINE void quit_program (int code);
////////////////////////////////////////////////////////////////////////////////
class ConsoleLockScope{
public:
    ConsoleLockScope();
    ~ConsoleLockScope();

    ConsoleLockScope(const ConsoleLockScope&) = delete;
    void operator=(const ConsoleLockScope&) = delete;
};
////////////////////////////////////////////////////////////////////////////////
//  Chains
YM_NO_INLINE    upL_t   println         (const ColoredLine& str);
////////////////////////////////////////////////////////////////////////////////
//  Integer
YM_NO_INLINE    upL_t   print           (uiL_t x, char color = ' ');
YM_NO_INLINE    upL_t   print           (siL_t x, char color = ' ');
static          upL_t   print           (u32_t x, char color = ' '){ return print((uiL_t)x, color); }
static          upL_t   print           (s32_t x, char color = ' '){ return print((siL_t)x, color); }
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE    upL_t   println         (uiL_t x, char color = ' ');
YM_NO_INLINE    upL_t   println         (siL_t x, char color = ' ');
static          upL_t   println         (u32_t x, char color = ' '){ return println((uiL_t)x, color); }
static          upL_t   println         (s32_t x, char color = ' '){ return println((siL_t)x, color); }
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE    upL_t   print_commas    (uiL_t x, char color = ' ');
YM_NO_INLINE    upL_t   print_commas    (siL_t x, char color = ' ');
static          upL_t   print_commas    (u32_t x, char color = ' '){ return print_commas((uiL_t)x, color); }
static          upL_t   print_commas    (s32_t x, char color = ' '){ return print_commas((siL_t)x, color); }
YM_NO_INLINE    upL_t   println_commas  (uiL_t x, char color = ' ');
YM_NO_INLINE    upL_t   println_commas  (siL_t x, char color = ' ');
static          upL_t   println_commas  (u32_t x, char color = ' '){ return println_commas((uiL_t)x, color); }
static          upL_t   println_commas  (s32_t x, char color = ' '){ return println_commas((siL_t)x, color); }
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE    upL_t   print_bytes     (uiL_t x, char color = ' ');
YM_NO_INLINE    upL_t   println_bytes   (uiL_t x, char color = ' ');
YM_NO_INLINE    upL_t   print_ebytes    (uiL_t x, char color = ' ');
YM_NO_INLINE    upL_t   println_ebytes  (uiL_t x, char color = ' ');
YM_NO_INLINE    siL_t   scan_siL        (char color = 'T');
YM_NO_INLINE    siL_t   scan_siL_suffix (char color = 'T');
YM_NO_INLINE    uiL_t   scan_bytes      (char color = 'T');
////////////////////////////////////////////////////////////////////////////////
//  Floating Point
YM_NO_INLINE    upL_t   print_float     (double x, int precision = DEFAULT_PRECISION, char color = ' ');
YM_NO_INLINE    upL_t   println_float   (double x, int precision = DEFAULT_PRECISION, char color = ' ');
YM_NO_INLINE    upL_t   print_fixed     (double x, int precision = 3, char color = ' ');
YM_NO_INLINE    upL_t   println_fixed   (double x, int precision = 3, char color = ' ');
////////////////////////////////////////////////////////////////////////////////
//  Unit Tests
YM_NO_INLINE    void    print_test      (bool x);
YM_NO_INLINE    void    println_test    (bool x);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
