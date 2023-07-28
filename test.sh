#!/bin/sh
cmake -B ./cmake_build -DCMAKE_BUILD_TYPE=Release && \
cmake --build ./cmake_build --config Release && \
cd ./cmake_build && ctest -C Release --rerun-failed --output-on-failure --extra-verbose --debug