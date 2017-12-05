RTOSIM
======

RTOSIM is a set of efficient and extensible C++ libraries to connect OpenSim with different devices. RTOSIM can use data provided by motion capture systems to solve OpenSim inverse kinematics and inverse dynamics on a frame-by-frame basis. Multiple threads operate concurrently to remove idle times due to communications with input and output devices, and the data flow is automatically managed by RTOSIM in order to preserve data integrity and avoid race conditions.

Read more about RTOSIM at the [RTOSIM project page](https://simtk.org/home/rtosim)

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
RTOSIM works on Windows, Mac, and Linux. Binary packages will be available for Windows.

Build
-----

1. Get and compile [Simbody](https://github.com/simbody/simbody/tree/Simbody-3.5.3). **IMPORTANT**: if you want to use multiple threads to solve the Inverse Kinematics it is
necessary to first [patch Simbody](#patch-simbody).
2. Get and compile [OpenSim](https://github.com/opensim-org/opensim-core/tree/v3.2.0/OpenSim) 
3. If you want to enable the real-time stream from Vicon Nexus you need to download and install [Vicon DataStream SDK](http://www.vicon.com/downloads)
    * Add the environmental variable VICONDATASTREAM\_INSTALL\_DIR that points to the installation directory of the Vicon DataStream SDK (must contain the file Client.h), as example
       c:\Program Files (x86)\Vicon\DataStream SDK\Win32\CPP
4. Build your project using CMake.
5. Run the examples using the [test data](#test-data).

Patch Simbody
-------------

If you want to use multiple threads to solve the OpenSim inverse kinematics using IpOpt as optimisation algorithm (which is the default algorithm for constrained optimisations in OpenSim), 
you need to patch Simbody first. To do this, get the files [IpLapackSolverInterface.cpp](https://github.com/cpizzolato/simbody/blob/master/SimTKmath/Optimizers/src/IpOpt/IpLapackSolverInterface.cpp) and 
[IpLapackSolverInterface.hpp](https://github.com/cpizzolato/simbody/blob/master/SimTKmath/Optimizers/src/IpOpt/IpLapackSolverInterface.hpp) and use them to replace the corresponding 
files in your Simbody distribution in the directory SimTKmath/Optimizers/src/IpOpt. Then, compile and install Simbody.

Test Data
---------

Get the [test data](https://drive.google.com/open?id=0BzMAK5L0QV2PUXk4SW9qcV9jSVU)

1. Using text files (you may need to adapt the commands to your directory structure):
   * `rtosim_ik_from_file` solves the inverse kinematics from marker trajectory (.trc) files. 
        * Example of use: `rtosim_ik_from_file --model rtosim-testdata/models/2392_scaled_clusters.osim --trc rtosim-testdata/unfilteredRawData/walking/walking.trc --task-set rtosim-testdata/setup/walking/ik_taskset.xml -v`
   * `rtosim_id_from_file` solves the inverse dynamics from a motion (.mot) file and from ground reaction forces (.mot). This example functions similarly to the OpenSim inverse dynamics tool. 
    The joint angles are filtered (not in real-time) by `rtosim_id_from_file`, while the ground reaction forces have to be pre-filtered.
        * Example of use: `rtosim_id_from_file --model rtosim-testdata/models/2392_scaled_clusters.osim  --mot rtosim-testdata/filteredData8Hz/walking/walking.mot --ext-loads rtosim-testdata/setup/walking/externalLoads.xml`
   * `rtosim_ik_id_from_file` solves inverse kinematics and inverse dynamics from raw marker trajectories and raw ground reaction forces. It works in the same way as  `rtosim_ik_id_from_nexus` but uses files as input.
       * Example of use: `rtosim_ik_id_from_file --model rtosim-testdata/models/2392_scaled_clusters.osim --trc rtosim-testdata/unfilteredRawData/walking/walking.trc --task-set rtosim-testdata/setup/walking/ik_taskset.xml --ext-loads rtosim-testdata/setup/walking/externalLoads.xml -v`
 
2. Using Vicon Nexus:
   * You need to have RTOSIM, Vicon Nexus (tested with version 2.2.3), Vicon Virtual System (version 1.3.2+). and Vicon DataStream SDK (version 1.5+) installed on your system. 
   * Open Vicon Nexus, and navigate to the directory viconRawData provided in the test data.
   * The subject tab of Vicon Nexus should automatically populate
   * Press the 'Go Live' button to enter in live mode. This is the mode used to recorda data during an acquisition.
   * Open Vicon Virtual System and load the files walking.x1d and walking.x2d
   * Press the 'Stream' button in Vicon Virtual System to start streaming the raw data to Vicon Nexus
   * Markers should appear in Vicon Nexus. Markers should be correctly autolabelled in real-time
   * Be sure you can see the ground reaction forces during the stance phases. If not, check that the force plates correctly appear unser the menu Device. Also, under the source 
        menu, check that the force plates, which names are `#1 Right` and `#2 Left` are connected to `#2 MX Giganet - Slot 1`
   * Execute the example file rtosim\_ik\_id\_from\_nexus (you need to adapt the command to your directory structure)

        ` rtosim_ik_id_from_nexus --model rtosim-testdata/models/2392_scaled_clusters.osim --task-set rtosim-testdata/setup/walking/ik_taskset.xml --ext-loads rtosim-testdata/setup/walking/externalLoads_nexus.xml -v `

   * To close the execution of RTOSIM press any key followed by `enter`
   * For the available options execute `rtosim_ik_id_from_nexus -h`

Adapt the software for your gait laboratory
-------------------------------------------

The representation of force plates moments in the version of Vicon Nexus used to test RTOSIM is not what you may expect. 
To have the correct representation of joint moments in the global reference system of the gait laboratory is necessary to perform a further transformation.
This requires to know the position of the force plates. You can get this by opening Vicon Nexus, clicking on your force plate -> Show Advanced -> Position. 
Currently, these values are harcoded in RTOSIM for the Griffith University gait laboratory. However, what you have to do to adapt it to your lab is to modify the function 
`DataFromNexus::getForcePlatePosition() const` with your force plates position. **IMPORTANT**: the position of the force plates must be consistent with the rotation used in the function 
`DataFromNexus::setAxisMapping(VDS::Client& client) const`. You need to provide the position of the force plate **already rotated**.

Licensing
---------

Please see the file called LICENSE.txt.


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

C. Pizzolato, M. Reggiani, L. Modenese & D. G. Lloyd (2016): Real-time inverse kinematics and inverse dynamics for lower limb applications using OpenSim, _Computer Methods in Biomechanics and Biomedical Engineering_, DOI:
10.1080/10255842.2016.1240789
To link to this article: http://dx.doi.org/10.1080/10255842.2016.1240789

Acknowledgments
---------------
* Australian National Health and Medical Research Council (628850)
* Royal Society of NZ Marsden Fund (12-UOA-1221)
* US National Institutes of Health Grant (R01EB009351)
* Commission of the European Union Grant (IFP7-ICT-2013-10-611695)
