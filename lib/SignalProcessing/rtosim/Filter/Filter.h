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
