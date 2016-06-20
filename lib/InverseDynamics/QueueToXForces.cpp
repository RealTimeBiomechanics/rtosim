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

#include "rtosim/QueueToXForces.h"
#include "rtosim/ExternalForceFromQueue.h"
#include "rtosim/ExternalLoadProperties.h"
#include <limits>
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <SimTKcommon.h>
using SimTK::Vec3;

namespace rtosim {

    QueueToXForces::QueueToXForces(
        MultipleExternalForcesQueue& inputExternalForcesQueue,
        Concurrency::Latch& doneWithSubscriptions,
        Concurrency::Latch& doneWithExecution,
        const std::string& osimModelFilename,
        const std::string& externalLoadsXmlFilename) :
        inputExternalForcesQueue_(inputExternalForcesQueue),
        externalDoneWithSubscriptionsLatch_(doneWithSubscriptions),
        externalDoneWithExecutionLatch_(doneWithExecution),
        jointContactModel_(nullptr),
        splitForcesLatch_(2), osimModelFilename_(osimModelFilename)

    {
        ExternalLoadProperties loadProperties(externalLoadsXmlFilename);
        externalForcesProperties_ = loadProperties.getExternalForces();
    }

    QueueToXForces::QueueToXForces(
        MultipleExternalForcesQueue& inputExternalForcesQueue,
        Concurrency::Latch& doneWithSubscriptions,
        Concurrency::Latch& doneWithExecution,
        const std::string& osimModelFilename,
        const std::vector<ExternalForceProperties>& externalForceProperties) :
        inputExternalForcesQueue_(inputExternalForcesQueue),
        externalDoneWithSubscriptionsLatch_(doneWithSubscriptions),
        externalDoneWithExecutionLatch_(doneWithExecution),
        jointContactModel_(nullptr),
        splitForcesLatch_(2), osimModelFilename_(osimModelFilename),
        externalForcesProperties_(externalForceProperties)

    {
    }

    void QueueToXForces::initialiseContactModel() {

        initialiseExternalForces();

        //Using a pointer to delay the initialization
        jointContactModel_.reset(new JointContactFromOpensim(osimModelFilename_, extForces_));
    }

    void QueueToXForces::initialiseExternalForces() {

        for (auto& it : externalForcesProperties_) {

            singleExternalForceQueues_.emplace_back(new SingleExternalForceQueue());
            extForces_.push_back(new rtosim::ExternalForceFromQueue(
                singleExternalForceQueues_.back(),
                it.getForceIdentifier(),
                it.getPointIdentifier(),
                it.getTorqueIdentifier(),
                it.getAppliedToBody(),
                it.getForceExpressedInBody(),
                it.getPointExpressedInBody()));
        }
    }

    void QueueToXForces::distributeExternalForces() {

        //launch the thread, and let it go.. wait iws in the distructor
        splitForcesThread_ = (std::async(std::launch::async, [&](){

            inputExternalForcesQueue_.subscribe();
            splitForcesLatch_.wait();

            bool runCondition(true);
            do {
                QueueData<std::vector<ExternalForceData>> forces(inputExternalForcesQueue_.pop());
                if (forces.time < std::numeric_limits<double>::infinity()) {
                    for (size_t i(0); i < forces.data.size(); ++i) {
                        QueueData<ExternalForceData> currentForce;
                        currentForce.time = forces.time;
                        currentForce.data = forces.data.at(i);
                        singleExternalForceQueues_.at(i)->push(currentForce);
                    }
                }
                else {
                    runCondition = false;
                    QueueData<ExternalForceData> eof;
                    eof.time = std::numeric_limits<double>::infinity();
                    for (auto& forceQueue : singleExternalForceQueues_)
                        forceQueue->push(eof);
                }
            } while (runCondition);
        }));
    }

    void QueueToXForces::doneWithSubscriptions() {

        //from a queue with a vector of N forces we distribute the external forces in N queues..
        //each of them connected internally to an OpenSim model
        distributeExternalForces();
        splitForcesLatch_.wait();
        externalDoneWithSubscriptionsLatch_.wait();
    }

    void QueueToXForces::setState(double time, const vector<double>& q, const vector<double>& qd, const vector<double>& qdd) {

        jointContactModel_->setTime(time);
        jointContactModel_->setQ(q);
        jointContactModel_->setQd(qd);
        jointContactModel_->setQdd(qdd);
    }

    SimTK::Vector QueueToXForces::toSimTKVector(std::vector<double> v) {

        SimTK::Vector ansV(v.size(), .0);
        for (size_t i(0); i < v.size(); ++i)
            ansV.set(i, v.at(i));
        return ansV;
    }

    std::vector<double> QueueToXForces::fromSimTKVector(SimTK::Vector v) {

        std::vector<double> ansV(v.size(), .0);
        for (size_t i(0); i < v.size(); ++i)
            ansV[i] = v[i];
        return ansV;
    }

    void QueueToXForces::calculateInverseDynamics() {

        jointContactModel_->calculateInverseDynamics();
    }

    void QueueToXForces::getResidualMobilityForces(vector<double>& residualMobilityForces) const {

        SimTK::Vector allResidualMobilityForces;
        jointContactModel_->getResidualMobilityForces(allResidualMobilityForces);
        residualMobilityForces = fromSimTKVector(allResidualMobilityForces);
    }

    void QueueToXForces::doneWithExecution() {

        externalDoneWithExecutionLatch_.wait();
    }

    QueueToXForces::~QueueToXForces() {

        if (splitForcesThread_.valid())
            splitForcesThread_.wait();
        //     for (OpenSim::Force* it : extForces_)
        //        if (it != nullptr)
        //           delete it;
    }
}