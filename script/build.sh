#!/bin/bash
set -euxo pipefail

mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --parallel $(nproc)
ctest --output-on-failure
