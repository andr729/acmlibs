#!/bin/bash

file=$1; # todo: files
aux1="${file%.cpp}.exe"
out="bin/"${aux1#"libs/"};

set -x
g++ -std=c++17 -o "$out" "$file" "${@:2}";
set +x