#include "rtosim/OrientationsFromMR3.h"
#include "rtosim/EndOfData.h"

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


    OrientationsFromMR3::OrientationsFromMR3(
        OrientationSetQueue& outputOrientationSetQueue,
        rtb::Concurrency::Latch& doneWithSubscriptions,
        rtb::Concurrency::Latch& doneWithExecution,
        FlowControl& runCondition,
        const std::vector<std::string>& orientationNames):
        outputOrientationSetQueue_(outputOrientationSetQueue),
        doneWithSubscriptions_(doneWithSubscriptions),
        doneWithExecution_(doneWithExecution),
        runCondition_(runCondition),
        orientationNames_(orientationNames)
    {}


    void OrientationsFromMR3::operator()() {
       
        MR3tools::DeviceInterface device(true);
        device.connect();
        device.enableMotionData();
        auto namesFromDevice(device.getEnabledMotionComponentNames());
        if (orientationNames_.empty())
            orientationNames_ = namesFromDevice;
        std::cout << "Names from device\n";
        for (auto& n : namesFromDevice)
            std::cout << n << std::endl;
        mapper.setNames(namesFromDevice, orientationNames_);
        device.activate();

        doneWithSubscriptions_.wait();
        while (runCondition_.getRunCondition()) {
            auto data = device.getData();
            OrientationSetFrame frame;
            frame.time = data.time;
            data.motionData = mapper.map(data.motionData);
            for (auto& e : data.motionData) {
                frame.data.emplace_back(getFullQuaterion(e));
            }
            outputOrientationSetQueue_.push(frame);
        }
        outputOrientationSetQueue_.push(EndOfData::get<OrientationSetFrame>());
        device.disconnect();
        doneWithExecution_.wait();
    }
}