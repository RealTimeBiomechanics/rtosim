#ifndef rtosim_FileSystem_h
#define rtosim_FileSystem_h

#include <string>

namespace rtosim {

    namespace FileSystem {

        bool createDirectory(const std::string& directory);
    }
}

#endif