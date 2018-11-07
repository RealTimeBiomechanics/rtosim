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
#include "rtosim/QueueToInverseKinematicsOsens.h"
#include <OpenSim/Simulation/Model/OrientationSensor.h>
#include <OpenSim/Simulation/Model/OrientationSensorSet.h>
#include <OpenSim/Tools/InverseKinematicsExtendedTool.h>
#include <OpenSim/Tools/IKExtendedTaskSet.h>
#include <OpenSim/Simulation/Model/ComponentSet.h>
#include <OpenSim/Common/OrientationSensorData.h>

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
    cout << "--mot               MotFilename      Specify the name of the mot file to be used.\n";
    cout << "--task-set          TaskSetFilename  Specify the name of the XML TaskSet file containing the sensors weights to be used.\n";
    cout << "--fc                CutoffFrequency  Specify the name of lowpass cutoff frequency to filter IK data.\n";
    cout << "-j                  IK threads       Specify the number of IK threads to be used.\n";
    cout << "-a                  Accuracy         Specify the IK solver accuracy.\n";
    cout << "--output            OutputDir        Specify the output directory.\n";
    cout << "--push-frequency    PushFrequency    Specify the frequency to which the quaternions are read from the storage file.\n";
    cout << "-v                                   Show visualiser.\n";
}

int main(int argc, char* argv[]) {

  OpenSim::Object::registerType(OpenSim::OrientationSensor());
  OpenSim::Object::registerType(OpenSim::IKExtendedTaskSet());
  OpenSim::Object::registerType(OpenSim::IKOrientationSensorTask());
  OpenSim::Object::registerType(OpenSim::InverseKinematicsExtendedTool());
  
    ProgramOptionsParser po(argc, argv);
    if (po.exists("-h") || po.empty()) {
        printHelp();
        exit(EXIT_SUCCESS);
    }

    string osimModelFilename;
    if (po.exists("--model"))
        osimModelFilename = po.getParameter("--model");
    else {
		cout << "Missing --model option" << endl;
        printHelp();
        exit(EXIT_SUCCESS);
    }

    string motTrialFilename;
    if (po.exists("--mot"))
        motTrialFilename = po.getParameter("--mot");
    else {
		cout << "Missing --mot option" << endl;
        printHelp();
        exit(EXIT_SUCCESS);
    }

    string ikTaskFilename;
    if (po.exists("--task-set"))
        ikTaskFilename = po.getParameter("--task-set");
    else {
		cout << "Missing --task-set option" << endl;
        printHelp();
        exit(EXIT_SUCCESS);
    }

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

    double pushFrequency(-1);
    if (po.exists("--push-frequency"))
        pushFrequency = po.getParameter<double>("--push-frequency");

    bool showVisualiser(false);
    if (po.exists("-v"))
        showVisualiser = true;

    resultDir = rtosim::FileSystem::getAbsolutePath(resultDir);
    rtosim::FileSystem::createDirectory(resultDir);
    string stopWatchResultDir(resultDir);
	bool isAbsolute; string trialFilename, tempDir, tempExtension;
	SimTK::Pathname::deconstructPathname(motTrialFilename, isAbsolute, tempDir, trialFilename, tempExtension);
	string outputFilename = trialFilename + "-ik";
    //define the shared buffer
    rtosim::OrientationSetQueue orientationsSetQueue;
    rtosim::GeneralisedCoordinatesQueue generalisedCoordinatesQueue, filteredGeneralisedCoordinatesQueue;

    //define the barriers
    rtb::Concurrency::Latch doneWithSubscriptions;
    rtb::Concurrency::Latch doneWithExecution;

    //define the filter
    auto coordNames = getCoordinateNamesFromModel(osimModelFilename);
    rtosim::GeneralisedCoordinatesStateSpace gcFilt(fc, coordNames.size());

    //define the threads
    //read markers from file and push them in orientationsSetQueue
    rtosim::OrientationsFromMot orientationsFromMot(
        orientationsSetQueue,
        doneWithSubscriptions,
        doneWithExecution,
        osimModelFilename,
        motTrialFilename,
        false);

    orientationsFromMot.setOutputFrequency(pushFrequency);

    //read from markerSetQueue, calculate IK, and push results in generalisedCoordinatesQueue
    rtosim::QueueToInverseKinematicsOsens inverseKinematics(
        orientationsSetQueue,
        generalisedCoordinatesQueue,
        doneWithSubscriptions,
        doneWithExecution,
        osimModelFilename,
        nThreads, solverAccuracy);

    //read from generalisedCoordinatesQueue, filter using gcFilt,
    //and push filtered data in filteredGeneralisedCoordinatesQueue
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
        resultDir, outputFilename+ "-filtered", ".mot");
    //read from generalisedCoordinatesQueue and save to file
    rtosim::QueueToFileLogger<rtosim::GeneralisedCoordinatesData> rawIkLogger(
        generalisedCoordinatesQueue,
        doneWithSubscriptions,
        doneWithExecution,
        getCoordinateNamesFromModel(osimModelFilename),
        resultDir, outputFilename, ".mot");
    //calculate the ik throughput time
    rtosim::FrameCounter<rtosim::GeneralisedCoordinatesQueue> ikFrameCounter(
        generalisedCoordinatesQueue,
        "time-ik-throughput");
    //measures the time that takes every single frame to appear in two different queues
    rtosim::TimeDifference<
        rtosim::GeneralisedCoordinatesQueue,
        rtosim::GeneralisedCoordinatesQueue> gcQueueAdaptorTimeDifference(
        generalisedCoordinatesQueue,
        filteredGeneralisedCoordinatesQueue,
        doneWithSubscriptions,
        doneWithExecution);
    rtosim::TimeDifference<
        rtosim::OrientationSetQueue,
        rtosim::GeneralisedCoordinatesQueue> ikTimeDifference(
        orientationsSetQueue,
        generalisedCoordinatesQueue,
        doneWithSubscriptions,
        doneWithExecution);
    doneWithSubscriptions.setCount(7);
    doneWithExecution.setCount(7);

    //launch, execute, and join all the threads
    //all the multithreading is in this function
   
    if (showVisualiser) {
        rtosim::StateVisualiser visualiser(filteredGeneralisedCoordinatesQueue, osimModelFilename);
        rtosim::QueuesSync::launchThreads(
            orientationsFromMot,
            inverseKinematics,
            gcQueueAdaptor,
            filteredIkLogger,
            gcQueueAdaptorTimeDifference,
            ikTimeDifference,
            rawIkLogger,
            ikFrameCounter,
            visualiser
            );
    }
    else {
        rtosim::QueuesSync::launchThreads(
            orientationsFromMot,
            inverseKinematics,
            gcQueueAdaptor,
            filteredIkLogger,
            gcQueueAdaptorTimeDifference,
            ikTimeDifference,
            rawIkLogger,
            ikFrameCounter
            );
    }
    
    //multithreaded part is over, all threads are joined
    auto stopWatches = inverseKinematics.getProcessingTimes();
    rtosim::StopWatch combinedSW("time-ikparallel-processing");
    for (auto& s : stopWatches)
        combinedSW += s;
    std::cout << "Printing results in " << stopWatchResultDir << endl;
    combinedSW.print(stopWatchResultDir);
    ikFrameCounter.getProcessingTimes().print(stopWatchResultDir);
    ikTimeDifference.getWallClockDifference().print(stopWatchResultDir + "/time-markerqueue-to-jointangles.txt");
    gcQueueAdaptorTimeDifference.getWallClockDifference().print(stopWatchResultDir + "/time-jointangles-to-filteredjointangles.txt");
    
    return 0;
}