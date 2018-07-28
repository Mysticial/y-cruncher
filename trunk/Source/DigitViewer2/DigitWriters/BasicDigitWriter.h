/* BasicDigitWriter.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/03/2018
 * Last Modified    : 02/03/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_BasicDigitWriter_H
#define ydv_DigitViewer_BasicDigitWriter_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <memory>
#include "PublicLibs/BasicLibs/Concurrency/BasicParallelizer.h"
#include "DigitViewer2/DigitReaders/BasicDigitReader.h"
namespace DigitViewer2{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class BasicDigitWriter{
public:
    //  Buffers need to be aligned to the disk I/O sector size since implementations
    //  will use raw I/O.
    static const upL_t BUFFER_ALIGNMENT = 4096;

    static const upL_t MAX_RECOMMENDED_BUFFER_SIZE = (upL_t)1 << 30;

public:
    virtual ~BasicDigitWriter() = default;

    char radix() const{ return m_radix; }

    //  Recommend a buffer size (in bytes) for "store_digits()" that will
    //  maximize performance.
    virtual upL_t recommend_buffer_size(uiL_t digits, upL_t limit = MAX_RECOMMENDED_BUFFER_SIZE) const = 0;

    virtual std::unique_ptr<BasicDigitReader> close_and_get_basic_reader() = 0;

    //  Stores the digits to [offset, offset + digits).
    //  Must be thread-safe even if there is no benefit to concurrent calls.
    virtual void store_digits(
        const char* input,
        uiL_t offset, upL_t digits,                 //  Range of digits to store.
        void* P, upL_t Pbytes,                      //  Scratch buffer. Must be aligned to DEFAULT_ALIGNMENT.
        BasicParallelizer& parallelizer, upL_t tds  //  Parallelism
    ) = 0;

protected:
    char m_radix;   //  Radix of the digits. (10 or 16)
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
