/* ExceptionHolder.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 11/09/2021
 *  Last Modified   : 11/09/2021
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <memory>
#include <exception>
#include "PublicLibs/ConsoleIO/BasicIO.h"
//#include "PrivateLibs/DeprecatedLibs/ErrorHandling.h"
#include "ExceptionHolder.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void ExceptionHolder::throw_not_handled(){
    if (std::uncaught_exceptions() != 0){
//        cout << "already an exception" << endl;
        return;
    }
    Console::warning("Exceptions not handled!");
//    std::terminate();
    Console::quit_program(1);
//    ymo_error("Exceptions not handled!");
}
void ExceptionHolder::store_current_exception(){
    std::exception_ptr ptr = std::current_exception();
    if (ptr == nullptr){
        return;
    }
    try{
        set_exception(new std::exception_ptr(std::move(ptr)));
    }catch (...){
        Console::warning("Unable to store exception.");
        std::terminate();
    }
}
void ExceptionHolder::set_exception(void* exception){
    std::unique_ptr<std::exception_ptr> uptr((std::exception_ptr*)exception);
    void* expected = nullptr;
    if (m_pending_exception.compare_exchange_strong(expected, uptr.get())){
        uptr.release();
    }
}
void ExceptionHolder::free(void* pending_exception){
    delete (std::exception_ptr*)pending_exception;
}
void ExceptionHolder::rethrow(void* pending_exception){
    //  Only rethrow if we're not already handling an exception.
    if (std::uncaught_exceptions() == 0){
        std::rethrow_exception(*(std::exception_ptr*)pending_exception);
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
