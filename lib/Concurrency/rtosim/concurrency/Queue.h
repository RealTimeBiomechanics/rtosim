/* -------------------------------------------------------------------------- *
 * CEINMS is a standalone toolbox for neuromusculoskeletal modelling and      *
 * simulation. CEINMS can also be used as a plugin for OpenSim either         *
 * through the OpenSim GUI or API. See https://simtk.org/home/ceinms and the  *
 * NOTICE file for more information. CEINMS development was coordinated       *
 * through Griffith University and supported by the Australian National       *
 * Health and Medical Research Council (NHMRC), the US National Institutes of *
 * Health (NIH), and the European Union Framework Programme 7 (EU FP7). Also  *
 * see the PROJECTS file for more information about the funding projects.     *
 *                                                                            *
 * Copyright (c) 2010-2015 Griffith University and the Contributors           *
 *                                                                            *
 * CEINMS Contributors: C. Pizzolato, M. Reggiani, M. Sartori,                *
 *                      E. Ceseracciu, and D.G. Lloyd                         *
 *                                                                            *
 * Author(s): M. Reggiani                                                     *
 *                                                                            *
 * CEINMS is licensed under the Apache License, Version 2.0 (the "License").  *
 * You may not use this file except in compliance with the License. You may   *
 * obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0.*
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

#ifndef rtosim_Queue_h
#define rtosim_Queue_h

#include <list>
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace rtosim {
    namespace Concurrency{

        template <typename T>
        class Queue {
        public:
            typedef T type;
            Queue() = default;
            Queue(const Queue&) = delete;
            Queue& operator=(const Queue&) = delete;
            void subscribe();
            void unsubscribe();
            T pop();
            void push(const T& item);
            template <typename B>
            friend std::ostream& operator<< (std::ostream& os, const Queue<B>& queue);
        private:
            // decided to go with a list so we can trust the iterator. With other containers you can have
            // reallocation that invalidates iterator
            // CP: c++11 should have introduced std::next and std::prev for iterators..
            std::list<T> queue_;
            typedef typename std::list<T>::iterator QueueIterator;
            // could be a single map with a structure. But keep in this way cause it helps in function unsubscribe
            std::map< std::thread::id, QueueIterator > subscribersNextRead_;
            std::map< std::thread::id, int > subscribersMissingRead_;
            std::mutex mutex_;
            std::condition_variable cond_;

            // utility function used to find the maximum on a map
            static bool pred(const std::pair< std::thread::id, int>& lhs, const std::pair< std::thread::id, int>& rhs);
            bool someoneSlowerThanMe();
        };
    }
}

#include "Queue.cpp"
#endif
