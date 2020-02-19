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

#include "rtosim/DataFromNexus.h"
#include "rtosim/ArrayConverter.h"
#include "rtosim/StopWatch.h"
#include "rtosim/QueuesSync.h"

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


std::string adapt(bool v) {

    std::string retStr("True");
    if (!v)
        retStr = "False";
    return retStr;
}

namespace rtosim {

    DataFromNexus::DataFromNexus(
        MarkerSetQueue& outputMarkerSetQueue,
        rtb::Concurrency::Latch& doneWithSubscriptions,
        rtb::Concurrency::Latch& doneWithExecution,
        FlowControl& runCondition,
		const std::vector<std::string>& markersNames,
        const std::string& hostname) :
        useMarkerData_(true),
        outputMarkerSetQueue_(&outputMarkerSetQueue),
        useGrfData_(false),
        outputGrfQueue_(nullptr),
		useEmgData_(false),
		outputEmgQueue_(nullptr),
        doneWithSubscriptions_(doneWithSubscriptions),
        doneWithExecution_(doneWithExecution),
        runCondition_(runCondition),
        hostName_(hostname),
        fromNexusToModelLengthConversion_(1.),
        lastFrameNumberOfTheLoop_(0),
        previousFrameNumber_(0),
		emgSubSamples_(0),
		markersToStream_(markersNames),
		xMapping_(VDS::Direction::Right),
		yMapping_(VDS::Direction::Up),
		zMapping_(VDS::Direction::Backward)
    {
    }


    DataFromNexus::DataFromNexus(
        MultipleExternalForcesQueue& outputGrfQueue,
        rtb::Concurrency::Latch& doneWithSubscriptions,
        rtb::Concurrency::Latch& doneWithExecution,
        FlowControl& runCondition,
		const std::vector<std::string>& markersNames,
        const std::string& hostname) :
        useMarkerData_(false),
        outputMarkerSetQueue_(nullptr),
        useGrfData_(true),
        outputGrfQueue_(&outputGrfQueue),
		useEmgData_(false),
		outputEmgQueue_(nullptr),
        doneWithSubscriptions_(doneWithSubscriptions),
        doneWithExecution_(doneWithExecution),
        runCondition_(runCondition),
        hostName_(hostname),
        fromNexusToModelLengthConversion_(1.),
        lastFrameNumberOfTheLoop_(0),
        previousFrameNumber_(0),
		emgSubSamples_(0),
		markersToStream_(markersNames),
		xMapping_(VDS::Direction::Right),
		yMapping_(VDS::Direction::Up),
		zMapping_(VDS::Direction::Backward)
    {
    }


    DataFromNexus::DataFromNexus(
        MarkerSetQueue& outputMarkerSetQueue,
        MultipleExternalForcesQueue& outputGrfQueue,
        rtb::Concurrency::Latch& doneWithSubscriptions,
        rtb::Concurrency::Latch& doneWithExecution,
        FlowControl& runCondition,
		const std::vector<std::string>& markersNames,
        const std::string& hostname) :
        useMarkerData_(true),
        outputMarkerSetQueue_(&outputMarkerSetQueue),
        useGrfData_(true),
        outputGrfQueue_(&outputGrfQueue),
		useEmgData_(false),
		outputEmgQueue_(nullptr),
        doneWithSubscriptions_(doneWithSubscriptions),
        doneWithExecution_(doneWithExecution),
        runCondition_(runCondition),
        hostName_(hostname),
        fromNexusToModelLengthConversion_(1.),
        lastFrameNumberOfTheLoop_(0),
        previousFrameNumber_(0),
		emgSubSamples_(0),
		markersToStream_(markersNames),
		xMapping_(VDS::Direction::Right),
		yMapping_(VDS::Direction::Up),
		zMapping_(VDS::Direction::Backward)
    {
        
    }

