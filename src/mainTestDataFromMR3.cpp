#include "rtosim/OrientationsFromMR3.h"
#include "rtb/Concurrency/Concurrency.h"
#include "rtosim/queue/OrientationSetQueue.h"
#include "rtosim/EndOfData.h"
#include "rtosim/QueuesSync.h"
#include <vector>
#include <string>
using namespace std;
using namespace rtosim;
using namespace SimTK;


Body::Rigid getImuBody() {
    
    Body::Rigid imuBody;
    imuBody.addDecoration(
        Transform(), 
        SimTK::DecorativeBrick(SimTK::Vec3(0.05, 0.025, 0.015)*10.)
        .setColor(Red)
        .setOpacity(.4));
    imuBody.addDecoration(Transform(), 
        DecorativeFrame(1)
        .setColor(SimTK::Red)
        .setLineThickness(5));
    return imuBody;
}


void setupVisualizer(Visualizer& viz) {

    viz.setBackgroundType(SimTK::Visualizer::BackgroundType::SolidColor);
    viz.setBackgroundColor(SimTK::White);
    viz.setDesiredFrameRate(50);
    viz.setMode(SimTK::Visualizer::Mode::Sampling);
    viz.setShowFrameRate(true);
    viz.setShutdownWhenDestructed(true);
    viz.setDesiredBufferLengthInSec(1);
}

class DataReader {
public:

    DataReader(
        OrientationSetQueue& inputQueue,
        rtb::Concurrency::Latch& ready,
        rtb::Concurrency::Latch& done) :
        inputQueue_(inputQueue),
        ready_(ready),
        done_(done)
        {}

    void operator()() {
        
        MultibodySystem system;
        SimbodyMatterSubsystem matter(system);
        Visualizer viz(system);
        setupVisualizer(viz);

        //generate imus based on the first frame
        Body::Rigid imuBody(getImuBody());
        MobilizedBody::Free imuMobilizer(matter.updGround(), imuBody);
        SimTK::State state = system.realizeTopology();


        // Using the following coordinate reference systems
        // `O` Simbody Origin
        // `imuO` Origin for the IMU
        // `imuL` Local for the IMU
        // `imuC` Calibrate imu refence frame
        // Using the following matrix notation
        // `R_P_C`, where P is parent and C is child

        Rotation R_O_imuO(-0.5*Pi, XAxis);
        // Rotation R_O_imuC;
        //local calibrated 
        Rotation R_O_imuLC;
        Rotation R_imuO_imuL;
        Rotation R_imuC_imuO;

        inputQueue_.subscribe();
        ready_.wait();

        bool runCondition(true);
        while (runCondition) {
            auto frame(inputQueue_.pop());
            runCondition = !EndOfData::isEod(frame);
            if (runCondition) {
                state.updTime() = frame.time;

                // read the imu rotation from the frame
                R_imuO_imuL = Rotation(frame.data[0]);
                //calculate local calibrated to respect of origin
                R_O_imuLC = R_O_imuO * R_imuC_imuO * R_imuO_imuL;
                imuMobilizer.setQToFitRotation(state, R_O_imuLC);
                viz.report(state);
      //          if (resetOffset) {
      //              //calibrate again
      //              R_imuC_imuO = R_imuO_imuL.transpose();
      //              resetOffset = false;
    //            }
            }
        }
        done_.wait();

    }

private:
    OrientationSetQueue& inputQueue_;
    rtb::Concurrency::Latch& ready_;
    rtb::Concurrency::Latch& done_;

};



int main() {

    FlowControl flowControl(true);
    rtb::Concurrency::Latch ready(2), done(2);
    OrientationSetQueue orientationQueue;

    vector<string> names{ "lt.leg.thigh" };
    OrientationsFromMR3 producer(orientationQueue, ready, done, flowControl, names);
    DataReader consumer(orientationQueue, ready, done);
    auto trigger([&flowControl]() {
        std::string cmd;
        std::cin >> cmd;
        flowControl.setRunCondition(false);
    });

    QueuesSync::launchThreads(producer, consumer, trigger);

    
    return 0;
}