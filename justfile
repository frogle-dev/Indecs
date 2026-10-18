build:
    cmake -B build
    cmake --build build --parallel

run:
    ./build/Indecs

buildrun:
    cmake -B build
    cmake --build build --parallel
    ./build/Indecs
