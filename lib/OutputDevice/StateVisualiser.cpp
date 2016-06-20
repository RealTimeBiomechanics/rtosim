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

#include "rtosim/StateVisualiser.h"
#include "rtosim/ArrayConverter.h"
#include "rtosim/EndOfData.h"

#include <string>
using std::string;
#include <chrono>
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <OpenSim/Simulation/Model/ModelVisualizer.h>

namespace rtosim{

    StateVisualiser::StateVisualiser(
        rtosim::GeneralisedCoordinatesQueue& inputGeneralisedCoordinatesQueue,
        const std::string& osimModelFilename) :
        inputGeneralisedCoordinatesQueue_(inputGeneralisedCoordinatesQueue),
        model_(osimModelFilename)
    {}

    void StateVisualiser::operator()() {

        inputGeneralisedCoordinatesQueue_.subscribe();

        model_.setUseVisualizer(true);
        SimTK::State s(model_.initSystem());
        SimTK::Visualizer &viz = model_.updVisualizer().updSimbodyVisualizer();
        viz.setShowFrameRate(false);
        viz.setShutdownWhenDestructed(true);
        viz.setMode(SimTK::Visualizer::Mode::Sampling);
        viz.setShutdownWhenDestructed(true);
        viz.setDesiredBufferLengthInSec(5);
        viz.setDesiredFrameRate(60);
        ShowInfo* info = new ShowInfo();
        viz.addDecorationGenerator(info);
        auto start = std::chrono::system_clock::now();
        int frameCounter = 0;
        bool localRunCondition(true);
        while (localRunCondition) {

            rtosim::GeneralisedCoordinatesFrame currentCoordinatesFrame(inputGeneralisedCoordinatesQueue_.pop());
            if (rtosim::EndOfData::isEod(currentCoordinatesFrame))
                localRunCondition = false;
            else {
                rtosim::GeneralisedCoordinatesData currentCoordinates(currentCoordinatesFrame.data);
                auto q(currentCoordinates.getQ());
                for (unsigned i(0); i < currentCoordinates.getNCoordinates(); ++i)
                    s.updQ()[i] = q[i];
            }

            viz.report(s);
            frameCounter++;

            if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start)) > std::chrono::milliseconds(1000)) {
                info->setText("IK FPS: " + std::to_string(frameCounter));
                start = std::chrono::system_clock::now();
                frameCounter = 0;
            }
        }
    }
}