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

#include "rtosim/OrientationSensorsReferenceFromQueue.h"
#include "rtosim/EndOfData.h"
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <limits>

namespace rtosim{

    OrientationSensorsReferenceFromQueue::OrientationSensorsReferenceFromQueue(
        ThreadPoolJobs<OrientationSetFrame>& inputOrientationSetFrameQueue,
        const std::vector<std::string>& names,
        const std::vector<double>& weights) :
        inputOrientationSetFrameQueue_(inputOrientationSetFrameQueue),
        weights_(weights),
        time_(std::numeric_limits<double>::infinity()),
        lastQueueTime_(-std::numeric_limits<double>::infinity()){

	OpenSim::Set<OpenSim::OrientationSensorWeight> weightSet;
        for (unsigned i(0); i < names.size(); ++i) {
            names_.push_back(names[i]);
	    OpenSim::OrientationSensorWeight weight(names[i], weights_[i]);
	 weightSet.cloneAndAppend(weight);   
	}
	OpenSim::OrientationSensorsReference::setOrientationSensorWeightSet(weightSet);

    }

    bool OrientationSensorsReferenceFromQueue::isEndOfData() const {

        OrientationSetFrame frame(inputOrientationSetFrameQueue_.front());
        return rtosim::EndOfData::isEod(frame);
    }

    void OrientationSensorsReferenceFromQueue::purgeCurrentFrame() {
        inputOrientationSetFrameQueue_.pop();
        lastQueueTime_ = -std::numeric_limits<double>::infinity();
    }

    void OrientationSensorsReferenceFromQueue::getValues(const SimTK::State &s, SimTK::Array_<SimTK::Quaternion> &values) const {

        values.clear();
        OrientationSetFrame frame;
        double currentTime(inputOrientationSetFrameQueue_.front().time);
        if (currentTime < lastQueueTime_)
            frame = getPastFrame(currentTime);
        else
            frame = getFrameFromQueue();

       // if (rtosim::EndOfData::isEod(frame))
        //    for (auto& m : frame.data)
        //        m.set(true);
        for (auto& quaternion : frame.data)
            values.push_back(quaternion);
        time_ = frame.time;

    }

    OrientationSetFrame OrientationSensorsReferenceFromQueue::getPastFrame(double time) const {

        auto it(std::lower_bound(pastFrames_.begin(), pastFrames_.end(), time, [](
            OrientationSetFrame& lhs,
            double t){ return lhs.time < t; }));
        return *it;
    }

    OrientationSetFrame OrientationSensorsReferenceFromQueue::getFrameFromQueue() const {

	OrientationSetFrame frame(inputOrientationSetFrameQueue_.pop());
        lastQueueTime_ = frame.time;

	pastFrames_.push_back(frame);
        if (pastFrames_.size() < MaxFramesToStore)
	  pastFrames_.pop_front();

        return frame;
    }

    const SimTK::Array_<std::string>& OrientationSensorsReferenceFromQueue::getNames() const {

        return names_;
    }

   /* void OrientationSensorsReferenceFromQueue::getWeights(const SimTK::State &s, SimTK::Array_<double> &weights) const {

        weights = weights_;
    }
    */
}