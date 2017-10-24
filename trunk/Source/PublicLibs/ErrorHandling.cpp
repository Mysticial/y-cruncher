/* ErrorHandling.cpp - Global Error Handling
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 09/28/2009
 * Last Modified    : 09/28/2009
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "ErrorHandling.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const wchar_t ymo_str_Assertion_Failure[]                   = L"FATAL ERROR: Assertion Failure";
const wchar_t ymo_str_Assertion_Failure_Description[]       = L"If this error shows up consistantly with the current options,\n"
                                                              L"please report it as a bug.\n"
                                                              L"\n"
                                                              L"If this error comes and goes with the current options,\n"
                                                              L"please check your hardware for reliability issues.\n"
                                                              L"Are you overclocking?\n"
                                                              L"\n"
                                                              L"Terminating Program...\n";
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Assertions
void ym_assertion_failure(const char* type, const char* name, const char* msg){
    Console::SetColor('R');
    Console::print("\n\n");
    Console::print(ymo_str_Assertion_Failure);
    Console::print("\n\n");
    Console::print(type); Console::print(name); Console::print("\n\n");
    Console::print(msg);
    Console::print("\n\n\n");
    Console::print(ymo_str_Assertion_Failure_Description);
    Console::SetColor('w');
    Console::Pause();
    int *p = NULL;
    Console::Quit(*p);
}
void ym_exceeded_limit(const char* type, const char* name, const char* msg){
    Console::print("\n\n");
    Console::print("FATAL ERROR: Limit Exceeded");
    Console::print("\n\n");
    Console::print(type); Console::print(name); Console::print("\n\n");
    Console::print(msg);
    Console::print("\n");
    Console::Pause();
    int *p = NULL;
    Console::Quit(*p);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Errors
[[noreturn]] YM_NO_INLINE void ymo_error(const char* msg){
    Console::println("\n");
    Console::println(msg, 'R');
    Console::println("\n", 'w');
#ifdef YCC_ENABLE_CRASH_DEBUGGING
    Console::Pause();
    int *p = nullptr;
    *p = 0;
#endif
    Console::Quit(1);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Error Correction
volatile uiL_t yms_fixed_errors = 0;    //  -1 will terminate program on error
void ymo_retry_success(upL_t retries){
    Console::SetColor('R');
    Console::print("\n");
    Console::print("Succeeded after ");
    Console::print(retries);
    Console::print(" re-attempt(s). Please check your hardware for stability.");
    Console::print("\n\n");
    Console::SetColor('w');
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
