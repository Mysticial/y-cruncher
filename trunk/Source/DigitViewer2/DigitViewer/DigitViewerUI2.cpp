/* DigitViewerUI2.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/18/2018
 * Last Modified    : 02/18/2018
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <memory>
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/SystemLibs/FileIO/FileException.h"
#include "DigitViewer2/Globals.h"
#include "DigitViewer2/DigitReaders/BasicDigitReader.h"
#include "DigitViewer2/DigitReaders/BasicTextReader.h"
#include "DigitViewer2/DigitReaders/BasicYcdSetReader.h"
#include "DigitViewerTasks.h"
#include "DigitViewerUI2.h"
namespace DigitViewer2{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void Menu_TextFile(BasicTextReader& reader){
    Console::println("\n\nText File");
    Console::println();

    Console::println("First Digits:", 'w');
    Console::println(reader.first_digits(), 'Y');
    Console::println("", 'w');

    Console::print("Digits after Decimal Place: ");
    uiL_t digits = reader.stream_end();
    Console::println_commas(digits, 'G');
    Console::println();


    Console::println("What would you like to do?\n", 'w');
    Console::print("  0     ");
    Console::println("View a small range of digits.", 'G');
    Console::print("  1     ", 'w');
    Console::println("Compute Stats (Digit Counts + Hash).", 'G');
    Console::print("  2     ", 'w');
    Console::println("Extract digits into a .txt file.", 'G');
    Console::print("  3     ", 'w');
    Console::println("Compress digits 1 - N into one or more .ycd files.", 'G');
    Console::print("  4     ", 'w');
    Console::println("Compress a subset of digits into .ycd files.", 'G');

    Console::println("\nEnter your choice:", 'w');
    upL_t c = Console::scan_label_upL_range("option: ", 0, 4);
    Console::println();

    switch (c){
        case 0:
            view_range(reader);
            return;
        case 1:
            compute_stats(reader);
            return;
        case 2:
            to_text_file(reader);
            return;
        case 3:
            to_ycd_file_all(reader);
            return;
        case 4:
            to_ycd_file_partial(reader);
            return;
        default:;
    }
}
void Menu_YcdFile(BasicYcdSetReader& reader){
    while (true){
        Console::println("\n\nCompressed Digit File");
        Console::println();

        Console::println("First Digits:", 'w');
        Console::println(reader.first_digits(), 'Y');
        Console::println("", 'w');

        Console::print("Digits after Decimal Place: ");
        uiL_t digits = reader.stream_end();
        if (digits == 0){
            Console::println("Unknown - Extended File Format", 'G');
        }else{
            Console::println_commas(digits, 'G');
        }
        Console::println();

        Console::println("Digits per File:", 'w');
        Console::println_commas(reader.digits_per_file(), 'Y');
        Console::println();

        Console::println("Search paths:", 'w');
        Console::SetColor('Y');
        reader.print_paths();
        Console::println();


        Console::println("What would you like to do?\n", 'w');
        Console::print("  0     ");
        Console::println("View a small range of digits.", 'G');
        Console::print("  1     ", 'w');
        Console::println("Compute Stats (Digit Counts + Hash).", 'G');
        Console::print("  2     ", 'w');
        Console::println("Extract digits into a .txt file.", 'G');
        Console::print("  3     ", 'w');
        Console::println("Compress digits 1 - N into one or more .ycd files.", 'G');
        Console::print("  4     ", 'w');
        Console::println("Compress a subset of digits into .ycd files.", 'G');
        Console::println();

        Console::print("  5     ", 'w');
        Console::print("Add search directory.", 'G');
        Console::println(" (if .ycd files are in multiple paths)", 'Y');

        Console::println("\nEnter your choice:", 'w');
        upL_t c = Console::scan_label_upL_range("option: ", 0, 5);
        Console::println();

        switch (c){
            case 0:
                view_range(reader);
                return;
            case 1:
                compute_stats(reader);
                return;
            case 2:
                to_text_file(reader);
                return;
            case 3:
                to_ycd_file_all(reader);
                return;
            case 4:
                to_ycd_file_partial(reader);
                return;
            case 5:
                Console::println("\nEnter directory:");
                reader.add_search_path(Console::scan_utf8());
                break;
            default:;
        }
    }
}
void Menu_DigitViewer(bool skip_header){
    if (!skip_header){
        Console::println("\n\n");
        Console::println(VERSION, 'G');
        Console::println();
    }

    Console::println("Enter the path of the file (include .txt or .ycd extensions):", 'w');

    //  Prompt for path
    std::string path = Console::scan_utf8();


    //  Extract the extension
    size_t extension_offset = path.rfind('.');
    if (extension_offset >= path.size()){
        throw FileIO::FileException("Menu_DigitViewer()", path, "No Extension found.");
    }
    std::string extension = path.substr(extension_offset);

    //  Open the file
    if (extension == ".txt"){
        BasicTextReader reader(path);
        Menu_TextFile(reader);
    }else if (extension == ".ycd"){
        BasicYcdSetReader reader(path);
        Menu_YcdFile(reader);
    }else{
        throw FileIO::FileException("Menu_DigitViewer()", path, "Unrecognized Extension: " + extension);
    }
}
void Menu_Main(){
    Console::println(VERSION, 'G');
    Console::println();

    Console::print("Copyright ", 'T');
    Console::print("2008-2019 Alexander J. Yee");
    Console::print("      ");
    Console::print("( a-yee@u.northwestern.edu )");
    Console::println("\n");

    Console::print("Distribute Freely - Please Report any Bugs", 'w');
    Console::println("\n\n");

    Menu_DigitViewer(true);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
