#include "rtosim/rtosim.h"
using namespace rtosim;

#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <Simbody.h>

string getResultDir() {

    return "";
}

string getStopWatchResultDir() {

    return "";
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
    std::string resultDir(getResultDir());
    std::string stopWatchResultDir(getStopWatchResultDir());
    /*
    auto coordNames = getCoordinateNamesFromModel(osimModelFilename);
    vosl::GeneralisedCoordinatesStateSpace gcFilt(fc, coordNames.size());

    vosl::IKFromQueue ikFromQueue(
    markerSetQueue,
    generalisedCoordinatesQueue,
    doneWithSubscriptions,
    doneWithExecution,
    osimModelFilename,
    nThreads, ikTaskFilename, solverAccuracy);

    vosl::MarkersFromTrc markersFromTrc(
    markerSetQueue,
    doneWithSubscriptions,
    doneWithExecution,
    osimModelFilename,
    trcTrialFilename,
    false);
    markersFromTrc.setSpeedFactor(1000);

    vosl::QueueAdapter < ceinms::GeneralisedCoordinatesQueue, ceinms::GeneralisedCoordinatesQueue, vosl::GeneralisedCoordinatesStateSpace > gcQueueAdaptor(
    generalisedCoordinatesQueue, filteredGeneralisedCoordinatesQueue, doneWithSubscriptions, doneWithExecution, gcFilt);

    vosl::QueueLogger< ceinms::GeneralisedCoordinatesQueue> logIK(filteredGeneralisedCoordinatesQueue, doneWithSubscriptions, doneWithExecution);

    ceinms::QueueToFileLogger<ceinms::GeneralisedCoordinatesData> filteredIkLogger(
    filteredGeneralisedCoordinatesQueue,
    doneWithSubscriptions,
    doneWithExecution,
    getCoordinateNamesFromModel(osimModelFilename),
    "filtered_ik_from_file", resultDir, "sto");

    ceinms::QueueToFileLogger<ceinms::GeneralisedCoordinatesData> rawIkLogger(
    generalisedCoordinatesQueue,
    doneWithSubscriptions,
    doneWithExecution,
    getCoordinateNamesFromModel(osimModelFilename),
    "raw_ik_from_file", resultDir, "sto");

    vosl::FrameCounter< ceinms::GeneralisedCoordinatesQueue> ikFrameCounter(filteredGeneralisedCoordinatesQueue, "Filtered_IK_Queue");

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

    logIK.getResult(result);

    auto stopWatches = ikFromQueue.getProcessingTimes();

    vosl::StopWatch combinedSW("Combined_IK_solvers");
    for (auto& s : stopWatches)
    combinedSW += s;
    cout << combinedSW << endl;
    vosl::StopWatch ikOutputStopWatch = ikFrameCounter.getProcessingTimes();
    cout << ikOutputStopWatch;

    combinedSW.print(stopWatchResultDir);
    ikOutputStopWatch.print(stopWatchResultDir);
    */
}