#ifndef rtosim_QueueAdapter_h
#define rtosim_QueueAdapter_h

#include "rtosim/concurrency/Queue.h"
#include "rtosim/concurrency/Latch.h"
#include "rtosim/EndOfData.h"

namespace rtosim {

    //todo: get Qin and Qout types from T
    template<typename Qin, typename Qout, typename T>
    class QueueAdapter {

    public:
        QueueAdapter() = delete;
        QueueAdapter(QueueAdapter&) = delete;
        QueueAdapter(
            Qin& qIn,
            Qout& qOut,
            rtosim::Concurrency::Latch& doneWithSubscriptions,
            rtosim::Concurrency::Latch& doneWithExecution,
            const T& adapter
            );
        void operator()();
        //debug
        T getAdapter() const { return adapter_; }
    private:
        Qin& qIn_;
        Qout& qOut_;
        rtosim::Concurrency::Latch& doneWithSubscriptions_;
        rtosim::Concurrency::Latch& doneWithExecution_;
        T adapter_;
    };
}

#include "QueueAdapter.cpp"
#endif
