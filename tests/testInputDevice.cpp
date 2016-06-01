#include <OpenSim/OpenSim.h>
#include <string>
using namespace std;

#include "rtosim/ExternalForcesFromStorageFile.h"
#include "rtosim/Common.h"
#include "rtosim/queue/MultipleExternalForcesQueue.h"
#include "rtosim/concurrency/Concurrency.h"
#include "rtosim/OutputDevice.h"

int main() {

    rtosim::MultipleExternalForcesQueue forcesQueue;
    rtosim::Concurrency::Latch subscriptionLatch(2), executionLatch(2);
    string filename = "C:/Data/RealTime/ElaboratedData/S04/20151120/inverseDynamics/q/walking_1ms_1/Setup/external_loads.xml";
    rtosim::ExternalForcesFromStorageFile reader(forcesQueue, subscriptionLatch, executionLatch, filename);
    rtosim::QueueToFileLogger<rtosim::MultipleExternalForcesData> logger(forcesQueue, subscriptionLatch, executionLatch, {}, "output", "test");
    rtosim::QueuesSync::launchThreads(reader, logger);

    return 0;
}