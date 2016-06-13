#include "rtosim/FlowControl.h"

namespace rtosim {

    FlowControl::FlowControl() {
        //initialisation from constructor gives some probles, so it's in the body
        runCondition_.store(true);
    }

    FlowControl::FlowControl(bool value) {
        //initialisation from constructor gives some probles, so it's in the body
        runCondition_.store(value);
    }

    bool FlowControl::getRunCondition() const {
        return runCondition_.load();
    }

    void FlowControl::setRunCondition(bool run) {
        runCondition_.store(run);
    }
}