#include "rtosim/rtosim.h"
#include <OpenSim/Simulation/Model/OrientationSensor.h>
#include <vector>
#include <string>
using namespace std;
using namespace rtosim;
using namespace SimTK;

void printHelp() {
    auto progName(SimTK::Pathname::getThisExecutablePath());
    bool isAbsolute;
    string dir, filename, ext;
    SimTK::Pathname::deconstructPathname(progName, isAbsolute, dir, filename, ext);
    std::cout << "Usage: " << filename << " <output_filename>\n";
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printHelp();
        exit(EXIT_SUCCESS);
    }

    string outputFilename(argv[1]);
    bool isAbsolute;
    string dir, filename, ext;
    SimTK::Pathname::deconstructPathname(outputFilename, isAbsolute, dir, filename, ext);

    FlowControl flowControl(true);
    rtb::Concurrency::Latch ready(2), done(2);
    OrientationSetQueue orientationQueue;
    
    vector<string> names{"rt.leg.thigh", "rt.leg.shank", "rt.leg.foot" };
    OrientationsFromMR3 producer(orientationQueue, ready, done, flowControl, names);
    QueueToFileLogger<OrientationSetData> logger(orientationQueue, ready, done, names, dir, filename, ext);


    auto trigger([&flowControl]() {
        std::string cmd;
        std::cin >> cmd;
        flowControl.setRunCondition(false);
    });

    QueuesSync::launchThreads(producer, logger, trigger);


    return 0;
}