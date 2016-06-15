RTOSIM
======

RTOSIM is a set of efficient and extensible C++ libraries to connect OpenSim with different devices. RTOSIM can use data provided by motion capture systems to solve OpenSim inverse kinematics and inverse dynamics on a frame-by-frame basis. Multiple threads operate concurrently to remove idle times due to communications with input and output devices, and the data flow is automatically managed by RTOSIM in order to preserve data integrity and avoid race conditions.

Read more about RTOSIM at the [RTOSIM project page](https://simtk.org/home/rtosim)

Quick Start
-----------
Test data and examples

Dependencies
------------
RTOSIM depends on the following 
* cross-platform building: [CMake](http://www.cmake.org/) 2.8.8 or later
* compiler: [Visual Studio](http://www.visualstudio.com) 2013 or later (Windows only), [gcc](http://gcc.gnu.org/) 4.8.1 or later (typically on Linux), or [Clang](http://clang.llvm.org/) 3.4 or later (typically on Mac, possibly through Xcode)
* file managment: [Boost libraries](http://www.boost.org/), or [Boost libraries binaries](http://sourceforge.net/projects/boost/files/boost-binaries) (Windows only)


Install
-------
[RTOSIM Win32 installer](https://simtk.org/frs/download.php?file_id=4494) (does not require any external library).

Acknowledgments
---------------
* Australian National Health and Medical Research Council (628850)
* Royal Society of NZ Marsden Fund (12-UOA-1221)
* US National Institutes of Health Grant (R01EB009351)
* Commission of the European Union Grant (IFP7-ICT-2013-10-611695)
