#include "rtosim/OrientationsFromMR3.h"
#include "rtosim/concurrency/Concurrency.h"
#include "rtosim/queue/OrientationSetQueue.h"
#include "rtosim/EndOfData.h"
#include "rtosim/QueuesSync.h"
#include "rtosim/QueueToInverseKinematicsOsens.h"
#include "rtosim/StateVisualiser.h"
#include <OpenSim/Simulation/Model/OrientationSensor.h>
#include <vector>
#include <string>
using namespace std;
using namespace rtosim;
using namespace SimTK;

int main() {

    OpenSim::Object::registerType(OpenSim::OrientationSensor());
    FlowControl flowControl(true);
    Concurrency::Latch ready(2), done(2);
    OrientationSetQueue orientationQueue;
    GeneralisedCoordinatesQueue coordinatesQueue;

    string osimModelFilename("C:/Users/s2849511/coding/versioning/rtosim/data/arm26_osens.osim");

//    vector<string> names{ "device.player.player.mm_gait;line.human.lt.leg.thigh;type.input.motion.rot;" };
    vector<string> names;
    OrientationsFromMR3 producer(orientationQueue, ready, done, flowControl, names);
    QueueToInverseKinematicsOsens consumer(
        orientationQueue,
        coordinatesQueue,
        ready,
        done,
        osimModelFilename,
        1);
    
    StateVisualiser stateVisualiser(coordinatesQueue, osimModelFilename);
    auto trigger([&flowControl]() {
        std::string cmd;
        std::cin >> cmd;
        flowControl.setRunCondition(false);
    });

    QueuesSync::launchThreads(producer, consumer, stateVisualiser, trigger);


    return 0;
}