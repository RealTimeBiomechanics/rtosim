/* -------------------------------------------------------------------------- *
 * Copyright (c) 2010-2016 C. Pizzolato, M. Reggiani                          *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License");            *
 * you may not use this file except in compliance with the License.           *
 * You may obtain a copy of the License at:                                   *
 * http://www.apache.org/licenses/LICENSE-2.0                                 *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

#ifndef rtosim_QueueAdapter_h
#define rtosim_QueueAdapter_h

#include "rtosim/concurrency/Queue.h"
#include "rtosim/concurrency/Latch.h"
#include "rtosim/EndOfData.h"

namespace rtosim {

    //todo: get Qin and Qout types from T
    template<typename Qin, typename Qout, typename T>
    class QueueAdapter {

    public:
        QueueAdapter() = delete;
        QueueAdapter(QueueAdapter&) = delete;
        QueueAdapter(
            Qin& qIn,
            Qout& qOut,
            rtosim::Concurrency::Latch& doneWithSubscriptions,
            rtosim::Concurrency::Latch& doneWithExecution,
            const T& adapter
            );
        void operator()();
        //debug
        T getAdapter() const { return adapter_; }
    private:
        Qin& qIn_;
        Qout& qOut_;
        rtosim::Concurrency::Latch& doneWithSubscriptions_;
        rtosim::Concurrency::Latch& doneWithExecution_;
        T adapter_;
    };
}

#include "QueueAdapter.cpp"
#endif
