#!/bin/bash
set -euxo pipefail

CLANG_FORMAT=clang-format-18

$CLANG_FORMAT --version

find . -not -path "./build/*" -regex '.*\.\(cpp\|hpp\|cu\|c\|h\)' -exec $CLANG_FORMAT --verbose -style=file -i {} \;

# pip3 install --upgrade --user cmake-format
cmake-format --version

find . -not -path "./build/*" -name 'CMakeLists.txt' -exec cmake-format -i {} \;
