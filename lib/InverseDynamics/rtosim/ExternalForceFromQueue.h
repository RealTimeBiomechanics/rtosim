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

#ifndef rtosim_ExternalForceFromQueue_h
#define rtosim_ExternalForceFromQueue_h

#include "rtosim/queue/SingleExternalForceQueue.h"
#include "rtosim/ExternalForceProperties.h"
#include <OpenSim/OpenSim.h>
#include <Simbody.h>

namespace rtosim {

    //thic class reads the force data from a Queue rather than a storage
    //This can be connected to an OpenSim model
    class ExternalForceFromQueue : public OpenSim::ExternalForce{
    public:
        ExternalForceFromQueue() = delete;
        ExternalForceFromQueue(ExternalForceFromQueue&) = delete;
        ExternalForceFromQueue(
            SingleExternalForceQueue* externalForceQueue,
            const std::string& forceIdentifier,
            const std::string& pointIdentifier,
            const std::string& torqueIdentifier,
            const std::string& appliedToBodyName,
            const std::string& forceExpressedInBodyName,
            const std::string& pointExpressedInBodyName);
        ExternalForceFromQueue(
            SingleExternalForceQueue* externalForceQueue,
            SimTK::Xml::Element& aNode);
        ExternalForceFromQueue(
            SingleExternalForceQueue* externalForceQueue,
            const ExternalForceProperties& externalForceProperties);

        virtual ~ExternalForceFromQueue();

    protected:
        virtual void computeForce(const SimTK::State& state,
            SimTK::Vector_<SimTK::SpatialVec>& bodyForces,
            SimTK::Vector& generalizedForces) const override;
        void connectToModel(OpenSim::Model& model) override;

    private:
        ExternalForceData getExternalForceDataFromPastValues(double time) const;
        ExternalForceData getExternalForceDataFromQueue(double time) const;

        //need to use raw pointers because opensim deletes stuff for me.. and it's a mess
        bool isSubscribed;
        SingleExternalForceQueue* externalForceQueue_;
        mutable std::list<SingleExternalForceFrame> pastExternalForceFrames_;
        static const unsigned MaxStorageLength_ = 1000;
        mutable double lastTime_;
        SimTK::ReferencePtr<const OpenSim::Body> appliedToBody_;
        SimTK::ReferencePtr<const OpenSim::Body> forceExpressedInBody_;
        SimTK::ReferencePtr<const OpenSim::Body> pointExpressedInBody_;
        mutable bool runCondition_;
    };
}

#endif