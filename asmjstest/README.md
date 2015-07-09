## Compile steps

Under the git root, do

    mkdir build
    cd build
    cmake -DCMAKE_TOOLCHAIN_FILE=<Emscripten SDK Home>/cmake/Modules/Platform/Emscripten.cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" ../
    make
    cd asmjstest
    python -m SimpleHTTPServer 7000

Then visit http://localhost:7000/test.html

## Data file
500k pairs (1 million floats) of coordinates from https://github.com/natronics/ISS-photo-locations/tree/master/data

Compressed with 64MB dictionary size