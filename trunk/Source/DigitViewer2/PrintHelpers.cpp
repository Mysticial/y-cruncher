/* PrintHelpers.cpp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 01/15/2015
 *  Last Modified   : 02/09/2024
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/BasicLibs/StringTools/ToString.h"
#include "PrintHelpers.h"
namespace DigitViewer2{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const int ROW_WIDTH = 79;
void clear_line(){
    //  Clears the a 79-character line.
    Console::print("\r");

    int c = 0;
    while (c++ < ROW_WIDTH){
        Console::print(" ");
    }

    Console::print("\r");
}
void display_char(char digit, uiL_t& c, upL_t& b, upL_t& r){
    //  Print a single digit.

    //Parameters:
    //  -   c       =   position from the decimal place
    //  -   b       =   position in the current 10-digit block
    //  -   r       =   position in the current 50-digit row

    c++;
    b++;
    r++;

    char str[] = {digit, '\0'};
    Console::print(str, 'Y');
    Console::set_color('w');

    if (b == 10){
        b = 0;
        Console::print(" ");
    }
    if (r == 50){
        r = 0;
        Console::print(" :  ");
        Console::println_commas(c, 'G');
        Console::set_color('w');
    }
}
void display_fancy(uiL_t pos, const char* str, upL_t digits){
    //  Print a string of digits in fancy format.

    uiL_t end = pos + digits;

    uiL_t c = pos - pos % 50;
    upL_t b = 0;
    upL_t r = 0;

    //  Leading spaces
    while (c < pos){
        display_char(' ', c, b, r);
    }

    //  Print digits
    while (c < end){
        char ch = *str++;
        if (ch == '\0'){
            break;
        }
        display_char(ch, c, b, r);
    }

    if (c < 50){
        Console::println("\n");
    }else{
        Console::println();
    }
}
std::string to_string_formatted(uiL_t pos, const char* str, upL_t digits){
    uiL_t end = pos + digits;

    uiL_t c = pos - pos % 50;
    upL_t b = 0;
    upL_t r = 0;
    uiL_t stop = (end + 49) / 50 * 50;

    bool first = true;

    std::string ret;
    while (c < stop){
        if (r == 0 && !first){
            ret += "\r\n";
        }
        first = false;
        c++;
        b++;
        r++;
        ret += c < pos || c > end ? ' ' : *str++;
        if (b == 10){
            b = 0;
            ret += ' ';
        }
        if (r == 50){
            r = 0;
            ret += " :  ";
            ret += StringTools::tostr(c, StringTools::COMMAS);
        }
    }
//    ret += "\r\n";

    return ret;
}
uiL_t prompt_digits(const std::string& label, uiL_t min, uiL_t max){
    Console::println("Enter the number of digits: (e.g. \"1000000\", \"25m\", \"10b\")", 'G');
    Console::println();

    Console::println("Suffixes:", 'w');
    Console::println("    m = 1,000,000             ( 10^6 )");
    Console::println("    b = 1,000,000,000         ( 10^9 )");
    Console::println("    t = 1,000,000,000,000     ( 10^12 )");
    Console::println("    q = 1,000,000,000,000,000 ( 10^15 )");
    Console::println("    K = 1,024                 ( 2^10 )");
    Console::println("    M = 1,048,576             ( 2^20 )");
    Console::println("    G = 1,073,741,824         ( 2^30 )");
    Console::println("    T = 1,099,511,627,776     ( 2^40 )");
    Console::println("    P = 1,125,899,906,842,624 ( 2^50 )");
    Console::println();

    return Console::scan_label_uiL_suffix_range(label, min, max);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
