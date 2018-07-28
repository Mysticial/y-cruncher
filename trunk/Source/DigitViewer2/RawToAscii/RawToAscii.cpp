/* RawToAscii.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/14/2018
 * Last Modified    : 02/10/2018
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string.h>
#include "PublicLibs/BasicLibs/Alignment/AlignmentTools.h"
#include "PublicLibs/BasicLibs/Memory/TrivialArray.h"
#include "PublicLibs/BasicLibs/Concurrency/BlockSplitting.h"
#include "PublicLibs/SystemLibs/ProcessorCapability/ProcessorCapability.h"
#include "RawToAscii.h"
////////////////////////////////////////////////////////////////////////////////
#if 0
#elif defined YMP_Arch_2017_x64_Skylake
#include "Kernels/RawToAscii_x64_AVX512-BW.ipp"
#elif defined YMP_Arch_2013_x64_Haswell
#include "Kernels/RawToAscii_x64_AVX2.ipp"
#elif defined YMP_Arch_2004_x86_SSE3 || defined YMP_Arch_2004_x64
#include "Kernels/RawToAscii_x86_SSE2.ipp"
#elif defined YMP_Arch_2000_x86
#include "Kernels/RawToAscii_Default32.ipp"
#else
#include "Kernels/RawToAscii_Default.ipp"
#endif
////////////////////////////////////////////////////////////////////////////////
namespace DigitViewer2{
namespace RawToAscii{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class Action_Convert : public BasicAction{
    ConvertFunction m_fp_convert;
    char* m_output;
    const char* m_input;
    upL_t m_digits;

    bool* m_bad;
    upL_t m_unit_L;

public:
    Action_Convert(
        ConvertFunction fp_convert,
        char* output, const char* input, upL_t digits,
        bool* bad, upL_t unit_L
    )
        : m_fp_convert(fp_convert)
        , m_output(output), m_input(input), m_digits(digits)
        , m_bad(bad), m_unit_L(unit_L)
    {}

    virtual void run(upL_t index) override{
        upL_t si = m_unit_L * index;
        upL_t ei = si + m_unit_L;
        if (si >= m_digits){
            return;
        }
        if (ei > m_digits){
            ei = m_digits;
        }

        m_bad[index] = m_fp_convert(m_output + si, m_input + si, ei - si);
    }
};
bool parallel_convert(
    ConvertFunction convert,
    char* output, const char* input, upL_t digits,
    BasicParallelizer& parallelizer, upL_t tds
){
    //  Optimization: If "output" is misaligned, do enough to align it.
    upL_t align = Alignment::ptr_to_aligned<DEFAULT_ALIGNMENT>(output);
    if (align != 0 && align <= digits){
        convert(output, input, align);
        output += align;
        input  += align;
        digits -= align;
    }

    const upL_t THRESHOLD = 10000;

    if (digits < THRESHOLD || tds <= 1){
        return convert(output, input, digits);
    }

    upL_t unit_L = parallel_split_words_aligned<char, DEFAULT_ALIGNMENT>(
        digits, tds, THRESHOLD
    );

    TrivialArray<bool> bad(tds);
    memset(bad, 0, tds * sizeof(bool));

    Action_Convert action(convert, output, input, digits, bad, unit_L);
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
