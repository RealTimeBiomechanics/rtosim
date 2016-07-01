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

#include "rtosim/IKSolverParallelOsens.h"
#include "rtosim/OrientationSensorsReferenceFromQueue.h"
#include "rtosim/ArrayConverter.h"
#include "rtosim/EndOfData.h"
#include "rtosim/GeneralisedCoordinatesData.h"
#include "rtosim/queue/GeneralisedCoordinatesQueue.h"
#include "rtosim/queue/OrientationSetQueue.h"
#include "rtosim/OsimUtilities.h"
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
#include <OpenSim/Simulation/InverseKinematicsExtendedSolver.h>
#include <memory>
using std::unique_ptr;
#include <limits>

namespace rtosim{

    IKSolverParallelOsens::IKSolverParallelOsens(
        ThreadPoolJobs<OrientationSetFrame>& inputThreadPoolJobs,
        IKoutputs<rtosim::GeneralisedCoordinatesFrame>& outputGeneralisedCoordinatesQueue,
        rtosim::Concurrency::Latch& doneWithSubscriptions,
        rtosim::Concurrency::Latch& doneWithExecution,
        const std::string& osimModelFilename,
        double solverAccuracy,
        double constraintWeight
        ) :
        inputThreadPoolJobs_(inputThreadPoolJobs),
        outputGeneralisedCoordinatesQueue_(outputGeneralisedCoordinatesQueue),
        doneWithSubscriptions_(doneWithSubscriptions),
        doneWithExecution_(doneWithExecution),
        osimModelFilename_(osimModelFilename),
        model_(osimModelFilename),
        sovlerAccuracy_(solverAccuracy),
        constraintWeight_(constraintWeight) {
	  
        names_ = getOsensNamesFromModel(osimModelFilename);
	attachOsensToModel(model_);
	
        nOsens_ = names_.size();
	OpenSim::Array<std::string> coordinateNamesArray;
        model_.getCoordinateSet().getNames(coordinateNamesArray);
        rtosim::ArrayConverter::toStdVector(coordinateNamesArray, coordinateNames_);
        nCoordinates_ = model_.getNumCoordinates();
        for (auto it : names_)
            weights_.insert(std::make_pair(it, 1)); //init weights to 1
        //  const OpenSim::CoordinateSet& coordSet(model_.getCoordinateSet());
    }

    void IKSolverParallelOsens::setInverseKinematicsTaskSet(const string& ikTaskSetFilename) {

        //this should be fixed
        OpenSim::Object* p = OpenSim::Object::makeObjectFromFile(ikTaskSetFilename);
        OpenSim::IKTaskSet ikTaskSet = *(dynamic_cast<OpenSim::IKTaskSet*>(p));
        delete p;

        for (size_t i(0); i < static_cast<size_t>(ikTaskSet.getSize()); ++i) {
            std::string currentName(ikTaskSet.get(i).getName());
            auto it = weights_.find(currentName);
            if (it != weights_.end() && ikTaskSet.get(i).getApply()) {
                weights_[ikTaskSet.get(i).getName()] = ikTaskSet.get(i).getWeight();
                cout << currentName << "\t" << ikTaskSet.get(i).getWeight() << endl;
            }
            else
                cout << currentName << "\tskip" << endl;
        }
    }

    void IKSolverParallelOsens::pushState(const SimTK::State& s) {

        GeneralisedCoordinatesData currentData(nCoordinates_);
        std::vector<double> q(nCoordinates_);
        SimTK::Vector stateQ(s.getQ());
        for (unsigned i(0); i < nCoordinates_; ++i)
            q[i] = stateQ[i];
        currentData.setQ(q);
        outputGeneralisedCoordinatesQueue_.push({ s.getTime(), currentData });
    }

    bool IKSolverParallelOsens::isWithinRom(const SimTK::State& s) const {

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

    void IKSolverParallelOsens::operator()(){

        SimTK::State s = model_.initSystem();
        bool localRunCondition(true);
        std::vector<double> sortedWeights;
	for (auto it : names_)
            sortedWeights.push_back(weights_[it]);
	std::cout << "Names from constructor" << std::endl;
	for (auto n : names_) 
	  std::cout << n << std::endl;
        //I may need to use a raw pointer because of OpenSim..
        unique_ptr<OrientationSensorsReferenceFromQueue> osensReference(
	  new OrientationSensorsReferenceFromQueue(inputThreadPoolJobs_, names_, sortedWeights));
    //    OpenSim::Set<OpenSim::OrientationSensorWeight> osimWeights;
    //    for (auto it : names_)
     //       osimWeights.adoptAndAppend(new OpenSim::OrientationSensorWeight(it, weights_[it]));
      // osensReference->setOrientationSensorWeightSet(osimWeights);
      
   
	
        SimTK::Array_<OpenSim::CoordinateReference> coordinateRefs;
        double previousTime(0.);
        double currentTime(0.);
	auto names = osensReference->getNames();
	std::cout << "Names " << std::endl;
	for(unsigned j(0); j < names.size(); ++j)
	  std::cout << names[j] << "\n" << std::flush; 
	std::cout << "model name" << model_ << std::endl;
	OpenSim::InverseKinematicsExtendedSolver ikSolver(model_, *osensReference, coordinateRefs, constraintWeight_);
        /* 
	SimTK::Array_<double> currentWeights;
	osensReference->getWeights(s, currentWeights);
	std::cout << "Printing weights " << std::endl << std::flush;
	for(unsigned j(0); j <currentWeights.size(); ++j)
	  std::cout << currentWeights[j] << std::endl << std::flush;

	std::cout << "Printing osens weights " << std::endl << std::flush;	
	auto osensWeightSet = osensReference->getOrientationSensorWeightSet();
	for(unsigned i(0); i < osensWeightSet.getSize(); ++i)
	  cout << osensWeightSet.get(i).getWeight() << std::endl << std::flush;
	*/
	s = model_.initSystem();
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
                osensReference->purgeCurrentFrame();
                isAssembled = false;
            }
        }
        SimTK::State defaultState(s);
        currentTime = osensReference->getCurrentTime();
        s.updTime() = currentTime;
        previousTime = currentTime;
        pushState(s);
        unsigned ct = 0;
        //     auto start = std::chrono::system_clock::now();
        //init the stats, so we can start measuring the frame processing time correctly
        while (localRunCondition) {
            if (!osensReference->isEndOfData()){
                try{
		  stopWatch_.init();
                    ikSolver.track(s);
                    stopWatch_.log();
             //       if (!isWithinRom(s))
               //         s = defaultState;
                }
                catch (...) {
                    s = defaultState;
                }
                SimTK::Array_<double> markerErrors;
               // ikSolver.computeCurrentMarkerErrors(markerErrors);
                currentTime = osensReference->getCurrentTime();
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

    IKSolverParallelOsens::~IKSolverParallelOsens()
    {
#ifdef RTOSIM_DEBUG
        cout << " IKSolver " << std::this_thread::get_id() << " is closing" << endl;
        cout << "Closing IKsolver... " << endl << std::flush;
#endif
    }
}