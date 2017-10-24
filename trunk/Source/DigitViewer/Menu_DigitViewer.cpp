/* Menu_DigitViewer.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/25/2015
 * Last Modified    : 01/25/2015
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "Globals.h"
#include "DigitReaders/DigitReader.h"
#include "DigitReaders/TextReader.h"
#include "DigitReaders/YCDReader.h"
#include "EndPointTasks.h"
#include "Menu_DigitViewer.h"
namespace DigitViewer{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Sub Menus
void Menu_TextFile(TextReader* reader){
    Console::println("\n\nText File\n");

    Console::print("Total Digits after decimal place: ");
    Console::println_commas(reader->get_digits(), 'G');
    Console::println();

    Console::println("First Digits:", 'w');
    Console::println(reader->get_first_digits(50).c_str(), 'Y');
    Console::println("\n");

    Console::println("What would you like to do?\n", 'w');
    Console::print("  0     ");
    Console::println("View a small range of digits.", 'G');
    Console::print("  1     ", 'w');
    Console::println("Count up the # of instances of each digit.", 'G');

    if (reader->get_radix() == 10){
        Console::print("  2     ", 'w');
        Console::println("Compute hash: Floor(x * 10^digits) mod (2^61 - 1)", 'G');
    }

    Console::print("  3     ", 'w');
    Console::println("Compress the digits into one or more .ycd file.", 'G');

    Console::println("\nEnter your choice:", 'w');
    upL_t c = Console::scan_label_upL_range("option: ", 0, 3);
    Console::println();

    switch (c){
        case 0:
            ViewRange(reader);
            break;
        case 1:
            CountDigits(reader);
            break;
        case 2:
            if (reader->get_radix() == 10)
                ComputeHash(reader);
            break;
        case 3:
            ToYCDFileAll(reader);
            break;
        default:;
    }
}
void Menu_YCDFile(YCDReader* reader){
    while (true){
        Console::println("\n\nCompressed Digit File\n");

        Console::println("Digits Found:");
        uiL_t digits = reader->get_digits();
        if (digits == 0){
            Console::println("Unknown - Extended File Format", 'G');
        }else{
            Console::println_commas(digits, 'G');
        }
        Console::println();

        Console::println("First Digits:", 'w');
        Console::println(reader->get_first_digits(64).c_str(), 'Y');
        Console::println();

        Console::println("Digits per File:", 'w');
        Console::println_commas(reader->get_digits_per_file(), 'Y');
        Console::println();

        Console::println("Search paths:", 'w');
        Console::SetColor('Y');
        reader->print_paths();
        Console::println();

        Console::println("What would you like to do?\n", 'w');
        Console::print("  0     ");
        Console::println("View a small range of digits.", 'G');
        Console::print("  1     ", 'w');
        Console::println("Count up the # of instances of each digit.", 'G');

        if (reader->get_radix() == 10){
            Console::print("  2     ", 'w');
            Console::println("Compute hash: Floor(x * 10^digits) mod (2^61 - 1)", 'G');
        }

        Console::print("  3     ", 'w');
        Console::println("Decompress a range of digits to a text file.", 'G');
        Console::print("  4     ", 'w');
        Console::println("Recompress a range of digits to one or more .ycd files.", 'G');
        Console::println("        (Advanced - Don't use unless you know what you're doing...)", 'Y');
        Console::println();
        Console::print("  5     ", 'w');
        Console::print("Add search directory.", 'G');
        Console::println(" (if .ycd files are in multiple paths)", 'Y');

        Console::println("\nEnter your choice:", 'w');
        upL_t c = Console::scan_label_upL_range("option: ", 0, 5);

        switch (c){
            case 0:
                ViewRange(reader);
                return;
            case 1:
                CountDigits(reader);
                return;
            case 2:
                if (reader->get_radix() == 10)
                    ComputeHash(reader);
                return;
            case 3:
                ToTextFile(reader);
                return;
            case 4:
                ToYCDFilePartial(reader);
                return;
            case 5:
            {
                Console::println("\nEnter directory:");
                reader->add_search_path(Console::scan_utf8());
                break;
            }
            default:;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void Menu_DigitViewer(){
    Console::println("\n\n");
    Console::println(VERSION, 'G');
    Console::println();

    Console::println("Enter the path of the file (include .txt or .ycd extensions):", 'w');

    //  Prompt for path
    auto path = Console::scan_utf8();

    auto reader = OpenDigitFile(std::move(path));

#if (defined _CPPRTTI) || (defined __GXX_RTTI)
    //  The stand-alone Digit Viewer enables RTTI, so we can use this.

    //  Use RTTI.
    TextReader* text = dynamic_cast<TextReader*>(reader.get());
    if (text != NULL){
        Menu_TextFile(text);
        return;
    }
    YCDReader  *ycd  = dynamic_cast<YCDReader*>(reader.get());
    if (ycd != NULL){
        Menu_YCDFile(ycd);
        return;
    }
#else
    //  The Digit Viewer comes bundled with y-cruncher v0.6.8 in the same binary.
    //  But y-cruncher disables RTTI. So we need to do something different here.

    std::string extension(reader->get_extension());
    if (extension == "txt"){
        Menu_TextFile(static_cast<TextReader*>(reader.get()));
        return;
    }
    if (extension == "ycd"){
        Menu_YCDFile(static_cast<YCDReader*>(reader.get()));
        return;
    }

#endif
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
