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

#include <sys/types.h>
#include <sys/stat.h>
#include <Simbody.h>
#ifdef _WIN32
#include <direct.h>
#define CREATE_DIR(path)  _mkdir(path)
#else
#define CREATE_DIR(path)  mkdir(path, 0755)
#endif

namespace rtosim {

    namespace FileSystem {

        bool createDirectory(const std::string& path) {

            auto fullPath(SimTK::Pathname::getAbsoluteDirectoryPathname(path));
            int err = CREATE_DIR(fullPath.c_str());
            //if err is 0, the directory was successfully created
            return err;
        }

        std::string getAbsolutePath(const std::string& path) {

            return SimTK::Pathname::getAbsolutePathname(path);
        }

        bool directoryExists(const std::string& path) {

            struct stat info;
            stat(path.c_str(), &info);
            return info.st_mode & S_IFDIR;
        }

        std::string concatenatePaths(const std::string& path1, const std::string& path2) {

            std::string fs(SimTK::Pathname::getPathSeparator());
            std::string concatenated;
            if (path1.empty())
                concatenated = path2;
            else if (path1.back() == fs.front())
                concatenated = path1 + path2;
            else  
                path1 + fs + path2;
            return concatenated;
        }
    }
}