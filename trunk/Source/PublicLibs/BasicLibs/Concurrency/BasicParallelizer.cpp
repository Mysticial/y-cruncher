/* BasicParallelizer.cpp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 08/27/2014
 *  Last Modified   : 07/22/2023
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <algorithm>
#include "BasicParallelizer.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class BasicActionWrapperB : public BasicAction{
    BasicParallelizer& m_parallelizer;
    BasicAction& m_action;
    upL_t m_si;
    upL_t m_ei;

public:
    BasicActionWrapperB(BasicParallelizer& parallelizer, BasicAction& action, upL_t si, upL_t ei)
        : m_parallelizer(parallelizer)
        , m_action(action)
        , m_si(si), m_ei(ei)
    {}

private:
    virtual void run(upL_t) override{
        m_parallelizer.run_in_parallel(m_action, m_si, m_ei);
    }
};
void BasicParallelizer::run_in_parallel(BasicAction& action, upL_t si, upL_t ei){
    if (ei <= si){
        return;
    }
    if (ei - si == 1){
        action.run(si);
        return;
    }

    upL_t mi = si + (ei - si) / 2;

    BasicActionWrapperB a0(*this, action, si, mi);
    BasicActionWrapperB a1(*this, action, mi, ei);
    run_in_parallel(a0, a1);
}
void BasicParallelizer::run_in_parallel(upL_t max_sequential_dispatch, BasicAction& action, upL_t si, upL_t ei){
    //  Make sure it's at least 4 or it's inefficient.
    max_sequential_dispatch = std::max(max_sequential_dispatch, (upL_t)4);

    upL_t num_tasks = ei - si;

    //  Fits within sequential dispatch limit.
    if (num_tasks <= max_sequential_dispatch){
        run_in_parallel(action, si, ei);
        return;
    }

    //  Recursively split it up.

    upL_t block_size = (num_tasks - 1) / max_sequential_dispatch + 1;
    upL_t block_count = max_sequential_dispatch;

    BasicActionSplitter splitter(*this, action, si, ei, block_size);
    run_in_parallel(max_sequential_dispatch, splitter, 0, block_count);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
