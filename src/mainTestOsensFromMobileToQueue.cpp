#include "rtosim/rtosim.h"
#include <OpenSim/OpenSim.h>
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
  rtosim::Concurrency::Latch doneWithSubscriptions(1), doneWithExecution(1);
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

  
  auto candyBar([&orientationSetQueue](){
    
    orientationSetQueue.subscribe();    
    SimTK::MultibodySystem system;
    SimTK::SimbodyMatterSubsystem matter(system);
    SimTK::Body::Rigid candyBarBody;
    candyBarBody.addDecoration(SimTK::Transform(), 
			       SimTK::DecorativeBrick({1,2,3}).setColor(SimTK::Red));
    SimTK::MobilizedBody::Free freeCandyBar(matter.updGround(), candyBarBody);
    SimTK::Visualizer viz(system);
    viz.setShowFrameRate(true);
    viz.setBackgroundType(SimTK::Visualizer::BackgroundType::SolidColor);
    viz.setBackgroundColor(SimTK::Black);
    viz.setMode(SimTK::Visualizer::Mode::Sampling);
    viz.setShutdownWhenDestructed(true);
    viz.setDesiredBufferLengthInSec(1);
    system.realizeTopology();
    SimTK::State state = system.getDefaultState();
    freeCandyBar.setQ(state, SimTK::Vec7(0, 0, 0, 0, 0, 0, 0));
    system.realize(state);
    viz.report(state);
    
    bool localRunCondition = true;
    while(localRunCondition) {
      
      auto frame = orientationSetQueue.pop();
      localRunCondition = !rtosim::EndOfData::isEod(frame);
      if(localRunCondition) {
	freeCandyBar.setQToFitRotation(state, SimTK::Rotation(frame.data.at(0)));
	viz.report(state);
      }
    }

    
  });
  
  auto trigger([&runCondition](){
      std::string cmd;
      bool run(true);
      std::cin >> cmd;
      runCondition.setRunCondition(false);
  });

 QueuesSync::launchThreads(orientationsFromMobile,trigger, candyBar);
  
  return 0;
}