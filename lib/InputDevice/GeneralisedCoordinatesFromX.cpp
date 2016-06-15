#include "rtosim/GeneralisedCoordinatesFromX.h"
#include "rtosim/GeneralisedCoordinatesData.h"
#include "rtosim/EndOfData.h"
#include <vector>
using std::vector;

namespace rtosim {

    GeneralisedCoordinatesFromX::GeneralisedCoordinatesFromX(
        GeneralisedCoordinatesQueue& outputGeneralisedCoordinatesQueue,
        Concurrency::Latch& doneWithSubscriptions,
        Concurrency::Latch& doneWithExecution) :
        outputGeneralisedCoordinatesQueue_(outputGeneralisedCoordinatesQueue),
        doneWithSubscriptions_(doneWithSubscriptions),
        doneWithExecution_(doneWithExecution)

    { }

    void GeneralisedCoordinatesFromX::updateGeneralisedCoordinates(const GeneralisedCoordinatesData& currentGeneralisedCoordinatesData, double currentTime) {

        GeneralisedCoordinatesFrame frame{ currentTime, currentGeneralisedCoordinatesData };
        outputGeneralisedCoordinatesQueue_.push(frame);
    }

    void GeneralisedCoordinatesFromX::doneWithSubscriptions() {

        doneWithSubscriptions_.wait();
    }

    void GeneralisedCoordinatesFromX::doneWithExecution() {

        doneWithExecution_.wait();
    }

    void GeneralisedCoordinatesFromX::sendEndOfData() {

        outputGeneralisedCoordinatesQueue_.push(EndOfData::get<GeneralisedCoordinatesFrame>());
    }
}
