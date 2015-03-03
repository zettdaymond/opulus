# Description: #
Opulus is a simple Petri Net simulator/editor written in C++/Qt5. This is the fork of [opulus.sourceforge.net](opulus.sourceforge.net).

# Features: #
* Intuitive interface.
* Create and edit Petri Net in several ways: using visual graph, D+ and D- matrices, or IO-functions.
* Generate the Petri Net coverage graph.
* Basic net property and transition property analysis.
* Export net graph to SVG, PNG and PDF.

# Dependencies: #
To compile Opulus you will need:

* *Qt* >= 5.3
* *CMake* >= 2.8.12
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

# Autors: #
- Hugo Parente <hugo.pl@gmail.com>
- Adauto Trigueiro de Almeida Filho <adautofilho@gmail.com>
- Clerton Ribeiro de Araújo Filho <clertonfilho@gmail.com>

# Contributors #
- Carlos Sanchis (Spanish translation) <csanchisb@yahoo.es>
- cat <captainslowpoke@gmail.com>
- zett <zettday@gmail.com>