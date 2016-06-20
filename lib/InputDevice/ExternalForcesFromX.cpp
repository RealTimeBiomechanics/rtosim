#include "rtosim/ExternalForcesFromX.h"
#include "rtosim/EndOfData.h"
#include <vector>
using std::vector;

namespace rtosim {

    ExternalForcesFromX::ExternalForcesFromX(
        MultipleExternalForcesQueue& outputMultipleExternalForcesQueue,
        Concurrency::Latch& doneWithSubscriptions,
        Concurrency::Latch& doneWithExecution) :
        outputMultipleExternalForcesQueue_(outputMultipleExternalForcesQueue),
        doneWithSubscriptions_(doneWithSubscriptions),
        doneWithExecution_(doneWithExecution)
    { }

    void ExternalForcesFromX::updateExternalForces(const MultipleExternalForcesData& currentExternalForceData, double currentTime) {

        MultipleExternalForcesFrame frame{ currentTime, currentExternalForceData };
        outputMultipleExternalForcesQueue_.push(frame);
    }

    void ExternalForcesFromX::sendEndOfData() {

        outputMultipleExternalForcesQueue_.push(EndOfData::get<MultipleExternalForcesFrame>());
    }

    void ExternalForcesFromX::doneWithSubscriptions() {

        doneWithSubscriptions_.wait();
    }

    void ExternalForcesFromX::doneWithExecution() {

        doneWithExecution_.wait();
    }
}
