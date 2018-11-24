/* AlignedMalloc.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 09/06/2016
 * Last Modified    : 09/23/2016
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string.h>
#include <stdlib.h>
#include <new>
#include "PublicLibs/Exceptions/InvalidParametersException.h"
#include "PublicLibs/Exceptions/AlgorithmFailedException.h"
#include "AlignedMalloc.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define YMP_ENABLE_MALLOC_CHECKING
//#define YMP_ZERO_INITIALIZE
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#if YMP_PTR_MAG == 5
const upL_t BUFFER_CHECK_BOT = 0x4bd0187d;
const upL_t BUFFER_CHECK_TOP = 0x0559cfe4;
#else
const upL_t BUFFER_CHECK_BOT = 0xea097be64bd0187d;
const upL_t BUFFER_CHECK_TOP = 0xc72bf73f0559cfe4;
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void* aligned_malloc(upL_t bytes, upL_t align){
    if (align < sizeof(upL_t)){
        align = sizeof(upL_t);
    }
#ifdef YMP_ENABLE_MALLOC_CHECKING
    if ((align & (align - 1)) != 0){
        throw InvalidParametersException("aligned_malloc()", "Alignment must be a power of two.");
    }
#endif

    upL_t actual_bytes = bytes + align + sizeof(upL_t)*4;
    void *free_ptr = malloc(actual_bytes);
    if (free_ptr == nullptr){
        throw std::bad_alloc();
//        return nullptr;
    }
#ifdef YMP_ZERO_INITIALIZE
    memset(free_ptr, 0, actual_bytes);
#endif
    upL_t free_address = (upL_t)free_ptr;
    upL_t min_ret_address = free_address + sizeof(upL_t)*3;

    //  Align
    upL_t ret_address = min_ret_address;
    ret_address &= ~(upL_t)(align - 1);
    ret_address += align;

    upL_t *ret = (upL_t*)ret_address;
    ret[-3] = free_address;

#ifdef YMP_ENABLE_MALLOC_CHECKING
    ret[-2] = bytes;
    ret[-1] = BUFFER_CHECK_BOT;
    memcpy((char*)ret + bytes, &BUFFER_CHECK_TOP, sizeof(upL_t));
#endif

    return ret;
}
void aligned_free(void *ptr){
    if (ptr == nullptr){
        return;
    }

    upL_t* ret = (upL_t*)ptr;
    upL_t free_int = ret[-3];

#ifdef YMP_ENABLE_MALLOC_CHECKING
    upL_t bytes = ret[-2];
    upL_t check = ret[-1];
    if (check != BUFFER_CHECK_BOT){
        throw AlgorithmFailedException("aligned_free()", "Memory buffer has been underrun.");
    }
    memcpy(&check, (char*)ptr + bytes, sizeof(upL_t));
    if (check != BUFFER_CHECK_TOP){
        throw AlgorithmFailedException("aligned_free()", "Memory buffer has been overrun.");
    }
#endif

    ptr = (void*)free_int;
    free(ptr);
}
void check_aligned_ptr(const void *ptr){
    if (ptr == nullptr){
        return;
    }

    const upL_t* ret = (const upL_t*)ptr;
//    upL_t free_int = ret[-3];

    upL_t bytes = ret[-2];
    upL_t check = ret[-1];
    if (check != BUFFER_CHECK_BOT){
        throw AlgorithmFailedException("aligned_free()", "Memory buffer has been underrun.");
    }
    memcpy(&check, (const char*)ptr + bytes, sizeof(upL_t));
    if (check != BUFFER_CHECK_TOP){
        throw AlgorithmFailedException("aligned_free()", "Memory buffer has been overrun.");
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
