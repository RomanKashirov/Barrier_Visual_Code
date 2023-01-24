RD /s /Q build-Debug build-Release build-RelWithDebInfo build-MinSizeRel
mkdir build-Debug
cd build-Debug
cmake -DCMAKE_BUILD_TYPE=Debug .. -G "MinGW Makefiles"
cmake --build .
cd ..
