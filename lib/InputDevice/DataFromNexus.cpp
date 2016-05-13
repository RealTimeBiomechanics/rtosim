#include "vosl/SharedData.h"
#include "vosl/DataFromNexus.h"
#include "vosl/MotionCaptureSystemInfo.h"
#include "vosl/MarkerSetQueue.h"
#include <ceinms/ExternalForceData.h>
#include <ceinms/ExternalForceDataSetup.h>
#include <ceinms/queues/MultipleExternalForcesQueue.h>

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
#include <ceinms/ArrayConverter.h>
#include <OpenSim/OpenSim.h>
#include <thread>
#include "Client.h"

std::string adapt(bool v) {

    std::string retStr("True");
    if (!v)
        retStr = "False";
    return retStr;
}

namespace vosl {

    DataFromNexus::DataFromNexus(
        MarkerSetQueue& outputMarkerSetQueue,
        ceinms::Concurrency::Latch& doneWithSubscriptions,
        ceinms::Concurrency::Latch& doneWithExecution,
        const std::string& osimFilename,
        const std::string& hostname) :
        useMarkerData_(true),
        outputMarkerSetQueue_(&outputMarkerSetQueue),
        useGrfData_(false),
        outputGrfQueue_(nullptr),
        useEmgData_(false),
        outputEmgQueue_(nullptr),
        doneWithSubscriptions_(doneWithSubscriptions),
        doneWithExecution_(doneWithExecution),
        hostName_(hostname),
        fromNexusToModelLengthConversion_(1.),
        lastFrameNumberOfTheLoop_(0),
        previousFrameNumber_(0)
    {
        initialiseMarkerNames(osimFilename);
    }

    DataFromNexus::DataFromNexus(
        ceinms::MultipleExternalForcesQueue& outputGrfQueue,
        ceinms::Concurrency::Latch& doneWithSubscriptions,
        ceinms::Concurrency::Latch& doneWithExecution,
        const std::string& hostname) :
        useMarkerData_(false),
        outputMarkerSetQueue_(nullptr),
        useGrfData_(true),
        outputGrfQueue_(&outputGrfQueue),
        useEmgData_(false),
        outputEmgQueue_(nullptr),
        doneWithSubscriptions_(doneWithSubscriptions),
        doneWithExecution_(doneWithExecution),
        hostName_(hostname),
        fromNexusToModelLengthConversion_(1.),
        lastFrameNumberOfTheLoop_(0),
        previousFrameNumber_(0){}

    DataFromNexus::DataFromNexus(
        ceinms::EmgQueue& outputEmgQueue,
        ceinms::Concurrency::Latch& doneWithSubscriptions,
        ceinms::Concurrency::Latch& doneWithExecution,
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
        hostName_(hostname),
        fromNexusToModelLengthConversion_(1.),
        lastFrameNumberOfTheLoop_(0),
        previousFrameNumber_(0){}

    DataFromNexus::DataFromNexus(MarkerSetQueue& outputMarkerSetQueue,
        ceinms::MultipleExternalForcesQueue& outputGrfQueue,
        ceinms::Concurrency::Latch& doneWithSubscriptions,
        ceinms::Concurrency::Latch& doneWithExecution,
        const std::string& osimFilename,
        const std::string& hostname) :
        useMarkerData_(true),
        outputMarkerSetQueue_(&outputMarkerSetQueue),
        useGrfData_(true),
        outputGrfQueue_(&outputGrfQueue),
        useEmgData_(false),
        outputEmgQueue_(nullptr),
        doneWithSubscriptions_(doneWithSubscriptions),
        doneWithExecution_(doneWithExecution),
        hostName_(hostname),
        fromNexusToModelLengthConversion_(1.),
        lastFrameNumberOfTheLoop_(0),
        previousFrameNumber_(0)

    {
        initialiseMarkerNames(osimFilename);
    }

    DataFromNexus::DataFromNexus(
        MarkerSetQueue& outputMarkerSetQueue,
        ceinms::MultipleExternalForcesQueue& outputGrfQueue,
        ceinms::EmgQueue& outputEmgQueue,
        ceinms::Concurrency::Latch& doneWithSubscriptions,
        ceinms::Concurrency::Latch& doneWithExecution,
        const std::string& osimFilename,
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
        hostName_(hostname),
        fromNexusToModelLengthConversion_(1.),
        lastFrameNumberOfTheLoop_(0),
        previousFrameNumber_(0)
    {
        initialiseMarkerNames(osimFilename);
    }

    void DataFromNexus::initialiseMarkerNames(const std::string& osimFilename) {

        OpenSim::Model model(osimFilename);
        OpenSim::Array<std::string> markerNamesArray;
        const_cast<OpenSim::MarkerSet&>(model.getMarkerSet()).getMarkerNames(markerNamesArray);
        ceinms::ArrayConverter::toStdVector(markerNamesArray, markerNamesFromModel_);
        if (model.getLengthUnits().getType() == OpenSim::Units::Meters)
            fromNexusToModelLengthConversion_ = 1. / 1000.;
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
        if (useEmgData_ || useGrfData_)
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
        while (forcePlateCount_ < 1 && Shared::flowControl.getRunCondition()) {
            client.GetFrame();
            auto forcePlatesInfo = client.GetForcePlateCount();
            if (forcePlatesInfo.Result == VDS::Result::Success) {
                cout << "Waiting for devices..." << endl;
                forcePlateCount_ = forcePlatesInfo.ForcePlateCount;
            }
        }
        cout << "Found " << forcePlateCount_ << " force plates" << endl;
        Shared::motionCaptureSystemInfo.setNumberOfForcePlates(forcePlateCount_);

        for (unsigned i(0); i < client.GetDeviceCount().DeviceCount; ++i)
            if (client.GetDeviceName(i).DeviceType == VDS::DeviceType::ForcePlate) {
                std::string fpName = client.GetDeviceName(i).DeviceName;
                forcePlateNames_.push_back(fpName);
                cout << fpName << endl;
            }
        Shared::motionCaptureSystemInfo.setForcePlateNames(forcePlateNames_);
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
                    for (unsigned markerIndex = 0; markerIndex < markerCount; ++markerIndex)
                        markerNamesFromNexus.push_back(client.GetMarkerName(subjectName, markerIndex).MarkerName);
                    isFrameComplete = true;
                } //end else
            }

            cout << ".";
        } while (!isFrameComplete);
        cout << "\nFound " << markerNamesFromNexus.size() << " markers" << endl;

