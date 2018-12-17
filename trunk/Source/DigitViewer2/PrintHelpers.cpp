/* PrintHelpers.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/15/2015
 * Last Modified    : 01/15/2015
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PrintHelpers.h"
namespace DigitViewer2{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const int ROW_WIDTH = 79;
void ClearLine(){
    //  Clears the a 79-character line.
    Console::print("\r");

    int c = 0;
    while (c++ < ROW_WIDTH){
        Console::print(" ");
    }

    Console::print("\r");
}
void DisplayChar(char digit, upL_t *b, upL_t *r, uiL_t *c){
    //  Print a single digit.

    //Parameters:
    //  -   b       =   position in the current 10-digit block
    //  -   r       =   position in the current 50-digit row
    //  -   c       =   position from the decimal place

    (*b)++;
    (*r)++;
    (*c)++;

    char str[] = {digit, '\0'};
    Console::print(str, 'Y');
    Console::SetColor('w');

    if (*b == 10){
        *b = 0;
        Console::print(" ");
    }
    if (*r == 50){
        *r = 0;
        Console::print(" :  ");
        Console::println_commas(*c, 'G');
        Console::SetColor('w');
    }
}
void DisplayFancy(uiL_t pos, const char *str, upL_t digits){
    //  Print a string of digits in fancy format.

    uiL_t end = pos + digits;

    uiL_t c = pos - pos % 50;
    upL_t b = 0;
    upL_t r = 0;

    //  Leading spaces
    while (c < pos){
        DisplayChar(' ', &b, &r, &c);
    }

    //  Print digits
    while (c < end){
        char ch = *str++;
        if (ch == '\0'){
            break;
        }
        DisplayChar(ch, &b, &r, &c);
    }

    if (c < 50){
        Console::println("\n");
    }else{
        Console::println();
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
