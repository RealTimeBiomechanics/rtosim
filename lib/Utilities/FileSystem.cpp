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

        bool directoryExists(const std::string& path) {

            struct stat info;
            stat(path.c_str(), &info);
            return info.st_mode & S_IFDIR;
        }

        std::string concatenatePaths(const std::string& path1, const std::string& path2) {

            std::string fs(SimTK::Pathname::getPathSeparator());
            return path1 + fs + path2;
        }
    }
}