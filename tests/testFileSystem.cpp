#include "rtosim/FileSystem.h"
#include <iostream>
int main() {

    bool r1 = rtosim::FileSystem::createDirectory("testDir");
    std::cout << "testDir ";
    if (!r1)
        std::cout << "not ";
    std::cout << "created\n";

    bool r2 = rtosim::FileSystem::createDirectory("aDir/anotherDir");
    std::cout << "aDir/anotherDir ";
    if (!r2)
        std::cout << "not ";
    std::cout << "created\n";

    return 0;
}