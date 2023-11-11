#!/bin/bash
PICO_SDK_PATH=/opt/pico-sdk/

mkdir -p "build/" && cd "build/"
if [[ "$?" != 0 ]]; then
    echo 'directory error!'
    exit
fi

cmake -DCMAKE_BUILD_TYPE=Release .. && make
if [[ "$?" != 0 ]]; then
    echo 'build failed at make or cmake!'
    exit
fi
