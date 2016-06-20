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
    namespace Filter {

        template<typename T>
        void  TransferFunction<T>::normalize() {

            Complex const normalFactor(a(0));
            if (std::abs(normalFactor) != 0) {
                numerator_.divideByScalar(normalFactor);
                denominator_.divideByScalar(normalFactor);
            }
        }

        template<typename T>
        TransferFunction<T>  TransferFunction<T>::operator* (const TransferFunction<T>& tf) const {

            return TransferFunction<T>(numerator_*tf.numerator_, denominator_*tf.denominator_);
        }

        template<typename T>
        typename TransferFunction<T>::Complex TransferFunction<T>::getResponseAt(double w) const {

            return numerator_.solveFor(std::exp(Complex(0, w))) / denominator_.solveFor(std::exp(Complex(0, w)));
        }

        template<typename T>
        double TransferFunction<T>::getGainAt(double w) const {

            return 20 * log10(std::abs(getResponseAt(w)));
        }

        template<typename T>
        double TransferFunction<T>::getPhaseAt(double w) const {

            return -std::arg<T>(getResponseAt(w));
        }

        template<typename T>
        void TransferFunction<T>::getBode(std::vector<double>& xValues, std::vector<double>& gain, std::vector<double>& phase) const {

            xValues.clear();
            gain.clear();
            phase.clear();

            for (unsigned i{ 1 }; i < static_cast<unsigned>(ceil(samplingFrequency_ / 2.)); ++i) {
                double w{ 2 * M_PI / samplingFrequency_ * i };
                xValues.push_back(i);
                gain.push_back(getGainAt(w));
                phase.push_back(getPhaseAt(w));
            }
        }

        template<typename U>
        std::ostream& operator<<(std::ostream& os, const TransferFunction<U>& tf) {

            os << "Num: " << tf.numerator_ << std::endl;
            os << "Den: " << tf.denominator_ << std::endl;
            return os;
        }
    }
} // namespace