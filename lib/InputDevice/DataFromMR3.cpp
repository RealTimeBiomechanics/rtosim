#include "rtosim/DataFromMR3.h"
#include "rtosim/EndOfData.h"
using std::begin;
using std::end;

/*
Components names from MR3 player

device.player.player.mm_gait;line.human.head;type.input.motion.rot;
device.player.player.mm_gait;line.human.spine.upper;type.input.motion.rot;
device.player.player.mm_gait;line.human.lt.arm.upper;type.input.motion.rot;
device.player.player.mm_gait;line.human.lt.arm.fore;type.input.motion.rot;
device.player.player.mm_gait;line.human.lt.arm.hand;type.input.motion.rot;
device.player.player.mm_gait;line.human.rt.arm.upper;type.input.motion.rot;
device.player.player.mm_gait;line.human.rt.arm.fore;type.input.motion.rot;
device.player.player.mm_gait;line.human.rt.arm.hand;type.input.motion.rot;
device.player.player.mm_gait;line.human.spine.lower;type.input.motion.rot;
device.player.player.mm_gait;line.human.pelvis;type.input.motion.rot;
device.player.player.mm_gait;line.human.lt.leg.thigh;type.input.motion.rot;
device.player.player.mm_gait;line.human.lt.leg.shank;type.input.motion.rot;
device.player.player.mm_gait;line.human.lt.leg.foot;type.input.motion.rot;
device.player.player.mm_gait;line.human.rt.leg.thigh;type.input.motion.rot;
device.player.player.mm_gait;line.human.rt.leg.shank;type.input.motion.rot;
device.player.player.mm_gait;line.human.rt.leg.foot;type.input.motion.rot;
*/

namespace rtosim {

DataFromMR3::DataFromMR3(
    OrientationSetQueue& outputOrientationSetQueue,
    ScalarVectorQueue& outputEmgQueue,
    rtb::Concurrency::Latch& doneWithSubscriptions,
    rtb::Concurrency::Latch& doneWithExecution,
    FlowControl& runCondition,
    const std::vector<std::string>& orientationNames,
    const std::vector<std::string>& emgNames) :
    outputOrientationSetQueue_(outputOrientationSetQueue),
    outputEmgQueue_(outputEmgQueue),
    doneWithSubscriptions_(doneWithSubscriptions),
    doneWithExecution_(doneWithExecution),
    runCondition_(runCondition),
    orientationNames_(orientationNames),
    emgNames_(emgNames)
{}


void DataFromMR3::operator()() {

    MR3tools::DeviceInterface device(true);
    device.connect();
    device.enableMotionData();
    device.enableEmgData();
    
    auto orientationNamesFromDevice(device.getEnabledMotionComponentNames());
    if (orientationNames_.empty())
        orientationNames_ = orientationNamesFromDevice;
    std::cout << "MotionData names from device\n";
    for (auto& n : orientationNamesFromDevice)
        std::cout << n << std::endl;
    orientationMapper_.setNames(orientationNamesFromDevice, orientationNames_);

    auto emgNamesFromDevice(device.getEnabledEmgComponentNames());
    if (emgNames_.empty())
        emgNames_= emgNamesFromDevice;
    std::cout << "EmgData names from device\n";
    for (auto& n : emgNamesFromDevice)
        std::cout << n << std::endl;
    emgMapper_.setNames(emgNamesFromDevice, emgNames_);
    device.activate();
    doneWithSubscriptions_.wait();
    while (runCondition_.getRunCondition()) {
        auto data = device.getData();
        pushOrientationData(data);
        pushEmgData(data);


    }
    outputOrientationSetQueue_.push(EndOfData::get<OrientationSetFrame>());
    device.disconnect();
    doneWithExecution_.wait();
}

void DataFromMR3::pushEmgData(const MR3tools::MR3Frame& dataFrame) {
    ScalarVectorFrame emgFrame;
    if (!dataFrame.emgData.empty()) {
        emgFrame.time = dataFrame.time;
        std::transform(begin(dataFrame.emgData), end(dataFrame.emgData),
            std::back_inserter(emgFrame.data), [](auto& e) {
            return e[0];
        });
        emgFrame.data = emgMapper_.map(emgFrame.data);
        outputEmgQueue_.push(emgFrame);
    }
}


void DataFromMR3::pushOrientationData(const MR3tools::MR3Frame& dataFrame) {
    OrientationSetFrame orientationFrame;
    if (!dataFrame.motionData.empty()) {

        orientationFrame.time = dataFrame.time;
        std::transform(begin(dataFrame.motionData), end(dataFrame.motionData),
            std::back_inserter(orientationFrame.data), [](auto& e) {
            return DataFromMR3::getFullQuaterion(e);
        });
        orientationFrame.data = orientationMapper_.map(orientationFrame.data);
        outputOrientationSetQueue_.push(orientationFrame);
    }
}


}