# OpenCLTask1

## Requirements

    apt install nvidia-opencl-dev libdevil-dev

Set path to opencl include files in CMakeLists.txt

To find files, use:

    # updatedb
    locate cl.h

## Build

    mkdir build
    cd build
    cmake ..
    make

## Run

    ./OpenCLTask1
