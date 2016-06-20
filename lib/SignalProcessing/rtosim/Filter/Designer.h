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

#ifndef rtosim_Filter_Designer_h
#define rtosim_Filter_Designer_h

#include <cmath>

#include "TransferFunction.h"

namespace rtosim {
    namespace Filter {
        enum class Type { LowPass, HighPass, BandPass };

        template<typename T>
        TransferFunction<T> butter2ndOrder(double wc) {
            double const k1{ M_SQRT2*wc };
            double const k2{ wc*wc };
            double const b0{ k2 / (1 + k1 + k2) };
            double const b1{ 2 * b0 };
            double const b2{ b0 };
            double const a0{ 1 };
            double const a1{ -2 * b0*(1. / k2 - 1.) };
            double const a2{ b0 + b1 + b2 - a1 - 1. };
            Polynomial<T> num({ b0, b1, b2 });
            Polynomial<T> den({ a0, a1, a2 });
            return TransferFunction<T>(num, den);
        }

        template<typename T>
        TransferFunction<T> butter2ndOrder(double fc, double fs) {

            double const wc{ tan((M_PI*fc) / fs) };
            TransferFunction<T> tf(butter2ndOrder<T>(wc));
            tf.setSamplingFrequency(fs);
            return tf;
        }

        template<typename T>
        TransferFunction<T> butter(unsigned order, double fc, double fs) {
            TransferFunction<T> tf(butter2ndOrder<T>(fc, fs));
            for (unsigned i{ 2 }; i < order; i += 2)
                tf = tf*butter2ndOrder<T>(fc, fs);
            tf.setSamplingFrequency(fs);
            return tf;
        }
    }
}

#endif