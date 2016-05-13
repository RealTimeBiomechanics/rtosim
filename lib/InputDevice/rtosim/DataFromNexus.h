#ifndef vosl_DataFromNexus_h
#define vosl_DataFromNexus_h

#include <string>
#include <thread>
#include <memory>

#include "Client.h"
#include <OpenSim/OpenSim.h>
#include <ceinms/Mapper.h>
#include <ceinms/concurrency/Queue.h>
#include <ceinms/concurrency/Latch.h>
#include <ceinms/QueueData.h>
#include <ceinms/queues/MultipleExternalForcesQueue.h>
#include <ceinms/queues/EmgQueue.h>
#include "vosl/MarkerSetQueue.h"

namespace VDS = ViconDataStreamSDK::CPP;
namespace vosl {
    class DataFromNexus{

    public:
        DataFromNexus() = delete;
        DataFromNexus(DataFromNexus&) = delete;

        DataFromNexus(
            MarkerSetQueue& outputMarkerSetQueue,
            ceinms::Concurrency::Latch& doneWithSubscriptions,
            ceinms::Concurrency::Latch& doneWithExecution,
            const std::string& osimFilename,
            const std::string& hostname = "127.0.0.1:801");

        DataFromNexus(
            ceinms::MultipleExternalForcesQueue& outputGrfQueue,
            ceinms::Concurrency::Latch& doneWithSubscriptions,
            ceinms::Concurrency::Latch& doneWithExecution,
            const std::string& hostname = "127.0.0.1:801");

        /*The order of the names in emgChannelNames is important,
         *it dictates the output order of the raw emg values in
         *in the queue*/
        DataFromNexus(
            ceinms::EmgQueue& outputEmgQueue,
            ceinms::Concurrency::Latch& doneWithSubscriptions,
            ceinms::Concurrency::Latch& doneWithExecution,
            const std::vector<std::string>& emgChannelNames,
            const std::string& hostname = "127.0.0.1:801");

        DataFromNexus(
            MarkerSetQueue& outputMarkerSetQueue,
            ceinms::MultipleExternalForcesQueue& outputGrfQueue,
            ceinms::Concurrency::Latch& doneWithSubscriptions,
            ceinms::Concurrency::Latch& doneWithExecution,
            const std::string& osimFilename,
            const std::string& hostname = "127.0.0.1:801");

        DataFromNexus(
            MarkerSetQueue& outputMarkerSetQueue,
            ceinms::MultipleExternalForcesQueue& outputGrfQueue,
            ceinms::EmgQueue& outputEmgQueue,
            ceinms::Concurrency::Latch& doneWithSubscriptions,
            ceinms::Concurrency::Latch& doneWithExecution,
            const std::string& osimFilename,
            const std::vector<std::string>& emgChannelNames,
            const std::string& hostname = "127.0.0.1:801");

        void setLoopMode() {
            increaseFrameNumbers_ = true;
        }
        virtual ~DataFromNexus();
        void operator()();

    private:
        void initialiseMarkerNames(const std::string&);
        void initialiseDataToStream(VDS::Client& client);
        void waitForForcePlates(VDS::Client& client);
        void waitForEmgs(VDS::Client& client);
        void waitForCompleteFrame(VDS::Client& client);
        void connectToServer(VDS::Client& client) const;
        void getFrame(VDS::Client& client);
        void pushMarkerData(VDS::Client& client);
        void pushForcePlateData(VDS::Client& client);
        void pushEmgData(VDS::Client& client);
        MarkerSetQueue* outputMarkerSetQueue_;
        ceinms::MultipleExternalForcesQueue* outputGrfQueue_;
        ceinms::EmgQueue* outputEmgQueue_;
        ceinms::Concurrency::Latch& doneWithSubscriptions_;
        ceinms::Concurrency::Latch& doneWithExecution_;
        std::string hostName_;
        std::vector<std::string> markerNamesFromModel_;
        std::vector<std::string> emgChannelNames_; //this are the names the user asks for
        std::vector<std::string> enabledEmgChannelNames_; //this are the names required by the user that are also available
        std::vector<std::string> forcePlateNames_;
        std::string emgDeviceName_;
        //    SimTK::Vec3 originTraslation_;
        double fromNexusToModelLengthConversion_;
        bool useMarkerData_, useGrfData_, useEmgData_;
        unsigned forcePlateCount_;
        bool increaseFrameNumbers_;
        unsigned previousFrameNumber_, frameNumber_, lastFrameNumberOfTheLoop_;
    };
}

#endif
