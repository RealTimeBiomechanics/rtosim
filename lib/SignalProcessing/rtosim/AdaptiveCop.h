#ifndef rtosim_AdaptiveCop_h
#define rtosim_AdaptiveCop_h

#include "rtosim/queue/MarkerSetQueue.h"
#include "rtosim/queue/MultipleExternalForcesQueue.h"
#include "rtosim/concurrency/Concurrency.h"
#include "rtosim/ExternalLoadProperties.h"
#include <map>

namespace rtosim{

    class AdaptiveCop {

    public:
        AdaptiveCop(
            MarkerSetQueue& inputMarkerSetQueue,
            MultipleExternalForcesQueue& inputExternalForcesQueue,
            MultipleExternalForcesQueue& outputExternalForcesQueue,
            Concurrency::Latch& doneWithSubscriptions,
            Concurrency::Latch& doneWithExecution,
            const std::map<std::string, std::string>& forcePlatesToMarkerNames,
            const std::vector<std::string>& markerNames);

        AdaptiveCop(
            MarkerSetQueue& inputMarkerSetQueue,
            MultipleExternalForcesQueue& inputExternalForcesQueue,
            MultipleExternalForcesQueue& outputExternalForcesQueue,
            Concurrency::Latch& doneWithSubscriptions,
            Concurrency::Latch& doneWithExecution,
            const std::string& osimModelFilename,
            const std::string& externalLoadsFilename);

        AdaptiveCop(
            MarkerSetQueue& inputMarkerSetQueue,
            MultipleExternalForcesQueue& inputExternalForcesQueue,
            MultipleExternalForcesQueue& outputExternalForcesQueue,
            Concurrency::Latch& doneWithSubscriptions,
            Concurrency::Latch& doneWithExecution,
            const std::string& osimModelFilename,
            const std::vector<ExternalForceProperties>& externalForcesProperties);

        void operator()();

    private:
        void updateForcePlatesToMarkerIndex(const std::vector<ExternalForceProperties>& externalForcesProperties, const std::string& osimModelFilename);
        void adaptCop(ExternalForceData& forceData, const MarkerData& markerData) const;
        MarkerSetQueue& inputMarkerSetQueue_;
        MultipleExternalForcesQueue& outputExternalForcesQueue_;
        MultipleExternalForcesQueue& inputExternalForcesQueue_;
        Concurrency::Latch& doneWithSubscriptions_;
        Concurrency::Latch& doneWithExecution_;
        std::map<std::string, unsigned> forcePlatesToMarkerIndex_;
    };
}

#endif