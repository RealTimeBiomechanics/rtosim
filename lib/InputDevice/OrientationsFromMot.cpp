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

#include "rtosim/OrientationsFromMot.h"
#include "rtosim/EndOfData.h"
#include "rtosim/Mapper.h"
#include "rtosim/ArrayConverter.h"
#include <OpenSim/Common/OrientationSensorData.h>
#include <OpenSim/Simulation/Model/OrientationSensorSet.h>
#include <OpenSim/Simulation/Model/OrientationSensor.h>
//#include <OpenSim/OpenSim.h>
#include <OpenSim/Simulation/Model/ComponentSet.h>
#include <chrono>
#include <thread>
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;

namespace rtosim {

    OrientationsFromMot::OrientationsFromMot(
        OrientationSetQueue& outputOrientationSetQueue,
        rtosim::Concurrency::Latch& doneWithSubscriptions,
        rtosim::Concurrency::Latch& doneWithExecution,
        const std::string& osimModelFilename,
        const std::string& motFilename,
        bool loop) :
        outputOrientationSetQueue_(outputOrientationSetQueue),
        doneWithSubscriptions_(doneWithSubscriptions),
        doneWithExecution_(doneWithExecution),
        motFilename_(motFilename),
        model_(osimModelFilename),
        loop_(loop),
        speedFactor_(-1),
        framesToSkip_(0),
        noSensors_(0){

        model_.initSystem();


	const OpenSim::ComponentSet& componentSet(model_.getMiscModelComponentSet());
	OpenSim::OrientationSensorSet orientationSensorSet;
	for(unsigned i(0); i < componentSet.getSize(); ++i) {
	  OpenSim::OrientationSensor* sensor(dynamic_cast<OpenSim::OrientationSensor*>(&componentSet.get(i)));
	  if(sensor != nullptr)
	    orientationSensorSet.adoptAndAppend(sensor->clone());
	}
	
        OpenSim::Array<std::string> sensorsNamesArray;
	orientationSensorSet.getOSensorNames(sensorsNamesArray);
        std::vector<std::string> sensorsNamesFromModel;
        rtosim::ArrayConverter::toStdVector(sensorsNamesArray, sensorsNamesFromModel);

        OpenSim::OrientationSensorData motFile(motFilename_);
        sampleFrequency_ = static_cast<unsigned>(motFile.getDataRate());
        std::vector<std::string> sensorsNamesFromFile;
        rtosim::ArrayConverter::toStdVector(motFile.getOrientationSensorNames(), sensorsNamesFromFile);

        rtosim::Mapper markersMapper(sensorsNamesFromFile, sensorsNamesFromModel);

        for (size_t i(0); i < static_cast<size_t>(motFile.getNumFrames()); ++i) {
            auto frame(motFile.getFrame(i));
            OrientationSetData orientationsFromMot;
            for (unsigned j(0); j < frame.getOrientationSensors().size(); ++j)
                orientationsFromMot.emplace_back(frame.getOrientationSensor(j));

            OrientationSetFrame currentFrame;
            currentFrame.time = frame.getFrameTime();
            currentFrame.data = markersMapper.map(orientationsFromMot);
            frames_.push_back(currentFrame);
        }
    }

    void OrientationsFromMot::setOutputFrequency(double frequency) {

        speedFactor_ = frequency / sampleFrequency_;
    }

    void OrientationsFromMot::setSpeedFactor(double speedFactor) {

        speedFactor_ = speedFactor;
    }

    void OrientationsFromMot::setFramesToSkip(unsigned n) {

        framesToSkip_ = n;
    }

    void OrientationsFromMot::operator()(){
        const std::chrono::milliseconds sleepTimeMilliseconds(getSleepTime());
        unsigned skipped(0);
//        unsigned count(0);

        size_t noFrames(frames_.size());
        if (noFrames > 0)
            noSensors_ = frames_.front().data.size();
        doneWithSubscriptions_.wait();

        std::chrono::steady_clock::time_point timeOutTime = std::chrono::steady_clock::now();
        //todo, fix shared variables
	unsigned i(0);
        do {
            for(auto& frame : frames_) {
                if (speedFactor_ > 0) {
                    timeOutTime += sleepTimeMilliseconds;
                    std::this_thread::sleep_until(timeOutTime);
                }
                if (skipped == framesToSkip_) {
//		    std::cout << "Pushing frame " << i++ << std::endl;
                    outputOrientationSetQueue_.push(frame);
                    skipped = 0;
                }
                else
                    ++skipped;
            }

        } while(loop_);
        sendEndOfData();
        doneWithExecution_.wait();

    }

    std::chrono::milliseconds OrientationsFromMot::getSleepTime() const {

        return std::chrono::milliseconds(static_cast<unsigned>(std::floor(1000. / (sampleFrequency_*speedFactor_))));
    }


    void OrientationsFromMot::sendEndOfData() {

        auto eodFrame(rtosim::EndOfData::get<OrientationSetFrame>());
        eodFrame.data.resize(noSensors_);
        outputOrientationSetQueue_.push(eodFrame);
#ifdef RTOSIM_DEBUG
        cout << "OrientationsFromMot waiting\n";
#endif
    }

    OrientationsFromMot::~OrientationsFromMot() {
#ifdef RTOSIM_DEBUG
        cout << "Closing OrientationsFromMot..." << endl;
#endif
    }
}
