//CP

#ifndef rtosim_FlowControl_h
#define rtosim_FlowControl_h

#include <atomic>

namespace rtosim {

    class FlowControl {
    public:
        FlowControl();
        FlowControl(FlowControl&) = delete;
        bool getRunCondition() const;
        void setRunCondition(bool run);
    private:
        std::atomic_bool runCondition_;
    };
}

#endif