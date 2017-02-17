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

#ifndef rtosim_DataFromNexus_h
#define rtosim_DataFromNexus_h

#include <string>
#include <thread>
#include <memory>
#include <OpenSim/OpenSim.h>
#include <map>
#include <Client.h> //from ViconSDK
#include "rtosim/Mapper.h"
#include "rtb/concurrency/Concurrency.h"
#include "rtosim/queue/MultipleExternalForcesQueue.h"
#include "rtosim/queue/MarkerSetQueue.h"
#include "rtosim/FlowControl.h"


namespace VDS = ViconDataStreamSDK::CPP;
namespace rtosim {

    class DataFromNexus{

    public:
        DataFromNexus() = delete;
        DataFromNexus(DataFromNexus&) = delete;

        DataFromNexus(
            MarkerSetQueue& outputMarkerSetQueue,
            rtb::Concurrency::Latch& doneWithSubscriptions,
            rtb::Concurrency::Latch& doneWithExecution,
            FlowControl& runCondition,
            const std::string& osimFilename,
            const std::string& hostname = "127.0.0.1:801");

        DataFromNexus(
            MultipleExternalForcesQueue& outputGrfQueue,
            rtb::Concurrency::Latch& doneWithSubscriptions,
            rtb::Concurrency::Latch& doneWithExecution,
            FlowControl& runCondition,
            const std::string& osimFilename,
            const std::string& hostname = "127.0.0.1:801");

        DataFromNexus(
            MarkerSetQueue& outputMarkerSetQueue,
            MultipleExternalForcesQueue& outputGrfQueue,
            rtb::Concurrency::Latch& doneWithSubscriptions,
            rtb::Concurrency::Latch& doneWithExecution,
            FlowControl& runCondition,
            const std::string& osimFilename,
            const std::string& hostname = "127.0.0.1:801");


        void setLoopMode(bool isLoop) {
            increaseFrameNumbers_ = isLoop;
        }
        virtual ~DataFromNexus();
        void operator()();

        void printLatencyData(const std::string& filename) const;

    private:

        std::vector<SimTK::Vec3> getForcePlatePosition() const;
        void setAxisMapping(VDS::Client& client) const;
        void initialiseMarkerNames(const std::string&);
        void initialiseDataToStream(VDS::Client& client);
        void waitForForcePlates(VDS::Client& client);
        void waitForCompleteFrame(VDS::Client& client);
        void connectToServer(VDS::Client& client) const;
        void getFrame(VDS::Client& client);
        void pushMarkerData(VDS::Client& client);
        void pushForcePlateData(VDS::Client& client);
        void pushEndOfData(VDS::Client& client);
        void updateLatency(VDS::Client& client);
        void pushToLatency(const std::string& key, double value);
        MarkerSetQueue* outputMarkerSetQueue_;
        MultipleExternalForcesQueue* outputGrfQueue_;
        rtb::Concurrency::Latch& doneWithSubscriptions_;
        rtb::Concurrency::Latch& doneWithExecution_;
        FlowControl& runCondition_;
        std::string hostName_;
        std::vector<std::string> markerNamesFromModel_;
        std::vector<std::string> forcePlateNames_;
        //    SimTK::Vec3 originTraslation_;
        double fromNexusToModelLengthConversion_;
        bool useMarkerData_, useGrfData_;
        unsigned forcePlateCount_;
        bool increaseFrameNumbers_;
        unsigned previousFrameNumber_, frameNumber_, lastFrameNumberOfTheLoop_;
        std::map<std::string, std::vector<double>> latencyData_;
    };
}

#endif
