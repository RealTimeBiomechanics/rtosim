#include "rtosim/rtosim.h"
#include "rtb/concurrency/Concurrency.h"
#include <string>
#include <thread>

/*This example shows how to use rtosim APIs to connect to Vicon Nexus.
Requirements: ViconDataStreamSDK 1.9+, Vicon Nexus, Vicon Virtual System [optional].

Tested using Vicon Nexus 2.10, ViconDataStreamSDK 1.9, Vicon Virtual System 1.4.1.
Before running this example, ensure your ViconDataStreamSDK directory containing the appriate dll
files is in your system path. For ViconDataStreamSDK 1.9 this directory is Vicon/DataStream SDK/Win64/CPP

Open Vicon Nexus, using the Data Management panel open the directory rtosim/data/ViconNexus. 
Open the trial walking_1 and click Play. In the 3D perspective you should see 
the labelled markers moving. This is all you need to do, leave Vicon Nexus playing the trail and 
launch this example. Every time you replay a C3D file, Vicon Nexus streams the data, which can be 
captured using the ViconDataStreamSDK. In this modality, Vicon Nexus does
not always stream all data frames, and some are dropped to maintain performance. If you want to force 
all frames to be streamed you need to select Play Every Frame in the dropdown menu close to the
Play button.

If you want to simulate laboratory condition, you can use the Vicon Virtual System. This allows
to emulate the connection with Vicon cameras, including real-time labelling of the subject.
Open Vicon Virtual System. Load the .x1d and .x2d files and click stream. Now open Vicon Nexus,
click on Go Live, under Manage Camera Calibration, load the .xcp file. You should now see the 
markers being streamed and automatically labelled in the 3D Perspective section of Vicon Nexus.
You can now adjust the camera frequency in the left panel. You can now launch this example.
*/


//Stream markers only
void streamMarkers(std::string hostname) {

	std::string inputDir = std::string(BASE_DIR) + "/tests/testViconNexus/input";
	std::string outputDir = std::string(BASE_DIR) + "/tests/testViconNexus/output";

	// Create latches to ensure all threads have finished their initialisation
	// before producing/consuming data
	rtb::Concurrency::Latch doneWithSubscription, doneWithExecution;
	
	// This is an atomic structure that allows to send a command to terminate the
	// flow of data from the DataFromNexus. 
	rtosim::FlowControl runCondition;

	//This is the queue where all the market data from Vicon Nexus will be stored. The markers 
	//will be stored with the same order as they appear in the OpenSim model.
	rtosim::MarkerSetQueue markerSetQueue;

	// Define the markers names to read from Nexus. Data will appear in this order in `markerSetQueue`
	std::vector<std::string> markersNames{ "RASI", "LASI"};

	//Create the reader from Vicon Nexus
	rtosim::DataFromNexus nexusReader(
		markerSetQueue, 
		doneWithSubscription, 
		doneWithExecution,
		runCondition,
		markersNames,
		hostname);
	
	// Create a file logger to log data of type MarkerSetData. This will read from
	// `markerSetQueue` until end of data. Data is printed at the end of the execution 
	// to prevent excessive writing operations to disk.
	rtosim::QueueToFileLogger<rtosim::MarkerSetData> markerLogger(
		markerSetQueue,
		doneWithSubscription,
		doneWithExecution,
		markersNames,
		outputDir,
		"markers", ".trc");


	// Send the shutdown message to `nexusReader` after 30 seconds
	auto countdown([&]() {
		doneWithSubscription.wait();
		for (unsigned i = 10; i > 0; --i) {
			std::cout << "Terminating execution in " << i << " s" << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		runCondition.setRunCondition(false);
		doneWithExecution.wait();
		});

	// Each of the two latches is used two times, so we need to set the counter at 2
	doneWithExecution.setCount(3);
	doneWithSubscription.setCount(3);
	
	rtosim::QueuesSync::launchThreads(
		nexusReader,
		markerLogger,
		countdown
	);

}

int main() {
	//You can connect to a different computer running Vicon Nexus by 
	//providing the appropriate address
	streamMarkers("127.0.0.1");
	return 0;
}