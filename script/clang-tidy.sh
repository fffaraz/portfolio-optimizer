#!/bin/bash
set -euxo pipefail

clang-tidy --version

find . -not -path "./build/*" -regex '.*\.\(cpp\|hpp\|cu\|c\|h\)' -exec clang-tidy {} -- -I . \;
