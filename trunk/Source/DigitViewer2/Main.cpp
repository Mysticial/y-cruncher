#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/Exceptions/Exception.h"
#include "DigitViewer/DigitViewerUI2.h"
using namespace ymp;

int main(){
    try{

        //  Launch the main menu.
        DigitViewer2::Menu_Main();

    }catch (Exception &e){
        e.print();
    }catch (std::exception &e){
        Console::print(e.what());
    }

    Console::println("\n");
    Console::Pause('w');
}
