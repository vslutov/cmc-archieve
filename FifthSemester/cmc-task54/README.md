# cmc-task54
Black hole visualization

    Подготовил
    Лютов Владимир Сергеевич
    Студент 320 группы ВМК МГУ

Реализована базовая часть и два допа: параллельное выполнение на OpenGL,
вращение с помощью мыши и просто медленное вращение.

Вращать изображение можно зажимая левую кнопку мыши и двигая ею по
окошку программы.

Чтобы изменить разрешение рендеринга просто измените размер окна.

Based on [OGLdev](http://ogldev.atspace.co.uk/)

## Параллельность

Так как использовался OpenGL параллельность была изначально.
На компьютере среднего уровня при разрешении 1900x1600 скорость работы 20 FPS.

## Prerequirement

* OpenGL
* freeglut
* glew

Install on Fedora

    dnf install freeglut glew freeimage

## Building

For building you need `gcc-c++`, `cmake` and development libraries.

Install on Fedora:

    dnf install gcc-c++ cmake freeglut-devel glew-devel freeimage-devel

Install on ubuntu:

    apt-get install gcc-c++ cmake freeglut3-dev libglew-dev libfreeimage-dev

For documentation:

    dnf install Doxygen

Building with cmake:

    mkdir build
    cd build
    cmake ..
    make
    make doc

Run:

    ./blackhole
