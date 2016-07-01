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

#include "rtosim/OrientationsFromMobile.h"
#include "rtosim/ArrayConverter.h"
#include "rtosim/StopWatch.h"
#include "rtosim/QueuesSync.h"
#include <OpenSim/Common/OrientationSensorData.h>
#include <OpenSim/Simulation/Model/OrientationSensorSet.h>
#include <OpenSim/Simulation/Model/OrientationSensor.h>
#include <OpenSim/OpenSim.h>
#include <OpenSim/Simulation/Model/ComponentSet.h>

#include <algorithm>
#include <chrono>
#include <thread>
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;
#include <OpenSim/OpenSim.h>
#include <thread>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <regex>
#ifdef WIN32
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif


std::string adapt(bool v) {

    std::string retStr("True");
    if (!v)
        retStr = "False";
    return retStr;
}

namespace rtosim {

    OrientationsFromMobile::OrientationsFromMobile(
        OrientationSetQueue& outputOrientationSetQueue,
        Concurrency::Latch& doneWithSubscriptions,
        Concurrency::Latch& doneWithExecution,
        FlowControl& runCondition,
        const std::string& osimFilename,
        const std::string& devicePort,
        const SimTK::Vec3& globalFrameRotations):
        useOSensorData_(true),
        outputOrientationSetQueue_(outputOrientationSetQueue),
        doneWithSubscriptions_(doneWithSubscriptions),
        doneWithExecution_(doneWithExecution),
        runCondition_(runCondition),
        devicePort_(std::stoi(devicePort)),
        lastFrameNumberOfTheLoop_(0),
        previousFrameNumber_(0)
       
        //model_(osimFilename)
    {
//        initialiseSensorNames();
        initialOffset_.setRotationFromThreeAnglesThreeAxes(SimTK::BodyOrSpaceType::SpaceRotationSequence, 0, SimTK::XAxis, 0, SimTK::YAxis, 0, SimTK::ZAxis);

      globalToOsim_.setRotationFromThreeAnglesThreeAxes(SimTK::BodyOrSpaceType::SpaceRotationSequence, globalFrameRotations[0], SimTK::XAxis, globalFrameRotations[1], SimTK::YAxis, globalFrameRotations[2], SimTK::ZAxis);
    }

    //void OrientationsFromMobile::initialiseSensorNames() {

    //    const OpenSim::ComponentSet& componentSet(model_.getMiscModelComponentSet());
    //    OpenSim::OrientationSensorSet orientationSensorSet;
    //    for (unsigned i(0); i < componentSet.getSize(); ++i) {
    //        OpenSim::OrientationSensor* sensor(dynamic_cast<OpenSim::OrientationSensor*>(&componentSet.get(i)));
    //        if (sensor != nullptr)
    //            orientationSensorSet.cloneAndAppend(*sensor);
    //    }

    //    OpenSim::Array<std::string> sensorsNamesArray;
    //    orientationSensorSet.getOSensorNames(sensorsNamesArray);
    //    rtosim::ArrayConverter::toStdVector(sensorsNamesArray, sensorNamesFromModel_);
    //}

    unsigned long long OrientationsFromMobile::connect() const {
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
        saddr.sin_port = htons(devicePort_);
        saddr.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(sock, (struct sockaddr*)&saddr, sizeof(saddr)) < 0)
            throw std::runtime_error{ "Could not bind Socket." };
        cout << "Connection successful" << endl;
        return sock;
    }

    void OrientationsFromMobile::initialiseOffset(unsigned long long sock){
      
        std::string data;
        bool waitingForFrame = true;
        while (waitingForFrame){
            try {
                data = getFrame(sock);
                initialOffset_ = evaluateOrientations(data);
                waitingForFrame = false;
            }
            catch (...){ std::cout << "..."; };
        }
    }
    
    
    OrientationSetFrame OrientationsFromMobile::getOrientationFrame(unsigned long long sock){
        std::string data;
        OrientationSetFrame frame;
        bool waitingForFrame = true;
        while (waitingForFrame){
            try {
                data = getFrame(sock);
                frame.time = evaluateTimestamp(data);
                SimTK::Rotation currData{ evaluateOrientations(data) };
		waitingForFrame = false;
		currData = initialOffset_.transpose()*currData;
                frame.data.push_back(currData.convertRotationToQuaternion());
            }
            catch (...){ std::cout << "..."; };
        }
        return frame;
    }

    std::string OrientationsFromMobile::getFrame(unsigned long long sock) {
        char buffer[500];
        int bytes = -1;
        do{
            bytes = recvfrom(sock, buffer, 500, 0, 0, 0);
        } while (bytes<=0);
        std::string data{buffer};
        return data;
    }

    double OrientationsFromMobile::evaluateTimestamp(std::string& data){
        std::regex regex_ts{ "^[0-9.]+" };
        std::smatch result_ts{};
        if (!std::regex_search(data, result_ts, regex_ts))
            throw std::runtime_error{ "No timestamp in data stream." };
        double timestamp{ std::stod(result_ts[0]) };
        return timestamp;
    }

    SimTK::Rotation OrientationsFromMobile::evaluateOrientations(std::string& data){
        std::regex regex_orientations{ ", 81, +( *-?[0-9.]+),+( *-?[0-9.]+),+( *-?[0-9.]+)" };
        std::smatch result_orientations{};
        if (!std::regex_search(data, result_orientations, regex_orientations))
            throw std::runtime_error{ "No orientations in data stream." };
        SimTK::Real xAngle{ SimTK::convertDegreesToRadians(std::stod(result_orientations[1])) };
        SimTK::Real yAngle{ SimTK::convertDegreesToRadians(std::stod(result_orientations[2])) };
        SimTK::Real zAngle{ SimTK::convertDegreesToRadians(std::stod(result_orientations[3])) };
        SimTK::Rotation rot;
        rot.setRotationFromThreeAnglesThreeAxes(SimTK::BodyOrSpaceType::SpaceRotationSequence, xAngle, SimTK::XAxis, yAngle, SimTK::YAxis, zAngle, SimTK::ZAxis);
       // SimTK::Rotation initialOffset{initialOffset_};
//	SimTK::Rotation currRotation = rot*globalToOsim_*initialOffset.transpose();
  	SimTK::Rotation currRotation = (globalToOsim_*rot).transpose();
	return currRotation;
    }

    void OrientationsFromMobile::pushEndOfData() {
        outputOrientationSetQueue_.push(EndOfData::get<OrientationSetFrame>());
    }

    void OrientationsFromMobile::operator()(){

        int numberOfFrame(0);
        int frameCounter(0);
        
        unsigned long long sock = connect();
        
        //receive the first full frame
        doneWithSubscriptions_.wait();

        auto start = std::chrono::system_clock::now();
	initialiseOffset(sock);
        OrientationSetFrame frame = getOrientationFrame(sock);
        firstTime_ = frame.time;
        frame.time = frame.time - firstTime_;
	
        while (runCondition_.getRunCondition()) {
            frame = getOrientationFrame(sock);
            frame.time = frame.time - firstTime_;
            outputOrientationSetQueue_.push(frame);
        }

        pushEndOfData();
        
        cout << "Disconnected" << std::endl;
        doneWithExecution_.wait();
    }

    OrientationsFromMobile::~OrientationsFromMobile() {

        cout << "Closing OrientationsFromMobile..." << endl;
    }
}
