#!/bin/bash
set -euxo pipefail

clang-format --version

find . -not -path "./build/*" -regex '.*\.\(cpp\|hpp\|cu\|c\|h\)' -exec clang-format --verbose -style=file -i {} \;

# pip3 install --user cmake-format
cmake-format --version

find . -not -path "./build/*" -name 'CMakeLists.txt' -exec cmake-format -i {} \;
