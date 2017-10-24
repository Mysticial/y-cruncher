/* rawdec_inverse_convert_AVX2.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/24/2016
 * Last Modified    : 01/24/2016
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include "rawdec_inverse_intrinsics_Default.h"
#include "rawdec_inverse_intrinsics_SSE2.h"
#include "rawdec_inverse_intrinsics_AVX2.h"
#include "rawdec.h"
namespace DigitViewer{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool dec_to_raw(char* T, upL_t L){
    char* stop = T + L;
    u64_t bad8 = 0;
    __m128i bad16 = _mm_setzero_si128();
    __m256i bad32 = _mm256_setzero_si256();

    //  Iterate until it is aligned.
    while (T < stop && (upL_t)T & 31){
        bad8 |= rawdec::convert_dec1_inverse_Default(T);
        T += 1;
    }

    //  Continue at full speed.
    while (T < stop - 63){
        bad32 = _mm256_or_si256(bad32, rawdec::convert_dec64_inverse_AVX2(T));
        T += 64;
    }
    while (T < stop - 31){
        bad32 = _mm256_or_si256(bad32, rawdec::convert_dec32_inverse_AVX2(T));
        T += 32;
    }
    while (T < stop - 15){
        bad16 = _mm_or_si128(bad16, rawdec::convert_dec16_inverse_SSE2(T));
        T += 16;
    }
    while (T < stop - 3){
        bad8 |= rawdec::convert_dec4_inverse_Default32(T);
        T += 4;
    }
    while (T < stop){
        bad8 |= rawdec::convert_dec1_inverse_Default(T);
        T += 1;
    }

    bad16 = _mm_or_si128(bad16, _mm256_castsi256_si128(bad32));
    bad16 = _mm_or_si128(bad16, _mm256_extracti128_si256(bad32, 1));
    bad8 |= _mm_cvtsi128_si64(bad16);
    bad8 |= _mm_extract_epi64(bad16, 1);

    return bad8 != 0;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
