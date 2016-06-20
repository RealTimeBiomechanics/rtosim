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

#include "rtosim/GeneralisedCoordinatesFromX.h"
#include "rtosim/GeneralisedCoordinatesData.h"
#include "rtosim/EndOfData.h"
#include <vector>
using std::vector;

namespace rtosim {

    GeneralisedCoordinatesFromX::GeneralisedCoordinatesFromX(
        GeneralisedCoordinatesQueue& outputGeneralisedCoordinatesQueue,
        Concurrency::Latch& doneWithSubscriptions,
        Concurrency::Latch& doneWithExecution) :
        outputGeneralisedCoordinatesQueue_(outputGeneralisedCoordinatesQueue),
        doneWithSubscriptions_(doneWithSubscriptions),
        doneWithExecution_(doneWithExecution)

    { }

    void GeneralisedCoordinatesFromX::updateGeneralisedCoordinates(const GeneralisedCoordinatesData& currentGeneralisedCoordinatesData, double currentTime) {

        GeneralisedCoordinatesFrame frame{ currentTime, currentGeneralisedCoordinatesData };
        outputGeneralisedCoordinatesQueue_.push(frame);
    }

    void GeneralisedCoordinatesFromX::doneWithSubscriptions() {

        doneWithSubscriptions_.wait();
    }

    void GeneralisedCoordinatesFromX::doneWithExecution() {

        doneWithExecution_.wait();
    }

    void GeneralisedCoordinatesFromX::sendEndOfData() {

        outputGeneralisedCoordinatesQueue_.push(EndOfData::get<GeneralisedCoordinatesFrame>());
    }
}
