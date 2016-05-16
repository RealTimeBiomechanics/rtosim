namespace rtosim {

    template <typename T>
    T IKoutputs<T>::pop(double currentTime) {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty() || queue_.top().time != currentTime)
        {
#ifdef RTOSIM_DEBUG
            std::cout << "current Time: " << currentTime << std::endl << std::flush;
            if (!queue_.empty())
                std::cout << "and got: " << queue_.top().time << std::endl << std::flush;
#endif
            cond_.wait(mlock);
        }
        auto val = queue_.top();
        queue_.pop();
        return val;
    }

    template <typename T>
    T IKoutputs<T>::pop()
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty())
        {
            cond_.wait(mlock);
        }
        auto val = queue_.top();
        queue_.pop();
        return val;
    }

    template <typename T>
    void IKoutputs<T>::pop(T& item)
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty())
        {
            cond_.wait(mlock);
        }
        item = queue_.top();
        queue_.pop();
    }

    template <typename T>
    void IKoutputs<T>::push(const T& item)
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        queue_.push(item);
        mlock.unlock();
        cond_.notify_one();
    }
}