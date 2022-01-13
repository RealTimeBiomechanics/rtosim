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

#include "rtosim/FileSystem.h"
#include "rtosim/queue/MarkerSetQueue.h"
namespace rtosim {

    template<typename DataType>
    FileLogger<DataType>::FileLogger(
        const std::vector<std::string>& columnLabels,
        const std::string& filename,
        const std::string& outputDir,
        const std::string& separator
        ) :
        columnLabels_(columnLabels),
        filename_(filename),
        outputDir_(outputDir),
        sp_(separator),
        conversion_(1.0)
    {    
        outputDir_ = FileSystem::concatenatePaths(outputDir_, "");
        std::cout << "Logging files in " << outputDir_ << std::endl;
    }

    template<typename DataType>
    void FileLogger<DataType>::logFrame(const FrameType& frame) {

        dataToWrite_.push_back(frame);
    }

    template<typename DataType>
    void FileLogger<DataType>::setFilename(const std::string& filename) {

        filename_ = filename;
    }

    template<typename DataType>
    void FileLogger<DataType>::setOutputDir(const std::string& outputDir) {

        outputDir_ = outputDir;
    }

    template<typename DataType>
    void FileLogger<DataType>::setColumnLabels(const std::vector<std::string>& columnLabels) {

        columnLabels_ = columnLabels;
    }

    template<typename DataType>
    void FileLogger<DataType>::clearData() {

        dataToWrite_.clear();
    }

    //return true if printed successfully
    template<typename DataType>
    bool FileLogger<DataType>::print() {

        if (dataToWrite_.size() < 1)
        {
            std::cout << "No data to write in " << filename_ << std::endl;
            return false;
        }
        if (!createFile())
            return false;
        initFile();
        writeToFile();
        outFile_->flush();
        outFile_->close();
        return true;
    }

    template<typename DataType>
    bool FileLogger<DataType>::printable() {

        if (createFile()) {
            outFile_->close();
            return true;
        }
        return false;
    }

    template<typename DataType>
    bool FileLogger<DataType>::createFile() {

        bool success(true);
        if (!outputDir_.empty()) {
            if (!FileSystem::directoryExists(outputDir_))
                success = !FileSystem::createDirectory(outputDir_);
            if (!success) {
                std::cout << "Cannot create output directory: " + outputDir_ << std::endl;
            }
        }
        std::string outFilename = FileSystem::concatenatePaths(outputDir_, filename_);

        outFile_.reset(new std::ofstream(outFilename.c_str()));

        if (!(outFile_->is_open()))  {
            std::cout << "ERROR: " + outFilename + " cannot be opened!\n";
            success = false;
        }
        return success;
    }

    template<typename DataType>
    void FileLogger<DataType>::initFile() {

        *outFile_ << "rtosim output" << std::endl;
        *outFile_ << "version=1" << std::endl;
        *outFile_ << "nRows=" << dataToWrite_.size() << std::endl;
        *outFile_ << "nColumns=" << dataToWrite_.back().data.size() + 1 << std::endl;
        *outFile_ << "inDegrees=" << (isInDegrees_ ? "yes" : "no") << std::endl;
        *outFile_ << "endheader" << std::endl;
        *outFile_ << "time" + sp_;
        for (auto& it : columnLabels_)
            *outFile_ << it << sp_;
        *outFile_ << std::endl;
    }

    template<>
    void FileLogger<std::vector<SimTK::Vec3>>::initFile()
    {

        *outFile_ << "rtosim output" << std::endl;
        *outFile_ << "version=1" << std::endl;
        *outFile_ << "nRows=" << dataToWrite_.size() << std::endl;
        *outFile_ << "nColumns=" << dataToWrite_.back().data.size() * 3 + 1 << std::endl;
        *outFile_ << "inDegrees=" << (isInDegrees_ ? "yes" : "no") << std::endl;
        *outFile_ << "endheader" << std::endl;
        *outFile_ << "time" + sp_;
        for (auto& it : columnLabels_)
            *outFile_ << it << "_x" << sp_ << it << "_y" << sp_ << it << "_z" << sp_;
        *outFile_ << std::endl;
    }

    template<>
    void FileLogger<MarkerSetData>::initFile()
    {

        *outFile_ << "rtosim output" << std::endl;
        *outFile_ << "version=1" << std::endl;
        *outFile_ << "nRows=" << dataToWrite_.size() << std::endl;
        *outFile_ << "nColumns=" << dataToWrite_.back().data.size() * 3 + 1 << std::endl;
        *outFile_ << "inDegrees=" << (isInDegrees_ ? "yes" : "no") << std::endl;
        *outFile_ << "endheader" << std::endl;
        *outFile_ << "time" + sp_;
        for (auto& it : columnLabels_)
            *outFile_ << it << "_x" << sp_ << it << "_y" << sp_ << it << "_z" << sp_;
        *outFile_ << std::endl;
    }

