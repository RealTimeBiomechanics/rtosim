#include "rtosim/rtosim.h"
#include <OpenSim/Simulation/Model/OrientationSensor.h>
#include <OpenSim/Simulation/Model/OrientationSensorSet.h>
#include <OpenSim/Tools/InverseKinematicsExtendedTool.h>
#include <OpenSim/Tools/IKExtendedTaskSet.h>
#include <OpenSim/Simulation/Model/ComponentSet.h>
#include <OpenSim/Common/OrientationSensorData.h>
#include <string>
#include <vector>
using namespace OpenSim;
using namespace SimTK;
using namespace rtosim;
using namespace std;

int main() {
  

  OpenSim::Object::registerType(OpenSim::OrientationSensor());
  //OpenSim::Object::registerType(OpenSim::IKExtendedTaskSet());
  //OpenSim::Object::registerType(OpenSim::IKOrientationSensorTask());
  //OpenSim::Object::registerType(OpenSim::InverseKinematicsExtendedTool());

  OrientationSetQueue orientationSetQueue;
  rtosim::Concurrency::Latch doneWithSubscriptions(2), doneWithExecution(2);
  std::vector<std::string> columnLabels{ "Imu_1" };
  std::string modelFileName("");
  FlowControl runCondition(true);
  OrientationsFromMobile orientationsFromMobile(
      orientationSetQueue,
      doneWithSubscriptions,
      doneWithExecution,
      runCondition,
      modelFileName,
      "5555",
      SimTK::Vec3{0,0,0}
  );
  
  QueueToFileLogger<OrientationSetData> logger(
     orientationSetQueue,
     doneWithSubscriptions,
     doneWithExecution,
     columnLabels,
     ".",
      "outputFile"
  );

  auto trigger([&runCondition](){
      std::string cmd;
      bool run(true);
      std::cin >> cmd;
      runCondition.setRunCondition(false);
  });

 QueuesSync::launchThreads(orientationsFromMobile,trigger, logger);
  
  return 0;
}