/* Margin.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 09/02/2014
 * Last Modified    : 09/02/2014
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/BasicLibs/StringTools/ToString.h"
#include "Label.h"
namespace ymp{
namespace Console{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  String Margins
YM_NO_INLINE    upL_t print_marginl(upL_t margin, std::string x, char color){
    SetColor(color);

    upL_t size = x.size();
    if (size >= margin){
        return print(x);
    }

    x.resize(margin, ' ');
    return print(x);
}
YM_NO_INLINE    upL_t println_marginl(upL_t margin, std::string x, char color){
    upL_t ret = 0;
    ret += print_marginl(margin, std::move(x), color);
    ret += println();
    return ret;
}
YM_NO_INLINE    upL_t print_marginr(upL_t margin, const std::string& x, char color){
    SetColor(color);

    upL_t size = x.size();
    if (size >= margin){
        return print(x);
    }

    std::string out(margin - size, ' ');
    out += x;
    return print(out);
}
YM_NO_INLINE    upL_t println_marginr(upL_t margin, const std::string& x, char color){
    upL_t ret = 0;
    ret += print_marginr(margin, x, color);
    ret += println();
    return ret;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Integer Margins
YM_NO_INLINE    upL_t print_marginl(upL_t margin, siL_t x, char color){
    return print_marginl(margin, StringTools::tostr(x, StringTools::NORMAL), color);
}
YM_NO_INLINE    upL_t println_marginl(upL_t margin, siL_t x, char color){
    return println_marginl(margin, StringTools::tostr(x, StringTools::NORMAL), color);
}
YM_NO_INLINE    upL_t print_marginr(upL_t margin, siL_t x, char color){
    return print_marginr(margin, StringTools::tostr(x, StringTools::NORMAL), color);
}
YM_NO_INLINE    upL_t println_marginr(upL_t margin, siL_t x, char color){
    return println_marginr(margin, StringTools::tostr(x, StringTools::NORMAL), color);
}
YM_NO_INLINE    upL_t print_marginl_commas(upL_t margin, siL_t x, char color){
    return print_marginl(margin, StringTools::tostr(x, StringTools::COMMAS), color);
}
YM_NO_INLINE    upL_t println_marginl_commas(upL_t margin, siL_t x, char color){
    return println_marginl(margin, StringTools::tostr(x, StringTools::COMMAS), color);
}
YM_NO_INLINE    upL_t print_marginr_commas(upL_t margin, siL_t x, char color){
    return print_marginr(margin, StringTools::tostr(x, StringTools::COMMAS), color);
}
YM_NO_INLINE    upL_t println_marginr_commas(upL_t margin, siL_t x, char color){
    return println_marginr(margin, StringTools::tostr(x, StringTools::COMMAS), color);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Float Margins
YM_NO_INLINE    upL_t print_marginl_float(upL_t margin, double x, int precision, char color){
    return print_marginl(margin, StringTools::tostr_float(x, precision), color);
}
YM_NO_INLINE    upL_t println_marginl_float(upL_t margin, double x, int precision, char color){
    return println_marginl(margin, StringTools::tostr_float(x, precision), color);
}
YM_NO_INLINE    upL_t print_marginr_fixed(upL_t margin, double x, int precision, char color){
    return print_marginr(margin, StringTools::tostr_fixed(x, precision), color);
}
YM_NO_INLINE    upL_t println_marginr_fixed(upL_t margin, double x, int precision, char color){
    return println_marginr(margin, StringTools::tostr_fixed(x, precision), color);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
