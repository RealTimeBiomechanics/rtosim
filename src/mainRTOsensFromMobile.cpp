#include "rtosim/rtosim.h"
#include <OpenSim/Simulation/Model/OrientationSensor.h>
#include <OpenSim/Simulation/Model/OrientationSensorSet.h>
#include <OpenSim/Tools/InverseKinematicsExtendedTool.h>
#include <OpenSim/Tools/IKExtendedTaskSet.h>
#include <OpenSim/Simulation/Model/ComponentSet.h>
#include <OpenSim/Common/OrientationSensorData.h>
#include <string>
#include <vector>
#include <regex>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifdef WIN32
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif
using namespace OpenSim;
using namespace SimTK;
using namespace rtosim;
//using namespace std;

unsigned openUdpSocket() {
    short PORT{ 5555 };

#ifdef WIN32
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    auto sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        throw std::runtime_error{ "Could not create Socket." };

    sockaddr_in saddr{};
    std::memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr*)&saddr, sizeof(saddr)) < 0)
        throw std::runtime_error{ "Could not bind Socket." };

    return sock;
}
int main() {
  // -------------------------- UDP Socket.
  unsigned BUFFSIZE{ 800 };

  auto sock = openUdpSocket();
  char buffer[800];
  bool ok = true;
  while (ok) { // first good sample
      int bytes = recvfrom(sock, buffer, 800, 0, 0, 0);
      if (bytes > 0) {
          std::string data{buffer};
          std::cout << "parsing: " << data << std::endl;
          }
  }
  
  
  /* 
  OpenSim::Object::registerType(OpenSim::OrientationSensor());
  OpenSim::Object::registerType(OpenSim::IKExtendedTaskSet());
  OpenSim::Object::registerType(OpenSim::IKOrientationSensorTask());
  OpenSim::Object::registerType(OpenSim::InverseKinematicsExtendedTool());
  
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
  */
  return 0;
}