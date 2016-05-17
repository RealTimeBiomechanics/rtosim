#include "rtosim/FileSystem.h"
#include <iostream>
int main() {

    bool r1 = rtosim::FileSystem::createDirectory("testDir");
    std::cout << "testDir ";
    if (!r1)
        std::cout << "not ";
    std::cout << "created\n";

    bool exists = rtosim::FileSystem::directoryExists("testDir");
    if (exists)
        std::cout << "exists\n";

    exists = rtosim::FileSystem::directoryExists("notExisting");
    if (exists)
        std::cout << "notExisting dir exists\n";
    else
        std::cout << "notExisting does not exist\n";

    bool r2 = rtosim::FileSystem::createDirectory("aDir/anotherDir");
    std::cout << "aDir/anotherDir ";
    if (!r2)
        std::cout << "not ";
    std::cout << "created\n";

    return 0;
}