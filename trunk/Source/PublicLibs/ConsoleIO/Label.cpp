/* Label.cpp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 08/31/2014
 *  Last Modified   : 09/19/2014
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/BasicLibs/StringTools/ToString.h"
#include "Margin.h"
#include "Label.h"
namespace ymp{
namespace Console{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  String Labels
YM_NO_INLINE std::string print_labelc(std::string label, const std::string& x, char color){
    Console::ConsoleLockScope lock;

    std::string ret;

    if (color != ' ') set_color('w');
    label += ": ";
    ret += print(label);

    if (color != ' ') set_color(color);
    ret += print(x);
    if (color != ' ') set_color('w');

    return ret;
}
YM_NO_INLINE std::string println_labelc(std::string label, const std::string& x, char color){
    Console::ConsoleLockScope lock;
    std::string ret;
    ret += print_labelc(label, x, color);
    ret += println();
    return ret;
}
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE std::string print_labelm(std::string label, const std::string& x, char color){
    return print_labelm(DEFAULT_MARGIN, std::move(label), x, color);
}
YM_NO_INLINE std::string println_labelm(std::string label, const std::string& x, char color){
    return println_labelm(DEFAULT_MARGIN, std::move(label), x, color);
}
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE std::string print_labelm(upL_t margin, std::string label, const std::string& x, char color){
    Console::ConsoleLockScope lock;

    std::string ret;

    if (color != ' ') set_color('w');
    label.resize(margin, ' ');
    label.back() = ' ';
    ret += print(label);

    if (color != ' ') set_color(color);
    ret += print(x);
    if (color != ' ') set_color('w');

    return ret;
}
YM_NO_INLINE std::string println_labelm(upL_t margin, std::string label, const std::string& x, char color){
    Console::ConsoleLockScope lock;
    std::string ret;
    ret += print_labelm(margin, label, x, color);
    ret += println();
    return ret;
}
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE std::string scan_labelc_utf8(std::string label, char color){
    label += ": ";
    print(label, color != ' ' ? 'w' : ' ');
    return scan_utf8(color);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Integer Labels
YM_NO_INLINE siL_t scan_label_siL(const std::string& label, char color){
    Console::ConsoleLockScope lock;
    print(label, color != ' ' ? 'w' : ' ');
    siL_t out = scan_int<siL_t>(color);
    if (color != ' '){
        set_color('w');
    }
    return out;
}
YM_NO_INLINE siL_t scan_label_siL_range(const std::string& label, siL_t low, siL_t high, char color){
    Console::ConsoleLockScope lock;
    siL_t out;
    do{
        out = scan_label_siL(label, color);
    }while (low > out || out > high);
    if (color != ' '){
        set_color('w');
    }
    return out;
}
YM_NO_INLINE siL_t scan_label_siL_suffix(const std::string& label, char color){
    Console::ConsoleLockScope lock;
    print(label, color != ' ' ? 'w' : ' ');
    siL_t out = scan_int_suffix<siL_t>(color);
    if (color != ' '){
        set_color('w');
    }
    return out;
}
YM_NO_INLINE upL_t scan_label_upL_range(const std::string& label, upL_t low, upL_t high, char color){
    return (upL_t)scan_label_uiL_range(label, low, high, color);
}
YM_NO_INLINE upL_t scan_label_upL_suffix_range(const std::string& label, upL_t low, upL_t high, char color){
    return (upL_t)scan_label_uiL_suffix_range(label, low, high, color);
}
YM_NO_INLINE uiL_t scan_label_uiL_range(const std::string& label, uiL_t low, uiL_t high, char color){
    Console::ConsoleLockScope lock;
    uiL_t out;
    do{
        out = scan_label_siL(label, color);
    }while (low > out || out > high);
    if (color != ' '){
        set_color('w');
    }
    return out;
}
YM_NO_INLINE uiL_t scan_label_uiL_suffix_range(const std::string& label, uiL_t low, uiL_t high, char color){
    Console::ConsoleLockScope lock;
    uiL_t out;
    do{
        out = scan_label_siL_suffix(label, color);
    }while (low > out || out > high);
    if (color != ' '){
        set_color('w');
    }
    return out;
}
YM_NO_INLINE uiL_t scan_label_bytes(const std::string& label, char color){
    Console::ConsoleLockScope lock;
    print(label, color != ' ' ? 'w' : ' ');
    return scan_bytes<uiL_t>(color);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Integer Labels with Margin and Units
YM_NO_INLINE std::string print_labelm_commas(std::string label, siL_t x, const std::string& units, char color){
    return print_labelm_commas(DEFAULT_MARGIN, std::move(label), x, units, color);
}
YM_NO_INLINE std::string println_labelm_commas(std::string label, siL_t x, const std::string& units, char color){
    return println_labelm_commas(DEFAULT_MARGIN, std::move(label), x, units, color);
}
YM_NO_INLINE std::string print_labelm_commas(upL_t margin, std::string label, siL_t x, const std::string& units, char color){
    return print_labelm(
        margin,
        std::move(label),
        StringTools::tostr(x, StringTools::COMMAS) + " " + units,
        color
    );
}
YM_NO_INLINE std::string println_labelm_commas(upL_t margin, std::string label, siL_t x, const std::string& units, char color){
    return println_labelm(
        margin,
        std::move(label),
        StringTools::tostr(x, StringTools::COMMAS) + " " + units,
        color
    );
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Float Labels with Margin
YM_NO_INLINE std::string print_labelm_float(std::string label, double x, int precision, char color){
    return print_labelm_float(DEFAULT_MARGIN, label, x, precision, color);
}
YM_NO_INLINE std::string println_labelm_float(std::string label, double x, int precision, char color){
    return println_labelm_float(DEFAULT_MARGIN, label, x, precision, color);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE std::string print_labelm_float(upL_t margin, std::string label, double x, int precision, char color){
    return print_labelm(
        margin,
        std::move(label),
        StringTools::tostr_float(x, precision),
        color
    );
}
YM_NO_INLINE std::string println_labelm_float(upL_t margin, std::string label, double x, int precision, char color){
    return println_labelm(
        margin,
        std::move(label),
        StringTools::tostr_float(x, precision),
        color
    );
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Float with Units
YM_NO_INLINE std::string print_unitl_float(const std::string& units, double x, int precision, char color){
    return print_unitl_float(16, units, x, precision, color);
}
YM_NO_INLINE std::string println_unitl_float(const std::string& units, double x, int precision, char color){
    return println_unitl_float(16, units, x, precision, color);
}
YM_NO_INLINE std::string print_unitl_float(upL_t margin, const std::string& units, double x, int precision, char color){
    Console::ConsoleLockScope lock;
    std::string ret;
    ret += print_marginL_float(margin, x, precision, color);
    if (color != ' '){
        set_color('w');
    }
    ret += print(units);
    return ret;
}
YM_NO_INLINE std::string println_unitl_float(upL_t margin, const std::string& units, double x, int precision, char color){
    Console::ConsoleLockScope lock;
    std::string ret;
    ret += print_marginL_float(margin, x, precision, color);
    if (color != ' '){
        set_color('w');
    }
    ret += println(units);
    return ret;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
