#include "rtosim/Filter/Polynomial.h"
#include "rtosim/Filter/Filter.h"
#include "rtosim/Filter/TransferFunction.h"
#include "rtosim/Filter/Designer.h"
#include "rtosim/Filter/StateSpaceFilter.h"
using namespace rtosim::Filter;

#include <array>
#include <iostream>
#include <fstream>
#include <vector>
using std::vector;
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <complex>

using std::cout;
using std::endl;

template <typename T>
void stepResponse(Filter<T>& filter, std::string outFilename) {

    std::vector<double> zeroes(1, 0.0);
    std::vector<double> ones(1000, 1.0);
    std::vector<double> step(zeroes);
    step.insert(step.end(), ones.begin(), ones.end());

    std::ofstream outF(outFilename);
    std::vector<double> stepResponse(filter.pass(step));
    cout << "Printing step response to " << outFilename << endl;
    for (unsigned i{ 0 }; i < step.size(); ++i)
        outF << step.at(i) << ", " << stepResponse.at(i) << endl;
    outF.close();
}

template <typename T>
void stepResponse(StateSpaceFilter<T>& filter, double f0, std::string outFilename) {

    std::vector<double> zeroes(1, 0.0);
    std::vector<double> ones(1000, 1.0);
    std::vector<double> step(zeroes);
    step.insert(step.end(), ones.begin(), ones.end());

    std::ofstream outF(outFilename);
    std::vector<double> stepResponse(filter.pass(step, f0));
    cout << "Printing step response to " << outFilename << endl;
    for (unsigned i{ 0 }; i < step.size(); ++i)
        outF << step.at(i) << ", " << stepResponse.at(i) << endl;
    outF.close();
}

template<typename T>
void bodeDiagram(const TransferFunction<T>& tf, std::string outFilename) {

    cout << "Printing bode diagram to " << outFilename << endl;
    std::ofstream outF(outFilename);
    std::vector<double> xValues, gain, phase;
    tf.getBode(xValues, gain, phase);
    for (unsigned i{ 0 }; i < xValues.size(); ++i)
        outF << xValues.at(i) << ", " << gain.at(i) << ", " << phase.at(i) << endl;
    outF.close();
}

