#!/bin/bash
set -euxo pipefail

mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="/home/faraz/Qt/5.15.2/gcc_64/" ..
cmake --build .
