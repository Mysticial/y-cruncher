/* MemoryFence.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 11/11/2016
 *  Last Modified   : 05/17/2023
 * 
 *      In this project, we regularly violate memory ordering with non-temporal
 *  stores and similar stuff without issuing sfence for the sake of performance.
 * 
 *  So unlike normal C++ code, we actually violate the C++ virtual machine's
 *  memory model. C++ on x86 assumes that you will not break x86's implicit
 *  acquire-release semantics for normal load/stores. Therefore std::atomic
 *  store-release is not sufficient as a store-release and must be preceded by
 *  an sfence.
 * 
 *  To fix this, we must issue an "sfence" on all operations where "bulk" data
 *  may cross thread boundaries.
 * 
 *  This file provides various fencing functions to be called before each type
 *  of C++ synchronization mechanism. If the C++ mechanism is sufficient to
 *  correct the problem, then the function does nothing. Otherwise, it will be
 *  an sfence.
 * 
 *  Since nearly all concurrency is handled by the parallel frameworks, it is
 *  in those frameworks where this is done.
 * 
 *  Therefore in all frameworks, we call a fencing function:
 * 
 *      1.  Prior to forking a task to a different thread where the new thread
 *          will read bulk data from the current thread.
 * 
 *      2.  At the end of a task just before it is joined if it created any
 *          bulk data. (basically when joining a task with unfenced NT-stores)
 * 
 *      3.  Calling into any external API that may do the above.
 * 
 * 
 *  Examples:
 * 
 *      WaitableBasicAction uses a lockless mechanism to notify listeners that it
 *  is done. Without sfence prior to notifying listeners, it is possible for
 *  unfinished non-temporal stores to "leak" to the threads reading the data.
 * 
 *      External parallel frameworks like TBB and Cilk are blackboxes. Since we
 *  do not know what they use underneath, we must assume worst case and issue
 *  sfence prior to forking and joining.
 * 
 *      Disk I/O operations are blackboxes. It is possible for such a call to be
 *  immediately tossed to a different thread or DMA'ed. Therefore, we must issue
 *  sfence prior to calling any external I/O function that reads bulk data from
 *  buffers that we give it.
 * 
 * 
 *  What is "Bulk Data"?
 * 
 *      "Bulk Data" is defined as any piece of memory with unknown origin that
 *  may have been lasted touched by unfenced NT-stores. Since this concept is
 *  new, pointers and function parameters that are classified as bulk data are
 *  not marked in the codebase. But any large piece of memory that contains
 *  computational data (such bignums or FFT transform data) are considered bulk.
 * 
 */

#pragma once
#ifndef ymp_Intrinsics_MemoryFence_H
#define ymp_Intrinsics_MemoryFence_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/ArchSpecificLibs/LoadStoreTypes.h"
#include "PublicLibs/SystemLibs/ProcessorCapability/ProcessorCapability.h"
#ifdef YMP_Arch_2001_x86_SSE2
#include <xmmintrin.h>
#endif
namespace ymp{
namespace Intrinsics{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Skip sfence calls that normally follow non-temporal stores.
//  This should be on to maximize performance. But if you suspect that unfenced
//  NT-stores are leaking to different threads, you can disable this option to
//  force all NT-store usage back to spec. Useful for debugging.
#define YMP_X86_SKIP_LOCAL_SFENCE
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef YMP_Arch_2001_x86_SSE2
YM_FORCE_INLINE void sfence(){
    _mm_sfence();
}
template <StoreType type>
YM_FORCE_INLINE void sfence(){
    if constexpr (type == StoreType::NON_TEMPORAL){
        _mm_sfence();
    }
}
YM_FORCE_INLINE void sfence_store_release(){
    _mm_sfence();
}
YM_FORCE_INLINE void sfence_cmpxchg(){
    //  All x86 lock-prefix instructions are full memory barriers.
}
YM_FORCE_INLINE void sfence_mutex_lock(){
    //  All lock acquisitions require a lock-prefix instruction at some point.
}
#else
//  Nothing to do since we don't violate memory ordering outside of x86 SSE.
YM_FORCE_INLINE void sfence(){}
template <StoreType type>
YM_FORCE_INLINE void sfence(){}
YM_FORCE_INLINE void sfence_store_release(){}
YM_FORCE_INLINE void sfence_cmpxchg(){}
YM_FORCE_INLINE void sfence_mutex_lock(){}
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
