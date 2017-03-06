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

    cout << "Real-time OpenSim inverse dynamics" << endl;
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
    cout << "--mot               MotFilename      Specify the name of the mot file containing the results of IK.\n";
    cout << "--ext-loads         LoadsFilename    Specify the name of the XML ExternalLoads file.\n";
    cout << "--fc                CutoffFrequency  Specify the name of lowpass cutoff frequency to filter IK data.\n";
    cout << "--output            OutputDir        Specify the output directory.\n";
    cout << "--push-frequency    PushFrequency    Specify the frequency to which the GRF are read from the storage file.\n";
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

    string motTrialFilename;
    if (po.exists("--mot"))
        motTrialFilename = po.getParameter("--mot");
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

    double fc(8);
    if (po.exists("--fc"))
        fc = po.getParameter<double>("--fc");

    string resultDir("Output");
    if (po.exists("--output"))
        resultDir = po.getParameter("--output");

    double pushFrequency(-1);
    if (po.exists("--push-frequency"))
        pushFrequency = po.getParameter<double>("--push-frequency");

    resultDir = rtosim::FileSystem::getAbsolutePath(resultDir);
    FileSystem::createDirectory(resultDir);
    string stopWatchResultDir(resultDir);
    std::vector<string> dofLabels(getCoordinateNamesFromModel(osimModelFilename));

    MultipleExternalForcesQueue grfQueue;
    GeneralisedCoordinatesQueue coordinatesQueue;
    ExternalTorquesQueue jointMomentsQueue;
    rtb::Concurrency::Latch doneWithSubscription, doneWithExecution;

    ExternalForcesFromStorageFile grfProducer(
        grfQueue,
        doneWithSubscription,
        doneWithExecution,
        externalLoadsXml);

    grfProducer.setOutputFrequency(pushFrequency);
    //prefilter the data
    GeneralisedCoordinatesFromStorageFile ikProducer(
        coordinatesQueue,
        doneWithSubscription,
        doneWithExecution,
        osimModelFilename,
        motTrialFilename,
        fc);

    QueuesToInverseDynamics inverseDynamicsFromQueue(
        coordinatesQueue,
        grfQueue,
        jointMomentsQueue,
        doneWithSubscription,
        doneWithExecution,
        osimModelFilename,
        externalLoadsXml);

    QueueToFileLogger<ExternalTorquesData> inverseDynamicsFileLogger(
        jointMomentsQueue,
        doneWithSubscription,
        doneWithExecution,
        dofLabels,
        resultDir,
        "id",
        "sto");

    FrameCounter<ExternalTorquesQueue> idFrameCounter(jointMomentsQueue, "time-id-throughput");

    doneWithSubscription.setCount(4);
    doneWithExecution.setCount(4);

    QueuesSync::launchThreads(
        grfProducer,
        ikProducer,
        inverseDynamicsFromQueue,
        inverseDynamicsFileLogger,
        idFrameCounter
        );

    inverseDynamicsFromQueue.getProcessingTimes().print(stopWatchResultDir);

    return 0;
}