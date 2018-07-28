/* RawToCompressed.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/10/2018
 * Last Modified    : 02/10/2018
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string.h>
#include <algorithm>
#include "PublicLibs/BasicLibs/Alignment/AlignmentTools.h"
#include "PublicLibs/BasicLibs/Memory/TrivialArray.h"
#include "PublicLibs/BasicLibs/Concurrency/BlockSplitting.h"
#include "PublicLibs/SystemLibs/ProcessorCapability/ProcessorCapability.h"
#include "RawToCompressed.h"
////////////////////////////////////////////////////////////////////////////////
#if 0
#elif defined YMP_Arch_2017_x64_Skylake
#include "Kernels/i64ToRaw_x64_AVX512-BW.ipp"
#elif defined YMP_Arch_2013_x64_Haswell
#include "Kernels/i64ToRaw_x64_AVX2.ipp"
#elif defined YMP_Arch_2007_x64_Penryn
#include "Kernels/i64ToRaw_x64_SSE41.ipp"
#elif defined YMP_Arch_2004_x86_SSE3 || defined YMP_Arch_2004_x64
#include "Kernels/i64ToRaw_x86_SSE2.ipp"
#elif defined YMP_Arch_2000_x86
#include "Kernels/i64ToRaw_LittleEndian.ipp"
#else
#error "No generic implementation available."
#endif
////////////////////////////////////////////////////////////////////////////////
namespace DigitViewer2{
namespace RawToCompressed{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class Action_RawToI64 : public BasicAction{
    RawToI64Function m_convert;
    upL_t m_digits_per_word;

    u64_t* m_T;
    const char* m_raw;
    upL_t m_words;

    bool* m_bad;
    upL_t m_unit_L;

public:
    Action_RawToI64(
        RawToI64Function convert, upL_t digits_per_word,
        u64_t* T, const char* raw, upL_t words,
        bool* bad, upL_t unit_L
    )
        : m_convert(convert), m_digits_per_word(digits_per_word)
        , m_T(T), m_raw(raw), m_words(words)
        , m_bad(bad), m_unit_L(unit_L)
    {}

    virtual void run(upL_t index) override{
        upL_t si = m_unit_L * index;
        upL_t ei = si + m_unit_L;
        if (si >= m_words){
            return;
        }
        if (ei > m_words){
            ei = m_words;
        }

        m_bad[index] = m_convert(
            m_T + si,
            m_raw + si * m_digits_per_word,
            ei - si
        );
    }
};
bool raw_to_i64(
    RawToI64Function convert, upL_t digits_per_word,
    u64_t* T, const char* raw, upL_t words,
    BasicParallelizer& parallelizer, upL_t tds
){
    //  Optimization: If "output" is misaligned, do enough to align it.
    upL_t align = Alignment::ptr_to_aligned<DEFAULT_ALIGNMENT>(T);
    if (align != 0 && align <= words){
        if (convert(T, raw, align)){
            return true;
        }
        T     += align;
        raw   += align * digits_per_word;
        words -= align;
    }

    const upL_t THRESHOLD = 1000;

    if (words < THRESHOLD || tds <= 1){
        return convert(T, raw, words);
    }

    upL_t unit_L = parallel_split_words_aligned<u64_t, DEFAULT_ALIGNMENT>(
        words, tds, THRESHOLD
    );

    TrivialArray<bool> bad(tds);
    memset(bad, 0, tds * sizeof(bool));

    Action_RawToI64 action(convert, digits_per_word, T, raw, words, bad, unit_L);
    parallelizer.run_in_parallel(action, 0, tds);

    bool ret = false;
    for (upL_t c = 0; c < tds; c++){
        ret |= bad[c];
    }
    return ret;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
