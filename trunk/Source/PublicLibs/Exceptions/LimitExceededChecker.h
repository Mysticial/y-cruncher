/* LimitExceededChecker.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 02/18/2019
 *  Last Modified   : 02/18/2019
 * 
 *      Lightweight header. Use this if you only need to throw and you don't
 *  want to pull in all the headers that "Exception.h" pulls in.
 * 
 */

#pragma once
#ifndef ymp_Exceptions_LimitExceededChecker_H
#define ymp_Exceptions_LimitExceededChecker_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
[[noreturn]] YM_NO_INLINE void throw_LimitExceededException(const char* function, uiL_t limit, uiL_t attempted);
[[noreturn]] YM_NO_INLINE void throw_LimitExceededException(const char* function, const char* message, uiL_t limit, uiL_t attempted);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void check_LimitExceededException(const char* function, uiL_t limit, uiL_t attempted){
    if (attempted > limit){
        throw_LimitExceededException(function, limit, attempted);
    }
}
YM_FORCE_INLINE void check_LimitExceededException(const char* function, const char* message, uiL_t limit, uiL_t attempted){
    if (attempted > limit){
        throw_LimitExceededException(function, message, limit, attempted);
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif