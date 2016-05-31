#include "rtosim/rtosim.h"
using namespace rtosim;

#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <Simbody.h>

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
    cout << "--model             ModelFilename    Specify the name of the osim model file for the investigation.\n";
    cout << "--trc               TrcFilename      Specify the name of the trc file to be used.\n";
    cout << "--task-set          TaskSetFilename  Specify the name of the XML TaskSet file containing the marker weights to be used.\n";
    cout << "--fc                CutoffFrequency  Specify the name of lowpass cutoff frequency to filter IK data.\n";
    cout << "-j                  IK threads       Specify the number of IK threads to be used.\n";
    cout << "-a                  Accuracy         Specify the IK solver accuracy.\n";
    cout << "--output            OutputDir        Specify the output directory\n";
    cout << "-v                                   Show visualiser\n";
}

int main(int argc, char* argv[]) {

    ProgramOptionsParser po(argc, argv);
    if (po.exists("-h") || po.empty()) {
        printHelp();
        exit(EXIT_SUCCESS);
    }

    string osimModelFilename;
    if (po.exists("--model"))
        osimModelFilename = po.getParameter("--model");
    else {
        printHelp();
        exit(EXIT_SUCCESS);
    }

    string trcTrialFilename;
    if (po.exists("--trc"))
        trcTrialFilename = po.getParameter("--trc");
    else {
        printHelp();
        exit(EXIT_SUCCESS);
    }

    string ikTaskFilename;
    if (po.exists("--task-set"))
        ikTaskFilename = po.getParameter("--task-set");
    else {
        printHelp();
        exit(EXIT_SUCCESS);
    }

    double fc(8);
    if (po.exists("--fc"))
        fc = po.getParameter<double>("--fc");
    else {
        printHelp();
        exit(EXIT_SUCCESS);
    }

    unsigned nThreads(1);
    if (po.exists("-j"))
        nThreads = po.getParameter<unsigned>("-j");

    double solverAccuracy(1e-5);
    if (po.exists("-a"))
        nThreads = po.getParameter<double>("-a");

    string resultDir("Output");
    if (po.exists("-output"))
        resultDir = po.getParameter("-output");

    bool showVisualiser(false);
    if (po.exists("-v"))
        showVisualiser = true;

    string stopWatchResultDir(resultDir);

    //define the shared buffer
    rtosim::MarkerSetQueue markerSetQueue;
    rtosim::GeneralisedCoordinatesQueue generalisedCoordinatesQueue, filteredGeneralisedCoordinatesQueue;

    //define the barriers
    rtosim::Concurrency::Latch doneWithSubscriptions;
    rtosim::Concurrency::Latch doneWithExecution;

    //define the filter
    auto coordNames = getCoordinateNamesFromModel(osimModelFilename);
    rtosim::GeneralisedCoordinatesStateSpace gcFilt(fc, coordNames.size());

    //define the threads
    //#read markers from file and save them in markerSetQueue
    rtosim::MarkersFromTrc markersFromTrc(
        markerSetQueue,
        doneWithSubscriptions,
        doneWithExecution,
        osimModelFilename,
        trcTrialFilename,
        false);

    //read from markerSetQueue, calculate IK, and save results in generalisedCoordinatesQueue
    rtosim::QueueToInverseKinametics inverseKinematics(
        markerSetQueue,
        generalisedCoordinatesQueue,
        doneWithSubscriptions,
        doneWithExecution,
        osimModelFilename,
        nThreads, ikTaskFilename, solverAccuracy);

    //read from generalisedCoordinatesQueue, filter using gcFilt,
    //and save filtered data in filteredGeneralisedCoordinatesQueue
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

    //read from filteredGeneralisedCoordinatesQueue and save to file
    rtosim::QueueToFileLogger<rtosim::GeneralisedCoordinatesData> filteredIkLogger(
        filteredGeneralisedCoordinatesQueue,
        doneWithSubscriptions,
        doneWithExecution,
        getCoordinateNamesFromModel(osimModelFilename),
        resultDir, "filtered_ik_from_file", "sto");

    //read from generalisedCoordinatesQueue and save to file
    rtosim::QueueToFileLogger<rtosim::GeneralisedCoordinatesData> rawIkLogger(
        generalisedCoordinatesQueue,
        doneWithSubscriptions,
        doneWithExecution,
        getCoordinateNamesFromModel(osimModelFilename),
        resultDir, "raw_ik_from_file", "sto");

    //read the frames from generalisedCoordinatesQueue and calculates some stats
    rtosim::FrameCounter<rtosim::GeneralisedCoordinatesQueue> ikFrameCounter(
        generalisedCoordinatesQueue,
        "raw_ik_from_file_timer");

    doneWithSubscriptions.setCount(5);
    doneWithExecution.setCount(5);

    //launch, execute, and join all the threads
    //all the multithreading is in this function
    if (showVisualiser) {
        rtosim::StateVisualiser visualiser(generalisedCoordinatesQueue, osimModelFilename);
        rtosim::QueuesSync::launchThreads(
            markersFromTrc,
            inverseKinematics,
            gcQueueAdaptor,
            filteredIkLogger,
            rawIkLogger,
            ikFrameCounter,
            visualiser
            );
    }
    else {
        rtosim::QueuesSync::launchThreads(
            markersFromTrc,
            inverseKinematics,
            gcQueueAdaptor,
            filteredIkLogger,
            rawIkLogger,
            ikFrameCounter
            );
    }
    //multithreaded part is over, all threads are joined

    //get execution time info from IK
    auto stopWatches = inverseKinematics.getProcessingTimes();

    rtosim::StopWatch combinedSW("Combined_IK_solvers");
    for (auto& s : stopWatches)
        combinedSW += s;
    cout << combinedSW << endl;
    rtosim::StopWatch ikOutputStopWatch = ikFrameCounter.getProcessingTimes();
    cout << ikOutputStopWatch;

    combinedSW.print(stopWatchResultDir);
    ikOutputStopWatch.print(stopWatchResultDir);
}