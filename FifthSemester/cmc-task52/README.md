# cmc-task52
Gear recognition

## Usage

There are three way to interact with program:

    $ ./gear <in_image.bmp> <out_image.bmp> <out_result.txt>
    $ ./gear --plugin <in_image.bmp> <out_image.bmp>
    $ ./gear --gui

## Execution requirements

You need `fltk` for execution. Try this command to install it:

Debian-based:

    # apt-get install libfltk1.3

Fedora-based:

    # dnf install fltk

## Compilation requirements

* [c/c++ compiler](http://gcc.gnu.org/)
* [cmake](https://cmake.org/)
* [fltk-devel](https://cmake.org/)

For example if you are using Fedora Linux, you may install all prerequirements:

    # dnf install gcc gcc-c++ cmake fltk-devel fltk-static

For Ubuntu:

    # apt-get install gcc gcc-c++ cmake libfltk1.3-dev

## Compilation

    $ cd PROJECT_FOLDER
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

For static build:

    $ ../build-static.sh
