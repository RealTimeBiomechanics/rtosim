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
* Author(s): E. Ceseracciu, C. Pizzolato, M. Reggiani                        *
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

#ifndef rtosim_FileLogger_h
#define rtosim_FileLogger_h

//#include "ceinms/ExternalForceData.h"
#include "rtosim/queue/MultipleExternalForcesQueue.h"
#include "rtosim/GeneralisedCoordinatesData.h"

#include <vector>
#include <list>
#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <SimTKcommon.h>

namespace rtosim {
    template<typename DataType>
    class FileLogger {
    public:
        using FrameType = QueueData < DataType > ;
        using QueueType = Concurrency::Queue < FrameType > ;
        FileLogger(
            const std::vector<std::string>& columnLabels,
            const std::string& filename,
            const std::string& outputDir,
            const std::string& separator = "\t"
            );
        void logFrame(const FrameType& frame);
        void setFilename(const std::string& filename);
        void setOutputDir(const std::string& outputDir);
        void setColumnLabels(const std::vector<std::string>& columnLabels);
        std::string getFilename() { return filename_; }
        void clearData();
        bool print();
        bool printable();
    private:
        bool createFile();
        void initFile();
        void writeToFile();
        std::vector<std::string> columnLabels_;
        std::string outputDir_;
        std::string sp_;
        std::string filename_;
        std::shared_ptr<std::ofstream> outFile_;
        std::list< FrameType > dataToWrite_;
    };
}

#include "FileLogger.cpp"
#endif
