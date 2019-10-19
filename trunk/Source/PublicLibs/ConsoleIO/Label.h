/* Label.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/31/2014
 * Last Modified    : 08/31/2014
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
//  String Labels
YM_NO_INLINE    upL_t   print_labelc            (std::string label, const std::string& x, char color = ' ');
YM_NO_INLINE    upL_t   println_labelc          (std::string label, const std::string& x, char color = ' ');
YM_NO_INLINE    upL_t   print_labelm            (std::string label, const std::string& x, char color = ' ');
YM_NO_INLINE    upL_t   println_labelm          (std::string label, const std::string& x, char color = ' ');
YM_NO_INLINE    upL_t   print_labelm            (upL_t margin, std::string label, const std::string& x, char color = ' ');
YM_NO_INLINE    upL_t   println_labelm          (upL_t margin, std::string label, const std::string& x, char color = ' ');
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE    upL_t   print_labelc            (std::string label, const std::wstring& x, char color = ' ');
YM_NO_INLINE    upL_t   println_labelc          (std::string label, const std::wstring& x, char color = ' ');
YM_NO_INLINE    upL_t   print_labelm            (std::string label, const std::wstring& x, char color = ' ');
YM_NO_INLINE    upL_t   println_labelm          (std::string label, const std::wstring& x, char color = ' ');
YM_NO_INLINE    upL_t   print_labelm            (upL_t margin, std::string label, const std::wstring& x, char color = ' ');
YM_NO_INLINE    upL_t   println_labelm          (upL_t margin, std::string label, const std::wstring& x, char color = ' ');
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE    std::string     scan_labelc_utf8(std::string label, char color = 'T');
YM_NO_INLINE    std::wstring    scan_labelc_wstr(std::string label, char color = 'T');
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Integer Labels
YM_NO_INLINE    upL_t   print_labelc            (std::string label, siL_t x, char color = ' ');
YM_NO_INLINE    upL_t   println_labelc          (std::string label, siL_t x, char color = ' ');
YM_NO_INLINE    upL_t   print_labelc_commas     (std::string label, siL_t x, char color = ' ');
YM_NO_INLINE    upL_t   println_labelc_commas   (std::string label, siL_t x, char color = ' ');
YM_NO_INLINE    upL_t   print_labelc_bytes      (std::string label, siL_t x, char color = ' ');
YM_NO_INLINE    upL_t   println_labelc_bytes    (std::string label, siL_t x, char color = ' ');
YM_NO_INLINE    upL_t   print_labelc_ebytes     (std::string label, siL_t x, char color = ' ');
YM_NO_INLINE    upL_t   println_labelc_ebytes   (std::string label, siL_t x, char color = ' ');
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE    siL_t   scan_label_siL                  (const std::string& label, char color = 'T');
YM_NO_INLINE    siL_t   scan_label_siL_range            (const std::string& label, siL_t low, siL_t high, char color = 'T');
YM_NO_INLINE    upL_t   scan_label_upL_range            (const std::string& label, upL_t low = 0, upL_t high = (upL_t)0 - 1, char color = 'T');
YM_NO_INLINE    uiL_t   scan_label_uiL_range            (const std::string& label, uiL_t low = 0, uiL_t high = (uiL_t)0 - 1, char color = 'T');
YM_NO_INLINE    siL_t   scan_label_siL_suffix           (const std::string& label, char color = 'T');
YM_NO_INLINE    upL_t   scan_label_upL_suffix_range     (const std::string& label, upL_t low = 0, upL_t high = (upL_t)0 - 1, char color = 'T');
YM_NO_INLINE    uiL_t   scan_label_uiL_suffix_range     (const std::string& label, uiL_t low = 0, uiL_t high = (uiL_t)0 - 1, char color = 'T');
YM_NO_INLINE    uiL_t   scan_label_bytes                (const std::string& label = "Bytes: ", char color = 'T');
////////////////////////////////////////////////////////////////////////////////
//  Integer Labels with Margin
YM_NO_INLINE    upL_t   print_labelm            (std::string label, siL_t x, char color = ' ');
YM_NO_INLINE    upL_t   println_labelm          (std::string label, siL_t x, char color = ' ');
YM_NO_INLINE    upL_t   print_labelm_commas     (std::string label, siL_t x, char color = ' ');
YM_NO_INLINE    upL_t   println_labelm_commas   (std::string label, siL_t x, char color = ' ');
YM_NO_INLINE    upL_t   print_labelm_bytes      (std::string label, siL_t x, char color = ' ');
YM_NO_INLINE    upL_t   println_labelm_bytes    (std::string label, siL_t x, char color = ' ');
YM_NO_INLINE    upL_t   print_labelm_ebytes     (std::string label, siL_t x, char color = ' ');
YM_NO_INLINE    upL_t   println_labelm_ebytes   (std::string label, siL_t x, char color = ' ');
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE    upL_t   print_labelm            (upL_t margin, std::string label, siL_t x, char color = ' ');
YM_NO_INLINE    upL_t   println_labelm          (upL_t margin, std::string label, siL_t x, char color = ' ');
YM_NO_INLINE    upL_t   print_labelm_commas     (upL_t margin, std::string label, siL_t x, char color = ' ');
YM_NO_INLINE    upL_t   println_labelm_commas   (upL_t margin, std::string label, siL_t x, char color = ' ');
YM_NO_INLINE    upL_t   print_labelm_bytes      (upL_t margin, std::string label, siL_t x, char color = ' ');
YM_NO_INLINE    upL_t   println_labelm_bytes    (upL_t margin, std::string label, siL_t x, char color = ' ');
YM_NO_INLINE    upL_t   print_labelm_ebytes     (upL_t margin, std::string label, siL_t x, char color = ' ');
YM_NO_INLINE    upL_t   println_labelm_ebytes   (upL_t margin, std::string label, siL_t x, char color = ' ');
////////////////////////////////////////////////////////////////////////////////
//  Integer Labels with Margin and Units
YM_NO_INLINE    upL_t   print_labelm_commas     (std::string label, siL_t x, const std::string& units, char color = ' ');
YM_NO_INLINE    upL_t   println_labelm_commas   (std::string label, siL_t x, const std::string& units, char color = ' ');
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE    upL_t   print_labelm_commas     (upL_t margin, std::string label, siL_t x, const std::string& units, char color = ' ');
YM_NO_INLINE    upL_t   println_labelm_commas   (upL_t margin, std::string label, siL_t x, const std::string& units, char color = ' ');
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Float Labels with Margin
YM_NO_INLINE    upL_t   print_labelm_float      (std::string label, double x, int precision = DEFAULT_PRECISION, char color = ' ');
YM_NO_INLINE    upL_t   println_labelm_float    (std::string label, double x, int precision = DEFAULT_PRECISION, char color = ' ');
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE    upL_t   print_labelm_float      (upL_t margin, std::string label, double x, int precision = DEFAULT_PRECISION, char color = ' ');
YM_NO_INLINE    upL_t   println_labelm_float    (upL_t margin, std::string label, double x, int precision = DEFAULT_PRECISION, char color = ' ');
////////////////////////////////////////////////////////////////////////////////
//  Float with Units
YM_NO_INLINE    upL_t   print_unitl_float       (const std::string& units, double x, int precision = DEFAULT_PRECISION, char color = ' ');
YM_NO_INLINE    upL_t   println_unitl_float     (const std::string& units, double x, int precision = DEFAULT_PRECISION, char color = ' ');
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE    upL_t   print_unitl_float       (upL_t margin, const std::string& units, double x, int precision = DEFAULT_PRECISION, char color = ' ');
YM_NO_INLINE    upL_t   println_unitl_float     (upL_t margin, const std::string& units, double x, int precision = DEFAULT_PRECISION, char color = ' ');
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
