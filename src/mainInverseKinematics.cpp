#include "rtosim/rtosim.h"
using namespace rtosim;

#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <Simbody.h>

string getOpenSimModelFilename() {

    return "";
}

string getResultDir() {

    return "";
}

string getStopWatchResultDir() {

    return "";
}

double getCutoffFrequency() {

    return -1;
}

void printAuthors() {

    cout << "Real-time OpenSim inverse kinematics" << endl;
    cout << "Authors: Claudio Pizzolato <claudio.pizzolato@griffithuni.edu.au>" << endl;
    cout << "         Monica Reggiani <monica.reggiani@unipd.it>" << endl << endl;
}

void printHelp() {

    printAuthors();

    auto progName(SimTK::Pathname::getThisExecutablePath());
    bool isAbsolute;
    string dir, filename, ext;
    SimTK::Pathname::deconstructPathname(progName, isAbsolute, dir, filename, ext);

    cout << "Option              Argument         Action / Notes\n";
    cout << "------              --------         --------------\n";
    cout << "-h                                   Print the command-line options for " << filename << ".\n";
    cout << "-PS                                  Print a default setup file for " << filename << " (setup_default.xml).\n";
    cout << "-S                  SetupFileName    Specify the name of the XML setup file for the investigation.\n";
}

int main(int argc, char* argv[]) {

    ProgramOptionsParser po(argc, argv);
    if (po.exists("-h")) {
        printHelp();
    }

    string setupFilename;
    if (po.exists("-Setup") || po.exists("-S"))
        setupFilename = po.getParameter("-S");
    else {
        printHelp();
        exit(EXIT_SUCCESS);
    }

    rtosim::MarkerSetQueue markerSetQueue;
    rtosim::GeneralisedCoordinatesQueue generalisedCoordinatesQueue, filteredGeneralisedCoordinatesQueue;
    rtosim::Concurrency::Latch doneWithSubscriptions;
    rtosim::Concurrency::Latch doneWithExecution;

    string resultDir(getResultDir());
    string stopWatchResultDir(getStopWatchResultDir());
    string osimModelFilename(getOpenSimModelFilename());
    string trcTrialFilename(getTrcFilename());
    double fc(getCutoffFrequency());
    double solverAccuracy(getSolverAccuracy());
    unsigned nThreads(getNumberOfThreads());

    auto coordNames = getCoordinateNamesFromModel(osimModelFilename);
    rtosim::GeneralisedCoordinatesStateSpace gcFilt(fc, coordNames.size());

    rtosim::IKFromQueue ikFromQueue(
        markerSetQueue,
        generalisedCoordinatesQueue,
        doneWithSubscriptions,
        doneWithExecution,
        osimModelFilename,
        nThreads, ikTaskFilename, solverAccuracy);

    rtosim::MarkersFromTrc markersFromTrc(
        markerSetQueue,
        doneWithSubscriptions,
        doneWithExecution,
        osimModelFilename,
        trcTrialFilename,
        false);

    //markersFromTrc.setSpeedFactor(1000);

    rtosim::QueueAdapter <
        rtosim::GeneralisedCoordinatesQueue,
        rtosim::GeneralisedCoordinatesQueue,
        rtosim::GeneralisedCoordinatesStateSpace
    > gcQueueAdaptor(
    generalisedCoordinatesQueue,
    filteredGeneralisedCoordinatesQueue,
    doneWithSubscriptions,
    doneWithExecution,
    gcFilt);

    rtosim::QueueToFileLogger<rtosim::GeneralisedCoordinatesData> filteredIkLogger(
        filteredGeneralisedCoordinatesQueue,
        doneWithSubscriptions,
        doneWithExecution,
        getCoordinateNamesFromModel(osimModelFilename),
        resultDir, "filtered_ik_from_file", "sto");

    rtosim::QueueToFileLogger<rtosim::GeneralisedCoordinatesData> rawIkLogger(
        generalisedCoordinatesQueue,
        doneWithSubscriptions,
        doneWithExecution,
        getCoordinateNamesFromModel(osimModelFilename),
        resultDir, "raw_ik_from_file", "sto");

    rtosim::FrameCounter< ceinms::GeneralisedCoordinatesQueue> ikFrameCounter(filteredGeneralisedCoordinatesQueue, "Filtered_IK_Queue");

    doneWithSubscriptions.setCount(6);
    doneWithExecution.setCount(6);

    vosl::launchThreads(
        markersFromTrc,
        ikFromQueue,
        gcQueueAdaptor,
        logIK,
        filteredIkLogger,
        rawIkLogger,
        ikFrameCounter
        );

    auto stopWatches = ikFromQueue.getProcessingTimes();

    rtosim::StopWatch combinedSW("Combined_IK_solvers");
    for (auto& s : stopWatches)
        combinedSW += s;
    cout << combinedSW << endl;
    vosl::StopWatch ikOutputStopWatch = ikFrameCounter.getProcessingTimes();
    cout << ikOutputStopWatch;

    combinedSW.print(stopWatchResultDir);
    ikOutputStopWatch.print(stopWatchResultDir);
}