        Shared::motionCaptureSystemInfo.setVideoSamplingFrequency(client.GetFrameRate().FrameRateHz);
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
            //     cout << "Device... " << name << endl;
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            if (name.find("emg") != std::string::npos) {
                emgDeviceName_ = client.GetDeviceName(i).DeviceName;
                unsigned nChannels(client.GetDeviceOutputCount(emgDeviceName_).DeviceOutputCount);
                vector<string> emgChannelNamesFromNexus;
                for (unsigned ch(0); ch < nChannels; ++ch) {
                    emgChannelNamesFromNexus.push_back(client.GetDeviceOutputName(emgDeviceName_, ch).DeviceOutputName);
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
    }

    void  DataFromNexus::getFrame(VDS::Client& client) {

        while (!(client.GetFrame().Result == VDS::Result::Success))
            cout << ".";
        unsigned currentFrameNumber = client.GetFrameNumber().FrameNumber;
        frameNumber_ = currentFrameNumber + lastFrameNumberOfTheLoop_;

        if (frameNumber_ < previousFrameNumber_ && increaseFrameNumbers_) {
            lastFrameNumberOfTheLoop_ += previousFrameNumber_;
            frameNumber_ = currentFrameNumber + lastFrameNumberOfTheLoop_;
        }

        previousFrameNumber_ = frameNumber_;
        //   cout << "frame# " << frameNumber_ << endl;
    }

    void DataFromNexus::pushMarkerData(VDS::Client& client) {

        //        auto frameNumber = client.GetFrameNumber();
        auto rate = client.GetFrameRate();

        unsigned subjectCount = client.GetSubjectCount().SubjectCount;
        if (subjectCount != 1)
            cout << "Warning: " << subjectCount << " subject(s) in the capture volume" << endl;
        else {

            unsigned subjectIndex(subjectCount - 1);
            string subjectName = client.GetSubjectName(subjectIndex).SubjectName;
            unsigned int markerCount = client.GetMarkerCount(subjectName).MarkerCount;
            MarkerSetFrame currentFrame;

            currentFrame.data.resize(markerNamesFromModel_.size());
            for (unsigned markerIndex{ 0 }; markerIndex < markerCount; ++markerIndex) {

                string markerName = client.GetMarkerName(subjectName, markerIndex).MarkerName;
                VDS::Output_GetMarkerGlobalTranslation markerGlobalTranslation = client.GetMarkerGlobalTranslation(subjectName, markerName);
                Marker marker(
                    markerGlobalTranslation.Translation[0] * fromNexusToModelLengthConversion_,
                    markerGlobalTranslation.Translation[1] * fromNexusToModelLengthConversion_,
                    markerGlobalTranslation.Translation[2] * fromNexusToModelLengthConversion_
                    );
                marker.setOccluded(markerGlobalTranslation.Occluded);
                //to change with a ceinms::Mapper
                auto posIt = std::find(markerNamesFromModel_.begin(), markerNamesFromModel_.end(), markerName);
                if (posIt != markerNamesFromModel_.end())
                    currentFrame.data.at(std::distance(markerNamesFromModel_.begin(), posIt)) = marker;
            } //end for markerIndex

            //TODO: change this time!!!
            currentFrame.time = 1. / rate.FrameRateHz*frameNumber_;
            outputMarkerSetQueue_->push(currentFrame);
            //       numberOfFrame++;
            //       frameCounter++;
            /*disable this one.. because I could have provlem with multiple threads writing on std::cout
            if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start)) > std::chrono::milliseconds(1000)) {
            cout << "\n\n NEXUS:" << frameCounter << "\n\n" << std::flush;
            start = std::chrono::system_clock::now();
            frameCounter = 0;
            }
            */
        } //end subjectCount else
    }

    void DataFromNexus::pushForcePlateData(VDS::Client& client) {

        //        SimTK::Vec3 fpPos2(.4826, .949, -.026);
        //        SimTK::Vec3 fpPos1(.3783, .949, -.026);

        SimTK::Vec3 fpPos2(-.949, -.026, -.4826);
        SimTK::Vec3 fpPos1(-.949, -.026, -.3783);
        vector<SimTK::Vec3> pfPos{ fpPos1, fpPos2 };

        //       auto frameNumber = client.GetFrameNumber();
        auto rate = client.GetFrameRate();
        //Force Plates
        // Output the force plate information.
        const unsigned forcePlateSubsamples = client.GetForcePlateSubsamples(0).ForcePlateSubsamples;
        for (unsigned int ssIdx = 0; ssIdx < forcePlateSubsamples; ++ssIdx)    {
            ceinms::MultipleExternalForcesFrame currentGrfFrame;
            currentGrfFrame.time = 1. / rate.FrameRateHz*(frameNumber_ + 1. / forcePlateSubsamples * ssIdx);
            for (unsigned fpIdx = 0; fpIdx < forcePlateCount_; ++fpIdx) {

                SimTK::Vec3 currentFpPos = pfPos.at(fpIdx);

                ceinms::ExternalForceData fpData;
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
                //       moments[0] = grfVec[2] * grfPoint[1] - grfVec[1] * grfPoint[2];
                //       moments[1] = grfVec[0] * grfPoint[2] - grfVec[2] * grfPoint[0];
                //       moments[2] = grfVec[1] * grfPoint[0] - grfVec[0] * grfPoint[1];
                //          moments[0] = 0;
                //         moments[1] = 0;
                //        moments[2] = 0;

                //      moments[2] = -momentVector.MomentVector[2];

                SimTK::Vec3 grfTorque;
                grfTorque[0] = 0;
                grfTorque[1] = (moments[1] - grfVec[0] * grfPoint[2] + grfVec[2] * grfPoint[0]);
                grfTorque[2] = 0;
                //    fpData.setTorque(grfTorque);

                fpData.setForceVector(-grfVec);
                fpData.setMoments(-moments);
                fpData.setUseApplicationPoint(true);
                fpData.setTorque(-grfTorque);
                if (abs(grfVec[1]) < 10) {
                    grfPoint[0] = 0; grfPoint[2] = 0;
                }

                fpData.setApplicationPoint(grfPoint);
                currentGrfFrame.data.push_back(fpData);
            }
            outputGrfQueue_->push(currentGrfFrame);
        }
    }

    void DataFromNexus::pushEmgData(VDS::Client& client) {

        //      auto frameNumber = client.GetFrameNumber();
        auto rate = client.GetFrameRate();
        //Force Plates
        // Output the force plate information.
        const unsigned emgSubSamples = client.GetDeviceOutputSubsamples(emgDeviceName_, enabledEmgChannelNames_.front()).DeviceOutputSubsamples;
        for (unsigned int ssIdx = 0; ssIdx < emgSubSamples; ++ssIdx)    {

            ceinms::EmgData currentEmgs;
            for (auto& ch : enabledEmgChannelNames_)
                currentEmgs.emplace_back(client.GetDeviceOutputValue(emgDeviceName_, ch, ssIdx).Value);
            double currentTime(1. / rate.FrameRateHz*(frameNumber_ + 1. / emgSubSamples * ssIdx));

            outputEmgQueue_->push({ currentTime, currentEmgs });
        }
    }

    void DataFromNexus::operator()(){

        int numberOfFrame(0);
        int frameCounter(0);
        VDS::Client client;

        connectToServer(client);
        initialiseDataToStream(client);
        //map axis, try Backward, right, up for subject walking against the y direction of the lab..
        //thi is HARDCODED for the GU Bunker Lab.. also, the mapping makes no sense, I've used try and error approach to get the right one
        client.SetAxisMapping(
            VDS::Direction::Right,
            VDS::Direction::Up,
            VDS::Direction::Backward);

        //receive the first full frame
        doneWithSubscriptions_.wait();

        auto start = std::chrono::system_clock::now();

        while (Shared::flowControl.getRunCondition()) {

            getFrame(client);
            if (useMarkerData_)
                pushMarkerData(client);
            if (useGrfData_)
                pushForcePlateData(client);
            if (useEmgData_)
                pushEmgData(client);
        }

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