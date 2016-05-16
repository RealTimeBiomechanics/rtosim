#include "rtosim/FileSystem.h"

#include <Simbody.h>
#ifdef _WIN32
#include <direct.h>
#define CREATE_DIR(path)  _mkdir(path)
#else
#include <sys/stat.h>
#define CREATE_DIR(path)  mkdir(#path, 0755)
#endif

namespace rtosim {

    namespace FileSystem {

        bool createDirectory(const std::string& directory) {

            auto fullPath(SimTK::Pathname::getAbsoluteDirectoryPathname(directory));
            int err = CREATE_DIR(fullPath.c_str());
            //if err is 0, the directory was successfully created
            return err == 0;
        }
    }
}