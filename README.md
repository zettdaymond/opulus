# Description: #

Opulus is a simple Petri Net simulator/editor written in C++/Qt5. This is the fork of [opulus.sourceforge.net](opulus.sourceforge.net).

| Platform | CI |
|----------|:--:|
| <img src="https://raw.githubusercontent.com/zettdaymond/Font-Awesome/master/svgs/brands/linux.svg" height="20" /> GNU/Linux | ![GNU/Linux Build Status](https://github.com/zettdaymond/opulus/workflows/build/badge.svg)|
| <img src="https://raw.githubusercontent.com/zettdaymond/Font-Awesome/master/svgs/brands/apple.svg" height="20" /> macOS | ![macOS Build Status](https://github.com/zettdaymond/opulus/workflows/build/badge.svg)|

![1.png](https://bitbucket.org/repo/7X6BR9/images/653296000-1.png)

# Features: #
* Intuitive interface.
* Create and edit Petri Net in several ways: using visual graph, D+ and D- matrices, or IO-functions.
* Generate the Petri Net coverage graph.
* Basic net property and transition property analysis.
* Export net graph to SVG, PNG and PDF.

# Dependencies: #
To compile Opulus you will need:

* *Qt* >= 5.3
* *CMake* >= 3.4
* Compiler that supports C++14 (*GCC* >= 4.9 or *Clang* >= 3.4)
* *Eigen* >= 3.2

The coverage graph plugin uses *dot (graphviz)*, so you also need it installed if you want to use it.

# Compiling #
First of all, clone the repo to your machine:
```
$ git clone https://bitbucket.org/zettdaymond/opulus
$ cd ./opulus
```
Then to compile just type:
```
$ mkdir build
$ cd build
$ cmake ..
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

- **GRAPHVIZ_BACKEND=STATIC/SHARED/NONE**

Specifies what type of a graph-visualization backend the coverage graph plugin will use. STATIC - means that you have and want to use static Graphviz libraries, SHARED - the same, but shared libraries, NONE - you don't want to use any graph-visualization backend. Thus, the coverage graph, by itself, become unavailable, but the analisys result stays available.

# About Windows: #
The easiest way to compile on Windows is to use MSYS2. To install all dependencies execute following commands in MinGW shell:
```
$ pacman -Syu
$ pacman -S git mingw-w64-x86_64-gcc mingw-w64-x86_64-qt5 mingw-w64-x86_64-cmake mingw-w64-x86_64-eigen3
$ git clone https://bitbucket.org/zettdaymond/opulus
$ cd ./opulus
$ mkdir build && cd build
$ cmake -G "MSYS Makefiles" ..
```
Replace `mingw-w64-x86_64...` with `mingw-w64-i686...` if you want to build 32-bit binaries. 
If the build was successful, `cd` into `bin` directory and run the program: `$ ./opulus`
To distribute the program, you will need to copy some GCC and Qt .dll's to `bin` directory, along with entire *graphviz* distribution.
To determine .dll's you'll need to copy, run `ldd bin/opulus.exe` and copy all /mingw* .dll's.

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
