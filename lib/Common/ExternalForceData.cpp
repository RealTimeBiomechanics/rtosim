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
* Author(s): C. Pizzolato                                                    *
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

#include "rtosim/ExternalForceData.h"
#include <SimTKcommon.h>
using SimTK::Vec3;
#include <utility>
using std::pair;

namespace rtosim {

    ExternalForceData::ExternalForceData() :
        targetBody_(0),
        force_(std::make_pair(0, Vec3(0))),
        point_(std::make_pair(0, Vec3(0))),
        torque_(std::make_pair(0, Vec3(0))),
        sourceName_(""),
        useApplicationPoint_(true) { /*blank*/
    }

    ExternalForceData::ExternalForceData(unsigned targetBody) :
        targetBody_(targetBody),
        force_(std::make_pair(0, Vec3(0))),
        point_(std::make_pair(0, Vec3(0))),
        torque_(std::make_pair(0, Vec3(0))),
        sourceName_(""),
        useApplicationPoint_(true) { /*blank*/
    }

    void ExternalForceData::setTargetBody(unsigned targetBody) {

        targetBody_ = targetBody;
    }

    void ExternalForceData::setForceVector(const Vec3& forceVector, unsigned expressedInBodyIndex) {

        force_.first = expressedInBodyIndex;
        force_.second = forceVector;
    }

    void ExternalForceData::setApplicationPoint(const Vec3& point, unsigned expressedInBodyIndex) {

        point_.first = expressedInBodyIndex;
        point_.second = point;
    }

    void ExternalForceData::setMoments(const SimTK::Vec3& moments, unsigned expressedInBodyIndex) {
        moments_.first = expressedInBodyIndex;
        moments_.second = moments;
    }

    void ExternalForceData::setTorque(const Vec3& torque, unsigned expressedInBodyIndex) {

        torque_.first = expressedInBodyIndex;
        torque_.second = torque;
    }

    void  ExternalForceData::setSourceName(const std::string& name) {

        sourceName_ = name;
    }

    std::string  ExternalForceData::getSourceName() const {

        return sourceName_;
    }

    const Vec3& ExternalForceData::getForce() const {

        return force_.second;
    }

    void ExternalForceData::getForce(Vec3& force) const {

        force = force_.second;
    }

    unsigned ExternalForceData::getForceBodyIndex() const {

        return force_.first;
    }

    const Vec3& ExternalForceData::getApplicationPoint() const {

        return point_.second;
    }

    void ExternalForceData::getApplicationPoint(Vec3& applicationPoint) const {

        applicationPoint = point_.second;
    }

    unsigned ExternalForceData::getApplicationPointBodyIndex() const {

        return point_.first;
    }

    void ExternalForceData::setUseApplicationPoint(bool useApplicationPoint) {

        useApplicationPoint_ = useApplicationPoint;
    }

    bool ExternalForceData::getUseApplicationPoint() const {

        return useApplicationPoint_;
    }

    const Vec3& ExternalForceData::getMoments() const {

        return moments_.second;
    }

    void ExternalForceData::getMoments(Vec3& moments) const {

        moments = moments_.second;
    }

    unsigned ExternalForceData::getMomentsBodyIndex() const {

        return moments_.first;
    }

    const Vec3& ExternalForceData::getTorque() const {

        return torque_.second;
    }

    void ExternalForceData::getTorque(Vec3& torque) const {

        torque = torque_.second;
    }

    unsigned ExternalForceData::getTorqueBodyIndex() const {

        return torque_.first;
    }

    unsigned ExternalForceData::getTargetBodyIndex() const {

        return targetBody_;
    }

    void ExternalForceData::print(std::ostream& os) const {

        os << "Target body index " << getTargetBodyIndex() << std::endl;
        os << "Force" << std::endl;
        os << "--Reference System Index " << getForceBodyIndex() << std::endl;
        os << "--Value                  " << getForce() << std::endl;
        os << "Force Application Point" << std::endl;
        os << "--Reference System Index " << getApplicationPointBodyIndex() << std::endl;
        os << "--Value                  " << getApplicationPoint() << std::endl;
        os << "Torque" << std::endl;
        os << "--Reference System Index " << getTorqueBodyIndex() << std::endl;
        os << "--Value                  " << getTorque() << std::endl;
    }
}