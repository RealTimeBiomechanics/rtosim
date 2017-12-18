#ifndef rtosim_DataFromMR3_h
#define rtosim_DataFromMR3_h

#include <string>
#include <thread>
#include <memory>
#include <OpenSim/OpenSim.h>
#include <map>
#include "rtb/Concurrency/Concurrency.h"
#include "rtosim/queue/OrientationSetQueue.h"
#include "rtosim/queue/ScalarVectorQueue.h"
#include "rtosim/FlowControl.h"
#include "rtosim/Mapper.h"
#include <MR3tools/MR3tools.h>

namespace rtosim {

class DataFromMR3 {

public:
    DataFromMR3() = delete;
    DataFromMR3(DataFromMR3&) = delete;

    DataFromMR3(
        OrientationSetQueue& outputOrientationSetQueue,
        ScalarVectorQueue& outputEmgQueue,
        rtb::Concurrency::Latch& doneWithSubscriptions,
        rtb::Concurrency::Latch& doneWithExecution,
        FlowControl& runCondition,
        const std::vector<std::string>& orientationNames,
        const std::vector<std::string>& emgNames);
    void operator()();
private:

    template<typename T>
    static SimTK::Quaternion getFullQuaterion(const T& q);
    void pushOrientationData(const MR3tools::MR3Frame& dataFrame);
    void pushEmgData(const MR3tools::MR3Frame& dataFrame);

    OrientationSetQueue& outputOrientationSetQueue_;
    ScalarVectorQueue& outputEmgQueue_;
    rtb::Concurrency::Latch& doneWithSubscriptions_;
    rtb::Concurrency::Latch& doneWithExecution_;
    FlowControl& runCondition_;
    std::vector<std::string> orientationNames_;
    std::vector<std::string> emgNames_;
    Mapper orientationMapper_;
    Mapper emgMapper_;

};

template<typename T>
SimTK::Quaternion DataFromMR3::getFullQuaterion(const T& q) {
    typename T::value_type w = std::sqrt(1 - q[0] * q[0] - q[1] * q[1] - q[2] * q[2]);
    SimTK::Quaternion quat{ w, q[0], q[1], q[2] };
    quat = quat.normalize();
    return quat;
}

}

#endif