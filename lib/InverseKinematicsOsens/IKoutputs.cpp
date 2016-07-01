/* -------------------------------------------------------------------------- *
 * Copyright (c) 2010-2016 C. Pizzolato, M. Reggiani                          *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License");            *
 * you may not use this file except in compliance with the License.           *
 * You may obtain a copy of the License at:                                   *
 * http://www.apache.org/licenses/LICENSE-2.0                                 *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

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