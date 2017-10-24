#include "PublicLibs/Exceptions/Exception.h"
#include "DigitViewerUI.h"
using namespace ymp;

int main(){
    try{

        //  Launch the main menu.
        DigitViewer::Menu_Main();

    }catch (Exception &e){
        e.print();
    }catch (std::exception &e){
        Console::print(e.what());
    }

    Console::println("\n");
    Console::Pause('w');
}
