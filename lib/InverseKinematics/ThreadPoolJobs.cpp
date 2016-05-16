namespace rtosim {

    template <typename T>
    T ThreadPoolJobs<T>::pop()
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty())
        {
            cond_.wait(mlock);
        }
        auto val = queue_.front();
        queue_.pop();
        return val;
    }

    template <typename T>
    size_t ThreadPoolJobs<T>::size()
    {
        std::unique_lock<std::mutex> mlock(mutex_);

        return queue_.size();
    }

    template <typename T>
    void ThreadPoolJobs<T>::pop(T& item)
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty())
        {
            cond_.wait(mlock);
        }
        item = queue_.front();
        queue_.pop();
    }

    template <typename T>
    T ThreadPoolJobs<T>::front() {

        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty())
        {
            cond_.wait(mlock);
        }
        auto val = queue_.front();
        return val;
    }

    template <typename T>
    void ThreadPoolJobs<T>::front(T& item)
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty())
        {
            cond_.wait(mlock);
        }
        item = queue_.front();
    }

    template <typename T>
    void ThreadPoolJobs<T>::push(const T& item)
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        queue_.push(item);
        mlock.unlock();
        cond_.notify_one();
    }
}