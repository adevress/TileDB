/**
 * @file   tbb_state.cc
 *
 * @section LICENSE
 *
 * The MIT License
 *
 * @copyright Copyright (c) 2018 TileDB, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @section DESCRIPTION
 *
 This file declares the Intel TBB threading scheduler state, if TileDB is built
 with Intel TBB.
 */

#include "tiledb/sm/global_state/tbb_state.h"

#ifdef HAVE_TBB

#include <tbb/task_scheduler_init.h>
#include <sstream>

namespace tiledb {
namespace sm {
namespace global_state {

/** The TBB scheduler, used for controlling the number of TBB threads. */
static std::unique_ptr<tbb::task_scheduler_init> tbb_scheduler_;

/** The number of TBB threads the scheduler was configured with **/
static int tbb_nthreads_;

/**
 * Initialize the TBB thread runtime scheduler once per process with a given
 * number of threads
 *
 * @param nthreads Number of threads to use for TBB thread runtime scheduler
 */
Status init_tbb(int nthreads) {
  if (tbb_scheduler_) {
    if (nthreads != tbb_nthreads_) {
      // if the scheduler has been initialized and the number of scheduled
      // threads per process is different, error
      std::stringstream msg;
      msg << "TBB thread runtime must be initialized with the same number of "
             "threads per process: "
          << nthreads << " != " << tbb_nthreads_;
      return Status::Error(msg.str());
    }
  } else {
    // initialize scheduler in process for a custom number of threads (upon
    // first thread calling init_tbb)
    tbb_nthreads_ = nthreads;
    tbb_scheduler_ = std::unique_ptr<tbb::task_scheduler_init>(
        new tbb::task_scheduler_init(nthreads));
  }
  return Status::Ok();
}

}  // namespace global_state
}  // namespace sm
}  // namespace tiledb

#else

namespace tiledb {
namespace sm {
namespace global_state {

Status init_tbb(int nthreads) {
  return Status::OK();
}

}  // namespace global_state
}  // namespace sm
}  // namespace tiledb

#endif
