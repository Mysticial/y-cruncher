/* Margin.cpp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 09/02/2014
 *  Last Modified   : 09/02/2014
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/BasicLibs/StringTools/ToString.h"
#include "Margin.h"
namespace ymp{
namespace Console{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  String Margins
YM_NO_INLINE std::string print_marginL(upL_t margin, std::string x, char color){
    ConsoleLockScope lock;
    set_color(color);

    upL_t size = x.size();
    if (size >= margin){
        return print(x);
    }

    x.resize(margin, ' ');
    return print(x);
}
YM_NO_INLINE std::string println_marginL(upL_t margin, std::string x, char color){
    ConsoleLockScope lock;
    std::string ret;
    ret += print_marginL(margin, std::move(x), color);
    ret += println();
    return ret;
}
YM_NO_INLINE std::string print_marginR(upL_t margin, const std::string& x, char color){
    ConsoleLockScope lock;
    set_color(color);

    upL_t size = x.size();
    if (size >= margin){
        return print(x);
    }

    std::string out(margin - size, ' ');
    out += x;
    return print(out);
}
YM_NO_INLINE std::string println_marginR(upL_t margin, const std::string& x, char color){
    ConsoleLockScope lock;
    std::string ret;
    ret += print_marginR(margin, x, color);
    ret += println();
    return ret;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Float Margins
YM_NO_INLINE std::string print_marginL_float(upL_t margin, double x, int precision, char color){
    return print_marginL(margin, StringTools::tostr_float(x, precision), color);
}
YM_NO_INLINE std::string println_marginL_float(upL_t margin, double x, int precision, char color){
    return println_marginL(margin, StringTools::tostr_float(x, precision), color);
}
YM_NO_INLINE std::string print_marginR_fixed(upL_t margin, double x, int precision, char color){
    return print_marginR(margin, StringTools::tostr_fixed(x, precision), color);
}
YM_NO_INLINE std::string println_marginR_fixed(upL_t margin, double x, int precision, char color){
    return println_marginR(margin, StringTools::tostr_fixed(x, precision), color);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
