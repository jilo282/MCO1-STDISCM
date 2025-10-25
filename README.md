# MCO1-STDISCM
============================================================
BUILD AND RUN INSTRUCTIONS FOR PRIME FINDER VARIANTS
============================================================

These instructions assume you are using a modern C++ compiler like g++ (MinGW on Windows, GCC/Clang on Linux/macOS).

The programs require the C++17 standard and the threading library.

------------------------------------------------------------
STEP 1: Compiler Flags
------------------------------------------------------------
You must use the following compiler flags for ALL four programs:

    -std=c++17
    -pthread 

------------------------------------------------------------
STEP 2: Compilation (Using g++ command line)
------------------------------------------------------------
You must compile each file separately to create four independent executables (.exe or just the filename on Unix-like systems).

1.  To build variant1.cpp:
    g++ variant1.cpp -o variant1.exe -std=c++17 -pthread

2.  To build variant2.cpp:
    g++ variant2.cpp -o variant2.exe -std=c++17 -pthread

3.  To build variant3.cpp:
    g++ variant3.cpp -o variant3.exe -std=c++17 -pthread

4.  To build variant4.cpp:
    g++ variant4.cpp -o variant4.exe -std=c++17 -pthread

------------------------------------------------------------
STEP 3: Execution
------------------------------------------------------------
Ensure your "config.txt" file is in the same directory as the executable.

To run a program:

    ./variant1.exe
    ./variant2.exe
    ./variant3.exe
    ./variant4.exe

------------------------------------------------------------
IDE (Dev-C++, VS Code) NOTE
------------------------------------------------------------
If using an IDE, you need to configure the project settings or compiler options to include the "-std=c++17" and "-pthread" flags. You must create four separate projects, one for each variant.