	DataFromNexus::DataFromNexus(
		EmgQueue& outputEmgQueue,
		rtb::Concurrency::Latch& doneWithSubscriptions,
		rtb::Concurrency::Latch& doneWithExecution,
		FlowControl& runCondition,
		const std::vector<std::string>& emgChannelNames,
		const std::string& hostname) :
		useMarkerData_(false),
		outputMarkerSetQueue_(nullptr),
		useGrfData_(false),
		outputGrfQueue_(nullptr),
		useEmgData_(true),
		outputEmgQueue_(&outputEmgQueue),
		doneWithSubscriptions_(doneWithSubscriptions),
		doneWithExecution_(doneWithExecution),
		emgChannelNames_(emgChannelNames),
		runCondition_(runCondition),
		hostName_(hostname),
		fromNexusToModelLengthConversion_(1.),
		lastFrameNumberOfTheLoop_(0),
		previousFrameNumber_(0),
		emgSubSamples_(0),
		xMapping_(VDS::Direction::Right),
		yMapping_(VDS::Direction::Up),
		zMapping_(VDS::Direction::Backward) {}


	DataFromNexus::DataFromNexus(
		MarkerSetQueue& outputMarkerSetQueue,
		EmgQueue& outputEmgQueue,
		rtb::Concurrency::Latch& doneWithSubscriptions,
		rtb::Concurrency::Latch& doneWithExecution,
		FlowControl& runCondition,
		const std::vector<std::string>& markersNames,
		const std::vector<std::string>& emgChannelNames,
		const std::string& hostname) :
		useMarkerData_(true),
		outputMarkerSetQueue_(&outputMarkerSetQueue),
		useGrfData_(false),
		outputGrfQueue_(nullptr),
		useEmgData_(true),
		outputEmgQueue_(&outputEmgQueue),
		doneWithSubscriptions_(doneWithSubscriptions),
		doneWithExecution_(doneWithExecution),
		emgChannelNames_(emgChannelNames),
		runCondition_(runCondition),
		hostName_(hostname),
		fromNexusToModelLengthConversion_(1.),
		lastFrameNumberOfTheLoop_(0),
		previousFrameNumber_(0),
		emgSubSamples_(0),
		markersToStream_(markersNames),
		xMapping_(VDS::Direction::Right),
		yMapping_(VDS::Direction::Up),
		zMapping_(VDS::Direction::Backward) {

	}


	DataFromNexus::DataFromNexus(
		MarkerSetQueue& outputMarkerSetQueue,
		MultipleExternalForcesQueue& outputGrfQueue,
		EmgQueue& outputEmgQueue,
		rtb::Concurrency::Latch& doneWithSubscriptions,
		rtb::Concurrency::Latch& doneWithExecution,
		FlowControl& runCondition,
		const std::vector<std::string>& markersNames,
		const std::vector<std::string>& emgChannelNames,
		const std::string& hostname) :
		useMarkerData_(true),
		outputMarkerSetQueue_(&outputMarkerSetQueue),
		useGrfData_(true),
		outputGrfQueue_(&outputGrfQueue),
		useEmgData_(true),
		outputEmgQueue_(&outputEmgQueue),
		doneWithSubscriptions_(doneWithSubscriptions),
		doneWithExecution_(doneWithExecution),
		emgChannelNames_(emgChannelNames),
		runCondition_(runCondition),
		hostName_(hostname),
		fromNexusToModelLengthConversion_(1.),
		lastFrameNumberOfTheLoop_(0),
		previousFrameNumber_(0),
		emgSubSamples_(0),
		markersToStream_(markersNames),
		xMapping_(VDS::Direction::Right),
		yMapping_(VDS::Direction::Up),
		zMapping_(VDS::Direction::Backward) {
	}


