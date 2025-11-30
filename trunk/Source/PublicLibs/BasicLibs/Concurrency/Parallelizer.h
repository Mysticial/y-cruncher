/* Parallelizer.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 08/27/2014
 *  Last Modified   : 08/23/2024
 * 
 *  This is a basic framework for fork-join parallelism.
 * 
 *      ParallelAction is an action that takes an optional index parameter.
 *      Parallelizer is a interface for a parallelism framework.
 *      ParallelContext represents the current computing context.
 * 
 *  Be careful when running tasks that block on I/O or network. Some frameworks
 *  have a limited number of threads. So any threads that block may lead to
 *  idle CPU time.
 * 
 *  Work that is running inside a Parallelizer context should not touch or
 *  modify the thread priority or affinity. These may be controlled by the
 *  parallelizer implementation.
 * 
 * 
 *  ParallelContext consists of the Parallelizer that's being used as well as a
 *  pointer to parallelizer-specific information. This pointer may be used by
 *  the parallelizer implementation to track where the current task has been
 *  running in the topology.
 * 
 *  Users should not touch the internals of the ParallelContext object. Only
 *  parallelizers should do that. A null context pointer means the calling
 *  thread is not part of the ParallelContext and that it can assign it wherever
 *  it sees fit.
 * 
 * 
 *  Implementations of the Parallelizer interface are not allowed to deadlock
 *  unless there actually is a deadlock between the tasks. In other words,
 *  forward progress must always be made provided that at least one task is
 *  able to run.
 * 
 *  Implementations are required to pass through exceptions. If an exception is
 *  thrown by a ParallelAction inside a "run_in_parallel()" call, it must be either
 *  passed up or rethrown upon returning. If there are multiple exceptions, keep
 *  just one of them. It is not necessary to batch them all together.
 * 
 *  In all cases, the passing up of exceptions must be "clean". The class must
 *  not leak any resources or enter any sort of inconsistent state.
 * 
 */

#pragma once
#ifndef ymp_Concurrency_Parallelizer_H
#define ymp_Concurrency_Parallelizer_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "ParallelAction.h"
namespace ymp{
    class ParallelResourceAction;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class Parallelizer{
public:
    Parallelizer(const Parallelizer&) = delete;
    void operator=(const Parallelizer&) = delete;

    Parallelizer() = default;
    virtual ~Parallelizer() = default;

    friend class ParallelContext;


protected:
    //
    //      The functions below will run the specified actions in parallel
    //  assuming they are independent from each other. Furthermore, the
    //  parallelizer is expected to run them in parallel to the best of its
    //  ability given the environment.
    //
    //  While it is highly discouraged, actions with higher index are allowed
    //  to block (wait) on actions with a lower index. However, lower indices
    //  are not allowed to block on higher indices as that may lead to a deadlock.
    //
    //  The "tds" parameter is a hint that tells the parallelizer that the
    //  operation should run with that much parallelism. Exactly how the
    //  parallelizer uses this information is at its discretion.
    //
    //  The parallelizer may also use the "tds" parameter as a hint for core
    //  assignment.
    //

    //
    //  Run both actions using (index = 0) assuming independence.
    //
    //  While highly discouraged, "a1" is allowed to block on "a0".
    //  However, "a0" is not allowed to "a1". Doing so may deadlock.
    //
    //  Because this function only parallelizes two action, it can never
    //  achieve more than 2-way parallelism as is. The expected use-case is in
    //  a binary divide-and-conquer algorithm where it is called recursively
    //  until the desired parallelism is reached.
    //
    //  Even though this function never exceeds parallelism of two, it may still
    //  use the "tds" parameter as a hint for the purpose of topology
    //  scheduling. For example, small values of "tds" are likely to cause the
    //  actions to be scheduled on hardware that is close to each other (same
    //  NUMA node, same shared cache). Likewise, large values of "tds" may cause
    //  the two actions to be scheduled on hardware that is far apart under the
    //  expectation that the locality should be deferred to the lower-level calls.
    //
    virtual void run_in_parallel_2(
        ParallelContext& parallel_context, upL_t tds,
        ParallelAction& a0, ParallelAction& a1
    ) = 0;

    //
    //  Run the action for all indices in the range [si, ei) assuming independence.
    //
    //  While highly discouraged, higher indices are allowed to wait on lower indices.
    //  Lower indices are not allowed to wait on higher indices as that may deadlock.
    //
    //  Generally speaking, the amount of parallelism will not exceed "tds".
    //  If the # of tasks (ei - si) exceeds "tds", the parallelizer may batch
    //  them.
    //
    //  Starting from y-cruncher v0.8.6, it is safe to call this function with
    //  a large index range. Parallelizers are now required to batch them to
    //  avoid an explosion of threads.
    //
    //  In the case where (ei - si > tds), the parallelizer may choose to use
    //  dynamic scheduling at its discretion. But this doesn't mean you should
    //  abuse this and pass in a large number of tiny work units. Assume the
    //  overhead of each work unit to be as high as several context switches.
    //
    virtual void run_in_parallel_range(
        ParallelContext& parallel_context, upL_t tds,
        ParallelAction& action, upL_t si, upL_t ei
    ) = 0;


protected:
    //  Internal Only

    virtual void run_in_parallel_resource(
        ParallelContext& parallel_context, upL_t tds,
        ParallelResourceAction& action,
        upL_t r0, upL_t r1, upL_t r2, upL_t r3, upL_t r4 = 1
    );
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class ParallelContext{
public:
    //  The only thing that should be touching these fields are parallelizers.
    //  Users should not touch them. The only reason these are public is because
    //  we can't friend all the parallelizers.
    Parallelizer* parallelizer = nullptr;
    void* context = nullptr;


public:
    YM_FORCE_INLINE void run_in_parallel_2(ParallelAction& a0, ParallelAction& a1){
        parallelizer->run_in_parallel_2(*this, 2, a0, a1);
    }
    YM_FORCE_INLINE void run_in_parallel_2(upL_t tds, ParallelAction& a0, ParallelAction& a1){
        parallelizer->run_in_parallel_2(*this, tds, a0, a1);
    }

    YM_FORCE_INLINE void run_in_parallel_range(ParallelAction& action, upL_t si, upL_t ei){
        parallelizer->run_in_parallel_range(*this, ei - si, action, si, ei);
    }
    YM_FORCE_INLINE void run_in_parallel_range(upL_t tds, ParallelAction& action, upL_t si, upL_t ei){
        parallelizer->run_in_parallel_range(*this, tds, action, si, ei);
    }

    YM_FORCE_INLINE void run_in_parallel_resource(
        ParallelResourceAction& action,
        upL_t r0, upL_t r1, upL_t r2, upL_t r3, upL_t r4 = 1
    ){
        parallelizer->run_in_parallel_resource(*this, r3 - r2, action, r0, r1, r2, r3, r4);
    }
    YM_FORCE_INLINE void run_in_parallel_resource(
        upL_t tds,
        ParallelResourceAction& action,
        upL_t r0, upL_t r1, upL_t r2, upL_t r3, upL_t r4 = 1
    ){
        parallelizer->run_in_parallel_resource(*this, tds, action, r0, r1, r2, r3, r4);
    }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
