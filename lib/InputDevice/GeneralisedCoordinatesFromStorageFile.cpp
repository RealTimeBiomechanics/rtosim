/* -------------------------------------------------------------------------- *
* CEINMS is a standalone toolbox for neuromusculoskeletal modelling and      *
* simulation. CEINMS can also be used as a plugin for OpenSim either         *
* through the OpenSim GUI or API. See https://simtk.org/home/ceinms and the  *
* NOTICE file for more information. CEINMS development was coordinated       *
* through Griffith University and supported by the Australian National       *
* Health and Medical Research Council (NHMRC), the US National Institutes of *
* Health (NIH), and the European Union Framework Programme 7 (EU FP7). Also  *
* see the PROJECTS file for more information about the funding projects.     *
*                                                                            *
* Copyright (c) 2010-2015 Griffith University and the Contributors           *
*                                                                            *
* CEINMS Contributors: C. Pizzolato, M. Reggiani, M. Sartori,                *
*                      E. Ceseracciu, and D.G. Lloyd                         *
*                                                                            *
* Author(s): C. Pizzolato                                                    *
*                                                                            *
* CEINMS is licensed under the Apache License, Version 2.0 (the "License").  *
* You may not use this file except in compliance with the License. You may   *
* obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0.*
*                                                                            *
* Unless required by applicable law or agreed to in writing, software        *
* distributed under the License is distributed on an "AS IS" BASIS,          *
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
* See the License for the specific language governing permissions and        *
* limitations under the License.                                             *
* -------------------------------------------------------------------------- */

#include "rtosim/GeneralisedCoordinatesFromStorageFile.h"
#include "rtosim/GeneralisedCoordinatesData.h"
#include "rtosim/ArrayConverter.h"
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <OpenSim/OpenSim.h>

namespace rtosim {

    GeneralisedCoordinatesFromStorageFile::GeneralisedCoordinatesFromStorageFile(
        GeneralisedCoordinatesQueue& outputGeneralisedCoordinatesQueue,
        Concurrency::Latch& doneWithSubscriptions,
        Concurrency::Latch& doneWithExecution,
        const std::string& osimModelFilename,
        const std::string& kinematicsStorageFilename,
        double fc) :
        GeneralisedCoordinatesFromX(outputGeneralisedCoordinatesQueue, doneWithSubscriptions, doneWithExecution),
        model_(osimModelFilename),
        kinematicsStorageFilename_(kinematicsStorageFilename) {

        //for some mysterious reason, if I don't use c_str it doesn't work..
        OpenSim::Storage kinematics(kinematicsStorageFilename_.c_str());
        OpenSim::Array<double> timeColumnOS;
        kinematics.getTimeColumn(timeColumnOS);
        ArrayConverter::toStdVector(timeColumnOS, timeColumn_);
        updateKinematicsSplines(fc);
        OpenSim::Array<string> coordinateNamesOS;
        model_.getCoordinateSet().getNames(coordinateNamesOS);
        ArrayConverter::toStdVector(coordinateNamesOS, coordinateNamesFromOsimModel_);
        mapper_.setNames(coordinateNamesFromOsimModel_, coordinateNamesFromOsimModel_);
    }

    GeneralisedCoordinatesFromStorageFile::GeneralisedCoordinatesFromStorageFile(
        GeneralisedCoordinatesQueue& outputGeneralisedCoordinatesQueue,
        Concurrency::Latch& doneWithSubscriptions,
        Concurrency::Latch& doneWithExecution,
        const std::string& osimModelFilename,
        const std::string& kinematicsStorageFilename,
        const std::vector<std::string>& coordinatesToLog,
        double fc) :
        GeneralisedCoordinatesFromStorageFile(
        outputGeneralisedCoordinatesQueue,
        doneWithSubscriptions,
        doneWithExecution,
        osimModelFilename,
        kinematicsStorageFilename,
        fc)  {

        mapper_.setNames(coordinateNamesFromOsimModel_, coordinatesToLog);
    }

    void GeneralisedCoordinatesFromStorageFile::operator()() {
        std::cout << "Kinematics storage filename " << kinematicsStorageFilename_ << std::endl << std::flush;

        GeneralisedCoordinatesFromX::doneWithSubscriptions();
        for (auto t : timeColumn_) {

            GeneralisedCoordinatesData generalisedCoordinates(mapper_.getOutSize());
            OpenSim::Array<double> q;
            vector<double> stdQ;
            splines_.evaluate(q, 0, t);
            ArrayConverter::toStdVector(q, stdQ);
            generalisedCoordinates.setQ(mapper_.map(stdQ));

            OpenSim::Array<double> qd;
            vector<double> stdQd;
            splines_.evaluate(qd, 1, t);
            ArrayConverter::toStdVector(qd, stdQd);
            generalisedCoordinates.setQd(mapper_.map(stdQd));

            OpenSim::Array<double> qdd;
            vector<double> stdQdd;
            splines_.evaluate(qdd, 2, t);
            ArrayConverter::toStdVector(qdd, stdQdd);
            generalisedCoordinates.setQdd(mapper_.map(stdQdd));
            GeneralisedCoordinatesFromX::updateGeneralisedCoordinates(generalisedCoordinates, t);
        }
        GeneralisedCoordinatesFromX::sendEndOfData();
        GeneralisedCoordinatesFromX::doneWithExecution();
    }

    void GeneralisedCoordinatesFromStorageFile::updateKinematicsSplines(double fc) {

        //reinitialise because I modify the storage with padding and filtering..
        OpenSim::Storage kinematics(kinematicsStorageFilename_);
        if (fc > 0) {
            kinematics.pad(kinematics.getSize() / 2.);
            kinematics.lowpassIIR(fc);
        }
        if (kinematics.isInDegrees()){
            model_.getSimbodyEngine().convertDegreesToRadians(kinematics);
        }
        // Create differentiable splines of the coordinate data
        OpenSim::GCVSplineSet kinematicsSplines(5, &kinematics);
        //Functions must correspond to model coordinates and their order for the solver
        OpenSim::Array<string> coordinateNamesOS;
        model_.getCoordinateSet().getNames(coordinateNamesOS);
        for (size_t i(0); i < coordinateNamesOS.getSize(); ++i){
            if (kinematicsSplines.contains(coordinateNamesOS.get(i))){
                kinematicsSplines.insert(i, kinematicsSplines.get(coordinateNamesOS.get(i)));
            }
            else{
                kinematicsSplines.insert(i, OpenSim::Constant(model_.getCoordinateSet().get(i).getDefaultValue()));
            }
        }
        if (kinematicsSplines.getSize() > coordinateNamesOS.getSize()){
            kinematicsSplines.setSize(coordinateNamesOS.getSize());
        }
        splines_ = kinematicsSplines;
    }
}