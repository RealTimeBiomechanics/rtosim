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