    void DataFromNexus::connectToServer(VDS::Client& client) const {
        //connect to server
        while (!client.IsConnected().Connected) {
            if (client.Connect(hostName_).Result != VDS::Result::Success) {
                cout << "Warning - connect failed..." << endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }

        cout << "Connected" << endl;
    }

    void DataFromNexus::initialiseDataToStream(VDS::Client& client) {

        if (useMarkerData_)
            client.EnableMarkerData();
        else
            client.DisableMarkerData();
        if (useGrfData_ || useEmgData_)
            client.EnableDeviceData();
        else
            client.DisableDeviceData();
        client.DisableSegmentData();
        client.DisableUnlabeledMarkerData();
        cout << "Marker Data Enabled: " << adapt(client.IsMarkerDataEnabled().Enabled) << endl;
        cout << "Device Data Enabled: " << adapt(client.IsDeviceDataEnabled().Enabled) << endl;
        cout << "Segments Data Enabled: " << adapt(client.IsSegmentDataEnabled().Enabled) << endl;
        cout << "Unlabelled Markers Data Enabled: " << adapt(client.IsUnlabeledMarkerDataEnabled().Enabled) << endl;

        while (client.SetStreamMode(VDS::StreamMode::ServerPush).Result != VDS::Result::Success)
            cout << ".";
        cout << endl;
        if (useMarkerData_)
            waitForCompleteFrame(client);
        if (useGrfData_)
            waitForForcePlates(client);
		if (useEmgData_)
			waitForEmgs(client);
    }

    void DataFromNexus::waitForForcePlates(VDS::Client& client) {
        forcePlateCount_ = 0;
        while (forcePlateCount_ < 1 && runCondition_.getRunCondition()) {
            client.GetFrame();
            auto forcePlatesInfo = client.GetForcePlateCount();
            if (forcePlatesInfo.Result == VDS::Result::Success) {
                cout << "Waiting for devices..." << endl;
                forcePlateCount_ = forcePlatesInfo.ForcePlateCount;
            }
        }
        cout << "Found " << forcePlateCount_ << " force plates" << endl;
		if (forcePlateCount_ != forcePlatePositions_.size()) {
			cout << "Warning: Found a different number of force plates than the provided plate locations" << endl;
		}
        for (unsigned i(0); i < client.GetDeviceCount().DeviceCount; ++i)
            if (client.GetDeviceName(i).DeviceType == VDS::DeviceType::ForcePlate) {
                std::string fpName = client.GetDeviceName(i).DeviceName;
                forcePlateNames_.push_back(fpName);
                cout << fpName << endl;
            }
    }

    void DataFromNexus::waitForCompleteFrame(VDS::Client& client) {

        vector<string> markerNamesFromNexus;
        bool isFrameComplete(false);
        do{
            if (client.GetFrame().Result == VDS::Result::Success) {
                unsigned subjectCount = client.GetSubjectCount().SubjectCount;
                if (subjectCount != 1)
                    cout << "Warning: " << subjectCount << " subject(s) in the capture volume" << endl;
                else {
                    unsigned subjectIndex(subjectCount - 1);
                    string subjectName = client.GetSubjectName(subjectIndex).SubjectName;
                    unsigned int markerCount = client.GetMarkerCount(subjectName).MarkerCount;
                    for (unsigned markerIndex(0); markerIndex < markerCount; ++markerIndex)
                        markerNamesFromNexus.push_back(client.GetMarkerName(subjectName, markerIndex).MarkerName);
                    isFrameComplete = true;
                } //end else
            }

            cout << ".";
        } while (!isFrameComplete);
        cout << "\nFound " << markerNamesFromNexus.size() << " markers" << endl;
    }


	void DataFromNexus::waitForEmgs(VDS::Client& client) {
		cout << "EMG device initialisation" << endl;
		while (client.GetFrame().Result != VDS::Result::Success) {
			//       client.GetFrame();
			cout << "Wait for EMG devices..." << endl;
			//   std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		while (client.GetDeviceCount().DeviceCount < 1) {
			client.GetFrame();
		}
		unsigned nDevices(client.GetDeviceCount().DeviceCount);
		for (unsigned i(0); i < nDevices; ++i) {
			string name(client.GetDeviceName(i).DeviceName);
			cout << "Device... " << name << endl;
			std::transform(name.begin(), name.end(), name.begin(), ::tolower);
			if (name.find("emg") != std::string::npos || name.find("dts") != std::string::npos) {
				emgDeviceName_ = client.GetDeviceName(i).DeviceName;
				unsigned nChannels(client.GetDeviceOutputCount(emgDeviceName_).DeviceOutputCount);
				vector<string> emgChannelNamesFromNexus;
				cout << "EMG channel names available in Nexus:\n";
				for (unsigned ch(0); ch < nChannels; ++ch) {
					string chName{ client.GetDeviceOutputName(emgDeviceName_, ch).DeviceOutputName };
					if (chName == "v")
						chName = "EMG" + std::to_string(ch);
					cout << "#" << ch << chName << endl;
					emgChannelNamesFromNexus.push_back(chName);
				}

				cout << "Found EMG device: " << emgDeviceName_ << endl;
				cout << "Output: " << endl;
				//if I don't specity any emg, I just get them all
				if (emgChannelNames_.empty())
					emgChannelNames_ = emgChannelNamesFromNexus;
				for (auto v : emgChannelNames_) {
					cout << v << " ... ";
					//otherwise I just use the channels specified by the user
					if (std::find(emgChannelNamesFromNexus.begin(), emgChannelNamesFromNexus.end(), v) != emgChannelNamesFromNexus.end()) {
						enabledEmgChannelNames_.push_back(v);
						cout << "enabled" << endl;
					}
					else
						cout << "disabled" << endl;
				}
			}
		}
		
		emgSubSamples_ = client.GetDeviceOutputSubsamples(emgDeviceName_, 
			client.GetDeviceOutputName(emgDeviceName_, 0).DeviceOutputName).DeviceOutputSubsamples;
	}

    void DataFromNexus::getFrame(VDS::Client& client) {

        while (!(client.GetFrame().Result == VDS::Result::Success))
            cout << ".";
        unsigned currentFrameNumber = client.GetFrameNumber().FrameNumber;
        frameNumber_ = currentFrameNumber + lastFrameNumberOfTheLoop_;

        if (frameNumber_ < previousFrameNumber_ && increaseFrameNumbers_) {
            lastFrameNumberOfTheLoop_ += previousFrameNumber_;
            frameNumber_ = currentFrameNumber + lastFrameNumberOfTheLoop_;
        }

        previousFrameNumber_ = frameNumber_;
    }

    void DataFromNexus::pushToLatency(const std::string& key, double value) {

        auto it(latencyData_.find(key));
        if (it == latencyData_.end()) {
            latencyData_.insert(latencyData_.begin(), std::make_pair(key, std::vector<double>{}));
            it = latencyData_.find(key);
        }
        it->second.push_back(value);
    }

    void DataFromNexus::updateLatency(VDS::Client& client) {

        pushToLatency("Total", client.GetLatencyTotal().Total);
        for (unsigned i(0); i < client.GetLatencySampleCount().Count; ++i)
        {
            std::string name = client.GetLatencySampleName(i).Name;
            double value = client.GetLatencySampleValue(name).Value;
            pushToLatency(name, value);
        }
    }

    void DataFromNexus::printLatencyData(const std::string& filename) const {

        std::ofstream outF(filename);
        for (auto& e : latencyData_) {
            outF << "Name     : " << e.first << endl;
            outF << "#frames  : " << e.second.size() << std::endl;
            outF << "mean (s) : " << getMean(e.second) << std::endl;
            outF << "std (s)  : " << getStd(e.second) << std::endl;
        }
        outF.close();
    }

    void DataFromNexus::pushMarkerData(VDS::Client& client) {

        auto rate = client.GetFrameRate();

        unsigned subjectCount = client.GetSubjectCount().SubjectCount;
        if (subjectCount != 1)
            cout << "Warning: " << subjectCount << " subject(s) in the capture volume" << endl;
        else {

            unsigned subjectIndex(subjectCount - 1);
            string subjectName = client.GetSubjectName(subjectIndex).SubjectName;
            unsigned int markerCount = client.GetMarkerCount(subjectName).MarkerCount;
            MarkerSetFrame currentFrame;

            currentFrame.data.resize(markersToStream_.size());
            for (unsigned markerIndex{ 0 }; markerIndex < markerCount; ++markerIndex) {

                string markerName = client.GetMarkerName(subjectName, markerIndex).MarkerName;
                VDS::Output_GetMarkerGlobalTranslation markerGlobalTranslation = client.GetMarkerGlobalTranslation(subjectName, markerName);
                MarkerData marker(
                    markerGlobalTranslation.Translation[0] * fromNexusToModelLengthConversion_,
                    markerGlobalTranslation.Translation[1] * fromNexusToModelLengthConversion_,
                    markerGlobalTranslation.Translation[2] * fromNexusToModelLengthConversion_
                    );
                marker.setOccluded(markerGlobalTranslation.Occluded);
                //to change with a rtosim::Mapper
                auto posIt = std::find(markersToStream_.begin(), markersToStream_.end(), markerName);
                if (posIt != markersToStream_.end())
                    currentFrame.data.at(std::distance(markersToStream_.begin(), posIt)) = marker;
            } //end for markerIndex

            //TODO: change this time
            currentFrame.time = 1. / rate.FrameRateHz*frameNumber_;
            outputMarkerSetQueue_->push(currentFrame);

        } //end subjectCount else
    }

    void DataFromNexus::pushEndOfData(VDS::Client& client) {

        if (useMarkerData_)
            outputMarkerSetQueue_->push(EndOfData::get<MarkerSetFrame>());
        if (useGrfData_)
            outputGrfQueue_->push(EndOfData::get<MultipleExternalForcesFrame>());
		if (useEmgData_)
			outputEmgQueue_->push(EndOfData::get<EmgFrame>());
    }
	  
    void DataFromNexus::pushForcePlateData(VDS::Client& client) {

	    auto rate = client.GetFrameRate();
        //Force Plates
        // Output the force plate information.
        const unsigned forcePlateSubsamples = client.GetForcePlateSubsamples(0).ForcePlateSubsamples;
        for (unsigned int ssIdx = 0; ssIdx < forcePlateSubsamples; ++ssIdx)    {
            MultipleExternalForcesFrame currentGrfFrame;
            currentGrfFrame.time = 1. / rate.FrameRateHz*(frameNumber_ + 1. / forcePlateSubsamples * ssIdx);
            for (unsigned fpIdx = 0; fpIdx < forcePlateCount_; ++fpIdx) {

                SimTK::Vec3 currentFpPos = forcePlatePositions_.at(fpIdx);

                ExternalForceData fpData;
                fpData.setSourceName(forcePlateNames_.at(fpIdx));
                VDS::Output_GetGlobalForceVector forceVector = client.GetGlobalForceVector(fpIdx, ssIdx);
                SimTK::Vec3 grfVec(0.);
                grfVec[0] = forceVector.ForceVector[0];
                grfVec[1] = forceVector.ForceVector[1];
                grfVec[2] = forceVector.ForceVector[2];

                VDS::Output_GetGlobalCentreOfPressure centreOfPressure = client.GetGlobalCentreOfPressure(fpIdx, ssIdx);
                SimTK::Vec3 grfPoint;
                grfPoint[0] = centreOfPressure.CentreOfPressure[0]; // *fromNexusToModelLengthConversion_;
                grfPoint[1] = centreOfPressure.CentreOfPressure[1]; // *fromNexusToModelLengthConversion_;
                grfPoint[2] = centreOfPressure.CentreOfPressure[2]; // *fromNexusToModelLengthConversion_;

                //calculate the values of the moment of the 'position' reference system of the force plate in the global goordinate system
                SimTK::Vec3 momentOnPosition(0.);
                momentOnPosition[0] = currentFpPos[1] * grfVec[2] - currentFpPos[2] * grfVec[1];
                momentOnPosition[1] = currentFpPos[2] * grfVec[0] - currentFpPos[0] * grfVec[2];
                momentOnPosition[2] = currentFpPos[0] * grfVec[1] - currentFpPos[1] * grfVec[0];

                VDS::Output_GetGlobalMomentVector momentVector = client.GetGlobalMomentVector(fpIdx, ssIdx);

                //calculate the correct values of the moments relatively the global goordinate system by adding the missing position moment
                SimTK::Vec3 moments(0.);
                moments[0] = momentVector.MomentVector[0] + momentOnPosition[0];
                moments[1] = momentVector.MomentVector[1] + momentOnPosition[1];
                moments[2] = momentVector.MomentVector[2] + momentOnPosition[2];

                SimTK::Vec3 grfTorque;
                grfTorque[0] = 0;
                grfTorque[1] = (moments[1] - grfVec[0] * grfPoint[2] + grfVec[2] * grfPoint[0]);
                grfTorque[2] = 0;

                fpData.setForceVector(-grfVec);
                fpData.setMoments(-moments);
                fpData.setUseApplicationPoint(true);
                fpData.setTorque(-grfTorque);
                if (fabs(grfVec[1]) < 10.) {
                    grfPoint[0] = 0; grfPoint[2] = 0;
                }

                fpData.setApplicationPoint(grfPoint);
                currentGrfFrame.data.push_back(fpData);
            }
            outputGrfQueue_->push(currentGrfFrame);
        }
    }


	void DataFromNexus::setAxisMapping(VDS::Direction::Enum x, VDS::Direction::Enum y, VDS::Direction::Enum z) {
		xMapping_ = x;
		yMapping_ = y;
		zMapping_ = z;
	}


    void DataFromNexus::setAxisMapping(VDS::Client& client) const {

        //map axis, try Backward, right, up for subject walking against the y direction of the lab..
        //this is HARDCODED for the GU Undercroft Lab.. also, the mapping makes no sense IMO,
        //I've used try and error approach to get the right one
		client.SetAxisMapping(xMapping_, yMapping_, zMapping_);
    }

	void DataFromNexus::pushEmgData(VDS::Client& client) {

		auto rate = client.GetFrameRate();
		for (unsigned int ssIdx = 0; ssIdx < emgSubSamples_; ++ssIdx) {

			EmgData currentEmgs;
			for (auto& ch : enabledEmgChannelNames_)
				currentEmgs.emplace_back(client.GetDeviceOutputValue(emgDeviceName_, ch, ssIdx).Value);
			double currentTime(1. / rate.FrameRateHz*(frameNumber_ + 1. / emgSubSamples_ * ssIdx));

			outputEmgQueue_->push({ currentTime, currentEmgs });
		}
	}


    void DataFromNexus::operator()(){

        int numberOfFrame(0);
        int frameCounter(0);
        VDS::Client client;

        connectToServer(client);
        initialiseDataToStream(client);
        setAxisMapping(client);

        //receive the first full frame
        doneWithSubscriptions_.wait();

        auto start = std::chrono::system_clock::now();

        while (runCondition_.getRunCondition()) {

            getFrame(client);
            updateLatency(client);
            if (useMarkerData_)
                pushMarkerData(client);
            if (useGrfData_)
                pushForcePlateData(client);
			if (useEmgData_)
				pushEmgData(client);
        }

        pushEndOfData(client);
        client.DisableMarkerData();
        client.DisableDeviceData();
        cout << "Disconnecting from Nexus..." << std::endl;
        client.Disconnect();
        cout << "Disconnected" << std::endl;
        doneWithExecution_.wait();
    }

    DataFromNexus::~DataFromNexus() {

        cout << "Closing DataFromNexus..." << endl;
    }
}