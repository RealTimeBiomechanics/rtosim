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
#include <iostream>
#include <vector>
#include <SimTKcommon.h>

namespace rtosim{

    template <typename DataType>
    QueueToFileLogger<DataType>::QueueToFileLogger(
        QueueType& inputQueue,
        rtb::Concurrency::Latch& subscriptionLatch,
        rtb::Concurrency::Latch& readyToWriteLatch,
        const std::vector<std::string>& columnLabels,
        const std::string& outputDir,
        const std::string& filename,
        const std::string& extension) :
        inputQueue_(inputQueue),
        subscriptionLatch_(subscriptionLatch),
        readyToWriteLatch_(readyToWriteLatch),
        logger_(columnLabels, filename + "." + extension, outputDir)
    {

        if (!logger_.printable()) {
            std::cout << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    template <typename DataType>
    void QueueToFileLogger<DataType>::operator()() {

        inputQueue_.subscribe();
        subscriptionLatch_.wait();
        readyToWriteLatch_.wait();

        bool runCondition(true);
        while (runCondition) {
            FrameType currentFrame = inputQueue_.pop();
            if (EndOfData::isEod(currentFrame)) runCondition = false;
            if (runCondition) {
                logger_.logFrame(currentFrame);
            }
        }

        logger_.print();
    }
}