/* Label.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 08/31/2014
 *  Last Modified   : 08/31/2014
 * 
 */

#pragma once
#ifndef ymp_Console_Label_H
#define ymp_Console_Label_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "BasicIO.h"
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
YM_NO_INLINE    std::string print_labelc            (std::string label, const std::string& x, char color = ' ');
YM_NO_INLINE    std::string println_labelc          (std::string label, const std::string& x, char color = ' ');
YM_NO_INLINE    std::string print_labelm            (std::string label, const std::string& x, char color = ' ');
YM_NO_INLINE    std::string println_labelm          (std::string label, const std::string& x, char color = ' ');
YM_NO_INLINE    std::string print_labelm            (upL_t margin, std::string label, const std::string& x, char color = ' ');
YM_NO_INLINE    std::string println_labelm          (upL_t margin, std::string label, const std::string& x, char color = ' ');
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE    std::string     scan_labelc_utf8    (std::string label, char color = 'T');
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Integer Labels
template <typename IntegerType>
std::string print_labelc_int(std::string label, const IntegerType& x, char color = ' '){
    return print_labelc(std::move(label), StringTools::tostr(x, StringTools::NORMAL), color);
}
template <typename IntegerType>
std::string println_labelc_int(std::string label, const IntegerType& x, char color = ' '){
    return println_labelc(std::move(label), StringTools::tostr(x, StringTools::NORMAL), color);
}
template <typename IntegerType>
std::string print_labelc_commas(std::string label, const IntegerType& x, char color = ' '){
    return print_labelc(std::move(label), StringTools::tostr(x, StringTools::COMMAS), color);
}
template <typename IntegerType>
std::string println_labelc_commas(std::string label, const IntegerType& x, char color = ' '){
    return println_labelc(std::move(label), StringTools::tostr(x, StringTools::COMMAS), color);
}
template <typename IntegerType>
std::string print_labelc_bytes(std::string label, const IntegerType& x, char color = ' '){
    return print_labelc(std::move(label), StringTools::tostr(x, StringTools::BYTES), color);
}
template <typename IntegerType>
std::string println_labelc_bytes(std::string label, const IntegerType& x, char color = ' '){
    return println_labelc(std::move(label), StringTools::tostr(x, StringTools::BYTES), color);
}
template <typename IntegerType>
std::string print_labelc_ebytes(std::string label, const IntegerType& x, char color = ' '){
    return print_labelc(std::move(label), StringTools::tostr(x, StringTools::BYTES_EXPANDED), color);
}
template <typename IntegerType>
std::string println_labelc_ebytes(std::string label, const IntegerType& x, char color = ' '){
    return println_labelc(std::move(label), StringTools::tostr(x, StringTools::BYTES_EXPANDED), color);
}
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE    siL_t   scan_label_siL              (const std::string& label, char color = 'T');
YM_NO_INLINE    siL_t   scan_label_siL_range        (const std::string& label, siL_t low, siL_t high, char color = 'T');
YM_NO_INLINE    upL_t   scan_label_upL_range        (const std::string& label, upL_t low = 0, upL_t high = (upL_t)-1, char color = 'T');
YM_NO_INLINE    uiL_t   scan_label_uiL_range        (const std::string& label, uiL_t low = 0, uiL_t high = (uiL_t)-1, char color = 'T');
YM_NO_INLINE    siL_t   scan_label_siL_suffix       (const std::string& label, char color = 'T');
YM_NO_INLINE    upL_t   scan_label_upL_suffix_range (const std::string& label, upL_t low = 0, upL_t high = (upL_t)-1, char color = 'T');
YM_NO_INLINE    uiL_t   scan_label_uiL_suffix_range (const std::string& label, uiL_t low = 0, uiL_t high = (uiL_t)-1, char color = 'T');
YM_NO_INLINE    uiL_t   scan_label_bytes            (const std::string& label = "Bytes: ", char color = 'T');
////////////////////////////////////////////////////////////////////////////////
//  Integer Labels with Margin
template <typename IntegerType>
std::string print_labelm_int(std::string label, const IntegerType& x, char color = ' '){
    return print_labelm(std::move(label), StringTools::tostr(x, StringTools::NORMAL), color);
}
template <typename IntegerType>
std::string println_labelm_int(std::string label, const IntegerType& x, char color = ' '){
    return println_labelm(std::move(label), StringTools::tostr(x, StringTools::NORMAL), color);
}
template <typename IntegerType>
std::string print_labelm_commas(std::string label, const IntegerType& x, char color = ' '){
    return print_labelm(std::move(label), StringTools::tostr(x, StringTools::COMMAS), color);
}
template <typename IntegerType>
std::string println_labelm_commas(std::string label, const IntegerType& x, char color = ' '){
    return println_labelm(std::move(label), StringTools::tostr(x, StringTools::COMMAS), color);
}
template <typename IntegerType>
std::string print_labelm_bytes(std::string label, const IntegerType& x, char color = ' '){
    return print_labelm(std::move(label), StringTools::tostr(x, StringTools::BYTES), color);
}
template <typename IntegerType>
std::string println_labelm_bytes(std::string label, const IntegerType& x, char color = ' '){
    return println_labelm(std::move(label), StringTools::tostr(x, StringTools::BYTES), color);
}
template <typename IntegerType>
std::string print_labelm_ebytes(std::string label, const IntegerType& x, char color = ' '){
    return print_labelm(std::move(label), StringTools::tostr(x, StringTools::BYTES_EXPANDED), color);
}
template <typename IntegerType>
std::string println_labelm_ebytes(std::string label, const IntegerType& x, char color = ' '){
    return println_labelm(std::move(label), StringTools::tostr(x, StringTools::BYTES_EXPANDED), color);
}
////////////////////////////////////////////////////////////////////////////////
template <typename IntegerType>
std::string print_labelm_int(upL_t margin, std::string label, const IntegerType& x, char color = ' '){
    return print_labelm(margin, std::move(label), StringTools::tostr(x, StringTools::NORMAL), color);
}
template <typename IntegerType>
std::string println_labelm_int(upL_t margin, std::string label, const IntegerType& x, char color = ' '){
    return println_labelm(margin, std::move(label), StringTools::tostr(x, StringTools::NORMAL), color);
}
template <typename IntegerType>
std::string print_labelm_commas(upL_t margin, std::string label, const IntegerType& x, char color = ' '){
    return print_labelm(margin, std::move(label), StringTools::tostr(x, StringTools::COMMAS), color);
}
template <typename IntegerType>
std::string println_labelm_commas(upL_t margin, std::string label, const IntegerType& x, char color = ' '){
    return println_labelm(margin, std::move(label), StringTools::tostr(x, StringTools::COMMAS), color);
}
template <typename IntegerType>
std::string print_labelm_bytes(upL_t margin, std::string label, const IntegerType& x, char color = ' '){
    return print_labelm(margin, std::move(label), StringTools::tostr(x, StringTools::BYTES), color);
}
template <typename IntegerType>
std::string println_labelm_bytes(upL_t margin, std::string label, const IntegerType& x, char color = ' '){
    return println_labelm(margin, std::move(label), StringTools::tostr(x, StringTools::BYTES), color);
}
template <typename IntegerType>
std::string print_labelm_ebytes(upL_t margin, std::string label, const IntegerType& x, char color = ' '){
    return print_labelm(margin, std::move(label), StringTools::tostr(x, StringTools::BYTES_EXPANDED), color);
}
template <typename IntegerType>
std::string println_labelm_ebytes(upL_t margin, std::string label, const IntegerType& x, char color = ' '){
    return println_labelm(margin, std::move(label), StringTools::tostr(x, StringTools::BYTES_EXPANDED), color);
}
////////////////////////////////////////////////////////////////////////////////
//  Integer Labels with Margin and Units
YM_NO_INLINE    std::string print_labelm_commas     (std::string label, siL_t x, const std::string& units, char color = ' ');
YM_NO_INLINE    std::string println_labelm_commas   (std::string label, siL_t x, const std::string& units, char color = ' ');
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE    std::string print_labelm_commas     (upL_t margin, std::string label, siL_t x, const std::string& units, char color = ' ');
YM_NO_INLINE    std::string println_labelm_commas   (upL_t margin, std::string label, siL_t x, const std::string& units, char color = ' ');
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Float Labels with Margin
YM_NO_INLINE    std::string print_labelm_float      (std::string label, double x, int precision = DEFAULT_PRECISION, char color = ' ');
YM_NO_INLINE    std::string println_labelm_float    (std::string label, double x, int precision = DEFAULT_PRECISION, char color = ' ');
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE    std::string print_labelm_float      (upL_t margin, std::string label, double x, int precision = DEFAULT_PRECISION, char color = ' ');
YM_NO_INLINE    std::string println_labelm_float    (upL_t margin, std::string label, double x, int precision = DEFAULT_PRECISION, char color = ' ');
////////////////////////////////////////////////////////////////////////////////
//  Float with Units
YM_NO_INLINE    std::string print_unitl_float       (const std::string& units, double x, int precision = DEFAULT_PRECISION, char color = ' ');
YM_NO_INLINE    std::string println_unitl_float     (const std::string& units, double x, int precision = DEFAULT_PRECISION, char color = ' ');
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE    std::string print_unitl_float       (upL_t margin, const std::string& units, double x, int precision = DEFAULT_PRECISION, char color = ' ');
YM_NO_INLINE    std::string println_unitl_float     (upL_t margin, const std::string& units, double x, int precision = DEFAULT_PRECISION, char color = ' ');
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