    template<>
    void FileLogger<GeneralisedCoordinatesData>::initFile()
    {
        *outFile_ << "rtosim output" << std::endl;
        *outFile_ << "version=1" << std::endl;
        *outFile_ << "nRows=" << dataToWrite_.size() << std::endl;
        *outFile_ << "nColumns=" << dataToWrite_.back().data.getNCoordinates() * 3 + 1 << std::endl;
        *outFile_ << "inDegrees=" << (isInDegrees_ ? "yes" : "no") << std::endl;
        *outFile_ << "endheader" << std::endl;
        *outFile_ << "time" + sp_;
        for (auto& it : columnLabels_)
            *outFile_ << it << sp_ << it << "_vel" << sp_ << it << "_acc" << sp_;
        *outFile_ << std::endl;
    }

    template<>
    void FileLogger<MultipleExternalForcesData>::initFile()
    {
        *outFile_ << "rtosim output" << std::endl;
        *outFile_ << "version=1" << std::endl;
        *outFile_ << "nRows=" << dataToWrite_.size() << std::endl;
        *outFile_ << "nColumns=" << dataToWrite_.back().data.size() * 12 + 1 << std::endl;
        *outFile_ << "inDegrees=" << (isInDegrees_ ? "yes" : "no") << std::endl;
        *outFile_ << "endheader" << std::endl;
        *outFile_ << "time" + sp_;
        for (auto& it : columnLabels_)
            *outFile_ <<
            it << "_vx" << sp_ <<
            it << "_vy" << sp_ <<
            it << "_vz" << sp_ <<
            it << "_px" << sp_ <<
            it << "_py" << sp_ <<
            it << "_pz" << sp_ <<
            it << "_mx" << sp_ <<
            it << "_my" << sp_ <<
            it << "_mz" << sp_ <<
            it << "_tx" << sp_ <<
            it << "_ty" << sp_ <<
            it << "_tz" << sp_;
        ;
        *outFile_ << std::endl;
    }

   
    template<typename DataType>
    void FileLogger<DataType>::writeToFile() {

        for (auto& dataIt : dataToWrite_) {
            *outFile_ << dataIt.time << sp_;
            for (auto& sampleIt : dataIt.data)
                *outFile_ << sampleIt << sp_;
            *outFile_ << std::endl;
        }
    }

    template<>
    void FileLogger<std::vector<SimTK::Vec3>>::writeToFile() {

        for (auto& dataIt : dataToWrite_) {
            *outFile_ << dataIt.time << sp_;
            for (auto& sampleIt : dataIt.data)
                *outFile_ << sampleIt[0] << sp_
                << sampleIt[1] << sp_
                << sampleIt[2] << sp_;
            *outFile_ << std::endl;
        }
    }

    template<>
    void FileLogger<MarkerSetData>::writeToFile() {

        for (auto& dataIt : dataToWrite_) {
            *outFile_ << dataIt.time << sp_;
            for (auto& sampleIt : dataIt.data)
                *outFile_ << sampleIt.getCoordinates()[0] << sp_
                << sampleIt.getCoordinates()[1] << sp_
                << sampleIt.getCoordinates()[2] << sp_;
            *outFile_ << std::endl;
        }
    }

    template<>
    void FileLogger<GeneralisedCoordinatesData>::writeToFile() {

        for (auto& dataIt : dataToWrite_) {
            *outFile_ << dataIt.time << sp_;
            auto q(dataIt.data.getQ());
            auto qd(dataIt.data.getQd());
            auto qdd(dataIt.data.getQdd());
            std::vector<std::string> translationKeywords{ "_tx", "_ty", "_tz" };
            for (unsigned i(0); i < dataIt.data.getNCoordinates(); ++i) {
                if (std::any_of(std::begin(translationKeywords), std::end(translationKeywords),
                    [&](const auto& str) {return columnLabels_[i].find(str) != std::string::npos; })) {
                    *outFile_ << q[i]  << sp_
                        << qd[i] << sp_
                        << qdd[i] << sp_;
                }
                else {
                    *outFile_ << q[i] * conversion_ << sp_
                        << qd[i] * conversion_ << sp_
                        << qdd[i] * conversion_ << sp_;
                }
            }
            *outFile_ << std::endl;
        }
    }

    template<>
    void FileLogger<MultipleExternalForcesData>::writeToFile() {

        for (auto& dataIt : dataToWrite_) {
            *outFile_ << dataIt.time << sp_;
            for (auto& v : dataIt.data) {
                auto f(v.getForce());
                auto p(v.getApplicationPoint());
                auto m(v.getMoments());
                auto t(v.getTorque());
                *outFile_
                    << f[0] << sp_
                    << f[1] << sp_
                    << f[2] << sp_
                    << p[0] << sp_
                    << p[1] << sp_
                    << p[2] << sp_
                    << m[0] << sp_
                    << m[1] << sp_
                    << m[2] << sp_
                    << t[0] << sp_
                    << t[1] << sp_
                    << t[2] << sp_;
            }
            *outFile_ << std::endl;
        }
    }
}