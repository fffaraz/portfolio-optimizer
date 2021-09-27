#!/bin/bash
set -euxo pipefail

cmake-format --version

find . -not -path "./build/*" -name 'CMakeLists.txt' -exec cmake-format -i {} \;
