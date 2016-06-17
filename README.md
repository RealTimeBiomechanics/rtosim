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

* Cross-platform building: [CMake](http://www.cmake.org/) 2.8.8 or later
* Compiler: 
    * [Visual Studio](http://www.visualstudio.com) 2013 or later (Windows only) 
    * [gcc](http://gcc.gnu.org/) 4.8.1 or later (typically on Linux) 
    * [Clang](http://clang.llvm.org/) 3.4 or later (typically on Mac, possibly through Xcode)
* Required external libraries:
    * [Simbody](https://github.com/simbody/simbody/tree/Simbody-3.5.3) versions 3.5.1 to 3.5.3
    * [OpenSim](https://github.com/opensim-org/opensim-core/tree/v3.2.0/OpenSim) versions 3.2 or 3.3
* Optional external libraries: 
   * [Vicon DataStream SDK](http://www.vicon.com/downloads) version 1.5+


Install
-------
RTOSIM works on Windows, Mac, and Linux. Binary packages are available for Windows.

[RTOSIM Win32 installer](https://simtk.org/frs/download.php?file_id=4494) (does not require any external library).

Build
-----

1. Get and compile [Simbody](https://github.com/simbody/simbody/tree/Simbody-3.5.3). **IMPORTANT**: if you want to use multiple threads to solve the Inverse Kinematics it is
necessary to first [patch Simbody](#patch-simbody).
2. Get and compile [OpenSim](https://github.com/opensim-org/opensim-core/tree/v3.2.0/OpenSim) 
3. If you want to enable the real-time stream from Vicon Nexus you need to download and install [Vicon DataStream SDK](http://www.vicon.com/downloads)


Patch Simbody
-------------


Adapt the software for your gait laboratory
-------------------------------------------


Licensing
---------

Please see the file called License.


 Copyright (c) 2010-2016 C. Pizzolato, M. Reggiani                         

Licensed under the Apache License, Version 2.0 (the "License"); 
you may not use this file except in compliance with the License. 
You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0                   
                                                                           
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.                                             

Contacts
--------

If you need help using RTOSIM, please ask your questions in the RTOSIM Public Forum, available from the SimTK project page: https://simtk.org/projects/rtosim, 
or send an email to claudio.pizzolato@griffithuni.edu.au
You can send RTOSIM bug reports to claudio.pizzolato@griffithuni.edu.au
If you want to participate in developing RTOSIM, please send an email to monica.reggiani@gmail.com

Publications
------------

If you are using rtosim or part of it please cite:

*citation refs*


Acknowledgments
---------------
* Australian National Health and Medical Research Council (628850)
* Royal Society of NZ Marsden Fund (12-UOA-1221)
* US National Institutes of Health Grant (R01EB009351)
* Commission of the European Union Grant (IFP7-ICT-2013-10-611695)
