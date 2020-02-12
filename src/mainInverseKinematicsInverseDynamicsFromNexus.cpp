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

#include "rtosim/rtosim.h"
using namespace rtosim;

#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <Simbody.h>

void printAuthors() {

    cout << "Real-time OpenSim inverse kinematics and inverse dynamics" << endl;
    cout << "Authors: Claudio Pizzolato <c.pizzolato@griffit.edu.au>" << endl;
    cout << "         Monica Reggiani <monica.reggiani@unipd.it>" << endl << endl;
}

void printHelp() {

    printAuthors();

    auto progName(SimTK::Pathname::getThisExecutablePath());
    bool isAbsolute;
    string dir, filename, ext;
    SimTK::Pathname::deconstructPathname(progName, isAbsolute, dir, filename, ext);

    cout << "Option                Argument         Action / Notes\n";
    cout << "------                --------         --------------\n";
    cout << "-h                                     Print the command-line options for " << filename << ".\n";
    cout << "--model               ModelFilename    Specify the name of the osim model file for the investigation.\n";
    cout << "--task-set            TaskSetFilename  Specify the name of the XML TaskSet file containing the marker weights to be used.\n";
    cout << "--ext-loads           LoadsFilename    Specify the name of the XML ExternalLoads file.\n";
    cout << "--hostname            HostName         Specify IP address and port for Vicon Nexus.\n";
    cout << "--fc                  CutoffFrequency  Specify the name of lowpass cutoff frequency to filter IK and GRF data.\n";
    cout << "-j                    IK threads       Specify the number of IK threads to be used.\n";
    cout << "-a                    Accuracy         Specify the IK solver accuracy.\n";
    cout << "--output              OutputDir        Specify the output directory.\n";
    cout << "-v                                     Show visualiser.\n";
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

    string ikTaskFilename;
    if (po.exists("--task-set"))
        ikTaskFilename = po.getParameter("--task-set");
    else {
        printHelp();
        exit(EXIT_SUCCESS);
    }

    string externalLoadsXml;
    if (po.exists("--ext-loads"))
        externalLoadsXml = po.getParameter("--ext-loads");
    else {
        printHelp();
        exit(EXIT_SUCCESS);
    }


    string hostname("127.0.0.1:801");
    if (po.exists("--hostname"))
        hostname = po.getParameter("--hostname");

    double fc(8);
    if (po.exists("--fc"))
        fc = po.getParameter<double>("--fc");


    unsigned nThreads(1);
    if (po.exists("-j"))
        nThreads = po.getParameter<unsigned>("-j");

    double solverAccuracy(1e-5);
    if (po.exists("-a"))
        nThreads = po.getParameter<double>("-a");

    string resultDir("Output");
    if (po.exists("--output"))
        resultDir = po.getParameter("--output");

    bool showVisualiser(false);
    if (po.exists("-v"))
        showVisualiser = true;

    resultDir = rtosim::FileSystem::getAbsolutePath(resultDir);
    rtosim::FileSystem::createDirectory(resultDir);
    string stopWatchResultDir(resultDir);

    //define the shared buffers
    MarkerSetQueue markerSetQueue;
    GeneralisedCoordinatesQueue generalisedCoordinatesQueue, filteredGeneralisedCoordinatesQueue;
    MultipleExternalForcesQueue grfQueue, adaptedGrfQueue, filteredGrfQueue;
    ExternalTorquesQueue jointMomentsQueue;

    //define the barriers
    rtb::Concurrency::Latch doneWithSubscriptions;
    rtb::Concurrency::Latch doneWithExecution;
    FlowControl runCondition(true);

    //define the filter
    auto coordNames = getCoordinateNamesFromModel(osimModelFilename);
    GeneralisedCoordinatesStateSpace gcFilt(fc, coordNames.size());

    //define the threads
    DataFromNexus dataFromNexus(
        markerSetQueue,
        grfQueue,
        doneWithSubscriptions,
        doneWithExecution,
        runCondition,
        osimModelFilename,
        hostname);

    //read from markerSetQueue, calculate IK, and save results in generalisedCoordinatesQueue
    QueueToInverseKinametics inverseKinematics(
        markerSetQueue,
        generalisedCoordinatesQueue,
        doneWithSubscriptions,
        doneWithExecution,
        osimModelFilename,
        nThreads, ikTaskFilename, solverAccuracy);

    //read from generalisedCoordinatesQueue, filter using gcFilt,
    //and save filtered data in filteredGeneralisedCoordinatesQueue
    QueueAdapter <
        GeneralisedCoordinatesQueue,
        GeneralisedCoordinatesQueue,
        GeneralisedCoordinatesStateSpace
    > gcQueueAdaptor(
    generalisedCoordinatesQueue,
    filteredGeneralisedCoordinatesQueue,
    doneWithSubscriptions,
    doneWithExecution,
    gcFilt);

    //corrects the value of the COP when the
    //foot is not in contact with the force plate
    AdaptiveCop adaptiveCop(
        markerSetQueue,
        grfQueue,
        adaptedGrfQueue,
        doneWithSubscriptions,
        doneWithExecution,
        osimModelFilename,
        externalLoadsXml
        );

    using GrfFilter = QueueAdapter <
        MultipleExternalForcesQueue,
        MultipleExternalForcesQueue,
        MultipleExternalForcesDataFilterStateSpace > ;
    MultipleExternalForcesDataFilterStateSpace multipleExternalForcesDataFilterStateSpace(fc, 40, 2);
    GrfFilter grfFilter(
        adaptedGrfQueue,
        filteredGrfQueue,
        doneWithSubscriptions,
        doneWithExecution,
        multipleExternalForcesDataFilterStateSpace);

    //read the filtered coordinates from IK and the filtered GRF,
    //calculated the ID and write the results in jointMomentQueue
    QueuesToInverseDynamics queueToInverseDynamics(
        filteredGeneralisedCoordinatesQueue,
        filteredGrfQueue,
        jointMomentsQueue,
        doneWithSubscriptions,
        doneWithExecution,
        osimModelFilename,
        externalLoadsXml);

    //read from filteredGeneralisedCoordinatesQueue and save to file
    rtosim::QueueToFileLogger<GeneralisedCoordinatesData> filteredIkLogger(
        filteredGeneralisedCoordinatesQueue,
        doneWithSubscriptions,
        doneWithExecution,
        getCoordinateNamesFromModel(osimModelFilename),
        resultDir, "filtered_ik_from_nexus", "sto");

    //read from generalisedCoordinatesQueue and save to file
    rtosim::QueueToFileLogger<GeneralisedCoordinatesData> rawIkLogger(
        generalisedCoordinatesQueue,
        doneWithSubscriptions,
        doneWithExecution,
        getCoordinateNamesFromModel(osimModelFilename),
        resultDir, "raw_ik_from_nexus", "sto");

    //read from generalisedCoordinatesQueue and save to file
    rtosim::QueueToFileLogger<ExternalTorquesData> idLogger(
        jointMomentsQueue,
        doneWithSubscriptions,
        doneWithExecution,
        getCoordinateNamesFromModel(osimModelFilename),
        resultDir, "id_from_nexus", "sto");

    //read the frames from generalisedCoordinatesQueue and calculates some stats
    rtosim::FrameCounter<GeneralisedCoordinatesQueue> ikFrameCounter(
        generalisedCoordinatesQueue,
        "time-ik-throughput");

    //read the frames from generalisedCoordinatesQueue and calculates some stats
    rtosim::FrameCounter<ExternalTorquesQueue> idFrameCounter(
        jointMomentsQueue,
        "time-id-throughput");


    //measures the time that takes every single frame to appear in two different queues
    rtosim::TimeDifference<
        GeneralisedCoordinatesQueue,
        GeneralisedCoordinatesQueue> gcQueueAdaptorTimeDifference(
        generalisedCoordinatesQueue,
        filteredGeneralisedCoordinatesQueue,
        doneWithSubscriptions,
        doneWithExecution);

    rtosim::TimeDifference<
        MarkerSetQueue,
        GeneralisedCoordinatesQueue> ikTimeDifference(
        markerSetQueue,
        generalisedCoordinatesQueue,
        doneWithSubscriptions,
        doneWithExecution);

    rtosim::TimeDifference<
        MultipleExternalForcesQueue,
        MultipleExternalForcesQueue> grfFilterTimeDifference (
        grfQueue,
        filteredGrfQueue,
        doneWithSubscriptions,
        doneWithExecution);

    rtosim::TimeDifference<
        MarkerSetQueue,
        ExternalTorquesQueue> fromMarkersToIdTimeDifference(
        markerSetQueue,
        jointMomentsQueue,
        doneWithSubscriptions,
        doneWithExecution);


    auto trigger([&runCondition](){
        std::string cmd;
        bool run(true);
        std::cin >> cmd;
        runCondition.setRunCondition(false);
    });


    doneWithSubscriptions.setCount(13);
    doneWithExecution.setCount(13);

    //launch, execute, and join all the threads
    //all the multithreading is in this function
    if (showVisualiser) {
        StateVisualiser visualiser(generalisedCoordinatesQueue, osimModelFilename);
        QueuesSync::launchThreads(
            dataFromNexus,
            inverseKinematics,
            gcQueueAdaptor,
            adaptiveCop,
            grfFilter,
            queueToInverseDynamics,
            filteredIkLogger,
            rawIkLogger,
            idLogger,
            gcQueueAdaptorTimeDifference,
            ikTimeDifference,
            grfFilterTimeDifference,
            fromMarkersToIdTimeDifference,
            ikFrameCounter,
            idFrameCounter,
            trigger,
            visualiser
            );
    }
    else {
        QueuesSync::launchThreads(
            dataFromNexus,
            inverseKinematics,
            gcQueueAdaptor,
            adaptiveCop,
            grfFilter,
            queueToInverseDynamics,
            filteredIkLogger,
            rawIkLogger,
            idLogger,
            gcQueueAdaptorTimeDifference,
            ikTimeDifference,
            grfFilterTimeDifference,
            fromMarkersToIdTimeDifference,
            ikFrameCounter,
            idFrameCounter,
            trigger
            );
    }
    //multithreaded part is over, all threads are joined

    //get execution time infos
    auto stopWatches = inverseKinematics.getProcessingTimes();

    rtosim::StopWatch combinedSW("time-ikparallel-processing");
    for (auto& s : stopWatches)
        combinedSW += s;

    combinedSW.print(stopWatchResultDir);
    ikFrameCounter.getProcessingTimes().print(stopWatchResultDir);
    idFrameCounter.getProcessingTimes().print(stopWatchResultDir);
    queueToInverseDynamics.getProcessingTimes().print(stopWatchResultDir);

    ikTimeDifference.getWallClockDifference().print(stopWatchResultDir + "/time-markerqueue-to-jointangles.txt");
    gcQueueAdaptorTimeDifference.getWallClockDifference().print(stopWatchResultDir + "/time-jointangles-to-filteredjointangles.txt");
    grfFilterTimeDifference.getWallClockDifference().print(stopWatchResultDir + "/time-grf-to-filteredgrf.txt");
    fromMarkersToIdTimeDifference.getWallClockDifference().print(stopWatchResultDir + "/time-markers-to-jointmoments.txt");
    dataFromNexus.printLatencyData(stopWatchResultDir + "/time-latency-nexus.txt");

    return 0;
}
