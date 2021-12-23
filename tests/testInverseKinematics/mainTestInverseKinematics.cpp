#include "rtosim/rtosim.h"
#include <OpenSim/OpenSim.h>
#include <iostream>

template<typename T>
void assert_equal(T expected, T found, T tolerance, std::string file = "", int line = -1, std::string message = "") {
    if (found < expected - tolerance || found > expected + tolerance)
        throw OpenSim::Exception(message, file, line);
}

inline void assert_(bool cond, std::string file = "", int line = -1, std::string message = "Exception") {
    if (!cond) throw OpenSim::Exception(message, file, line);
}

void checkStorageAgainstStandard(
        OpenSim::Storage &result,
        OpenSim::Storage &standard,
        OpenSim::Array<double> tolerances,
        std::string testFile, int testFileLine,
        std::string errorMessage) {

    std::vector<std::string> columnsUsed;
    std::vector<double> comparisons;
    result.compareWithStandard(standard, columnsUsed, comparisons);

    int columns = columnsUsed.size();

    assert_(columns > 0, testFile, testFileLine, errorMessage + "- no common columns to compare!");

    for (int i = 0; i < columns; ++i) {
        std::cout << "column:    " << columnsUsed[i] << std::endl;
        std::cout << "RMS error: " << comparisons[i] << std::endl;
        std::cout << "tolerance: " << tolerances[i] << std::endl << std::endl;
        assert_(comparisons[i] < tolerances[i], testFile, testFileLine, errorMessage);
    }
}


std::string runRtosimInverseKinematics(std::string dataDir, std::string ikSetupFilename, unsigned nThreads) {

    //Extract all the necessary filenames from the setup file
    OpenSim::InverseKinematicsTool ikt(dataDir + ikSetupFilename);
    std::string markerDataFilename = dataDir + ikt.getMarkerDataFileName();
    std::string modelFilename = dataDir + ikt.getPropertyByName("model_file").getValue<std::string>();
    OpenSim::IKTaskSet taskSet = ikt.getIKTaskSet();
    double accuracy = ikt.getPropertyByName("accuracy").getValue<double>();
    double constraintWeight = ikt.getPropertyByName("constraint_weight").getValue<SimTK::Real>();
    // Create the data queues
    rtosim::MarkerSetQueue markerSetQueue;
    rtosim::GeneralisedCoordinatesQueue generalisedCoordinatesQueue;

    //Create the synchronisation latches
    rtb::Concurrency::Latch doneWithSubscription, doneWithExecution;

    //Read the data from a trc file and move it to the appropriate `Queue`
    rtosim::MarkersFromTrc markerDataProducer(
            markerSetQueue,
            doneWithSubscription,
            doneWithExecution,
            modelFilename,
            markerDataFilename,
            false);

    //Read the data from `markerSetQueue` and solves inverse kinematics
    rtosim::QueueToInverseKinematics ik(
            markerSetQueue,
            generalisedCoordinatesQueue,
            doneWithSubscription,
            doneWithExecution,
            modelFilename,
            nThreads,
            taskSet,
            accuracy,
            constraintWeight);

    // Save the data to file
    std::string outputFilename("rtosim_ik_j" + std::to_string(nThreads));
    std::vector<std::string> columnLabels = rtosim::getCoordinateNamesFromModel(modelFilename);
    rtosim::QueueToFileLogger<rtosim::GeneralisedCoordinatesData> coordinateLogger(
            generalisedCoordinatesQueue,
            doneWithSubscription,
            doneWithExecution,
            columnLabels,
            dataDir, outputFilename, "mot");
    coordinateLogger.setConvertFromRadToDeg();
    doneWithSubscription.setCount(3);
    doneWithExecution.setCount(3);

    rtosim::QueuesSync::launchThreads(markerDataProducer, ik, coordinateLogger);
    return outputFilename + ".mot";
}

int main() {
    try {
        //Test rtosim IK using up to 9 threads and assess output accuracy
        for (unsigned j(1); j < 10; ++j) {
            // RTOSIM vs standard
            std::string dataDir = std::string(BASE_DIR) + "/data/InverseKinematics/";
            std::string rtosimResultsFilename = runRtosimInverseKinematics(dataDir,
                                                                           "subject01_Setup_InverseKinematics.xml", j);
            OpenSim::Storage rtosimResults(dataDir + rtosimResultsFilename);
            OpenSim::Storage standard(dataDir + "std_subject01_walk1_ik.mot");
            std::cout << "Testing rtosim against synthetised data\n";
            checkStorageAgainstStandard(rtosimResults, standard, OpenSim::Array<double>(0.2, 24), __FILE__, __LINE__,
                                        "testInverseKinematicsGait2354 failed");
        }
    }
    catch (const OpenSim::Exception &e) {
        e.print(std::cerr);
        return 1;
    }
    std::cout << "Done" << std::endl;
    return 0;
}