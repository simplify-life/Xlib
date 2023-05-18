#!/bin/sh
cmake -B ./build -DCMAKE_BUILD_TYPE=Release && cmake --build ./build --config Release && cd ./build && ctest -C Release