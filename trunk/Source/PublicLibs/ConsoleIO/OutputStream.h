/* OutputStream.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 07/29/2024
 *  Last Modified   : 08/08/2024
 * 
 * 
 *      This file is the abstraction for console printing. This isn't just a
 *  normal logger as it has support for the refreshing status lines.
 * 
 *  A refreshing status line is a "temp line". Print a refreshable status line
 *  by calling "print_temp_line()". Calling it again will replace the line with
 *  new text.
 * 
 *  The moment you call a regular print "operator+=()", it clears all temp lines
 *  before printing the new text. There is one exception. If a regular print is
 *  empty or starts with a newline (either LF or CRLF), it will not clear the
 *  temp lines. This allows error handling to preserve the last known status line
 *  for easier debugging.
 * 
 */

#pragma once
#ifndef ymp_Console_OutputStream_H
#define ymp_Console_OutputStream_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <memory>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
#include "ColorStrings.h"
namespace ymp{
namespace Console{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
enum class StatusPrintMode{
    NONE,
    OVERWRITE,
    APPEND,
};
std::string to_string(StatusPrintMode mode);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class ConsoleLogger{
public:
    virtual ~ConsoleLogger() = default;
    virtual void flush(){}

    virtual void operator+=(const ColorString& str) = 0;
    virtual void operator+=(const ColoredLine& line);
    virtual void log_input(const std::string& str){}

    //  Clear all the temp lines. For console loggers, this will replace them
    //  with spaces.
    virtual void clear_temp_lines() = 0;

    //  Print the specified temp line(s). If there already are temp lines, they
    //  will be removed and replaced with the new one(s).
    virtual void print_temp_line(const ColoredLine& line) = 0;
    virtual void print_temp_lines(const std::vector<ColoredLine>& lines) = 0;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class ConsoleSink : public ConsoleLogger{
public:
    ConsoleSink(StatusPrintMode mode, bool enable_colors, upL_t line_length)
        : m_mode(mode)
        , m_enable_colors(enable_colors)
        , m_line_length(line_length)
        , m_temp_lines(0)
    {}

    virtual std::unique_ptr<ConsoleSink> clone(
        StatusPrintMode mode,
        bool enable_colors,
        upL_t line_length
    ) const = 0;

    StatusPrintMode status_line_mode() const{ return m_mode; }
    bool colors_enabled() const{ return m_enable_colors; }
    upL_t line_length() const{ return m_line_length; }

    virtual void print_temp_line(const ColoredLine& line);
    virtual void print_temp_lines(const std::vector<ColoredLine>& lines);

protected:
    //  Prepare to erase the all the temp lines and to print "lines" of new
    //  temp lines.
    //
    //  By default this will simply call "clear_temp_lines()" to erase all the
    //  existing temp lines (usually by printing spaces over them). But child
    //  classes are allowed to do something more efficient like simply moving
    //  the cursor and allowing the new prints to overwrite what's already there.
    virtual void prepare_overwrite(upL_t lines_to_replace_with);

    //  Child classes must implement this to print a single token.
    virtual void internal_print(const ColorString& str) = 0;

    //  Helper function for child classes to determine if it should clear all
    //  the temp lines before printing anything new.
    bool clear_temp_on_print(const std::string& text);

protected:
    const StatusPrintMode m_mode;
    const bool m_enable_colors;
    const upL_t m_line_length;

    upL_t m_temp_lines;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void trim_line_length_limit(
    upL_t& tokens, upL_t& line_length,
    const ColoredLine& line, upL_t length_limit
);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
std::string vterm_color(char color);
std::string vterm_window_size(int width, int height);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
