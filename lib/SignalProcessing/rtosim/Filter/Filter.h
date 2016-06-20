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

#ifndef rtosim_Filter_Filter_h
#define rtosim_Filter_Filter_h
#include <array>
#include <iostream>
#include <vector>
#include "TransferFunction.h"
#include <complex>

namespace rtosim {
    namespace Filter {

        template<typename T = double>
        class Filter {

        public:
            typedef std::complex<T> Complex;
            Filter();
            Filter(const TransferFunction<T>& tf);
            void setTransferFunction(const TransferFunction<T>& tf);
            void resetState();
            T filter(T value);
            std::vector<T> pass(const std::vector<T>& values);
            template<typename U>
            friend std::ostream& operator<< (std::ostream& os, const Filter<U>& tf);
        private:
            Complex directCoefficientsProduct() const;
            Complex feedbackCoefficientsProduct() const;
            void pushInput(Complex valueX);
            void updateOutputState(Complex valueY);
            TransferFunction<T> tf_;
            std::vector<Complex>  x_;
            std::vector<Complex>  y_;
            size_t count_;
            size_t n_, m_;
        };
    }
}

#include "Filter/Filter.cpp"

#endif
