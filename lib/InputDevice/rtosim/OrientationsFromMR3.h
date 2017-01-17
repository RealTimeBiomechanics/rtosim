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

#ifndef rtosim_OrientationsFromMR3_h
#define rtosim_OrientationsFromMR3_h

#include <string>
#include <thread>
#include <memory>
#include <OpenSim/OpenSim.h>
#include <map>
#include "rtosim/concurrency/Concurrency.h"
#include "rtosim/queue/OrientationSetQueue.h"
#include "rtosim/FlowControl.h"
#include "rtosim/Mapper.h"
#include <MR3tools/MR3tools.h>

namespace rtosim {

    class OrientationsFromMR3 {

    public:
        OrientationsFromMR3() = delete;
        OrientationsFromMR3(OrientationsFromMR3&) = delete;

        OrientationsFromMR3(
            OrientationSetQueue& outputOrientationSetQueue,
            Concurrency::Latch& doneWithSubscriptions,
            Concurrency::Latch& doneWithExecution,
            FlowControl& runCondition,
            const std::vector<std::string>& orientationNames);
        void operator()();
    private:
       
        template<typename T>
        SimTK::Quaternion getFullQuaterion(const T& q);
        OrientationSetQueue& outputOrientationSetQueue_;
        Concurrency::Latch& doneWithSubscriptions_;
        Concurrency::Latch& doneWithExecution_;
        FlowControl& runCondition_;
        std::vector<std::string> orientationNames_;
        Mapper mapper;
    };

    template<typename T>
    SimTK::Quaternion OrientationsFromMR3::getFullQuaterion(const T& q) {
        typename T::value_type w = std::sqrt(1 - q[0] * q[0] - q[1] * q[1] - q[2] * q[2]);
        SimTK::Quaternion quat{ w, q[0], q[1], q[2] };
        quat = quat.normalize();
        return quat;
    }

}

#endif
