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

#ifndef rtosim_AdaptiveCop_h
#define rtosim_AdaptiveCop_h

#include "rtosim/queue/MarkerSetQueue.h"
#include "rtosim/queue/MultipleExternalForcesQueue.h"
#include "rtb/concurrency/Concurrency.h"
#include "rtosim/ExternalLoadProperties.h"
#include <map>

namespace rtosim{

    //When the foot is not in contact with the force plate, the COP is
    //normally set to 0 by the input device. AdaptiveCop modify the value of the COP
    //when the foot is not in contact with the force plate, and sets it equal to the
    //projection of the marker defined in forcePlatesToMarkerNames on the force plate.
    //In this way, the COP follows the foot.
    //This helps to reduce the rise-time delay introduced
    //filtering the COP.
    class AdaptiveCop {

    public:
        AdaptiveCop(
            MarkerSetQueue& inputMarkerSetQueue,
            MultipleExternalForcesQueue& inputExternalForcesQueue,
            MultipleExternalForcesQueue& outputExternalForcesQueue,
            rtb::Concurrency::Latch& doneWithSubscriptions,
            rtb::Concurrency::Latch& doneWithExecution,
            const std::map<std::string, std::string>& forcePlatesToMarkerNames,
            const std::vector<std::string>& markerNames);

        //when using this constructor, the marker that is used to
        //adapt the COP corresponds to the most posterior marker
        //connected to the body defined in the applied_to_body tag in
        //externalLoadsFilename
        AdaptiveCop(
            MarkerSetQueue& inputMarkerSetQueue,
            MultipleExternalForcesQueue& inputExternalForcesQueue,
            MultipleExternalForcesQueue& outputExternalForcesQueue,
            rtb::Concurrency::Latch& doneWithSubscriptions,
            rtb::Concurrency::Latch& doneWithExecution,
            const std::string& osimModelFilename,
            const std::string& externalLoadsFilename);

        AdaptiveCop(
            MarkerSetQueue& inputMarkerSetQueue,
            MultipleExternalForcesQueue& inputExternalForcesQueue,
            MultipleExternalForcesQueue& outputExternalForcesQueue,
            rtb::Concurrency::Latch& doneWithSubscriptions,
            rtb::Concurrency::Latch& doneWithExecution,
            const std::string& osimModelFilename,
            const std::vector<ExternalForceProperties>& externalForcesProperties);

        void operator()();

    private:
        void updateForcePlatesToMarkerIndex(const std::vector<ExternalForceProperties>& externalForcesProperties, const std::string& osimModelFilename);
        void adaptCop(ExternalForceData& forceData, const MarkerData& markerData) const;
        MarkerSetQueue& inputMarkerSetQueue_;
        MultipleExternalForcesQueue& outputExternalForcesQueue_;
        MultipleExternalForcesQueue& inputExternalForcesQueue_;
        rtb::Concurrency::Latch& doneWithSubscriptions_;
        rtb::Concurrency::Latch& doneWithExecution_;
        std::map<std::string, unsigned> forcePlatesToMarkerIndex_;
    };
}

#endif