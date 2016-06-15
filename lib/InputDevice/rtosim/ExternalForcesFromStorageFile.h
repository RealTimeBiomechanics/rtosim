#ifndef rtosim_ExternalForcesFromStorageFile_h
#define rtosim_ExternalForcesFromStorageFile_h

#include "rtosim/ExternalForcesFromX.h"
#include "rtosim/queue/MultipleExternalForcesQueue.h"
#include "rtosim/concurrency/Concurrency.h"
#include "rtosim/ExternalLoadProperties.h"
#include <OpenSim/OpenSim.h>
#include <string>

namespace rtosim {

    class ExternalForcesFromStorageFile : public ExternalForcesFromX {
    public:
        //to parse the external forces easily, I need an opensim model.. otherwise I have to read manually from the storage
        ExternalForcesFromStorageFile(
            MultipleExternalForcesQueue& outputMultipleExternalForcesQueue,
            Concurrency::Latch& doneWithSubscriptions,
            Concurrency::Latch& doneWithExecution,
            const std::string& externalLoadsXmlFilename,
            double fc = -1);

        //using this constructor, grfFilename replace the grf in externalLoadsXml
        ExternalForcesFromStorageFile(
            MultipleExternalForcesQueue& outputMultipleExternalForcesQueue,
            Concurrency::Latch& doneWithSubscriptions,
            Concurrency::Latch& doneWithExecution,
            const std::string& externalLoadsXmlFilename,
            const std::string& grfFilename,
            double fc = -1);

        void setOutputFrequency(double frequency);
        void setSpeedFactor(double speedFactor);
        void setFramesToSkip(unsigned n);
        void operator()();

    private:
        void filter(double fc);
        SimTK::Vec3 getForce(const std::string& forceName, int timeIndex) const;
        SimTK::Vec3 getTorque(const std::string& forceName, int timeIndex) const;
        SimTK::Vec3 getApplicationPoint(const std::string& forceName, int timeIndex) const;
        std::chrono::milliseconds getSleepTime() const;

        ExternalLoadProperties externalLoadProperties_;
        OpenSim::Storage externalForcesStorage_;
        unsigned framesToSkip_;
        double speedFactor_;
        unsigned sampleFrequency_;
    };
}

#endif
