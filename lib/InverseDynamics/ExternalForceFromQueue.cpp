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

#include "rtosim/ExternalForceFromQueue.h"
#include "rtosim/queue/SingleExternalForceQueue.h"
#include "rtosim/EndOfData.h"
#include <string>
using std::string;

namespace rtosim {

    ExternalForceFromQueue::ExternalForceFromQueue
        (SingleExternalForceQueue* externalForceQueue,
        const string& forceIdentifier,
        const string& pointIdentifier,
        const string& torqueIdentifier,
        const string& appliedToBodyName,
        const string& forceExpressedInBodyName,
        const string& pointExpressedInBodyName) :
        externalForceQueue_(externalForceQueue),
        appliedToBody_(nullptr),
        forceExpressedInBody_(nullptr),
        pointExpressedInBody_(nullptr),
        lastTime_(std::numeric_limits<double>::lowest()),
        isSubscribed(false),
        runCondition_(true)
    {
        setForceIdentifier(forceIdentifier);
        setPointIdentifier(pointIdentifier);
        setTorqueIdentifier(torqueIdentifier);
        setAppliedToBodyName(appliedToBodyName);
        setForceExpressedInBodyName(forceExpressedInBodyName);
        setPointExpressedInBodyName(pointExpressedInBodyName);
    }

    ExternalForceFromQueue::ExternalForceFromQueue(SingleExternalForceQueue* externalForceQueue, SimTK::Xml::Element& node) :
        externalForceQueue_(externalForceQueue),
        appliedToBody_(nullptr),
        forceExpressedInBody_(nullptr),
        pointExpressedInBody_(nullptr),
        lastTime_(std::numeric_limits<double>::lowest()),
        isSubscribed(false),
        runCondition_(true)
    {
        ExternalForce::updateFromXMLNode(node, 4);
    }

    ExternalForceFromQueue::ExternalForceFromQueue(
        SingleExternalForceQueue* externalForceQueue,
        const ExternalForceProperties& externalForceProperties) :
        ExternalForceFromQueue
        (externalForceQueue,
        externalForceProperties.getForceIdentifier(),
        externalForceProperties.getPointIdentifier(),
        externalForceProperties.getTorqueIdentifier(),
        externalForceProperties.getAppliedToBody(),
        externalForceProperties.getForceExpressedInBody(),
        externalForceProperties.getPointExpressedInBody())
    {}

    ExternalForceData ExternalForceFromQueue::getExternalForceDataFromPastValues(double time) const {

        auto it(std::lower_bound(pastExternalForceFrames_.begin(), pastExternalForceFrames_.end(), time, [](
            SingleExternalForceFrame& lhs,
            double t){ return lhs.time < t; }));
        return it->data;
    }

    ExternalForceData ExternalForceFromQueue::getExternalForceDataFromQueue(double time) const {

        SingleExternalForceFrame forceFrame;
        while (time > lastTime_ && runCondition_){
            forceFrame = externalForceQueue_->pop();
            if (EndOfData::isEod(forceFrame)) {
                runCondition_ = false;
                forceFrame.data.setForceVector(SimTK::Vec3(0.));
                forceFrame.data.setApplicationPoint(SimTK::Vec3(0.));
                forceFrame.data.setTorque(SimTK::Vec3(0.));
            }
            if (runCondition_) {
                pastExternalForceFrames_.push_back(forceFrame);
                lastTime_ = forceFrame.time;
                if (pastExternalForceFrames_.size() > MaxStorageLength_)
                    pastExternalForceFrames_.pop_front();
            }
        }
        return forceFrame.data;
    }

    void ExternalForceFromQueue::computeForce(
        const SimTK::State& state,
        SimTK::Vector_<SimTK::SpatialVec>& bodyForces,
        SimTK::Vector& generalizedForces) const
    {
        if (runCondition_) {
            double time(state.getTime());
            ExternalForceData forceData;
            if (time <= lastTime_)
                forceData = getExternalForceDataFromPastValues(time);
            else
                forceData = getExternalForceDataFromQueue(time);

            SimTK::Vec3 torquesToApply(0.);

            const OpenSim::SimbodyEngine& engine = getModel().getSimbodyEngine();
            SimTK::Vec3 transformedForce, transformedPosition, transformedTorque;
            transformedForce = forceExpressedInBody_->expressVectorInGround(state, forceData.getForce());

            if (forceData.getUseApplicationPoint()) {
                //use COP + GRF + fre torque
                torquesToApply = forceExpressedInBody_->expressVectorInGround(state, forceData.getTorque());
                transformedPosition = pointExpressedInBody_->findStationLocationInAnotherFrame(state, forceData.getApplicationPoint(), *appliedToBody_);
            }
            else {
                //apply the grf and moments from the force plate directly to the body
                if (get_force_expressed_in_body() != "ground")
                {
                    transformedPosition = getModel().getBodySet().get(get_force_expressed_in_body()).getMassCenter();
                }
                torquesToApply = forceData.getMoments();
            }

            applyForceToPoint(state, *appliedToBody_, transformedPosition, transformedForce, bodyForces);
            applyTorque(state, *appliedToBody_, torquesToApply, bodyForces);
        }
    }

    void ExternalForceFromQueue::connectToModel(OpenSim::Model& model) {

        if (!isSubscribed) {
            externalForceQueue_->subscribe();
            isSubscribed = true;
        }
        Super::connectToModel(model);
        const std::string& appliedToBodyName = get_applied_to_body();
        const std::string& forceExpressedInBodyName = get_force_expressed_in_body();

        appliedToBody_ = &_model->updBodySet().get(appliedToBodyName);
        forceExpressedInBody_ = &_model->updBodySet().get(forceExpressedInBodyName);
        pointExpressedInBody_ = &_model->updBodySet().get(get_point_expressed_in_body());
    }

    ExternalForceFromQueue::~ExternalForceFromQueue() {

        if (externalForceQueue_ != nullptr)
            delete externalForceQueue_;

        //     externalForceQueue_.unsubscribe();
    }
}