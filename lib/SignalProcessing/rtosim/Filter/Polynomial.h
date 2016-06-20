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

#ifndef rtosim_Filter_Polynomial_h
#define rtosim_Filter_Polynomial_h
#include <vector>
#include <iostream>
#include <initializer_list>
#include <complex>

namespace rtosim {
    namespace Filter {

        template <typename T = double>
        class Polynomial {

        public:
            typedef std::complex<T> Complex;
            Polynomial() = delete;
            Polynomial(const std::vector<T>& coefficients);
            Polynomial(const std::vector<Complex>& coefficients);
            Polynomial(const std::initializer_list<Complex>& coefficients);
            Polynomial(const std::initializer_list<T>& coefficients);
            Polynomial(const Polynomial<T>& rhs);
            Polynomial<T>& operator=(const Polynomial<T>& rhs);

            Polynomial(size_t order);
            Complex solveFor(Complex value) const;
            unsigned getOrder() const;
            size_t getSize() const;
            Complex getCoefficient(unsigned i) const { return coefficients_.at(i); }
            void divideByScalar(Complex value);

            template<typename U>
            friend Polynomial<U> multiply(const Polynomial<U>& p1, const Polynomial<U>& p2);

            Polynomial operator* (const Polynomial<T>& p) const;

            template<typename U>
            friend Polynomial<U> add(const Polynomial<U>& p1, const Polynomial<U>& p2);

            template<typename U>
            friend std::ostream& operator<< (std::ostream& os, const Polynomial<U>& p);

        private:
            void removeHighOrderZeroCoefficients();
            void updateOrderAndSize();
            std::vector<Complex> coefficients_;
            size_t order_;
            size_t size_;
        };
    }
} // namespace
#include "Filter/Polynomial.cpp"
#endif