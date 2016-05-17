/* -------------------------------------------------------------------------- *
* CEINMS is a standalone toolbox for neuromusculoskeletal modelling and      *
* simulation. CEINMS can also be used as a plugin for OpenSim either         *
* through the OpenSim GUI or API. See https://simtk.org/home/ceinms and the  *
* NOTICE file for more information. CEINMS development was coordinated       *
* through Griffith University and supported by the Australian National       *
* Health and Medical Research Council (NHMRC), the US National Institutes of *
* Health (NIH), and the European Union Framework Programme 7 (EU FP7). Also  *
* see the PROJECTS file for more information about the funding projects.     *
*                                                                            *
* Copyright (c) 2010-2015 Griffith University and the Contributors           *
*                                                                            *
* CEINMS Contributors: C. Pizzolato, M. Reggiani, M. Sartori,                *
*                      E. Ceseracciu, and D.G. Lloyd                         *
*                                                                            *
* Author(s): E. Ceseracciu, C. Pizzolato, M. Reggiani                        *
*                                                                            *
* CEINMS is licensed under the Apache License, Version 2.0 (the "License").  *
* You may not use this file except in compliance with the License. You may   *
* obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0.*
*                                                                            *
* Unless required by applicable law or agreed to in writing, software        *
* distributed under the License is distributed on an "AS IS" BASIS,          *
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
* See the License for the specific language governing permissions and        *
* limitations under the License.                                             *
* -------------------------------------------------------------------------- */

#ifndef rtosim_QueueToFileLogger_h
#define rtosim_QueueToFileLogger_h

#include "rtosim/concurrency/Latch.h"
#include "rtosim/concurrency/Queue.h"
#include "rtosim/QueueData.h"
#include "rtosim/FileLogger.h"

namespace rtosim {

    template<typename DataType>
    class QueueToFileLogger {
    public:
        using FrameType = QueueData < DataType > ;
        using QueueType = rtosim::Concurrency::Queue < FrameType > ;
        QueueToFileLogger() = delete;
        QueueToFileLogger(const QueueToFileLogger&) = delete;
        QueueToFileLogger& operator=(const QueueToFileLogger&) = delete;

        QueueToFileLogger(
            QueueType& inputQueue,
            rtosim::Concurrency::Latch& subscriptionLatch,
            rtosim::Concurrency::Latch& readyToWriteLatch,
            const std::vector<std::string>& columnLabels,
            const std::string& outputDir,
            const std::string& filename,
            const std::string& extension = "sto");
        ~QueueToFileLogger() = default;
        void operator()();

    private:
        QueueType& inputQueue_;
        rtosim::Concurrency::Latch& subscriptionLatch_;
        rtosim::Concurrency::Latch& readyToWriteLatch_;
        FileLogger<DataType> logger_;
    };
}

#include "QueueToFileLogger.cpp"
#endif