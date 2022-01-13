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

namespace rtosim {

    template<typename Qin, typename Qout, typename T>
    QueueAdapter<Qin, Qout, T>::QueueAdapter(
        Qin& qIn,
        Qout& qOut,
        rtb::Concurrency::Latch& doneWithSubscriptions,
        rtb::Concurrency::Latch& doneWithExecution,
        const T& adapter
        ) :
        qIn_(qIn),
        qOut_(qOut),
        doneWithSubscriptions_(doneWithSubscriptions),
        doneWithExecution_(doneWithExecution),
        adapter_(adapter) {}

    template<typename Qin, typename Qout, typename T>
    void QueueAdapter<Qin, Qout, T>::operator()() {

        bool localRunCondition(true);
        qIn_.subscribe();
        doneWithSubscriptions_.wait();
        while (localRunCondition) {
            auto frame(qIn_.pop());
            if (rtosim::EndOfData::isEod(frame)) {
                localRunCondition = false;
                qOut_.push(rtosim::EndOfData::get<typename Qout::type>());
            }
            else
                qOut_.push(adapter_(frame));
        }

        doneWithExecution_.wait();
    }
}