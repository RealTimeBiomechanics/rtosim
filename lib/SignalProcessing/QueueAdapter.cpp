namespace rtosim {

    template<typename Qin, typename Qout, typename T>
    QueueAdapter<Qin, Qout, T>::QueueAdapter(
        Qin& qIn,
        Qout& qOut,
        rtosim::Concurrency::Latch& doneWithSubscriptions,
        rtosim::Concurrency::Latch& doneWithExecution,
        const T& adapter
        ) :
        qIn_(qIn),
        qOut_(qOut),
        doneWithSubscriptions_(doneWithSubscriptions),
        doneWithExecution_(doneWithExecution),
        adapter_(adapter) {}

    template<typename Qin, typename Qout, typename T>
    void QueueAdapter<Qin, Qout, T>::operator()() {

        bool localRunCondition(true);
        qIn_.subscribe();
        doneWithSubscriptions_.wait();
        while (localRunCondition) {
            auto frame(qIn_.pop());
            if (rtosim::EndOfData::isEod(frame)) {
                localRunCondition = false;
                qOut_.push(rtosim::EndOfData::get<typename Qout::type>());
            }
            else
                qOut_.push(adapter_(frame));
        }

        doneWithExecution_.wait();
    }
}