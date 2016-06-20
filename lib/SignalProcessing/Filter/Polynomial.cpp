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

#include <algorithm>
#include <vector>

namespace rtosim {
    namespace Filter {

        template <typename T>
        Polynomial<T>::Polynomial(const std::vector<Complex>& coefficients) :
            coefficients_(coefficients) {

            removeHighOrderZeroCoefficients();
            updateOrderAndSize();
        }

        template <typename T>
        Polynomial<T>::Polynomial(const std::vector<T>& coefficients)
        {
            for (auto c : coefficients)
                coefficients_.push_back(c);

            removeHighOrderZeroCoefficients();
            updateOrderAndSize();
        }

        template <typename T>
        Polynomial<T>::Polynomial(const std::initializer_list<Complex>& coefficients) :
            coefficients_(coefficients) {

            removeHighOrderZeroCoefficients();
            updateOrderAndSize();
        }

        template <typename T>
        Polynomial<T>::Polynomial(const std::initializer_list<T>& coefficients) {

            for (auto c : coefficients)
                coefficients_.push_back(c);

            removeHighOrderZeroCoefficients();
            updateOrderAndSize();
        }

        template <typename T>
        Polynomial<T>::Polynomial(const Polynomial<T>& rhs) :
            coefficients_(rhs.coefficients_),
            order_(rhs.order_),
            size_(rhs.size_) { }

        template <typename T>
        Polynomial<T>& Polynomial<T>::operator=(const Polynomial<T>& rhs)  {

            coefficients_ = rhs.coefficients_;
            order_ = rhs.order_;
            size_ = rhs.size_;
            return *this;
        }

        template <typename T>
        void Polynomial<T>::updateOrderAndSize() {

            size_ = coefficients_.size();
            order_ = size_ - 1;
        }

        template <typename T>
        void Polynomial<T>::removeHighOrderZeroCoefficients() {

            Complex sum(0);
            for (auto c : coefficients_)
                sum += c;

            if (coefficients_.size() > 1 && std::abs(sum) != 0) {
                auto it = std::find_if(coefficients_.rbegin(), coefficients_.rend(), [](Complex v){ return std::abs(v) != 0;      });
                coefficients_.erase(it.base(), coefficients_.end());
            }
            updateOrderAndSize();
        }

        template <typename T>
        Polynomial<T>::Polynomial(size_t order) :
            coefficients_(order + 1, .0), size_(order + 1), order_(order) { }

        template <typename T>
        unsigned Polynomial<T>::getOrder() const {

            return order_;
        }

        template <typename T>
        size_t Polynomial<T>::getSize() const {

            return size_;
        }

        template <typename T>
        Polynomial<T> add(const Polynomial<T>& p1, const Polynomial<T>& p2) {

            Polynomial<T> result(std::max(p1.getOrder(), p2.getOrder()));
            for (unsigned i{ 0 }; i < result.getSize(); ++i) {
                if (i < p1.getSize())
                    result.coefficients_.at(i) += p1.coefficients_.at(i);
                if (i < p2.getSize())
                    result.coefficients_.at(i) += p2.coefficients_.at(i);
            }
            result.removeHighOrderZeroCoefficients();
            return result;
        }

        template <typename T>
        Polynomial<T> multiply(const Polynomial<T>& p1, const Polynomial<T>& p2) {

            typedef typename Polynomial<T>::Complex Complex;

            size_t resultOrder(p1.getOrder() + p2.getOrder());
            Polynomial<T> result(resultOrder);
            if (resultOrder == 0) {
                result.coefficients_.at(0) = p1.coefficients_.at(0)*p2.coefficients_.at(0);
            }
            else {
                std::vector<Complex> resultCoefficients(2 * resultOrder + 1);
                std::vector<Complex> temp1(2 * resultOrder + 1), temp2(2 * resultOrder + 1);
                for (unsigned i{ 0 }; i < p1.getSize(); ++i)
                    temp1.at(i) = p1.coefficients_.at(i);

                for (unsigned i{ 0 }; i < p2.getSize(); ++i)
                    temp2.at(i) = p2.coefficients_.at(i);

                for (unsigned k{ 0 }; k < resultCoefficients.size() - 1; ++k)
                    for (unsigned i{ 0 }; i <= k; ++i)
                        resultCoefficients.at(k) += temp1.at(i)*temp2.at(k - i);

                auto it = std::find_if(resultCoefficients.rbegin(), resultCoefficients.rend(), [](Complex v){ return std::abs(v) != 0;      });

                Polynomial<T> polyResult(std::vector<Complex>(resultCoefficients.begin(), it.base()));
                result = polyResult;
            }
            return result;
        }

        template <typename T>
        void Polynomial<T>::divideByScalar(Complex value){

            for (Complex& c : coefficients_)
                c /= value;
        }

        template <typename T>
        typename Polynomial<T>::Complex Polynomial<T>::solveFor(Complex value) const {

            Complex r{ 0 };
            for (unsigned i{ 0 }; i < size_; ++i)
                r += coefficients_.at(i)*std::pow(value, i);
            return r;
        }

        template <typename T>
        Polynomial<T> Polynomial<T>::operator* (const Polynomial<T>& p) const {

            return multiply(*this, p);
        }

        template <typename T>
        std::ostream& operator<< (std::ostream& os, const Polynomial<T>& p) {

            os << "[";
            for (auto c(p.coefficients_.begin()); c != p.coefficients_.end() - 1; ++c)
                os << *c << ", ";
            os << p.coefficients_.back() << "]";

            return os;
        }
    }
} // namespace