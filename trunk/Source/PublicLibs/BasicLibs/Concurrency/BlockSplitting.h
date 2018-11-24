/* BasicParallelizer.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/17/2018
 * Last Modified    : 02/17/2018
 * 
 */

#pragma once
#ifndef ymp_Concurrency_BlockSplitting_H
#define ymp_Concurrency_BlockSplitting_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/BasicLibs/Alignment/AlignmentTools.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Given "total" work units, split it into no more than "tds" regions of
//  approximately equal size.
//    - "total" must be greater than 0.
//    - The return value is # of units per region.
//    - The return value will never be less than "threshold".
//    - "tds" may be decreased.
template <typename LengthType, typename ThresholdType>
LengthType parallel_split(LengthType total, upL_t& tds, ThresholdType threshold){
    static_assert(std::is_unsigned<LengthType>::value, "Length must be an unsigned integer.");
    static_assert(std::is_unsigned<ThresholdType>::value, "Threshold must be an unsigned integer.");

    LengthType desired_block_size = (total - 1) / tds + 1;
    LengthType corrected_block_size = desired_block_size;

    //  Make sure the block size is no smaller than the threshold.
    corrected_block_size = corrected_block_size < threshold
        ? threshold
        : corrected_block_size;

    tds = (upL_t)((total - 1) / corrected_block_size + 1);

    return corrected_block_size;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Given an array words of type "WordType" and a length of "total", split it
//  into no more than "tds" regions of approximately equal size.
//    - "total" must be greater than 0.
//    - The return value is # of words per region.
//    - The return value will never be less than "threshold".
//    - The return value will be aligned to the specified byte_alignment.
//    - "tds" may be decreased.
template <typename WordType, upL_t byte_alignment, typename LengthType, typename ThresholdType>
LengthType parallel_split_words_aligned(LengthType total, upL_t& tds, ThresholdType threshold){
    static_assert(std::is_unsigned<LengthType>::value, "Length must be an unsigned integer.");
    static_assert(std::is_unsigned<ThresholdType>::value, "Threshold must be an unsigned integer.");

    LengthType desired_block_size = (total - 1) / tds + 1;
    LengthType corrected_block_size = desired_block_size;

    //  Make sure the block size is no smaller than the threshold.
    corrected_block_size = corrected_block_size < threshold
        ? threshold
        : corrected_block_size;

    //  Align the block size.
    corrected_block_size = Alignment::align_length_up<WordType, byte_alignment>(corrected_block_size);

//    //  If anything has changed, recalculate the task decomposition.
//    if (corrected_block_size != desired_block_size){
//        tds = (upL_t)((total - 1) / corrected_block_size + 1);
//    }
    tds = (upL_t)((total - 1) / corrected_block_size + 1);

    return corrected_block_size;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Similar to above, but for arbitrary (non-power-of-two) and run-time alignment.
template <typename LengthType, typename ThresholdType>
LengthType parallel_split_aligned(LengthType total, upL_t& tds, LengthType alignment, ThresholdType threshold){
    static_assert(std::is_unsigned<LengthType>::value, "Length must be an unsigned integer.");
    static_assert(std::is_unsigned<ThresholdType>::value, "Threshold must be an unsigned integer.");

    LengthType desired_block_size = (total - 1) / tds + 1;
    LengthType corrected_block_size = desired_block_size;

    //  Make sure the block size is no smaller than the threshold.
    corrected_block_size = corrected_block_size < threshold
        ? threshold
        : corrected_block_size;

    //  Align the block size.
    {
        corrected_block_size += alignment - 1;
        corrected_block_size /= alignment;
        corrected_block_size *= alignment;
    }

//    //  If anything has changed, recalculate the task decomposition.
//    if (corrected_block_size != desired_block_size){
//        tds = (upL_t)((total - 1) / corrected_block_size + 1);
//    }
    tds = (upL_t)((total - 1) / corrected_block_size + 1);

    return corrected_block_size;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
