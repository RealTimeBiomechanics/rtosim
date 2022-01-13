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

#include "rtosim/EndOfData.h"

namespace rtosim {

    template <typename T>
    FrameCounter<T>::FrameCounter(T& queueIn, const std::string& name) :
        queueIn_(queueIn),
        stopWatch_(name){}

    template <typename T>
    StopWatch FrameCounter<T>::getProcessingTimes() const {

        return stopWatch_;
    }

    template <typename T>
    void FrameCounter<T>::operator()() {

        bool localRunCondition(true);
        queueIn_.subscribe();
        //start the timer after popping the first item from the queue
        queueIn_.pop();
        stopWatch_.init();
        while (localRunCondition) {
            if (rtosim::EndOfData::isEod(queueIn_.pop()))
                localRunCondition = false;
            stopWatch_.log();
        }
    }
}