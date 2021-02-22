# Description: #

Opulus is a simple Petri Net simulator/editor written in C++/Qt5. This is the fork of [opulus.sourceforge.net](opulus.sourceforge.net).

![1.png](https://bitbucket.org/repo/7X6BR9/images/653296000-1.png)

## Build status ##
| Platform | CI |
|----------|--:|
| <img src="https://raw.githubusercontent.com/zettdaymond/Font-Awesome/master/svgs/brands/linux.svg?sanitize=true" height="20" /> Linux | ![Linux](https://github.com/zettdaymond/opulus/workflows/Linux/badge.svg)|
| <img src="https://raw.githubusercontent.com/zettdaymond/Font-Awesome/master/svgs/brands/apple.svg?sanitize=true" height="20" /> MacOS | ![MacOS](https://github.com/zettdaymond/opulus/workflows/MacOS/badge.svg)|
| <img src="https://raw.githubusercontent.com/zettdaymond/Font-Awesome/master/svgs/brands/windows.svg?sanitize=true" height="20" /> Windows | ![Windows](https://github.com/zettdaymond/opulus/workflows/Windows/badge.svg) |


# Features: #
* Intuitive interface.
* Create and edit Petri Net in several ways: using visual graph, D+ and D- matrices, or IO-functions.
* Generate the Petri Net coverage graph.
* Basic net property and transition property analysis.
* Export net graph to SVG, PNG and PDF.

# Dependencies: #
To compile Opulus you will need:

* *Qt* >= 5.3
* *CMake* >= 3.15
* Compiler that supports C++14 (*GCC* >= 4.9 or *Clang* >= 3.4)
* *Eigen* >= 3.2

The coverage graph plugin uses *dot (graphviz)*, so you also need it installed if you want to use it.

# Compiling #
First of all, clone the repo to your machine:
```
$ git clone https://bitbucket.org/zettdaymond/opulus
$ cd ./opulus
$ git submodule update --init
```
Then to compile just type:
```
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build . --config Release --target all
```
If you want to use another install prefix (the default is /usr/local) type:
```
$ cmake -DCMAKE_INSTALL_PREFIX=/my/prefix ..
```
instead of `cmake ..`

On some distros (like Arch Linux) /usr/local/bin is not in the path, so is better to install on /usr, e.g:
```
$ cmake -DCMAKE_INSTALL_PREFIX=/usr ..
```

# Packaging #
On some platforms CMake can zip opulus and all dependent libraries together to prepare for distribution.
For this, after compilation, just type in build directory:
```
$ cmake --build package .
```

# Build options #

- **WITH_PLUGINS=ON/OFF** 

Switch it ON, if you want to build and install reachability and coverage graph analysis plugins.

- **WITH_TRANSLATIONS=ON/OFF**

Switch it ON, if you want to build and install Spanish(partially), Portuguese(partially) and Russian translation files.

- **WITH_TESTS=ON/OFF**

Switch it ON, if you want to build program tests.

- **BUILD_STATIC_CORE_LIBS=ON/OFF** 

Switch it ON, if you want to embed opulusgui and opuluscore libraries statically in application.

- **BUILD_STATIC_PLUGINS=ON/OFF**

Switch it ON, if you want to embed reachability and coverage graph analysis plugins in application.

- **BUILD_WITH_QT5_STATIC=ON/OFF** 

Switch it ON, if you build Opulus with **STATIC** version of Qt5. Build will fail, in another case.

- **USE_REVISION** 

Use revision number and commit hash instead of numeric version. Disable if building final release or without git.

- **DEPLOY_QT_AND_SYSTEM_LIBRARIES=ON/OFF**

Switch it ON, if you want redistribute Qt and system shared libraries during `cmake --build . --target package` command.


- **COVERAGE_GRAPH_DOT=ON/OFF**

Switch it ON, if you want to build coverage graph plugin dynamically linked with GraphViz libraries. CMake will try to find GraphViz libraries on your system.

- **COVERAGE_GRAPH_DOT_static=ON/OFF**

Switch it ON, if you want to build coverage graph plugin statically linked with GraphViz libraries. In this case, CMake build dependent liraries itself (git submodule)

# About Windows: #
The easiest way to compile on Windows is to use MSYS2. To install all dependencies and build opulus execute following commands in MinGW shell:
```
$ pacman -Syu
$ pacman -S git mingw-w64-x86_64-gcc mingw-w64-x86_64-gcc-libs mingw-w64-x86_64-cmake mingw-w64-x86_64-qt5 mingw-w64-x86_64-cmake mingw-w64-x86_64-eigen3 mingw-w64-x86_64-ninja  mingw-w64-x86_64-libtool
$ git clone https://bitbucket.org/zettdaymond/opulus
$ cd ./opulus
$ git submodule update --init
$ mkdir build && cd build
$ cmake -G "Ninja" -DDEPLOY_QT_AND_SYSTEM_LIBRARIES=ON -DCOVERAGE_GRAPH_DOT_STATIC=ON ..
$ cmake --build . --config Release --target package
```
or using qt5-static distribution:
```
$ pacman -Syu
$ pacman -S git mingw-w64-x86_64-gcc mingw-w64-x86_64-gcc-libs mingw-w64-x86_64-cmake mingw-w64-x86_64-qt5-static mingw-w64-x86_64-cmake mingw-w64-x86_64-eigen3 mingw-w64-x86_64-ninja  mingw-w64-x86_64-libtool
$ git clone https://bitbucket.org/zettdaymond/opulus
$ cd ./opulus
$ git submodule update --init
$ mkdir build && cd build
$ cmake -G "Ninja" -DQt5_DIR=/mingw64/qt5-static/lib/cmake/Qt5 -DQT_QMAKE_EXECUTABLE=/mingw64/qt5-static/qmake -DQt5Core_DIR=/mingw64/qt5-static/lib/cmake/Qt5Core -DBUILD_STATIC_CORE_LIBS=ON -DBUILD_STATIC_PLUGINS=ON -DCOVERAGE_GRAPH_DOT_STATIC=ON ..
$ cmake --build . --config Release --target package
```

Replace `mingw-w64-x86_64...` with `mingw-w64-i686...` if you want to build 32-bit binaries. 

# Releases #
We provide binary builds for Windows. See https://bitbucket.org/zettdaymond/opulus/downloads. 

Note, that the our build will not work on processors, older than Intel Core2Duo.


# Autors: #
- Hugo Parente <hugo.pl@gmail.com>
- Adauto Trigueiro de Almeida Filho <adautofilho@gmail.com>
- Clerton Ribeiro de Araújo Filho <clertonfilho@gmail.com>
- Zett Daymond <zettday@gmail.com>
- Cat <captainslowpoke@gmail.com>

# Contributors #
- Carlos Sanchis (Spanish translation) <csanchisb@yahoo.es>
