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

            //	mV.show(inQueue_.pop().data);
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