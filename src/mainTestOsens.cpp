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
/*
vector<string> getOsensNamesFromModel(const string& osimModelFilename) {

    Model model(osimModelFilename);
    const OpenSim::ComponentSet& componentSet(model.getMiscModelComponentSet());
	OpenSim::OrientationSensorSet orientationSensorSet;
	for(unsigned i(0); i < componentSet.getSize(); ++i) {
	  OpenSim::OrientationSensor* sensor(dynamic_cast<OpenSim::OrientationSensor*>(&componentSet.get(i)));
	  if(sensor != nullptr)
	    orientationSensorSet.cloneAndAppend(*sensor);
	}
	
        OpenSim::Array<std::string> sensorsNamesArray;
	orientationSensorSet.getOSensorNames(sensorsNamesArray);
        std::vector<std::string> sensorsNamesFromModel;
        rtosim::ArrayConverter::toStdVector(sensorsNamesArray, sensorsNamesFromModel);
    return sensorsNamesFromModel;
  
}
*/
int main() {
    OpenSim::Object::registerType(OpenSim::OrientationSensorSet());
 // OpenSim::Object::registerType(OpenSim::OrientationSensor());
//  OpenSim::Object::registerType(OpenSim::IKExtendedTaskSet());
//  OpenSim::Object::registerType(OpenSim::IKOrientationSensorTask());
//  OpenSim::Object::registerType(OpenSim::InverseKinematicsExtendedTool());
  
  string motFilename("/home/claudio/Data/IMU_test_data/cq_trial04_res7ms.mot");
  string osimModelFilename("/home/claudio/Data/IMU_test_data/phantomModel_pelvisLocked.osim");
  OrientationSetQueue orientationSetQueue;
  rtosim::Concurrency::Latch doneWithSubscriptions(2), doneWithExecution(2);
  
  auto columnLabels(getOsensNamesFromModel(osimModelFilename));
  
  OrientationsFromMot orientationsFromMot(
    orientationSetQueue,
    doneWithSubscriptions,
    doneWithExecution,
    osimModelFilename,
    motFilename
  );
  
  orientationsFromMot.setOutputFrequency(200);
  
  QueueToFileLogger<OrientationSetData> logger(
     orientationSetQueue,
     doneWithSubscriptions,
     doneWithExecution,
     columnLabels,
     ".",
      "outputFile"
  );
  
 QueuesSync::launchThreads(orientationsFromMot, logger);
  
  return 0;
}