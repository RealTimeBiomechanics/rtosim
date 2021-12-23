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

#include "rtosim/MarkersFromTrc.h"
#include "rtosim/EndOfData.h"
#include "rtosim/Mapper.h"
#include "rtosim/ArrayConverter.h"

#include <OpenSim/OpenSim.h>
#include <OpenSim/Common/MarkerData.h>
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

    MarkersFromTrc::MarkersFromTrc(
        MarkerSetQueue& outputMarkerSetQueue,
        rtb::Concurrency::Latch& doneWithSubscriptions,
        rtb::Concurrency::Latch& doneWithExecution,
        const std::string& osimModelFilename,
        const std::string& trcFilename,
        bool loop) :
        outputMarkerSetQueue_(outputMarkerSetQueue),
        doneWithSubscriptions_(doneWithSubscriptions),
        doneWithExecution_(doneWithExecution),
        trcFilename_(trcFilename),
        model_(osimModelFilename),
        loop_(loop),
        speedFactor_(-1),
        framesToSkip_(0),
        noMarkers_(0){

        model_.initSystem();

        OpenSim::Array<std::string> markerNamesArray;
        std::vector<std::string> markerNamesFromModel;
        const_cast<OpenSim::MarkerSet&>(model_.getMarkerSet()).getMarkerNames(markerNamesArray);
        rtosim::ArrayConverter::toStdVector(markerNamesArray, markerNamesFromModel);

        OpenSim::MarkerData trcFile(trcFilename_);
        sampleFrequency_ = static_cast<unsigned>(trcFile.getCameraRate());
        trcFile.convertToUnits(model_.getLengthUnits());
        std::vector<std::string> markerNamesFromFile;
        rtosim::ArrayConverter::toStdVector(trcFile.getMarkerNames(), markerNamesFromFile);

        rtosim::Mapper markersMapper(markerNamesFromFile, markerNamesFromModel);

        for (size_t i(0); i < static_cast<size_t>(trcFile.getNumFrames()); ++i) {
            auto markersFromTrcArr(trcFile.getFrame(i).getMarkers());
            MarkerSetData markersFromTrc;
            for (unsigned j(0); j < markersFromTrcArr.size(); ++j)
                markersFromTrc.push_back(markersFromTrcArr[j]);

            MarkerSetFrame currentFrame;
            currentFrame.time = trcFile.getFrame(i).getFrameTime();
            currentFrame.data = markersMapper.map(markersFromTrc);
            frames_.push_back(currentFrame);
        }
    }

    void MarkersFromTrc::setOutputFrequency(double frequency) {

        speedFactor_ = frequency / sampleFrequency_;
    }

    void MarkersFromTrc::setSpeedFactor(double speedFactor) {

        speedFactor_ = speedFactor;
    }

    void MarkersFromTrc::setFramesToSkip(unsigned n) {

        framesToSkip_ = n;
    }

    void MarkersFromTrc::operator()(){
        const std::chrono::milliseconds sleepTimeMilliseconds(getSleepTime());
        unsigned skipped(0);
//        unsigned count(0);

        size_t noFrames(frames_.size());
        if (noFrames > 0)
            noMarkers_ = frames_.front().data.size();
        doneWithSubscriptions_.wait();

        std::chrono::steady_clock::time_point timeOutTime = std::chrono::steady_clock::now();
        //todo, fix shared variables
        do {
            for(auto& frame : frames_) {
                if (speedFactor_ > 0) {
                    timeOutTime += sleepTimeMilliseconds;
                    std::this_thread::sleep_until(timeOutTime);
                }
                if (skipped == framesToSkip_) {
                    outputMarkerSetQueue_.push(frame);
                    skipped = 0;
                }
                else
                    ++skipped;
            }

        } while(loop_);
        sendEndOfData();
        doneWithExecution_.wait();

    }

    std::chrono::milliseconds MarkersFromTrc::getSleepTime() const {

        return std::chrono::milliseconds(static_cast<unsigned>(std::floor(1000. / (sampleFrequency_*speedFactor_))));
    }


    void MarkersFromTrc::sendEndOfData() {

        auto eodFrame(rtosim::EndOfData::get<MarkerSetFrame>());
        eodFrame.data.resize(noMarkers_);
        outputMarkerSetQueue_.push(eodFrame);
#ifdef RTOSIM_DEBUG
        cout << "MarkersFromTrc waiting\n";
#endif
    }

    MarkersFromTrc::~MarkersFromTrc() {
#ifdef RTOSIM_DEBUG
        cout << "Closing MarkersProducer..." << endl;
#endif
    }
}
