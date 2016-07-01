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

#ifndef rtosim_OrientationSensorsReferenceFromQueue_h
#define rtosim_OrientationSensorsReferenceFromQueue_h

#include <vector>
#include <string>

#include <OpenSim/Simulation/OrientationSensorsReference.h>
//#include <SimTKcommon.h>
#include "rtosim/ThreadPoolJobs.h"
#include "rtosim/queue/OrientationSetQueue.h"
#include "rtosim/queue/MarkerSetQueue.h"

namespace rtosim{

    class OrientationSensorsReferenceFromQueue : public OpenSim::OrientationSensorsReference {

    public:
        OrientationSensorsReferenceFromQueue(
	  ThreadPoolJobs<OrientationSetFrame>& inputMarkerSetFrameQueue, 
	  const std::vector<std::string>& osensNames, 
	  const std::vector<double>& osensWeigths);
        virtual ~OrientationSensorsReferenceFromQueue() {}
        virtual void getValues(const SimTK::State &s, SimTK::Array_<SimTK::Quaternion> &values) const override;
 //       virtual void getWeights(const SimTK::State &s, SimTK::Array_<double> &weights) const override;
        virtual const SimTK::Array_<std::string>& getNames() const override;
        double getCurrentTime() { return time_; }
        bool isEndOfData() const;
        void purgeCurrentFrame();
    private:
        ThreadPoolJobs<OrientationSetFrame>& inputOrientationSetFrameQueue_;
        OrientationSetFrame getPastFrame(double time) const;
        OrientationSetFrame getFrameFromQueue() const;

        unsigned static const MaxFramesToStore = 1000;
        mutable std::list<OrientationSetFrame> pastFrames_;
        mutable double lastQueueTime_;
        mutable double time_;
        SimTK::Array_<std::string> names_;
        std::vector<double> weights_;
    };
}

#endif
