BambooMC - A Geant4-based Monte Carlo simulation Program
========================================================

BambooMC is a Geant4-based Monte Carlo simulation program. It is mainly used in the PandaX projects <https://pandax.sjtu.edu.cn> for the simulation of backgrounds. It can be easily extended for other purpose.

## Design Philosophy

The BambooMC is designed to be a modularized simulation program based on GEANT4 that enables users to combine different detector components, physics processes and data managers freely. That would be very effective for the optimization of particle detectors.

Users of the program need to provide an XML file to define some key parameters. One can find example XML files in the `xml` directory. 

A typical usage of the program is

```bash
${path_to_bamboomc}/BambooMC -x xml_file [ -m macro_file ] [ -n nevents ] [ -o output_file ] [ -i ]
```

The `xml_file` gives the definition of the detectors, event generator, physics and analysis used for the simulation. It is necessary for a simulation. Other paramters are optional. The macro file uses the default G4macro format and can used to define some parameters in the simulation. The `-n` parameter defines how many events (nevents) will be simulated. The `-o` parameter defines the name of the output file. The `-i` option will make the program runs in interactive mode. Following gives a summary of these parameter.

+    `-x`: gives the XML file,
+    `-f`: gives the macro file, same as "-m"
+    `-m`: gives the macro file, same as "-f"
+    `-n`: gives the number of events to be simulated
+    `-o`: gives the output parameter
+    `-i`: program runs in interactvie mode 

### Build the program

We recommend to build the program in GNU/Linux. Building it in other OS has not been tested and verified.

#### Prerequisite

A C++ compiler supporting the C++11 standard is required (GCC 4.8.5, GCC).

CMake version 3.0 and above, Geant4 version 10.02 and above, built with Qt support, ROOT 5.34 or ROOT 6 are required.

One need to setup the environment variables of above software.

#### Build the program

The simplest way to build the program is given below

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_DETECTOR_SETS=example_detector ..
make
```

The option `ENABLE_DETECTOR_SETS` means to include the sources in the detector directory. Multiple sets of detectors can be added, separated by comma.




