#ifndef rtosim_GeneralisedCoordinatesFromStorageFile_h
#define rtosim_GeneralisedCoordinatesFromStorageFile_h

#include "rtosim/GeneralisedCoordinatesFromX.h"
#include "rtosim/Mapper.h"
#include <OpenSim/OpenSim.h>

namespace rtosim {
    class GeneralisedCoordinatesFromStorageFile : public GeneralisedCoordinatesFromX {
    public:
        GeneralisedCoordinatesFromStorageFile(
            GeneralisedCoordinatesQueue& outputGeneralisedCoordinatesQueue,
            Concurrency::Latch& doneWithSubscriptions,
            Concurrency::Latch& doneWithExecution,
            const std::string& osimModelFilename,
            const std::string& kinematicsStorageFilename,
            double fc = -1);
        GeneralisedCoordinatesFromStorageFile(
            GeneralisedCoordinatesQueue& outputGeneralisedCoordinatesQueue,
            Concurrency::Latch& doneWithSubscriptions,
            Concurrency::Latch& doneWithExecution,
            const std::string& osimModelFilename,
            const std::string& kinematicsStorageFilename,
            const std::vector<std::string>& coordinatesToLog,
            double fc = -1);
        virtual ~GeneralisedCoordinatesFromStorageFile() override {}
        virtual void operator()() override;
    private:
        void updateKinematicsSplines(double fc);
        OpenSim::GCVSplineSet splines_;
        OpenSim::Model model_;
        std::string kinematicsStorageFilename_;
        std::vector<double> timeColumn_;
        std::vector<std::string> coordinateNamesFromOsimModel_;
        Mapper mapper_;
    };
}

#endif
