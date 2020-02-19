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

#ifndef rtosim_QueueToFileLogger_h
#define rtosim_QueueToFileLogger_h

#include "rtb/concurrency/Latch.h"
#include "rtb/concurrency/Queue.h"
#include "rtosim/QueueData.h"
#include "rtosim/FileLogger.h"

namespace rtosim {

    template<typename DataType>
    class QueueToFileLogger {
    public:
        using FrameType = QueueData < DataType > ;
        using QueueType = rtb::Concurrency::Queue < FrameType > ;
        QueueToFileLogger() = delete;
        QueueToFileLogger(const QueueToFileLogger<DataType>&) = delete;
        QueueToFileLogger& operator=(const QueueToFileLogger<DataType>&) = delete;

        QueueToFileLogger(
            QueueType& inputQueue,
            rtb::Concurrency::Latch& subscriptionLatch,
            rtb::Concurrency::Latch& readyToWriteLatch,
            const std::vector<std::string>& columnLabels,
            std::string outputDir,
            std::string filename,
            std::string extension = "sto");
        ~QueueToFileLogger() = default;
        void operator()();
		void setIsInDegrees(bool isInDegrees) { logger_.setIsInDegrees(isInDegrees); }
		void setConvertFromRadToDeg() { logger_.setConvertFromRadToDeg(); }

    private:
        QueueType& inputQueue_;
        rtb::Concurrency::Latch& subscriptionLatch_;
        rtb::Concurrency::Latch& readyToWriteLatch_;
        FileLogger<DataType> logger_;
    };
}

#include "QueueToFileLogger.cpp"
#endif