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

#include "rtosim/AdaptiveCop.h"
#include "rtosim/QueuesSync.h"
#include "rtosim/EndOfData.h"
#include "rtosim/OsimUtilities.h"
#include <OpenSim/OpenSim.h>
#include <tuple>
#include <string>
#include <iostream>
using std::string;

namespace rtosim {

    AdaptiveCop::AdaptiveCop(
        MarkerSetQueue& inputMarkerSetQueue,
        MultipleExternalForcesQueue& inputExternalForcesQueue,
        MultipleExternalForcesQueue& outputExternalForcesQueue,
        rtb::Concurrency::Latch& doneWithSubscriptions,
        rtb::Concurrency::Latch& doneWithExecution,
        const std::map<std::string, std::string>& forcePlatesToMarkerNames,
        const std::vector<std::string>& markerNames) :
        inputMarkerSetQueue_(inputMarkerSetQueue),
        inputExternalForcesQueue_(inputExternalForcesQueue),
        outputExternalForcesQueue_(outputExternalForcesQueue),
        doneWithSubscriptions_(doneWithSubscriptions),
        doneWithExecution_(doneWithExecution) {

        for (auto& it : forcePlatesToMarkerNames) {
            auto nIt(std::find(markerNames.begin(), markerNames.end(), it.second));
			if (nIt != markerNames.end()) {
				unsigned dist = std::distance(markerNames.begin(), nIt);
				forcePlatesToMarkerIndex_.insert(std::make_pair(it.first, dist));
			}
        }
    }

    AdaptiveCop::AdaptiveCop(
        MarkerSetQueue& inputMarkerSetQueue,
        MultipleExternalForcesQueue& inputExternalForcesQueue,
        MultipleExternalForcesQueue& outputExternalForcesQueue,
        rtb::Concurrency::Latch& doneWithSubscriptions,
        rtb::Concurrency::Latch& doneWithExecution,
        const std::string& osimModelFilename,
        const std::string& externalLoadsFilename) :
        inputMarkerSetQueue_(inputMarkerSetQueue),
        inputExternalForcesQueue_(inputExternalForcesQueue),
        outputExternalForcesQueue_(outputExternalForcesQueue),
        doneWithSubscriptions_(doneWithSubscriptions),
        doneWithExecution_(doneWithExecution)
    {
        ExternalLoadProperties extLoadProperties(externalLoadsFilename);
        updateForcePlatesToMarkerIndex(extLoadProperties.getExternalForces(), osimModelFilename);
    }

    AdaptiveCop::AdaptiveCop(
        MarkerSetQueue& inputMarkerSetQueue,
        MultipleExternalForcesQueue& inputExternalForcesQueue,
        MultipleExternalForcesQueue& outputExternalForcesQueue,
        rtb::Concurrency::Latch& doneWithSubscriptions,
        rtb::Concurrency::Latch& doneWithExecution,
        const std::string& osimModelFilename,
        const std::vector<ExternalForceProperties>& externalForcesProperties) :
        inputMarkerSetQueue_(inputMarkerSetQueue),
        inputExternalForcesQueue_(inputExternalForcesQueue),
        outputExternalForcesQueue_(outputExternalForcesQueue),
        doneWithSubscriptions_(doneWithSubscriptions),
        doneWithExecution_(doneWithExecution) {

        updateForcePlatesToMarkerIndex(externalForcesProperties, osimModelFilename);
    }

    void AdaptiveCop::updateForcePlatesToMarkerIndex(
        const std::vector<ExternalForceProperties>& externalForcesProperties,
        const std::string& osimModelFilename) {

        OpenSim::Model model(osimModelFilename);
        model.initSystem();

        auto markerSet(model.getMarkerSet());
        for (auto& forceSetup : externalForcesProperties) {

            string bodyName(forceSetup.getAppliedToBody());
            auto markersOnBody = getMarkerNamesOnBody(model.getBodySet().get(bodyName));
            auto postMarker(getMostPosteriorMarker(markersOnBody, model));
            forcePlatesToMarkerIndex_.insert(std::make_pair(forceSetup.getName(), model.getMarkerSet().getIndex(postMarker)));
            std::cout << "Pairing " << forceSetup.getName() << " to " << postMarker << std::endl;
        }
    }

    void  AdaptiveCop::operator()() {

        inputMarkerSetQueue_.subscribe();
        inputExternalForcesQueue_.subscribe();

        bool runCondition(true);
        doneWithSubscriptions_.wait();

        while (runCondition) {

            auto frames(QueuesSync::sync(inputMarkerSetQueue_, inputExternalForcesQueue_));
            std::optional<MultipleExternalForcesFrame> extForcesFrame_opt;
            std::optional<MarkerSetFrame> markerSetFrame_opt;
            std::tie(markerSetFrame_opt, extForcesFrame_opt) = frames;
            // frames is a tuple of two optional types

            MultipleExternalForcesFrame extForcesFrame = extForcesFrame_opt.value();
            MarkerSetFrame markerSetFrame = markerSetFrame_opt.value();

            if (EndOfData::isEod(markerSetFrame) ||
                EndOfData::isEod(extForcesFrame))
                runCondition = false;

            if (runCondition) {
                for (unsigned i(0); i < extForcesFrame.data.size(); ++i) {
                    string sourceName(extForcesFrame.data.at(i).getSourceName());
                    auto marker(markerSetFrame.data.at(forcePlatesToMarkerIndex_.at(sourceName)));
                    adaptCop(extForcesFrame.data.at(i), marker);
                }

                outputExternalForcesQueue_.push(extForcesFrame);
            }
            else
                outputExternalForcesQueue_.push(EndOfData::get<MultipleExternalForcesFrame>());
        }

        doneWithExecution_.wait();
    }

    void AdaptiveCop::adaptCop(ExternalForceData& forceData, const MarkerData& marker) const {

        if (!marker.getOccluded()) {
            auto cop(forceData.getApplicationPoint());
            auto markerCoords(marker.getCoordinates());
            if (forceData.getForce()[1] < 20) {
                cop[0] = markerCoords[0];
                cop[2] = markerCoords[2];
            }
            forceData.setApplicationPoint(cop);
        }
    }
}