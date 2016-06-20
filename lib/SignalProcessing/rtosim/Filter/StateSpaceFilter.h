/* -------------------------------------------------------------------------- *
 * Copyright (c) 2010-2016 T. van den Bogert, C. Pizzolato, M. Reggiani       *
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

#ifndef rtosim_StateSpaceFilter_h
#define rtosim_StateSpaceFilter_h
#include <array>
#include <iostream>
#include <vector>
#include <complex>

namespace rtosim {
    namespace Filter {

        template<typename T = double>
        class StateSpaceFilter {

        public:
            typedef std::complex<T> Complex;
            StateSpaceFilter();
            void resetState();
            void resetState(double lastTime, double lastValue, double lastFirstDerivative, double lastSecondDerivative);

            T filter(T value, double time, double f0);
            T getFilteredValue() {
                return lastValue_;
            };
            T getFilteredFirstDerivative()
            {
                return lastFirstDerivative_;
            };
            T getFilteredSecondDerivative()
            {
                return lastSecondDerivative_;
            };
            std::vector<T> pass(const std::vector<T>& values, double f0);
            template<typename U>
            friend std::ostream& operator<< (std::ostream& os, const StateSpaceFilter<U>& tf);
        private:
            double lastTime_;
            T lastValue_;
            T lastFirstDerivative_;
            T lastSecondDerivative_;
        };
    }
} // namespace

#include "Filter/StateSpaceFilter.cpp"

#endif
