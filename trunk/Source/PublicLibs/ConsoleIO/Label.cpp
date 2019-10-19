/* Label.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/31/2014
 * Last Modified    : 09/19/2014
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
const upL_t DEFAULT_MARGIN = 20;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  String Labels
YM_NO_INLINE upL_t print_labelc(std::string label, const std::string& x, char color){
    upL_t ret = 0;

    if (color != ' ') SetColor('w');
    label += ": ";
    ret += print(label);

    if (color != ' ') SetColor(color);
    ret += print(x);
    if (color != ' ') SetColor('w');

    return ret;
}
YM_NO_INLINE upL_t println_labelc(std::string label, const std::string& x, char color){
    upL_t ret = 0;
    ret += print_labelc(label, x, color);
    ret += println();
    return ret;
}
YM_NO_INLINE upL_t print_labelc(std::string label, const std::wstring& x, char color){
    upL_t ret = 0;

    if (color != ' ') SetColor('w');
    label += ": ";
    ret += print(label);

    if (color != ' ') SetColor(color);
    ret += print(x);
    if (color != ' ') SetColor('w');

    return ret;
}
YM_NO_INLINE upL_t println_labelc(std::string label, const std::wstring& x, char color){
    upL_t ret = 0;
    ret += print_labelc(label, x, color);
    ret += println();
    return ret;
}
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE upL_t print_labelm(std::string label, const std::string& x, char color){
    return print_labelm(DEFAULT_MARGIN, std::move(label), x, color);
}
YM_NO_INLINE upL_t println_labelm(std::string label, const std::string& x, char color){
    return println_labelm(DEFAULT_MARGIN, std::move(label), x, color);
}
YM_NO_INLINE upL_t print_labelm(std::string label, const std::wstring& x, char color){
    return print_labelm(DEFAULT_MARGIN, std::move(label), x, color);
}
YM_NO_INLINE upL_t println_labelm(std::string label, const std::wstring& x, char color){
    return println_labelm(DEFAULT_MARGIN, std::move(label), x, color);
}
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE upL_t print_labelm(upL_t margin, std::string label, const std::string& x, char color){
    upL_t ret = 0;

    if (color != ' ') SetColor('w');
    label.resize(margin, ' ');
    label.back() = ' ';
    ret += print(label);

    if (color != ' ') SetColor(color);
    ret += print(x);
    if (color != ' ') SetColor('w');

    return ret;
}
YM_NO_INLINE upL_t println_labelm(upL_t margin, std::string label, const std::string& x, char color){
    upL_t ret = 0;
    ret += print_labelm(margin, label, x, color);
    ret += println();
    return ret;
}
YM_NO_INLINE upL_t print_labelm(upL_t margin, std::string label, const std::wstring& x, char color){
    upL_t ret = 0;

    if (color != ' ') SetColor('w');
    label.resize(margin, ' ');
    label.back() = ' ';
    ret += print(label);

    if (color != ' ') SetColor(color);
    ret += print(x);
    if (color != ' ') SetColor('w');

    return ret;
}
YM_NO_INLINE upL_t println_labelm(upL_t margin, std::string label, const std::wstring& x, char color){
    upL_t ret = 0;
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
YM_NO_INLINE std::wstring scan_labelc_wstr(std::string label, char color){
    label += ": ";
    print(label, color != ' ' ? 'w' : ' ');
    return scan_wstr(color);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Integer Labels
YM_NO_INLINE upL_t print_labelc(std::string label, siL_t x, char color){
    return print_labelc(
        std::move(label),
        StringTools::tostr(x, StringTools::NORMAL),
        color
    );
}
YM_NO_INLINE upL_t println_labelc(std::string label, siL_t x, char color){
    return println_labelc(
        std::move(label),
        StringTools::tostr(x, StringTools::NORMAL),
        color
    );
}
YM_NO_INLINE upL_t print_labelc_commas(std::string label, siL_t x, char color){
    return print_labelc(
        std::move(label),
        StringTools::tostr(x, StringTools::COMMAS),
        color
    );
}
YM_NO_INLINE upL_t println_labelc_commas(std::string label, siL_t x, char color){
    return println_labelc(
        std::move(label),
        StringTools::tostr(x, StringTools::COMMAS),
        color
    );
}
YM_NO_INLINE upL_t print_labelc_bytes(std::string label, siL_t x, char color){
    return print_labelc(
        std::move(label),
        StringTools::tostr(x, StringTools::BYTES),
        color
    );
}
YM_NO_INLINE upL_t println_labelc_bytes(std::string label, siL_t x, char color){
    return println_labelc(
        std::move(label),
        StringTools::tostr(x, StringTools::BYTES),
        color
    );
}
YM_NO_INLINE upL_t print_labelc_ebytes(std::string label, siL_t x, char color){
    return print_labelc(
        std::move(label),
        StringTools::tostr(x, StringTools::BYTES_EXPANDED),
        color
    );
}
YM_NO_INLINE upL_t println_labelc_ebytes(std::string label, siL_t x, char color){
    return println_labelc(
        std::move(label),
        StringTools::tostr(x, StringTools::BYTES_EXPANDED),
        color
    );
}
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE siL_t scan_label_siL(const std::string& label, char color){
    print(label, color != ' ' ? 'w' : ' ');
    siL_t out = scan_siL(color);
    if (color != ' '){
        SetColor('w');
    }
    return out;
}
YM_NO_INLINE siL_t scan_label_siL_range(const std::string& label, siL_t low, siL_t high, char color){
    siL_t out;
    do{
        out = scan_label_siL(label, color);
    }while (low > out || out > high);
    if (color != ' '){
        SetColor('w');
    }
    return out;
}
YM_NO_INLINE siL_t scan_label_siL_suffix(const std::string& label, char color){
    print(label, color != ' ' ? 'w' : ' ');
    siL_t out = scan_siL_suffix(color);
    if (color != ' '){
        SetColor('w');
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
    uiL_t out;
    do{
        out = scan_label_siL(label, color);
    }while (low > out || out > high);
    if (color != ' '){
        SetColor('w');
    }
    return out;
}
YM_NO_INLINE uiL_t scan_label_uiL_suffix_range(const std::string& label, uiL_t low, uiL_t high, char color){
    uiL_t out;
    do{
        out = scan_label_siL_suffix(label, color);
    }while (low > out || out > high);
    if (color != ' '){
        SetColor('w');
    }
    return out;
}
YM_NO_INLINE uiL_t scan_label_bytes(const std::string& label, char color){
    print(label, color != ' ' ? 'w' : ' ');
    return scan_bytes(color);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Integer Labels with Margin
YM_NO_INLINE upL_t print_labelm(std::string label, siL_t x, char color){
    return print_labelm(DEFAULT_MARGIN, std::move(label), x, color);
}
YM_NO_INLINE upL_t println_labelm(std::string label, siL_t x, char color){
    return println_labelm(DEFAULT_MARGIN, std::move(label), x, color);
}
YM_NO_INLINE upL_t print_labelm_commas(std::string label, siL_t x, char color){
    return print_labelm_commas(DEFAULT_MARGIN, std::move(label), x, color);
}
YM_NO_INLINE upL_t println_labelm_commas(std::string label, siL_t x, char color){
    return println_labelm_commas(DEFAULT_MARGIN, std::move(label), x, color);
}
YM_NO_INLINE upL_t print_labelm_bytes(std::string label, siL_t x, char color){
    return print_labelm_bytes(DEFAULT_MARGIN, std::move(label), x, color);
}
YM_NO_INLINE upL_t println_labelm_bytes(std::string label, siL_t x, char color){
    return println_labelm_bytes(DEFAULT_MARGIN, std::move(label), x, color);
}
YM_NO_INLINE upL_t print_labelm_ebytes(std::string label, siL_t x, char color){
    return print_labelm_ebytes(DEFAULT_MARGIN, std::move(label), x, color);
}
YM_NO_INLINE upL_t println_labelm_ebytes(std::string label, siL_t x, char color){
    return println_labelm_ebytes(DEFAULT_MARGIN, std::move(label), x, color);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE upL_t print_labelm(upL_t margin, std::string label, siL_t x, char color){
    return print_labelm(
        margin,
        std::move(label),
        StringTools::tostr(x, StringTools::NORMAL),
        color
    );
}
YM_NO_INLINE upL_t println_labelm(upL_t margin, std::string label, siL_t x, char color){
    return println_labelm(
        margin,
        std::move(label),
        StringTools::tostr(x, StringTools::NORMAL),
        color
    );
}
YM_NO_INLINE upL_t print_labelm_commas(upL_t margin, std::string label, siL_t x, char color){
    return print_labelm(
        margin,
        std::move(label),
        StringTools::tostr(x, StringTools::COMMAS),
        color
    );
}
YM_NO_INLINE upL_t println_labelm_commas(upL_t margin, std::string label, siL_t x, char color){
    return println_labelm(
        margin,
        std::move(label),
        StringTools::tostr(x, StringTools::COMMAS),
        color
    );
}
YM_NO_INLINE upL_t print_labelm_bytes(upL_t margin, std::string label, siL_t x, char color){
    return print_labelm(
        margin,
        std::move(label),
        StringTools::tostr(x, StringTools::BYTES),
        color
    );
}
YM_NO_INLINE upL_t println_labelm_bytes(upL_t margin, std::string label, siL_t x, char color){
    return println_labelm(
        margin,
        std::move(label),
        StringTools::tostr(x, StringTools::BYTES),
        color
    );
}
YM_NO_INLINE upL_t print_labelm_ebytes(upL_t margin, std::string label, siL_t x, char color){
    return print_labelm(
        margin,
        std::move(label),
        StringTools::tostr(x, StringTools::BYTES_EXPANDED),
        color
    );
}
YM_NO_INLINE upL_t println_labelm_ebytes(upL_t margin, std::string label, siL_t x, char color){
    return println_labelm(
        margin,
        std::move(label),
        StringTools::tostr(x, StringTools::BYTES_EXPANDED),
        color
    );
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Integer Labels with Margin and Units
YM_NO_INLINE upL_t print_labelm_commas(std::string label, siL_t x, const std::string& units, char color){
    return print_labelm_commas(DEFAULT_MARGIN, std::move(label), x, units, color);
}
YM_NO_INLINE upL_t println_labelm_commas(std::string label, siL_t x, const std::string& units, char color){
    return println_labelm_commas(DEFAULT_MARGIN, std::move(label), x, units, color);
}
YM_NO_INLINE upL_t print_labelm_commas(upL_t margin, std::string label, siL_t x, const std::string& units, char color){
    return print_labelm(
        margin,
        std::move(label),
        StringTools::tostr(x, StringTools::COMMAS) + " " + units,
        color
    );
}
YM_NO_INLINE upL_t println_labelm_commas(upL_t margin, std::string label, siL_t x, const std::string& units, char color){
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
YM_NO_INLINE upL_t print_labelm_float(std::string label, double x, int precision, char color){
    return print_labelm_float(DEFAULT_MARGIN, label, x, precision, color);
}
YM_NO_INLINE upL_t println_labelm_float(std::string label, double x, int precision, char color){
    return println_labelm_float(DEFAULT_MARGIN, label, x, precision, color);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE upL_t print_labelm_float(upL_t margin, std::string label, double x, int precision, char color){
    return print_labelm(
        margin,
        std::move(label),
        StringTools::tostr_float(x, precision),
        color
    );
}
YM_NO_INLINE upL_t println_labelm_float(upL_t margin, std::string label, double x, int precision, char color){
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
YM_NO_INLINE upL_t print_unitl_float(const std::string& units, double x, int precision, char color){
    return print_unitl_float(16, units, x, precision, color);
}
YM_NO_INLINE upL_t println_unitl_float(const std::string& units, double x, int precision, char color){
    return println_unitl_float(16, units, x, precision, color);
}
YM_NO_INLINE upL_t print_unitl_float(upL_t margin, const std::string& units, double x, int precision, char color){
    upL_t ret = 0;
    ret += print_marginl_float(margin, x, precision, color);
    if (color != ' ')
        SetColor('w');
    ret += print(units);
    return ret;
}
YM_NO_INLINE upL_t println_unitl_float(upL_t margin, const std::string& units, double x, int precision, char color){
    upL_t ret = 0;
    ret += print_marginl_float(margin, x, precision, color);
    if (color != ' ')
        SetColor('w');
    ret += println(units);
    return ret;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
