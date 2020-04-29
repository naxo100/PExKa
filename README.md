# PExKa
Parallel Expanded Kappa Simulator implemented in C++

Install instructions:

``` [bash]
$ git clone
$ cd PExKa/src/grammar
$ make
$ cd -
$ cd PExKa/Release
$ make all
$ sudo cp PExKa /usr/bin
```
Dependencies:
- flex
- bison
- libboost_program_options
- libboost_system
- libboost_filesystem

PS:
For testing is useful to install debug version:
``` [bash]
$ cd PExKa/Debug
$ make all
$ sudo cp PExKa /usr/bin/PExKa-Debug
```
