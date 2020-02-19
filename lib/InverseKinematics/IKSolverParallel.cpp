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

#include "rtosim/IKSolverParallel.h"
#include "rtosim/MarkersReferenceFromQueue.h"
#include "rtosim/ArrayConverter.h"
#include "rtosim/EndOfData.h"
#include "rtosim/GeneralisedCoordinatesData.h"
#include "rtosim/queue/GeneralisedCoordinatesQueue.h"
using rtosim::GeneralisedCoordinatesData;
using rtosim::GeneralisedCoordinatesFrame;

#include <chrono>
#include <thread>
#include <chrono>
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;

#include <OpenSim/OpenSim.h>
#include <OpenSim/Simulation/CoordinateReference.h>
#include <OpenSim/Simulation/InverseKinematicsSolver.h>
#include <memory>
using std::unique_ptr;
#include <limits>

namespace rtosim{

    IKSolverParallel::IKSolverParallel(
        ThreadPoolJobs<MarkerSetFrame>& inputThreadPoolJobs,
        IKoutputs<rtosim::GeneralisedCoordinatesFrame>& outputGeneralisedCoordinatesQueue,
        rtb::Concurrency::Latch& doneWithSubscriptions,
        rtb::Concurrency::Latch& doneWithExecution,
        const std::string& osimModelFilename,
        double solverAccuracy,
        double contraintWeight
        ) :
        inputThreadPoolJobs_(inputThreadPoolJobs),
        outputGeneralisedCoordinatesQueue_(outputGeneralisedCoordinatesQueue),
        doneWithSubscriptions_(doneWithSubscriptions),
        doneWithExecution_(doneWithExecution),
        osimModelFilename_(osimModelFilename),
        model_(osimModelFilename),
        sovlerAccuracy_(solverAccuracy),
        contraintWeight_(contraintWeight) {

        OpenSim::Array<std::string> markerNamesArray, coordinateNamesArray;
        const_cast<OpenSim::MarkerSet&>(model_.getMarkerSet()).getMarkerNames(markerNamesArray);
        rtosim::ArrayConverter::toStdVector(markerNamesArray, markerNames_);
        nMarkers_ = markerNames_.size();

        model_.getCoordinateSet().getNames(coordinateNamesArray);
        rtosim::ArrayConverter::toStdVector(coordinateNamesArray, coordinateNames_);
        nCoordinates_ = model_.getNumCoordinates();

        for (auto it : markerNames_)
            markerWeights_.insert(std::make_pair(it, 1)); //init weights to 1

        //  const OpenSim::CoordinateSet& coordSet(model_.getCoordinateSet());
    }


	void IKSolverParallel::setInverseKinematicsTaskSet(const OpenSim::IKTaskSet& ikTaskSet) {

		for (size_t i(0); i < static_cast<size_t>(ikTaskSet.getSize()); ++i) {
			std::string currentMarkerName(ikTaskSet.get(i).getName());
			auto it = markerWeights_.find(currentMarkerName);
			if (it != markerWeights_.end() && ikTaskSet.get(i).getApply()) {
				markerWeights_[ikTaskSet.get(i).getName()] = ikTaskSet.get(i).getWeight();
			}
		}
	}

    void IKSolverParallel::setInverseKinematicsTaskSet(const string& ikTaskSetFilename) {

        //this should be fixed
        OpenSim::Object* p = OpenSim::Object::makeObjectFromFile(ikTaskSetFilename);
        OpenSim::IKTaskSet ikTaskSet = *(dynamic_cast<OpenSim::IKTaskSet*>(p));
        delete p;
		setInverseKinematicsTaskSet(ikTaskSet);
    }

    void IKSolverParallel::pushState(const SimTK::State& s) {

        GeneralisedCoordinatesData currentData(nCoordinates_);
        std::vector<double> q(nCoordinates_);
        SimTK::Vector stateQ(s.getQ());
        for (unsigned i(0); i < nCoordinates_; ++i)
            q[i] = stateQ[i];
        currentData.setQ(q);
        outputGeneralisedCoordinatesQueue_.push({ s.getTime(), currentData });
    }