int main() {

    typedef std::complex<double> Complex;
    cout << "--------------------------" << endl;
    cout << "|        TEST #1         |" << endl;
    cout << "|       Polynomial       |" << endl;
    cout << "--------------------------" << endl;

    Polynomial<double> p1{ 1, 2 };
    Polynomial<double> p2{ 1, -2, 0 };
    Polynomial<double> p3{ 0, 2, 3, 4 };
    Polynomial<double> p4{ 5 };

    cout << "p1 =" << p1;
    cout << "p2 =" << p2;
    cout << "p3 =" << p3;
    cout << "p4 =" << p4;
    cout << endl;
    cout << "Scalar Multiplication" << endl;
    cout << "p1 * 2 = " << multiply<double>(p1, { 2 }) << endl;
    cout << "p2 * 3 = " << multiply<double>(p2, { 3 }) << endl;
    cout << "p3 * 4 = " << multiply<double>(p3, { 4 }) << endl;
    cout << "p4 * 5 = " << multiply<double>(p4, { 5 }) << endl;
    cout << endl;
    cout << "Complex Multiplication" << endl;
    cout << "p1 * (2 + 2i) = " << multiply<double>(p1, { Complex(2, 2) }) << endl;
    cout << "p2 * (3 - 3i) = " << multiply<double>(p2, { Complex(3, -3) }) << endl;
    cout << "p3 * (4 + 4i) = " << multiply<double>(p3, { Complex(4, 4) }) << endl;
    cout << "p4 * (5 - 5i) = " << multiply<double>(p4, { Complex(5, -5) }) << endl;
    cout << endl;
    cout << "Polynomial Multiplication" << endl;
    cout << "p1 * p1 = " << p1*p1 << endl;
    cout << "p2 * p1 = " << p2*p1 << endl;
    cout << "p3 * p1 = " << p3*p1 << endl;
    cout << "p4 * p1 = " << p4*p1 << endl;
    cout << endl;
    cout << "Polynomial Addition" << endl;
    cout << "p1 + p1 = " << add<double>(p1, p1) << endl;
    cout << "p2 + p1 = " << add<double>(p2, p1) << endl;
    cout << "p3 + p1 = " << add<double>(p3, p1) << endl;
    cout << "p4 + p1 = " << add<double>(p4, p1) << endl;
    cout << endl;

    cout << "--------------------------" << endl;
    cout << "|        TEST #2         |" << endl;
    cout << "|    TransferFunction    |" << endl;
    cout << "--------------------------" << endl;
    cout << "Coefficients are normalised to the denominator" << endl;
    cout << "Pole-zero simplifications are NOT implemented" << endl;
    cout << "TF = p1/p2 " << endl << TransferFunction<double>(p1, p2) << endl;
    cout << "TF = p2/p2 " << endl << TransferFunction<double>(p2, p2) << endl;
    cout << "TF = p3/p2 " << endl << TransferFunction<double>(p3, p2) << endl;
    cout << "TF = p4/p3 " << endl << TransferFunction<double>(p4, p3) << endl;
    cout << "TF = p3/p4 " << endl << TransferFunction<double>(p3, p4) << endl;
    cout << "TF = (1+2i)/p2 " << endl << TransferFunction<double>({ { 1, 2 } }, p2) << endl;
    cout << endl;

    cout << "--------------------------" << endl;
    cout << "|        TEST #3         |" << endl;
    cout << "|    Filter Designer     |" << endl;
    cout << "--------------------------" << endl;
    cout << "IIR Filter, 2nd Order Low Pass Butterworth, cutoff frequency 6Hz, sample frequency 1000Hz" << endl;
    TransferFunction<double> lowPass2nd(butter<double>(2, 6, 1000));
    cout << lowPass2nd;
    bodeDiagram(lowPass2nd, "lowPass2nd_bode.csv");
    cout << endl;

    cout << "IIR Filter, 6th Order Low Pass Butterworth, cutoff frequency 300Hz, sample frequency 1000Hz" << endl;
    TransferFunction<double> lowPass6th(butter<double>(6, 300, 1000));
    cout << lowPass6th;
    bodeDiagram(lowPass6th, "lowPass6th_bode.csv");
    cout << endl;

    cout << "--------------------------" << endl;
    cout << "|        TEST #4         |" << endl;
    cout << "|     Step Response      |" << endl;
    cout << "--------------------------" << endl;

    cout << "IIR Filter, 2nd Order Low Pass Butterworth, cutoff frequency 6Hz, sample frequency 1000Hz" << endl;
    Filter<double> filterLowPass2nd(lowPass2nd);
    stepResponse<double>(filterLowPass2nd, "lowPass2nd_step.csv");
    cout << endl;
    cout << "IIR Filter, 6th Order Low Pass Butterworth, cutoff frequency 300Hz, sample frequency 1000Hz" << endl;
    Filter<double> filterLowPass6th(lowPass6th);
    stepResponse<double>(filterLowPass6th, "lowPass6th_step.csv");
    cout << endl;

    StateSpaceFilter<double> ssFilter;
    std::vector<double> times = { 0.01, 0.02, 0.032, 0.038, 0.05, 0.063, 0.07, 0.09, 0.102, 0.11, 0.119, 0.14 };
    std::ofstream outF("stateSpaceFilter_300_step.csv");
    cout << "Printing step response to stateSpaceFilter_300_step.csv" << endl;
    outF << 0.0 << ", " << 0.0 << ", " << ssFilter.filter(0.0, 0.0, 300) << std::endl;
    for (auto t : times)
        outF << t << ", " << 1.0 << ", " << ssFilter.filter(1.0, t, 300) << std::endl;
    outF.close();

    cout << "--------------------------" << endl;
    cout << "|        TEST #5         |" << endl;
    cout << "|Filter def. constructor |" << endl;
    cout << "--------------------------" << endl;

    Filter<double> emptyFilter;
    emptyFilter.setTransferFunction(lowPass2nd);
    cout << "Late initialization. IIR Filter, 2nd Order Low Pass Butterworth, cutoff frequency 6Hz, sample frequency 1000Hz" << endl;
    stepResponse<double>(emptyFilter, "lowPass2nd_step_lateInit.csv");

    cout << "Step response for state space filter with f0=8Hz" << endl;
    StateSpaceFilter<double> ssfiler;
    stepResponse<double>(ssFilter, 8, "ssfilter8Hz_step.csv");

    cout << "Step response for moving average filter" << endl;
    std::vector<double> bMa(20, 1. / 20.);
    std::vector<double> aMa(1, 1.);

    Filter<double> maFilter(TransferFunction<>(bMa, aMa));
    stepResponse<double>(maFilter, "moving_avg_filter_step.csv");

    /*
    std::cout << "p1 = " << p1;
    std::cout << "p1(3) = " << p1.solveFor(-1) << std::endl;

    //step response
    std::vector<double> zeroes(1, 0.0);
    std::vector<double> ones(1000, 1.0);
    std::vector<double> step(zeroes);
    step.insert(step.end(), ones.begin(), ones.end());

    TransferFunction<double> tfDesigned(Designer::butter2ndOrder<double>(6, 1000.));

    std::cout << "Designed filter\n";
    std::cout << tfDesigned.getSamplingFrequency() << std::endl;
    Filter<double> filter(tfDesigned);

    Filter<double> filter4th(Designer::butter<double>(4, 6, 1000));

    //std::cout << filter;
    //std::cout << filter4th;
    //      std::cout << filterDouble;

    /*
    std::ofstream oF("output.csv");
    for (double val : step)
    oF << val << "," << std::abs(filter.filter(val)) << "," << std::abs(filter4th.filter(val)) << std::endl;
    oF.close();

    std::ofstream oFBode("bode_gain.csv");
    std::vector<double> xValues, gain, phase;
    tfDesigned.getBode(xValues, gain, phase);
    for (unsigned i{ 0 }; i < xValues.size(); ++i)
    oFBode << xValues.at(i) << "," << gain.at(i) << "," << phase.at(i) << std::endl;
    oFBode.close();
    /*
    std::ofstream oFBodePhase("bode_phase.csv");
    for (unsigned i{ 1 }; i < 1000; ++i)
    oFBodePhase << M_PI / 1000 * i << "," << tfDesigned.getPhaseAt(M_PI / 1000 * i) << std::endl;
    oFBodePhase.close();

    /*

    //test filtfilt
    vector<double> firstPass(filter.pass(step));
    std::reverse(firstPass.begin(), firstPass.end());
    vector<double> secondPass(filter.pass(firstPass));
    std::reverse(secondPass.begin(), secondPass.end());

    std::reverse(firstPass.begin(), firstPass.end());

    std::ofstream oF2("filtfilt.csv");
    for (unsigned i{ 0 }; i < step.size(); ++i)
    oF2 << step.at(i) << "," << firstPass.at(i) << "," << secondPass.at(i) << std::endl;
    oF2.close();

    /*

    //test linear phase fir filter
    std::vector<double> firB( {
    0.00127199776427371,
    0.00175973248875467,
    -0.000474050123699974,
    -0.00507655778150986,
    -0.00730622779100510,
    -0.00139834985893385,
    0.0118182305510141,
    0.0208600394665444,
    0.0108509633977398,
    -0.0201948738097724,
    -0.0503421398292688,
    -0.0426075899832280,
    0.0273002961309331,
    0.144919701151270,
    0.257048286517307,
    0.303233271924363,
    0.257048286517307,
    0.144919701151270,
    0.0273002961309331,
    -0.0426075899832280,
    -0.0503421398292688,
    -0.0201948738097724,
    0.0108509633977398,
    0.0208600394665444,
    0.0118182305510141,
    -0.00139834985893385,
    -0.00730622779100510,
    -0.00507655778150986,
    -0.000474050123699974,
    0.00175973248875467,
    0.00127199776427371
    } );

    Polynomial<> n(firB), d{ 1 };

    TransferFunction<> tfFir(n, d);
    Filter<> firFilter(tfFir);
    vector<double> resultFir(firFilter.pass(step));

    std::ofstream oF3("comparison.csv");
    for (unsigned i{ 0 }; i < step.size(); ++i)
    oF3 << step.at(i) << "," << firstPass.at(i) << "," << secondPass.at(i) << ", " << resultFir.at(i) << std::endl;
    oF3.close();

    */
    //      TransferFunction<double> h1(p1, p2);

    //std::cout << add(p, Polynomial<double>(std::vector<double>{ 1, 2, 3, 4 }));

    std::exit(EXIT_SUCCESS);
}