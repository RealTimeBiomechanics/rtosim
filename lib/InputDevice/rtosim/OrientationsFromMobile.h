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

#ifndef rtosim_OrientationsFromMobile_h
#define rtosim_OrientationsFromMobile_h

#include <string>
#include <thread>
#include <memory>
#include <OpenSim/OpenSim.h>
#include <map>
#include "rtosim/Mapper.h"
#include "rtb/concurrency/Concurrency.h"
#include "rtosim/queue/OrientationSetQueue.h"
#include "rtosim/FlowControl.h"

namespace rtosim {

    class OrientationsFromMobile{

    public:
        OrientationsFromMobile() = delete;
        OrientationsFromMobile(OrientationsFromMobile&) = delete;

        OrientationsFromMobile(
            OrientationSetQueue& outputOrientationSetQueue,
            rtb::Concurrency::Latch& doneWithSubscriptions,
            rtb::Concurrency::Latch& doneWithExecution,
            FlowControl& runCondition,
            const std::string& osimFilename,
            const std::string& devicePort,
            const SimTK::Vec3& globalFrameRotations);

        void setLoopMode(bool isLoop) {
            increaseFrameNumbers_ = isLoop;
        }
        virtual ~OrientationsFromMobile();
        void operator()();

    private:
        //void initialiseSensorNames();
        unsigned long long connect() const;
        std::string getFrame(unsigned long long sock);
	void initialiseOffset(unsigned long long sock);
        void pushEndOfData();
        double evaluateTimestamp(std::string& data);
        SimTK::Rotation evaluateOrientations(std::string& data);
        OrientationSetFrame getOrientationFrame(unsigned long long sock);
        SimTK::Rotation globalToOsim_;
        OrientationSetQueue& outputOrientationSetQueue_;
        rtb::Concurrency::Latch& doneWithSubscriptions_;
        rtb::Concurrency::Latch& doneWithExecution_;
        FlowControl& runCondition_;
        short devicePort_;
        double firstTime_;
        std::vector<std::string> sensorNamesFromModel_;
        //OpenSim::Model model_;
        bool useOSensorData_;
        bool increaseFrameNumbers_;
        unsigned previousFrameNumber_, frameNumber_, lastFrameNumberOfTheLoop_;
	SimTK::Rotation initialOffset_;
    };
}

#endif
