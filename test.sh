#!/bin/sh
cmake -B ./build -DCMAKE_BUILD_TYPE=Release && \
cmake --build ./build --config Release && \
cd ./build && ctest -C Release --rerun-failed --output-on-failure --extra-verbose --debug