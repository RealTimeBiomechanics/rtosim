#include "rtosim/EndOfData.h"

namespace rtosim {

    template <typename T>
    FrameCounter<T>::FrameCounter(T& queueIn, const std::string& name) :
        queueIn_(queueIn),
        stopWatch_(name){}

    template <typename T>
    StopWatch FrameCounter<T>::getProcessingTimes() const {

        return stopWatch_;
    }

    template <typename T>
    void FrameCounter<T>::operator()() {

        bool localRunCondition(true);
        queueIn_.subscribe();
        //start the timer after popping the first item from the queue
        queueIn_.pop();
        stopWatch_.init();
        while (localRunCondition) {
            if (rtosim::EndOfData::isEod(queueIn_.pop()))
                localRunCondition = false;
            stopWatch_.log();
        }
    }
}