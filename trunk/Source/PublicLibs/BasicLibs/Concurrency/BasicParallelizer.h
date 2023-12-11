/* BasicParallelizer.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 08/27/2014
 *  Last Modified   : 02/19/2017
 * 
 *  This is a basic framework for fork-join parallelism.
 * 
 *      BasicAction is an action that takes an optional index parameter.
 *      BasicParallelizer is a interface for a parallelism framework.
 * 
 *  Be careful when running tasks that block on I/O or network. Some frameworks
 *  have a limited number of threads. So any threads that block will be wasted
 *  CPU time.
 * 
 *  Implementations of the BasicParallelizer interface are not allowed to
 *  deadlock unless there actually is a deadlock between the tasks. In other
 *  words, forward progress must always be made provided that at least one task
 *  is able to run.
 * 
 *  Implementations are required to pass through exceptions. If an exception is
 *  thrown by a BasicAction inside a "run_in_parallel()" call, it must be either
 *  passed up or rethrown upon returning. If there are multiple exceptions, keep
 *  just one of them. It is not necessary to batch them all together.
 * 
 *  In all cases, the passing up of exceptions must be "clean". The class must
 *  not leak any resources or enter any sort of inconsistent state.
 * 
 */

#pragma once
#ifndef ymp_Concurrency_BasicParallelizer_H
#define ymp_Concurrency_BasicParallelizer_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "BasicAction.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class BasicParallelizer{
public:
    BasicParallelizer(const BasicParallelizer&) = delete;
    void operator=(const BasicParallelizer&) = delete;

    BasicParallelizer() = default;
    virtual ~BasicParallelizer() = default;

public:
    //  Run both actions using (index = 0) assuming independence.
    virtual void run_in_parallel(BasicAction& a0, BasicAction& a1) = 0;

    //  Run the action for all indices in the range [si, ei) assuming independence.
    //  Don't do this with a large number of tasks as some frameworks will
    //  spawn as many threads to run them all in parallel. If the tasks are
    //  small enough to require batching, do it prior to getting here.
    virtual void run_in_parallel(BasicAction& action, upL_t si, upL_t ei);

    //
    //  A Note on Synchronization:
    //
    //      The functions above will run the specified actions in parallel
    //  assuming they are independent from each other. Furthermore, the actions
    //  should not attempt to synchronize with each other or they may deadlock.
    //
    //  This is because there is no guarantee that the actions actually run
    //  concurrently. If action A attempts to wait for action B to trigger
    //  something, action A may wait forever since the framework may not even
    //  start running B until A finishes.
    //
    //
    //  Starting from y-cruncher v0.7.6, all frameworks will support the
    //  following exception to this "no synchronization" rule.
    //
    //      Actions with a higher index are allowed to block on actions with a
    //  lower index. For the two-action function, a1 is allowed to wait for a0.
    //  However, if a lower index attempts to block on a higher index, it may
    //  still lead to a deadlock.
    //
    //  This exception is a temporary API change for an unrelated experiment.
    //  It will probably go away again in the future.
    //

protected:
    //  Helper function for child classes that want to control the radix of the
    //  recursive fan-out.
    void run_in_parallel(upL_t max_sequential_dispatch, BasicAction& action, upL_t si, upL_t ei);
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Child Class Helpers
class BasicActionSplitter : public BasicAction{
    BasicParallelizer& m_parallelizer;
    BasicAction& m_action;
    upL_t m_si;
    upL_t m_ei;
    upL_t m_block;

public:
    BasicActionSplitter(
        BasicParallelizer& parallelizer,
        BasicAction& action, upL_t si, upL_t ei,
        upL_t block
    )
        : m_parallelizer(parallelizer)
        , m_action(action)
        , m_si(si), m_ei(ei)
        , m_block(block)
    {}

    virtual void run(upL_t index) override{
        upL_t si = m_si + index * m_block;
        upL_t ei = si + m_block;
        si = si < m_si ? m_si : si;
        ei = ei > m_ei ? m_ei : ei;
        if (si == ei){
            return;
        }
        m_parallelizer.run_in_parallel(m_action, si, ei);
    }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
