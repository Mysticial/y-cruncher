/* BasicDigitReader.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/14/2018
 * Last Modified    : 01/14/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_BasicDigitReader_H
#define ydv_DigitViewer_BasicDigitReader_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/BasicLibs/LinearHeapBuffer.h"
#include "PublicLibs/BasicLibs/Concurrency/BasicParallelizer.h"
#include "DigitViewer2/Globals.h"
#include "DigitViewer2/DigitCount/DigitCount.h"
#include "DigitViewer2/DigitHash/DigitHash.h"
namespace DigitViewer2{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class DigitStats{
public:
    DigitStats(char radix, hash_t hash = 0)
        : m_radix(radix)
        , m_counts(radix)
        , m_hash(hash)
    {}

    const DigitCounts& counts() const{
        return m_counts;
    }
    hash_t hash() const{
        return m_hash;
    }


public:
    void clear_hash(){
        m_hash = 0;
    }
    void scale_up_hash(uiL_t digits){
        m_hash *= hash_t(m_radix) ^ digits;
    }

    void operator+=(const DigitStats& x){
        m_counts += x.m_counts;
        m_hash += x.m_hash;
    }
    void operator+=(hash_t hash){
        m_hash += hash;
    }

    void accumulate(const char* raw_digits, upL_t digits){
        switch (m_radix){
        case 10:
            m_counts.accumulate(raw_digits, digits);
            m_hash = DigitHash::accumulate<10>(m_hash, raw_digits, digits);
            break;
        case 16:
            m_counts.accumulate(raw_digits, digits);
            m_hash = DigitHash::accumulate<16>(m_hash, raw_digits, digits);
            break;
        }
    }


private:
    char m_radix;
    DigitCounts m_counts;
    hash_t m_hash;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class BasicDigitReader{
public:
    static const upL_t MAX_RECOMMENDED_BUFFER_SIZE = (upL_t)1 << 30;

public:
    virtual ~BasicDigitReader() = default;

    char radix() const{ return m_radix; }
    virtual std::string first_digits() = 0;

    //  Returns one past the offset of the last digit in this stream.
    //  Zero means the stream is unbounded.
    virtual uiL_t stream_end() const = 0;

    //  Returns true if the digits [offset, offset + digits) are accessible.
    virtual bool range_is_available(uiL_t offset, uiL_t digits) = 0;

    //  Recommend a buffer size (in bytes) for "load_stats()" and "load_digits"
    //  that will maximize performance.
    virtual upL_t recommend_buffer_size(uiL_t digits, upL_t limit = MAX_RECOMMENDED_BUFFER_SIZE) const = 0;

    //  Flush local cache and close an excess handles associated with this reader.
    virtual void flush_cache(){};

    //  Implementations are required to make both of these load functions
    //  thread-safe even if there is no benefit to concurrent calls.

    //  These load functions are also not const since implementations may need
    //  to keep mutable state (file pointer, mutex, etc...).

    //  Accumulate stats for digits in [offset, offset + digits).
    //
    //  Let: new_digits = [offset, offset + digits)
    //
    //    - The digit counts of "new_digits" are added to "stats.counts".
    //    - "stats.hash" is updated as: new_hash = old_hash * radix^digits + hash(new_digits)
    //
    //  If an exception is thrown, the contents of "stats" is undefined.
    virtual void load_stats(
        DigitStats& stats,
        uiL_t offset, uiL_t digits,                     //  Range of digits to load and process.
        const AlignedBufferC<BUFFER_ALIGNMENT>& buffer, //  Scratch buffer.
        BasicParallelizer& parallelizer, upL_t tds      //  Parallelism
    ) = 0;

    //  Load the digits [offset, offset + digits) and optionally computes stats.
    //
    //  If "stats" is not null, it is updated the same way as "load_stats()".
    //
    //  If an exception is thrown, the contents of "output" and "stats" are undefined.
    virtual void load_digits(
        char* output,
        DigitStats* stats,                              //  If null, no stats are computed.
        uiL_t offset, upL_t digits,                     //  Range of digits to load and process.
        const AlignedBufferC<BUFFER_ALIGNMENT>& buffer, //  Scratch buffer.
        BasicParallelizer& parallelizer, upL_t tds      //  Parallelism
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
