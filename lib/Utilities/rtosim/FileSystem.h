#ifndef rtosim_FileSystem_h
#define rtosim_FileSystem_h

#include <string>

namespace rtosim {

    namespace FileSystem {

        std::string getAbsolutePath(const std::string& path);
        bool createDirectory(const std::string& path);
        bool directoryExists(const std::string& path);
        std::string concatenatePaths(const std::string& path1, const std::string& path2);
    }
}

#endif