    bool IKSolverParallel::isWithinRom(const SimTK::State& s) const {

        bool isInRom(true);

        auto q(s.getQ());
        for (unsigned i(0); i < nCoordinates_; ++i) {
            auto rangeMax(model_.getCoordinateSet().get(i).getRangeMax());
            auto rangeMin(model_.getCoordinateSet().get(i).getRangeMin());

            if (q[i] > rangeMax || q[i] < rangeMin) {
                isInRom = false;
                std::cout << coordinateNames_[i] << " is outside its range of motion" << std::endl;
            }
        }
        return isInRom;
    }

    void IKSolverParallel::operator()(){

        SimTK::State s = model_.initSystem();
        bool localRunCondition(true);
        std::vector<double> sortedMarkerWeights;
        for (auto it : markerNames_)
            sortedMarkerWeights.push_back(markerWeights_[it]);
        //I may need to use a raw pointer because of OpenSim..
        unique_ptr<MarkersReferenceFromQueue> markerReference(new MarkersReferenceFromQueue(inputThreadPoolJobs_, markerNames_, sortedMarkerWeights));

        OpenSim::Set<OpenSim::MarkerWeight> osimMarkerWeights;
        for (auto it : markerNames_)
            osimMarkerWeights.adoptAndAppend(new OpenSim::MarkerWeight(it, markerWeights_[it]));
        markerReference->setMarkerWeightSet(osimMarkerWeights);
        SimTK::Array_<OpenSim::CoordinateReference> coordinateRefs;

        double previousTime(0.);
        double currentTime(0.);
        OpenSim::InverseKinematicsSolver ikSolver(model_, *markerReference, coordinateRefs, contraintWeight_);
        ikSolver.setAccuracy(sovlerAccuracy_);
        doneWithSubscriptions_.wait();
        bool isAssembled(false);
        while (!isAssembled) {
            try {

                ikSolver.assemble(s);
                isAssembled = true;
            }
            catch (...){
                std::cerr << "Time " << s.getTime() << " Model not assembled" << std::endl;
                markerReference->purgeCurrentFrame();
                isAssembled = false;
            }
        }

        SimTK::State defaultState(s);
        currentTime = markerReference->getCurrentTime();
        s.updTime() = currentTime;
        previousTime = currentTime;
        pushState(s);
        unsigned ct = 0;
        //     auto start = std::chrono::system_clock::now();
        //init the stats, so we can start measuring the frame processing time correctly

        while (localRunCondition) {
            if (!markerReference->isEndOfData()){
                try{
                    stopWatch_.init();
                    ikSolver.track(s);
                    stopWatch_.log();
                    if (!isWithinRom(s))
                        s = defaultState;
                }
                catch (...) {
                    s = defaultState;
                }
                SimTK::Array_<double> markerErrors;
                ikSolver.computeCurrentMarkerErrors(markerErrors);
                currentTime = markerReference->getCurrentTime();
                s.updTime() = currentTime;
                previousTime = currentTime;
                pushState(s);
                defaultState = s;
                ++ct;
            }
            else {
                localRunCondition = false;
                outputGeneralisedCoordinatesQueue_.push(rtosim::EndOfData::get<GeneralisedCoordinatesFrame>());
            }
        }
#ifdef RTOSIM_DEBUG
        cout << " IKSolver " << std::this_thread::get_id() << " is closing\n";
#endif
        doneWithExecution_.wait();
    }

    IKSolverParallel::~IKSolverParallel()
    {
#ifdef RTOSIM_DEBUG
        cout << " IKSolver " << std::this_thread::get_id() << " is closing" << endl;
        cout << "Closing IKsolver... " << endl << std::flush;
#endif
    }
}