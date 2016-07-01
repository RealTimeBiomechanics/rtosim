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
       SimTK::Vec3{1.57,-1.57,0}
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
    OpenSim::Model model;
    model.setUseVisualizer(true);
        
    OpenSim::Body* imu = new OpenSim::Body("imu", 1.0,SimTK::Vec3{0},SimTK::Inertia{0} );
    OpenSim::BallJoint* imuJoint = new OpenSim::BallJoint("imuToGround", 
				     model.getGroundBody(),
				     SimTK::Vec3{0,1,0.0},
				     SimTK::Vec3{-1.57,0,1.57},
				     *imu,
				     SimTK::Vec3{0},
				     SimTK::Vec3{0}
				    );
    OpenSim::DisplayGeometry imuBox("box.vtp");
    imuBox.setScaleFactors(SimTK::Vec3{0.016,0.03,0.005});
    imu->updDisplayer()->updGeometrySet().insert(0,imuBox);
    model.addBody(imu);

    OpenSim::Body* candyBar = new OpenSim::Body("candyBar", 1.0,SimTK::Vec3{0},SimTK::Inertia{0} );
    OpenSim::WeldJoint* candyBarJoint = new OpenSim::WeldJoint("candyBarToImu", 
				     *imu,
				     SimTK::Vec3{0,0,0.005},
				     SimTK::Vec3{0,0,0},
				     *candyBar,
				     SimTK::Vec3{0},
				     SimTK::Vec3{0}
				    );
    OpenSim::DisplayGeometry box("box.vtp");
    box.setScaleFactors(SimTK::Vec3{0.16,0.3,0.05});
    box.setOpacity(0.8);
    candyBar->updDisplayer()->updGeometrySet().insert(0,box);
    model.addBody(candyBar);
    model.print("boxModel.osim");
    SimTK::State s = model.initSystem();
/*    
    SimTK::MultibodySystem system;
    SimTK::SimbodyMatterSubsystem matter(system);
    SimTK::Body::Rigid candyBarBody;
    candyBarBody.addDecoration(SimTK::Transform(), 
			       SimTK::DecorativeBrick({0.16,0.3,0.05}).setColor(SimTK::Red));
    SimTK::MobilizedBody::Free freeCandyBar(matter.updGround(), candyBarBody);
    
    SimTK::Body::Rigid imuBody;
    imuBody.addDecoration(SimTK::Transform(), 
			       SimTK::DecorativeBrick({0.15,0.03,0.05}).setColor(SimTK::Green));
    SimTK::MobilizedBody::Weld weldedImu(candyBarBody, SimTK::Transform(),imuBody, SimTK::Transform());
*/
    SimTK::Vector& q = model.updMatterSubsystem().updQ(s);
    std::cout << q.size() <<std::endl;
    q[0] = 0.;
    q[1] = 0.;
    q[2] = 0.;
    SimTK::Visualizer viz = model.updVisualizer().updSimbodyVisualizer();
    viz.setShowFrameRate(true);
    viz.setBackgroundType(SimTK::Visualizer::BackgroundType::SolidColor);
    viz.setBackgroundColor(SimTK::Black);
    viz.setMode(SimTK::Visualizer::Mode::Sampling);
    viz.setShutdownWhenDestructed(true);
    viz.setDesiredBufferLengthInSec(1);
    viz.report(s);
    
    bool localRunCondition = true;
    while(localRunCondition) {
      
      auto frame = orientationSetQueue.pop();
      localRunCondition = !rtosim::EndOfData::isEod(frame);
      if(localRunCondition) {
	SimTK::Vector& q = model.updMatterSubsystem().updQ(s);
	SimTK::Rotation rot{frame.data[0]};
	SimTK::Vec3 tmp = rot.convertRotationToBodyFixedXYZ();
	
	q[0] = tmp[0];
	q[1] = tmp[1];
	q[2] = tmp[2];
	viz.report(s);
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