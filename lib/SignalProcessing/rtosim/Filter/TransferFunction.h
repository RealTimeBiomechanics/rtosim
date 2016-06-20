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

#ifndef rtosim_Filter_TransferFunction_h
#define rtosim_Filter_TransferFunction_h

#include "Polynomial.h"
#include <array>
#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <complex>
#include <cmath>

namespace rtosim {
    namespace Filter {

        template<typename T = double >
        class TransferFunction {

        public:
            typedef std::complex<T> Complex;

            TransferFunction() : numerator_({ { 1 } }), denominator_({ { 1 } }), samplingFrequency_(0.) { }

            TransferFunction(const Polynomial<T>& numerator, const Polynomial<T>& denominator) :
                numerator_(numerator), denominator_(denominator), samplingFrequency_(0.){
                normalize();
            }

            TransferFunction(const Polynomial<T>& numerator, const Polynomial<T>& denominator, double samplingFrequency) :
                numerator_(numerator), denominator_(denominator), samplingFrequency_(samplingFrequency) {
                normalize();
            }

            TransferFunction operator* (const TransferFunction<T>& tf) const;
            Complex a(unsigned i) const { return denominator_.getCoefficient(i); }
            Complex b(unsigned i) const { return numerator_.getCoefficient(i); }
            double getGainAt(double w) const;
            double getPhaseAt(double w) const;
            void getBode(std::vector<double>& xValues, std::vector<double>& gain, std::vector<double>& phase) const;
            double getSamplingFrequency() const { return samplingFrequency_; }
            void setSamplingFrequency(double samplingFrequency) { samplingFrequency_ = samplingFrequency; }

            size_t getNumeratorSize() { return numerator_.getSize(); }
            size_t getDenominatorSize() { return denominator_.getSize(); }

            template<typename U>
            friend std::ostream& operator<<(std::ostream& os, const TransferFunction<U>& tf);

        private:
            Complex getResponseAt(double w) const;
            void normalize();
            double samplingFrequency_;
            Polynomial<T> numerator_, denominator_;
        };
    }
} // namespace

#include "Filter/TransferFunction.cpp"

#endif